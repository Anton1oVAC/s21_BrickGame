CC=gcc
#CFLAGS=-std=c11 -Wall -Werror -Wextra

GAME=main.c
BACK=brick_game/tetris/tetris.c
FRONT=gui/cli/front.c
RESULT=tetris.out

NCURSES_FLAGS=-lncurses
TEST_FLAGS=-lcheck -lm -lpthread
TEST_PARTS=$(wildcard test.c)

all: install run

install: $(GAME) $(BACK) $(FRONT)
	$(CC) $(CFLAGS) $^ $(NCURSES) -o $(RESULT)
	echo 0 > max_score.txt

uninstall:

run: 
	./$(RESULT)

test:
	$(CC) $(TEST_PARTS) $(BACK) -o ./test.out $(TEST_FLAGS)
	./test.out

gcov_report:
clang_format_n:
clang_format_i:


clean:
	rm -rf *out max_score.txt




	