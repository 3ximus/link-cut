#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#define DEBUG 0

#define PRINT_START 1
#define PRINT_CUT 2
#define PRINT_LINK 3
#define PRINT_CONNECTED 4
#define PRINT_SPECIAL 5

#if DEBUG
    const char* COLORS[] = {
        "",
        "\033[30m", /*GREY */
        "\033[34m", /*BLUE */
        "\033[31m", /*RED */
        "\033[32m", /*GREEN */
        "\033[33m", /*YELLOW */
        "\033[35m", /*PURPLE */
        "\033[36m", /*CYAN */
    };
    const char* NO_COLOR = "\033[0m";
#endif

void LOG_CMD(int color, const char* fmt, ...) {
    #if DEBUG
        printf("%s", COLORS[(color % 7) + 1]);

        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("%s", NO_COLOR);

        fflush(stdout);
    #endif
}
/* ################################################# */
/* ################### NODE ######################## */
/* ################################################# */
typedef struct node_str Node;
struct node_str {
    int id; /* Unused really, just for representation in printing */

    Node *left;
    Node *right;
    Node *parent;
    Node *path_parent; /* We can in the end take this off (or not). Leave it for now, for simplicity. Here there is a tradeoff space/time. */
};

Node *make_node(int id) {
    Node *res = calloc(sizeof(Node), 1); /* Calloc makes everything NULL */
    res->id = id;

    return res;
}

Node* parent(Node* node) {
    return node ? node->parent : NULL;
}

Node* grand_parent(Node* node) {
    return parent(parent(node));
}

/* root of its aux tree */
int is_root(Node *node) {
    assert(node);
    return (node->parent != NULL);
}

int is_left_child(Node* node) {
    assert(node);
    assert(node->parent);
    return (node->parent->left == node);
}

int is_right_child(Node* node) {
    assert(node);
    assert(node->parent);
    return (node->parent->right == node);
}

/* This, where x a y can have subtrees as well
z           z
 \         /
  y   OR  y
   \       \
    x       x
 */
void rotate_left(Node* y) {
    assert(y);
    Node *x = y->right;
    assert(x);
    Node *z = y->parent;

    if (z == NULL) {
        /* skip */
    } else if (is_left_child(y)) {
        z->left = x;
    } else if (is_right_child(y)) {
        z->right = x;
    }
    y->right = x->left;
    x->left = y;
    x->parent = z;
    y->parent = x;

    if (y->right == NULL) {
        return;
    } else {
        y->right->parent = y;
    }
}

void rotate_right(Node* y) {
    assert(y);
    Node *x = y->left;
    assert(x);
    Node *z = y->parent;

    if (z == NULL) {
        /* skip */
    } else if (is_left_child(y)) {
        z->left = x;
    } else if (is_right_child(y)) {
        z->right = x;
    }

    y->left = x->right;
    x->right = y;
    x->parent = z;
    y->parent = x;

    if (y->left == NULL) {
        return;
    } else {
        y->left->parent = y;
    }
}

void splay(Node* x) {
    assert(x);
    LOG_CMD(0, "Splaying %d\n", x->id);

    Node *p, *pp;
    while (1) {
        p  = x->parent; /* Parent */
        if(p == NULL) {
            break;
        }

        pp = grand_parent(x); /* Grand-parent */
		if (pp == NULL) { /* p is root */
			x->path_parent = p->path_parent;
			p->path_parent = NULL;
		} else if (pp->parent == NULL) { /* pp is root */
			x->path_parent = pp->path_parent;
			pp->path_parent = NULL;
		}

        if (is_left_child(x)) {
            if (pp == NULL) { /* Zig */
                rotate_right(p);
                return;

            } else if (is_left_child(p)) { /* Zig-Zig -> x is left of p(x) and p(x) is left of g(x) */
                rotate_right(grand_parent(x));
                rotate_right(parent(x));

            } else { /* Zig-Zag -> x is left of p(x) and p(x) is right of g(x) */
                rotate_right(parent(x));
                rotate_left(parent(x));
            }
        } else { /* is_right_child(x) */
            if (pp == NULL) { /* Zig */
                rotate_left(p);
                return;

            } else if (is_right_child(p)) { /* Zig-Zig -> x is right of p(x) and p(x) is right of g(x) */
                rotate_left(grand_parent(x));
                rotate_left(parent(x));

            } else { /* Zig-Zag -> x is right of p(x) and p(x) is left of g(x) */
                rotate_left(parent(x));
                rotate_right(parent(x));
            }
        }
    }
}

/* Printing recursive auxiliary function */
int print_st_aux(Node* tree, int is_left, int offset, int depth, char s[20][255], int simple) {
    if(depth > 19) {
        printf("I'm in too deep.\n");
        return 0;
    }

    char b[20];
    int width = 4;
    if(!simple)
        width = 10;

    if (!tree)
        return 0;

    if(simple)
        sprintf(b, "(%02d)", tree->id);
    else
        sprintf(b, "(pp:%02d|%02d)", tree->path_parent ? tree->path_parent->id : -1, tree->id);

    int left = print_st_aux(tree->left, 1, offset, depth + 1, s, simple);
    int right = print_st_aux(tree->right, 0, offset + left + width, depth + 1, s, simple);

    int i;
    for (i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width / 2 + i] = '-';

        s[depth - 1][offset + left + width / 2] = '.';

    } else if (depth && !is_left) {

        for (i = 0; i < left + width; i++)
            s[depth - 1][offset - width / 2 + i] = '-';

        s[depth - 1][offset + left + width / 2] = '.';
    }

    return left + width + right;
}

/* Print an aux tree */
void print_st(Node* tree, int simple) {
    char s[20][255];
    int i;
    for (i = 0; i < 20; i++) {
        sprintf(s[i], "%80s", " ");
    }
    print_st_aux(tree, 0, 0, 0, s, simple);

    char test_str[81];
    sprintf(test_str, "%80s", " ");
    for (i = 0; i < 20; i++) {
        if(strncmp(s[i], test_str, 81) == 0) {
            break;
        }
        printf("%s\n", s[i]);
    }
}

/* LEL  */
void print_node(Node* node) {
    assert(node);
    printf("====================================\n");
    char buf[80];
    if (node->parent == NULL) strcpy(buf, "NULL");
    else sprintf(buf, "%d  ", node->parent->id);
    printf("    %4s    (parent)\n", buf);
    printf("     |             \n");

    sprintf(buf, "%d", node->id);
    printf("  %4s      (node) \n", buf);
    printf("    / \\          \n");

    if (node->left == NULL) strcpy(buf, "NULL");
    else sprintf(buf, "%d", node->left->id);
    printf("%4s ", buf);

    if (node->right == NULL) strcpy(buf, "NULL");
    else sprintf(buf, "%d  ", node->right->id);
    printf(" %4s (left, right)\n", buf);
    printf("====================================\n");
}


/* ################################################# */
/* ################### TREE ######################## */
/* ################################################# */
typedef struct tree_str Tree;
struct tree_str {
    Node** nodes;
    int size;
};
Tree *TREE = NULL;


void print_tree(int color, Tree *tree);
int connected(Tree *tree, int _v, int _w);

Tree *make_tree(int size) {
    LOG_CMD(PRINT_START, "Making a tree of size %d.\n", size);

    Tree* tree= (Tree *)malloc(sizeof(Tree));

    tree->nodes = (Node **)malloc(sizeof(Node *) * size);
    int i;
    for (i = 0; i < size; i++) {
        tree->nodes[i] = make_node(i+1);
    }
    tree->size = size;

    LOG_CMD(PRINT_START, "Initial tree: \n");
    print_tree(PRINT_START, tree);

    return tree;
}

/* Useless, but OK */
void free_tree(Tree *tree) {
    LOG_CMD(2, "Freeing tree.\n");
    assert(tree);

    int i;
    for (i = 0; i < tree->size; i++) {
        free(tree->nodes[i]);
        tree->nodes[i] = NULL;
    }

    free(tree->nodes);
    tree->nodes = NULL;

    free(tree);
    tree = NULL;
}

void access(Node *v) {
    assert(v);
    LOG_CMD(0, "Acessing %d.\n", v->id);

    /* Splays within its aux tree */
    splay(v);

    /* Remove v's preferred child */
    if (v->right) {
        v->right->path_parent = v;
        v->right->parent = NULL;
        v->right = NULL;
    }

    /* Changes the preferred path to be this new one */
    while(v->path_parent) {
		/* print_tree(0, TREE); */
        Node *w = v->path_parent;
        splay(w);

        /* Switch w's preferred child from whatever it was to v */
        if(w->right) {
            w->right->path_parent = w;
            w->right->parent = NULL;
        }
        w->right = v;
        v->parent = w;
        v->path_parent = NULL;

        splay(v);
    }
	/* print_tree(0, TREE); */
}

void cut(Tree *tree, int _v, int _w) {
    Node *v = tree->nodes[_v];
    Node *w = tree->nodes[_w];
    LOG_CMD(PRINT_CUT, "Cutting %d from %d (on the represented tree).\n", v->id, w->id);

	/* @SEE: Can we just do this here or do we need to do the access as well to make */
	/* Just remove the path parents if that is the case. This is O(1), so it must preserve O(log(N)) amortized */
	if (v->path_parent == w) {
		v->path_parent = NULL;
	} else if (w->path_parent == v) {
		w->path_parent = NULL;
	} else {
		access(w);
		access(v);

		if (v->left && v->left->id == w->id) { /* If the w node is above (the parent) on the represented tree */
			v->left->parent = NULL;
			v->left = NULL;
		} else {
            splay(w); /* Make w the root of its aux tree */
            if (w->path_parent == v) {
    			w->path_parent = NULL;
            } else {
	    	    LOG_CMD(PRINT_SPECIAL, "Tryed to cut nodes not connected.\n", v->id);
            }
		}
	}

    LOG_CMD(PRINT_CUT, "After cut: \n");
    print_tree(PRINT_CUT, tree);
}

void link(Tree *tree, int _v, int _w) {
    Node *v = tree->nodes[_v];
    Node *w = tree->nodes[_w];
    LOG_CMD(PRINT_LINK, "Linking %d and %d.\n", v->id, w->id);


	/* This is O(log(N)) */
	if (connected(tree, _v, _w)) {
	    LOG_CMD(PRINT_SPECIAL, "Tryed to link nodes already connected, so skip this one.\n", v->id);
		return;
	}

    /* v and w must be on different trees. */
    access(v);
    access(w);

    w->right = v;
    v->parent = w;

    LOG_CMD(PRINT_LINK, "After the link.\n");
    print_tree(PRINT_LINK, tree);
}

Node *find_root(Node *v) {
    assert(v);
    LOG_CMD(0, "Find root.\n");

    access(v);
    while(v->left) {
        v = v->left;
    }

    splay(v);
    return v;
}

int connected(Tree *tree, int _v, int _w) {
    Node *v = tree->nodes[_v];
    Node *w = tree->nodes[_w];
    LOG_CMD(PRINT_CONNECTED, "Testing connectivity from %d to %d.\n", v->id, w->id);

    if (_v == _w)  {
        return 1;
    }

    assert(w);
    assert(v);

    /* @SEE: Does this work, or does this only work to find if the nodes are within the same */
    /* splay tree (aka aux tree, aka preferred path) */
    Node *v_root = find_root(v);
    LOG_CMD(PRINT_CONNECTED, "After first find root: \n");
    print_tree(PRINT_CONNECTED, tree);

    int res = (v_root == find_root(w));

    LOG_CMD(PRINT_CONNECTED, "After connected: \n");
    print_tree(PRINT_CONNECTED, tree);

    return res;
}

void print_tree(int color, Tree *tree) {
    #if DEBUG
        assert(tree);
        Node *unique_aux_roots[tree->size];
        int inserted_roots = 0;

        /* Find all diferent roots of aux trees */
		int i;
        for (i = 0; i < tree->size; i++) {
            Node *root = tree->nodes[i];
            while(root->parent) {
                root = root->parent;
            }

            int found = 0;
	    	int j;
            for (j = 0; j < inserted_roots; j++) {
                if(unique_aux_roots[j] == root) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                unique_aux_roots[inserted_roots] = root;
                inserted_roots++;
            }
        }
        printf("%s", COLORS[(color % 7) + 1]);
        printf("============= TREE OF TREES ==============\n");
        for (i = 0; i < inserted_roots; i++) {
            printf("--------------------------------------\n");
            /*printf("Path parent = %p\n", unique_aux_roots[i]->path_parent ? unique_aux_roots[i]->path_parent->id : 0); */
            print_st(unique_aux_roots[i], 0);
        }
        printf("==========================================\n");
        printf("%s", NO_COLOR);
        printf("\n==========================================\n");
        printf("==========================================\n\n");
    #endif
}

int main() {
	char command;
	int arg1, arg2, size;

	scanf("%d\n", &size);
	Tree *tree = make_tree(size);
	TREE = tree;
	while (scanf("%c %d %d\n", &command, &arg1, &arg2) != EOF) {
		arg1--;
        arg2--;
        switch (command) {
			case 'L':
				link(tree, arg1, arg2);
				break;
			case 'C':
				cut(tree, arg2, arg1);
				/* cut(tree, arg1, arg2); */
				break;
			case 'Q':
				printf(connected(tree, arg1, arg2) ? "T\n" : "F\n");
				break;
			default:
				printf("Unknown command %c", command);
				exit(EXIT_FAILURE);
		}
	}
	return 0;
}
