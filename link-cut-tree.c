#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct LCT *LCT;

struct LCT {
	LCT left;
	LCT right;
	LCT* hook;
	int sum; /* sub-tree size, negative values mean left and right must be swamped */
};

LCT allocLCT(int size) {
	return (LCT)malloc(sizeof(LCT)*size);

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