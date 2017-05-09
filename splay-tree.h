#ifndef ___SPLAY_TREE_H__
#define ___SPLAY_TREE_H__

typedef struct LCT *LCT;

struct LCT {
	int id;
	LCT left;
	LCT right;
	LCT* hook;
	int sum; /* sub-tree size, negative values mean left and right must be swamped */
};


LCT rotate_right(LCT v) {
	LCT lnode = v->left;
	v->left = lnode->right;
	lnode->right = v;
	return lnode;
}

LCT rotate_left(LCT v) {
	LCT rnode = v->right;
	v->right = rnode->left;
	rnode->left = v;
	return rnode;
}

LCT splay(LCT root, int id) {
	if (root == NULL || root->id == id)
		return root;

	if (root->id > id) { /* id is in the left subtree */
		if (root->left == NULL) return root;

		if (root->left->id > id) { /* left left */
			root->left->left = splay(root->left->left, id);
			root = rotate_right(root);
		} else if (root->left->id < id) { /* left right */
			root->left->right = splay(root->left->right, id);
			if (root->left->right != NULL)
				root->left = rotate_left(root->left);
		}
		return (root->left == NULL) ? root : rotate_right(root);
	} else { /* id is in the right subtree */
		if (root->right == NULL) return root;

		if (root->right->id > id) { /* right left */
			root->right->left = splay(root->right->left, id);
			if (root->right->left != NULL)
				root->right = rotate_right(root->right);
		} else if (root->right->id < id) { /* right right */
			root->right->right = splay(root->right->right, id);
			root = rotate_left(root);
		}
		return (root->right == NULL) ? root : rotate_left(root);
	}
}

LCT find(LCT root, int id) {
	return splay(root, id);
}

#endif /*___SPLAY_TREE_H__*/