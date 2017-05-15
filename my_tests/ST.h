#ifndef ___SPLAY_TREE_H__
#define ___SPLAY_TREE_H__

typedef struct ST_str ST;
struct ST_str {
    int id;
    ST* left;
    ST* right;
    ST* parent;
};

ST* ST_make_ptr(int i);
ST* ST_p(ST* node);
ST* ST_g(ST* node);
int ST_is_root(ST* node);
int ST_is_left_child(ST* node);
int ST_is_right_child(ST* node);
void ST_rotate_left(ST* y);
void ST_rotate_right(ST* y);
void ST_splay(ST* x);
ST* ST_insert(ST* root, int i);
ST* ST_access(ST* root, int i);
void ST_delete(ST* root, int i);
void ST_print_node(ST* root);
int ST__print_t(ST* tree, int is_left, int offset, int depth, char s[20][255]);
void ST_print_t(ST* tree);

#endif /*___SPLAY_TREE_H__*/