#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "splay-tree.h"


LCT allocLCT(int size) {
	LCT sp = (LCT)malloc(sizeof(struct LCT)*size);
	int i;
	for (i = 0; i < size ; i++) { /* initialize nodes */
		(sp + i)->left = (sp + i)->right = NULL;
		(sp + i)->id = i;
	}
	return sp;
}

void freeLCT(LCT t) {
	free(t);
}


void access(LCT t, int v) {
	/* TODO */
}

void reRoot(LCT t, int v) {
	splay(t, v);
}


/**
 * MAIN METHODS
 */

void link(LCT t, int r, int v) {
	/* TODO */
}

void cut(LCT t, int r, int v) {
	/* TODO */
}

/**
 * Returns 0 - True or 1 - False
 */
int connectedQ(LCT t, int r, int v) {
	reRoot(t, r);
	access(t, v);
	/* TODO */
	return 0;
}

int main() {

	char command;
	int arg1, arg2, size;

	scanf("%d\n", &size);
	LCT t = allocLCT(size);

	while (scanf("%c %d %d\n", &command, &arg1, &arg2) != EOF) {
		switch (command) {
			case 'L':
				link(t, arg1, arg2);
				break;
			case 'C':
				cut(t, arg1, arg2);
				break;
			case 'Q':
				printf( (1 == connectedQ(t, arg1, arg2)) ? "F\n" : "T\n");
				break;
			default:
				printf("Unknown command %c", command);
				exit(EXIT_FAILURE);
		}
	}
	return 0;
}