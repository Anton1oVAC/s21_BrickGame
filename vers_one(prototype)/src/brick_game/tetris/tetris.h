#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

// Структура для игровых ситуаций
typedef struct Game {
  TetField* field;    // инфорация об поле
  TetFigure* figure;  // информация о падающей фигуре
  TetFiguresT* figurest;  // сведения, как выглядят фигуры
  TetPlayer* player;  // Для взаимодействия с игроком
  TetBlock** tet_templates;  // шаблоны фигур

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

// Набор констант, харак. набор действий игр.
typedef enum {
  Start,      // Старт игры
  Pause,      // Пауза игры
  Terminate,  // Закончить игру
  Left,       // влево
  Right,      // вправо
  Up,         // смена расп. фиг.
  Down,       // вниз
  Action      // Действие
} UserAction_t;

// Структура для блоков
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

// Структура для падающей фигуры
typedef struct TetFigure {
  int x;
  int y;
  int size;
  TetBlock** blocks;  // Массив из блоков соответствующей фигуры
} TetFigure;

// Структура для описания шаблона фигуры
typedef struct TetFiguresT {
  int count;
  int size;
  TetBlock* blocks;  // переменная ссылается на непрерывную область памяти
                     // информации о блоках всех шаблонов
} TetFiguresT;

// структура игрового поля
typedef struct TetField {
  int width;
  int height;
  TetBlock** blocks;  // Пер.указ. для хранения массива из блоков
} TetField;

// Структура опис. действия игрока
typedef struct TetPlayer {
  int action;
} TetPlayer;

// Набор констант, для возможных состояний игры
typedef enum { INIT, DROP, MOVING, COLLISION, PAUSE, GAMEOVER } GameState;

TetFiguresT* create_figuresT(int count, int figures_size,
                             TetBlock* figures_template);
void freeTetFiguresT(TetFiguresT* tetft);
TetField* create_field(int width, int height);
void free_field(TetField* tetf);
Game* create_game(
    int field_width, int field_height, int figures_size, int count,
    TetBlock* figures_template);  // Функция описания игровой структуры.
// Размер игрового поля, размер фигур, кол-во
// фигур, сведения шаблон фигур
int** create_next_block(int size);
void free_next_block(int** next, int size);
void free_gui(GameInfo_t game, int size, int height);
void free_game(Game* tetg);
TetBlock** create_templates();
void user_шnput(UserAction_t action, bool hold);
void move_figur_down(Game* tetg);
void move_figure_up(Game* tetg);
void move_figur_left(Game* tetg);
void move_figure_right(Game* tetg);
int collision(Game* tetg);
void handle_rotation(Game* tetg);
void plant_figure(Game* tetg);
int line_filled(int i, TetField* tfl);
void drop_line(int i, TetField* tfl);
int erase_lines(Game* tetg);
TetFigure* create_figure(Game* tetg);
int** create_print_field(int width, int height);
void free_print_field(int** print_field, int height);
void free_figure(TetFigure* tf);
void drop_new_figure(Game* tetg);
TetFigure* turn_figure(Game* tetg);
void calculate(Game* tetg);  // просчет одного такта
void save_score(int high_score);
int load_score();

#endif