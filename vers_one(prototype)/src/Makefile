CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

GAME=main.c
BACK=brick_game/tetris/tetris.c
FRONT=gui/cli/front.c

NCURSES=-lncurses

RESULT=tetris.out

all: install run

install: $(GAME) $(BACK) $(FRONT)
	$(CC) $(CFLAGS) $^ $(NCURSES) -o $(RESULT)
	echo 0 > max_score.txt

uninstall:

run: 
	./$(RESULT)

test:
gcov_report:
clang_format_n:
clang_format_i:


clean:
	rm -rf *out max_score.txt




	