CC=gcc
FLAGS=-O3 -ansi -Wall -g
# FLAGS=-Wall -Wextra -ggdb

all: link-cut-tree.c
	$(CC) $(FLAGS) link-cut-tree.c -o project

clean:
	if [[ -e project ]]; then rm project; fi;
