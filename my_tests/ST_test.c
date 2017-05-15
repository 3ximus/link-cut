#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_splay_tree.h"

int main()
{
    ST* root = NULL;

    root = ST_insert(root, 1);
    root = ST_insert(root, 2);
    root = ST_insert(root, 6);
    root = ST_insert(root, 5);
    root = ST_insert(root, 7);

    root = ST_access(root, 4);

    ST_print_node(root);
    ST_print_t(root);
}

