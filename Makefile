programs := src/main.c src/libs/init.c src/libs/shell-scripts.c 
flags := -O3 -Wall -Wextra -Wpedantic -lncurses

all:
	gcc ${programs} ${flags} -o main 
debug:
	gcc ${programs} ${flags} -g -o debug && gdb ./debug
clear:
	rm -f main vault-dir.txt debug
