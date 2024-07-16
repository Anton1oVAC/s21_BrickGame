CC=gcc
#CFLAGS=-std=c11 -Wall -Werror -Wextra

GAME=main.c
BACK=brick_game/tetris/tetris.c
FRONT=gui/cli/front.c
RESULT=tetris.out

NCURSES=-lncurses
TEST_FLAGS=-lcheck -lm -lpthread
GCOV_FLAGS= -fprofile-arcs -ftest-coverage
TEST_PARTS=$(wildcard test.c)
DIST_DIR=archive

OPEN =open
ifeq ($(OS), Linux)
	OPEN=xdg-open
else
	OPEN=open
endif

all: install run

install: $(GAME) $(BACK) $(FRONT)
	$(CC) $(CFLAGS) $^ $(NCURSES) -o $(RESULT)
	echo 0 > max_score.txt

uninstall:
	rm $(RESULT)

run: 
	./$(RESULT)

test:
	$(CC) $(TEST_PARTS) $(BACK) -o ./test.out $(TEST_FLAGS)
	./test.out

gcov_report:
	$(CC) $(TEST_PARTS) $(BACK) $(TEST_FLAGS) $(GCOV_FLAGS) -o ./gcov_report.out
	./gcov_report.out
	lcov -t "test" -o test.info -c -d .
	genhtml -o report/ test.info
	${OPEN} ./report/index.html
	make clean_gcov

dvi:
	$(OPEN) ../dvi/README.html

dist: clean_dist
	cd ../ && mkdir -p ${DIST_DIR}
	cd ../ && cp -rf src/brick_game ${DIST_DIR}/brick_game
	cd ../ && cp -rf src/gui ${DIST_DIR}/gui
	cd ../ && cp -rf src/Makefile ${DIST_DIR}/
	cd ../ && tar -czvf archive.tar.gz ${DIST_DIR}
	cd ../ && rm -rf ${DIST_DIR}

dist_unpack:
	cd ../ && tar -xzvf archive.tar.gz

clang_n:
	find .. -type f -name "*.c" -exec clang-format -n -style=Google {} \;
	find .. -type f -name "*.h" -exec clang-format -n -style=Google {} \;

clang_i:
	find .. -type f -name "*.c" -exec clang-format -i -style=Google {} \;
	find .. -type f -name "*.h" -exec clang-format -i -style=Google {} \;


clean:
	rm -rf *out max_score.txt *gcda *gcno *info ./report

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz




	