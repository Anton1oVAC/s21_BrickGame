#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

typedef enum { INIT, DROP, MOVING, COLLISION, PAUSE, GAMEOVER } GameState;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

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

typedef struct TetFigure {
  int x;
  int y;
  int size;
  TetBlock **blocks;
} TetFigure;

typedef struct TetFiguresT {
  int count;
  int size;
  TetBlock **blocks;
} TetFiguresT;

typedef struct TetField {
  int width;
  int height;
  TetBlock **blocks;
} TetField;

typedef struct TetPlayer {
  int action;
} TetPlayer;

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

void init_game();
Game *create_game(int field_width, int field_height, int figures_size,
                  int count);
TetField *create_field(int width, int height);
TetBlock **create_templates();
TetFiguresT *create_figuresT(int count, int figures_size,
                             TetBlock **figures_template);
TetFigure *create_figure(Game *tetg);
int **create_print_field(int width, int height);
int **create_next_block(int size);
void user_input(UserAction_t action, bool hold);
void drop_new_figure(Game *tetg);
GameInfo_t update_current_state();
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

extern Game *tetg;

// MEMORY FREE
void free_game(Game *tetg);
void free_field(TetField *tetf);
void free_figure(TetFigure *tf);
void free_figuresT(TetFiguresT *tetft);
void free_templates(TetBlock **templates);
void free_print_field(int **print_field, int height);
void free_next_block(int **next, int size);
void free_gui(GameInfo_t game, int size, int height);

#endif