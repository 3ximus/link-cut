#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#define DEBUG 1

void LOG_CMD(int color, const char* fmt, ...) {
    #if DEBUG
        const char* COLORS[] = {
            "",
            "\033[30m", //GREY
            "\033[33m", //YELLOW
            "\033[34m", //BLUE
            "\033[35m", //PURPLE
            "\033[32m", //GREEN
            "\033[36m", //CYAN
            "\033[31m", //RED
        };
        const char* NO_COLOR = "\033[0m";

        printf("%s", COLORS[(color % 7) + 1]);

        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("%s", NO_COLOR);

        fflush(stdout);
    #endif
}
// #################################################
// ################### NODE ########################
// #################################################
typedef struct node_str Node;
struct node_str {
    int id; // Unused really, just for representation in printing

    Node *left;
    Node *right;
    Node *parent;
    Node *path_parent; // We can in the end take this off (or not). Leave it for now, for simplicity. Here there is a tradeoff space/time.
};

Node *make_node(int id) {
    Node *res = calloc(sizeof(Node), 1); // Calloc makes everything NULL
    res->id = id;

    return res;
}

Node* parent(Node* node) {
    return node ? node->parent : NULL;
}

Node* grand_parent(Node* node) {
    return parent(parent(node));
}

// root of its aux tree
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
        // skip
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
        // skip
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
        p  = x->parent; // Parent
        if(p == NULL) {
            break;
        }

        pp = grand_parent(x); // Grand-parent
        if (is_left_child(x)) {
            if (pp == NULL) { // Zig
                rotate_right(p);
                return;

            } else if (is_left_child(p)) { // Zig-Zig -> x is left of p(x) and p(x) is left of g(x)
                rotate_right(grand_parent(x));
                rotate_right(parent(x));

            } else { // Zig-Zag -> x is left of p(x) and p(x) is right of g(x)
                rotate_right(parent(x));
                rotate_left(parent(x));
            }
        } else { // is_right_child(x)
            if (pp == NULL) { // Zig
                rotate_left(p);
                return;

            } else if (is_right_child(p)) { // Zig-Zig -> x is right of p(x) and p(x) is right of g(x)
                rotate_left(grand_parent(x));
                rotate_left(parent(x));

            } else { // Zig-Zag -> x is right of p(x) and p(x) is left of g(x)
                rotate_left(parent(x));
                rotate_right(parent(x));
            }
        }
    }
}

// Printing recursive auxiliary function
int print_st_aux(Node* tree, int is_left, int offset, int depth, char s[20][255]) {
    if(depth > 19) {
        printf("I'm in too deep.\n");
        return 0;
    }

    char b[20];
    int width = 5;

    if (!tree)
        return 0;

    sprintf(b, "(%03d)", tree->id);

    int left = print_st_aux(tree->left, 1, offset, depth + 1, s);
    int right = print_st_aux(tree->right, 0, offset + left + width, depth + 1, s);

    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width / 2 + i] = '-';

        s[depth - 1][offset + left + width / 2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width / 2 + i] = '-';

        s[depth - 1][offset + left + width / 2] = '.';
    }

    return left + width + right;
}

// Print an aux tree
void print_st(Node* tree) {
    char s[20][255];
    for (int i = 0; i < 20; i++) {
        sprintf(s[i], "%80s", " ");
    }
    print_st_aux(tree, 0, 0, 0, s);

    char test_str[81];
    sprintf(test_str, "%80s", " ");
    for (int i = 0; i < 20; i++) {
        if(strncmp(s[i], test_str, 81) == 0) {
            break;
        }
        printf("%s\n", s[i]);
    }
}

/* LEL */
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


// #################################################
// ################### TREE ########################
// #################################################
typedef struct tree_str Tree;
struct tree_str {
    Node** nodes;
    int size;
};

void print_tree(Tree *tree);

Tree *make_tree(int size) {
    LOG_CMD(2, "Making a tree of size %d.\n", size);

    Tree* tree= (Tree *)malloc(sizeof(Tree));

    tree->nodes = (Node **)malloc(sizeof(Node *) * size);
    for(int i = 0; i < size; i++) {
        tree->nodes[i] = make_node(i);
    }
    tree->size = size;

    print_tree(tree);
    return tree;
}

// Useless, but OK
void free_tree(Tree *tree) {
    LOG_CMD(2, "Freeing tree.\n");
    assert(tree);

    for(int i = 0; i < tree->size; i++) {
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

    // Splays within its aux tree
    splay(v);

    // Remove v's preferred child
    if (v->right) {
        v->right->path_parent = v;
        v->right->parent = NULL;
        v->right = NULL;
    }

    // Changes the preferred path to be this new one
    while(v->path_parent) {
        Node *w = v->path_parent;
        splay(w);

        // Switch w's preferred child from whatever it was to v
        if(w->right) {
            w->right->path_parent = w;
            w->right->parent = NULL;
            w->right = NULL;
        }
        v->parent = w;
        v->path_parent = NULL;

        splay(v);
    }
}

void cut(Tree *tree, int _v) {
    LOG_CMD(1, "Cutting %d from it's parent (on the represented tree).\n", _v);

    Node *v = tree->nodes[_v];

    access(v);
    if(v->left) {
        v->left->parent = NULL;
        v->left = NULL;
    }
}

void link(Tree *tree, int _v, int _w) {
    LOG_CMD(1, "Linking %d and %d.\n", _v, _w);

    Node *v = tree->nodes[_v];
    Node *w = tree->nodes[_w];

    printf("Before the link.\n");
    print_tree(tree);

    // v and w must be on different trees.
    access(v);
    access(w);

    // This could and probably should be (for better balancing)
    // w->right = v
    // v->parent = w
    v->left = w;
    w->parent = v;

    printf("After the link.\n");
    print_tree(tree);
}

Node *find_root(Node *v) {
    assert(v);

    access(v);
    while(v->left) {
        v = v->left;
    }

    splay(v);
    return v;
}

int connected(Tree *tree, int _v, int _w) {
    LOG_CMD(1, "Testing connectivity from %d to %d.\n", _v, _w);

    if (_v == _w)  {
        return 1;
    }

    Node *v = tree->nodes[_v];
    Node *w = tree->nodes[_w];

    assert(w);
    assert(v);
    // This could probably be improved, but I think it should work. If they have the same root, they are in the same path

    return find_root(v) == find_root(w);
}

void print_tree(Tree *tree) {
    assert(tree);
    Node *unique_aux_roots[tree->size];
    int inserted_roots = 0;

    // Find all diferent roots of aux trees
    for (int i = 0; i < tree->size; i++) {
        Node *root = tree->nodes[i];
        while(root->parent) {
            root = root->parent;
        }

        int found = 0;
        for (int j = 0; j < inserted_roots; j++) {
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

    printf("============= TREE OF TREES ==============\n");
    for(int i = 0; i < inserted_roots; i++) {
        printf("--------------------------------------\n");
        printf("Path parent = %p\n", unique_aux_roots[i]->path_parent ? unique_aux_roots[i]->path_parent->id : 0);
        print_st(unique_aux_roots[i]);
    }
    printf("==========================================\n");
    printf("==========================================\n");
}

int main() {
	char command;
	int arg1, arg2, size;

	scanf("%d\n", &size);
	Tree *tree = make_tree(size);

	while (scanf("%c %d %d\n", &command, &arg1, &arg2) != EOF) {
		arg1--;
        arg2--;
        switch (command) {
			case 'L':
				link(tree, arg1, arg2);
				break;
			case 'C':
                // @SEE: Our cut only receives an argument and cuts this node from its parent. Do we need to change this?
				cut(tree, arg1);
				// cut(tree, arg1, arg2);
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