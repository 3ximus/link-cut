#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define NANO_FACTOR 1E9
#include <time.h>
#define START_COUNTER() start_count()
#define STOP_COUNTER(...) log_time(__VA_ARGS__)

struct timespec startTime, endTime;
FILE* fp;

void start_count() {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
}

/* also stops counter */
void log_time(char* text) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
    double dt = endTime.tv_sec - startTime.tv_sec + (endTime.tv_nsec - startTime.tv_nsec) / NANO_FACTOR;
    fprintf(fp, "\t%s - %f seconds\n" , text, dt);
}

#define NOT_FLIPED 0
#define FLIPED 1

/* ################################################# */
/* ################### NODE ######################## */
/* ################################################# */
typedef struct node_str Node;
struct node_str {
    Node *left;
    Node *right;
    Node *parent;
    Node *path_parent; /* We can in the end take this off (or not). Leave it for now, for simplicity. Here there is a tradeoff space/time. */

    char is_fliped;
};

Node *make_node() {
    Node *res = calloc(sizeof(Node), 1); /* Calloc makes everything NULL */
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
    return (node->parent != NULL);
}

int is_left_child(Node* node) {
    return (node->parent->left == node);
}

int is_right_child(Node* node) {
    return (node->parent->right == node);
}

void unflip(Node *v) {
    /* Only something to do if we are fliped */
    if(v->is_fliped) {
        /* Swap them in memory*/
        Node *aux = v->right;
        v->right = v->left;
        v->left = aux;

        /* Make us not fliped and 'flip' the bits of the childs */
        v->is_fliped = NOT_FLIPED;
        if(v->left) {
            v->left->is_fliped = !(v->left->is_fliped);
        }
        if(v->right) {
            v->right->is_fliped = !(v->right->is_fliped);
        }
    }
}

/* This, where x a y can have subtrees as well
z           z
 \         /
  y   OR  y
   \       \
    x       x
 */
void rotate_left(Node* y) {
    Node *x = y->right;
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
    Node *x = y->left;
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
    Node *p, *pp;
    while (1) {
        p  = x->parent; /* Parent */
        if(p == NULL) {
            break;
        }

        pp = grand_parent(x); /* Grand-parent */

        /* Unflip if we have any node fliped (from the grandparent to the child) */
        if (pp) {
            unflip(pp);
        }
        unflip(p);
        unflip(x);

        /* Propagate path_parent */
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

    unflip(x);
}


/* ################################################# */
/* ################### TREE ######################## */
/* ################################################# */
typedef struct tree_str Tree;
struct tree_str {
    Node** nodes;
    int size;
};


Tree *make_tree(int size) {
    Tree* tree = (Tree *)malloc(sizeof(Tree));
    tree->nodes = (Node **)malloc(sizeof(Node *) * size);

    int i;
    for (i = 0; i < size; i++) {
        tree->nodes[i] = make_node();
    }
    tree->size = size;

    return tree;
}

void free_tree(Tree *tree) {
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
    /* Splays the aux tree */
    splay(v);

    /* Remove v's preferred child */
    if (v->right) {
        v->right->path_parent = v;
        v->right->parent = NULL;
        v->right = NULL;
    }

    /* Changes the preferred path to be this new one */
    while (v->path_parent) {
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
}

Node *get_right_most_node(Node *node) {
    unflip(node);
    while(node->right) {
        node = node->right;
        unflip(node);
    }
    return node;
}


void re_root(Node *new_root) {
    /* just flip a bit */
    access(new_root);
    new_root->is_fliped = !new_root->is_fliped;
}

void cut(Tree *tree, int _v, int _w) {
    Node *v = tree->nodes[_v];
    Node *w = tree->nodes[_w];

    if(_v == _w)
        return;

    /* Make v the root of the splay tree and the tree of trees (access) */
    /* 1. The node which we get by following v's left node to the right (aka the node which is the parent
          of v on the represented tree) */
    /* 2. Now either w (after splayed) points to v through the path_pointer (w is a child of v) or.. */
    access(v);
    if (v->left && get_right_most_node(v->left) == w) { /* w is a parent of v */
        v->left->parent = NULL;
        v->left = NULL;
    } else {
        /* Splay to make w have a path_parent (if any exist) */
        splay(w);
        if (w->left == NULL && w->path_parent == v) { /* w is a child of v */
            w->path_parent = NULL;
        }
    }
}


void link(Tree *tree, int _v, int _w) {
    Node *v = tree->nodes[_v];
    Node *w = tree->nodes[_w];

    if (_v == _w)
        return;

    /* Make v the root of the represented tree (to make the link possible) */
    re_root(v);
    access(w);

    /* This is suficient to determine if it would cause a loop. No need to do connected(...) */
    if (v->parent || v->path_parent) {
        return;
    } else {
        w->right = v;
        v->parent = w;
    }

}

Node *find_root(Node *v) {
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

    if (_v == _w)  {
        return 1;
    }

    re_root(v);
    access(w);
    /* If w 'messed' with v, making it not the root anymore, that means both are in the same aux tree, and since
       v is the root, it means the are connected */
    return (v->parent || v->path_parent);
}

int main() {
	char command;
	int arg1, arg2, size;

	scanf("%d\n", &size);
	Tree *tree = make_tree(size);

    fp = fopen("time.log", "w");
    start_count();

    while (scanf("%c %d %d\n", &command, &arg1, &arg2) != EOF) {
		arg1--;
        arg2--;
        switch (command) {
			case 'L':
				link(tree, arg1, arg2);
				break;
			case 'C':
				cut(tree, arg1, arg2);
				break;
			case 'Q':
				printf(connected(tree, arg1, arg2) ? "T\n" : "F\n");
				break;
			default:
				printf("Unknown command %c\n", command);
				exit(EXIT_FAILURE);
		}
	}

    log_time("");
    fclose(fp);

    free_tree(tree);
	return 0;
}