programs := src/main.c src/libs/app/init.c src/libs/utils/shell-scripts.c src/libs/ui/init-ui.c src/libs/ui/menu_navigation.c src/libs/ui/pattern_matching.c src/libs/ui/design.c src/libs/app/pass_api.c src/libs/ui/display.c src/libs/ui/form.c src/libs/utils/structs.c src/libs/utils/string-helper.c
flags := -Wall -Wextra -Wpedantic -lncurses -lmenu -lform

all:
	gcc ${programs} ${flags} -o main -O3
debug:
	gcc ${programs} ${flags} -g -o debug -O0 && gdb ./debug
clear:
	rm -f main vault-dir.txt debug vgcore.*
