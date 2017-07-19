#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_splay_tree.h"

ST* ST_make_ptr(int i) {
    ST *res  = (ST *) malloc(sizeof(ST));
    res->id = i;
    res->left = NULL;
    res->right = NULL;
    res->parent = NULL;
    return res;
}

ST* ST_p(ST* node) {
    if (!node) {
        return NULL;
    }
    return node->parent;
}

ST* ST_g(ST* node) {
    return ST_p(ST_p(node));
}

int ST_is_root(ST* node) {
    assert(node);
    return (node->parent != NULL);
}

int ST_is_left_child(ST* node) {
    assert(node);
    assert(ST_p(node));
    return (ST_p(node)->left == node);
}

int ST_is_right_child(ST* node) {
    assert(node);
    assert(ST_p(node));
    return (ST_p(node)->right == node);
}

/* This, where x a y can have subtrees as well
z           z
 \         /
  y   OR  y
   \       \
    x       x
*/
void ST_rotate_left(ST* y) {
    ST *x = y->right;
    ST *z = ST_p(y);

    if (z == NULL) {
        // skip
    } else if (ST_is_left_child(y)) {
        z->left = x;
    } else if (ST_is_right_child(y)) {
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

void ST_rotate_right(ST* y) {
    ST *x = y->left;
    ST *z = ST_p(y);

    if (z == NULL) {
        // skip
    } else if (ST_is_left_child(y)) {
        z->left = x;
    } else if (ST_is_right_child(y)) {
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

void ST_splay(ST* x) {
    assert(x);
    printf("Splaying %d\n", x->id);

    ST *p, *pp;
    while (1) {
        p  = ST_p(x); // Parent
        if(p == NULL) {
            break;
        }

        pp = ST_g(x); // Grand-parent
        if (ST_is_left_child(x)) {
            if (pp == NULL) { // Zig
                ST_rotate_right(p);
                return;

            } else if (ST_is_left_child(p)) { // Zig-Zig -> x is left of p(x) and p(x) is left of g(x)
                ST_rotate_right(ST_g(x));
                ST_rotate_right(ST_p(x));

            } else { // Zig-Zag -> x is left of p(x) and p(x) is right of g(x)
                ST_rotate_right(ST_p(x));
                ST_rotate_left(ST_p(x));
            }
        } else { // is_right_child(x)
            if (pp == NULL) { // Zig
                ST_rotate_left(p);
                return;

            } else if (ST_is_right_child(p)) { // Zig-Zig -> x is right of p(x) and p(x) is right of g(x)
                ST_rotate_left(ST_g(x));
                ST_rotate_left(ST_p(x));

            } else { // Zig-Zag -> x is right of p(x) and p(x) is left of g(x)
                ST_rotate_left(ST_p(x));
                ST_rotate_right(ST_p(x));
            }
        }
    }
}

// Returns the new root
ST *ST_insert(ST *root, int i) {
    ST *new_node = ST_make_ptr(i);
    // If there is no root yet, aka we are the first element, we are now the root
    if (root == NULL) {
        return new_node;
    }

    ST *pos = root;
    while(1) {
        if (i > pos->id) {
            if(pos->right == NULL) {
                pos->right = new_node;
                new_node->parent = pos;
                printf("Inserting element %d bellow %d.\n", new_node->id, pos->id);
                break;
            } else {
                pos = pos->right;
            }
        } else if (i < pos->id) {
            if(pos->left == NULL) {
                pos->left = new_node;
                new_node->parent = pos;
                printf("Inserting element %d bellow %d.\n", new_node->id, pos->id);
                break;
            } else {
                pos = pos->left;
            }
        } else {
            // We can't have repeated elements, aka i == pos->id cannot happen
            assert(0);
        }
    }

    //ST_print_node(root);
    ST_splay(new_node);
    return new_node; // Should be the root now
}

// Returns the new root
ST *ST_access(ST *root, int i) {
    printf("Accessing element %d.\n", i);
    if (root == NULL) {
        return NULL;
    }

    ST *pos = root;
    while(1) {
        if (i > pos->id) {
            if(pos->right == NULL) {
                // We still splain if we don't find the node, that's why we don't just return.
                break;
            } else {
                pos = pos->right;
            }
        } else if (i < pos->id) {
            if(pos->left == NULL) {
                // We still splain if we don't find the node, that's why we don't just return.
                break;
            } else {
                pos = pos->left;
            }
        } else {
            // We found it!
            break;
        }
    }


    ST_splay(pos);
    return pos; // The node we found or NULL if we did not find it.
}

void ST_delete(ST *root, int i) {
    // TODO: Do we need it?
}

/* LEL */
void ST_print_node(ST* node) {
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

int ST_print_t_aux(ST* tree, int is_left, int offset, int depth, char s[20][255])
{
    if(depth > 19) {
        printf("I'm in too deep.\n");
        return 0;
    }

    char b[20];
    int width = 5;

    if (!tree)
        return 0;

    sprintf(b, "(%03d)", tree->id);

    int left = ST_print_t_aux(tree->left, 1, offset, depth + 1, s);
    int right = ST_print_t_aux(tree->right, 0, offset + left + width, depth + 1, s);

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

void ST_print_t(ST* tree)
{
    char s[20][255];
    for (int i = 0; i < 20; i++) {
        sprintf(s[i], "%80s", " ");
    }
    ST_print_t_aux(tree, 0, 0, 0, s);


    char test_str[81];
    sprintf(test_str, "%80s", " ");
    for (int i = 0; i < 20; i++) {
        if(strncmp(s[i], test_str, 81) == 0) {
            break;
        }
        printf("%s\n", s[i]);
    }
}