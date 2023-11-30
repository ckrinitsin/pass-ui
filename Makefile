programs := src/main.cpp src/libs/app/init.cpp src/libs/utils/shell-scripts.cpp src/libs/ui/init-ui.cpp
flags := -std=c++20 -Wall -Wextra -Wpedantic -lncurses -lmenu

all:
	g++ ${programs} ${flags} -o main -O3
debug:
	g++ ${programs} ${flags} -g -o debug -O0 && gdb ./debug
clear:
	rm -f main vault-dir.txt debug vgcore.*
