#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Конст для кол-ва тактов между момен. падения на один блок
#define TET_TICKS_START 30
#define MAX_LEVEL 10
#define NEW_LEVEL_SCORE 600

// Структура для блоков
typedef struct TetBlock {
  int b;
} TetBlock;

// extern Block oFigure[5][5];
// extern Block iFigure[5][5];
// extern Block tFigure[5][5];
// extern Block sFigure[5][5];
// extern Block zFigure[5][5];
// extern Block jFigure[5][5];
// extern Block lFigure[5][5];

// Структура для падающей фигуры
typedef struct TetFigure {
  int x;
  int y;
  int size;
  TetBlock* blocks;  // Массив из блоков соответствующей фигуры
} TetFigure;

// Структура для описания шаблона фигуры
typedef struct TetFiguresT {
  int count;
  int size;
  TetBlock* blocks;  // переменная ссылается на непрерывную область памяти
                     // информации о блоках всех шаблонов
} TetFiguresT;

// структура игрового поля
typedef struct Field {
  int width;
  int height;
  TetBlock **blocks; // Пер.указ. для хранения массива из блоков
} Field;

// Набор констант, для возможных состояний игры
enum {
  TET_GAMEOVER = 0,
  TET_PLAYING,
};

// Набор констант, харак. набор действий игр.
typedef enum {
  Start, // Старт игры
  Pause,  // Пауза игры
  Terminate, // Закончить игру
  Left,  // влево
  Right, // вправо
  Up, // смена расп. фиг.
  Down, // вниз
  Action // Действие
} UserAction_t;


// Структура опис. действия игрока
typedef struct TetPlayer {
  int action;
} TetPlayer;


// Структура для игровых ситуаций
typedef struct Game {
  Field *field; // инфорация об поле
  Figure *figure; // информация о падающей фигуре
  FiguresT *figurest;  // сведения, как выглядят фигуры
  Player *player; // Для взаимодействия с игроком    
  Block **tet_templates; // шаблоны фигур

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

TetFiguresT* createTetFiguresT(int count, int figures_size,
                               TetBlock* figures_template);
void freeTetFiguresT(TetFiguresT* tetft);
TetField* createTetField(int width, int height);
void freeTetField(TetField* tetf);
TetGame* createTetGame(
    int field_width, int field_height, int figures_size, int count,
    TetBlock* figures_template);  // Функция описания игровой структуры.
                                  // Размер игрового поля, размер фигур, кол-во
                                  // фигур, сведения шаблон фигур
void freeTetGame(TetGame* tetg);
void moveFigureDown(TetGame* tetg);
void moveFigureUp(TetGame* tetg);
void moveFigureLeft(TetGame* tetg);
void moveFigureRight(TetGame* tetg);
int collisionTet(TetGame* tetg);
void plantFigure(TetGame* tetg);
int lineFilledTet(int i, TetField* tfl);
void dropLineTet(int i, TetField* tfl);
int eraseLinesTet(TetGame* tetg);
TetFigure* createTetFigure(TetGame* tetg);
void freeTetFigure(TetFigure* tf);
void dropNewFigure(TetGame* tetg);
TetFigure* turnTetFigure(TetGame* tetg);
void calculateTet(TetGame* tetg);  // просчет одного такта
void save_max_score(TetGame* tetg);
void update_level(TetGame* tetg);

#endif