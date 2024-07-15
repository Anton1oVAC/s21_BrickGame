#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Перечисление действий пользователя, таких как перемещение фигур, пауза,
// начало и окончание игры.
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

// КА. Перечисление состояний игры, включая начальное состояние, паузу, конец
// игры
typedef enum { INIT, DROP, MOVING, COLLISION, PAUSE, GAMEOVER } GameState;

// Структура, содержащая информацию о текущем состоянии игры, \
включая игровое поле, следующую фигуру, счет, рекорд, уровень, скорость и состояние паузы.
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

// Структура, представляющая блок фигуры в Тетрисе, для идентификации блока.
typedef struct TetBlock {
  int b;
} TetBlock;

extern TetBlock oFigure[5][5];
extern TetBlock iFigure[5][5];
extern TetBlock tFigure[5][5];
extern TetBlock sFigure[5][5];
extern TetBlock zFigure[5][5];
extern TetBlock jFigure[5][5];
extern TetBlock lFigure[5][5];

//Структура, представляющая фигуру в игре. \
Она содержит позицию фигуры, ее размер и ссылку на блоки, составляющие фигуру.
typedef struct TetFigure {
  int x;
  int y;
  int size;
  TetBlock **blocks;
} TetFigure;

// Структура, содержащая список всех фигур, доступных в игре.
typedef struct TetFiguresT {
  int count;
  int size;
  TetBlock **blocks;
} TetFiguresT;

// Структура, представляющая игровое поле. Она содержит ширину и высоту поля и ссылку на блоки, \
занимающие место на поле.
typedef struct TetField {
  int width;
  int height;
  TetBlock **blocks;
} TetField;

// Структура, содержащая текущее действие игрока.
typedef struct TetPlayer {
  int action;
} TetPlayer;

//Главная структура игры, включающая все вышеупомянутые компоненты \
и дополнительную информацию о текущем состоянии игры.
typedef struct Game {
  TetField *field;
  TetFigure *figure;
  TetFiguresT *figurest;
  TetPlayer *player;
  TetBlock **tet_templates;

  int score;
  int high_score;
  int ticks_left;
  int ticks;
  int speed;
  int level;
  int next;

  int pause;
  int state;

} Game;

TetBlock **create_templates();
void free_templates(TetBlock **templates);

// Объявление глобальной переменной указателя на структуру Game, \
которая будет использоваться для хранения состояния игры.
extern Game *tetg;
GameInfo_t update_current_state();

Game *create_game(int field_width, int field_height, int figures_size,
                  int count);

void init_game();

TetField *create_field(int width, int height);
void free_field(TetField *tetf);
TetFiguresT *create_figuresT(int count, int figures_size,
                             TetBlock **figures_template);
void free_figuresT(TetFiguresT *tetft);
TetFigure *create_figure(Game *tetg);
void free_figure(TetFigure *tf);
int **create_print_field(int width, int height);
void free_print_field(int **print_field, int height);
int **create_next_block(int size);
void free_next_block(int **next, int size);
void free_gui(GameInfo_t game, int size, int height);
void free_game(Game *tetg);
void drop_new_figure(Game *tetg);
void user_input(UserAction_t action, bool hold);
void calculate(Game *tetg);
void calculate_tet(Game *tetg);
void move_figure_down(Game *tetg);
void move_figure_up(Game *tetg);
void move_figure_right(Game *tetg);
void move_figure_left(Game *tetg);
int collision(Game *tetg);
int erase_lines(Game *tetg);
int line_filled(int i, TetField *tfl);
void drop_line(int i, TetField *tfl);
TetFigure *turn_figure(Game *tetg);
void handle_rotation(Game *tetg);
void plant_figure(Game *tetg);
void score(Game *tetg);
void save_score(int high_score);
int load_score();

#endif