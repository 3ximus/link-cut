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

void freeLCT(LCT T) {

}


void access(LCT T, int v) {

}

void reRoot(LCT T, int v) {

}


/**
 * MAIN METHODS
 */

void link(LCT T, int r, int v) {

}

void cut(LCT T, int r, int v) {

}

/**
 * Returns 0 - True or 1 - False
 */
int connectedQ(LCT T, int r, int v) {
	return 0;
}

int main() {

	char command;
	int arg1, arg2, size;

	scanf("%d\n", &size);
	LCT T = allocLCT(size);

	while (scanf("%c %d %d\n", &command, &arg1, &arg2) != EOF) {
		switch (command) {
			case 'L':
				link(T, arg1, arg2);
				break;
			case 'C':
				cut(T, arg1, arg2);
				break;
			case 'Q':
				printf( (1 == connectedQ(T, arg1, arg2)) ? "F\n" : "T\n");
				break;
			default:
				printf("Unknown command %c", command);
				exit(EXIT_FAILURE);
		}
	}
	return 0;
}