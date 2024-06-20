#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Конст для кол-ва тактов между момен. падения на один блок
#define TET_TICKS_START 30

// Структура для блоков
typedef struct TetBlock {
  int b;
} TetBlock;

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
typedef struct TetField {
  int width;
  int height;
  TetBlock* blocks;  // Пер.указ. для хранения массива из блоков
} TetField;

// Набор констант, для возможных состояний игры
enum {
  TET_GAMEOVER = 0,
  TET_PLAYING,
};

// Набор констант, харак. набор действий игр.
enum {
  TET_PLAYER_NOP = 0,
  TET_PLAYER_UP,         // смена расп. фиг.
  TET_PLAYER_DOWN,       // вниз
  TET_PLAYER_LEFT,       // влево
  TET_PLAYER_RIGHT,      // вправо
  TET_PLAYER_START,      // Старт игры (Не сделана)
  TET_PLAYER_PAUSE,      // Пауза игры (Не сделана)
  TET_PLAYER_TERMINATE,  // Закончить игру (Не сделана)
};

// Структура опис. действия игрока
typedef struct TetPlayer {
  int action;
} TetPlayer;

// Структура для игровых ситуаций
typedef struct TetGame {
  TetField* field;    // инфорация об поле
  TetFigure* figure;  // информация о падающей фигуре
  TetFiguresT* figurest;  // сведения, как выглядят фигуры
  TetPlayer* player;  // Для взаимодействия с игроком               //  ???
  int ticks;
  int ticks_left;
  int playing;
  int score;      // очки
  int highscore;  // рекорд
} TetGame;

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
int add_score(TetGame* tetg);
void read_highscore(TetGame* tetg);
void write_highscore(TetGame* tetg);
TetFigure* createTetFigure(TetGame* tetg);
void freeTetFigure(TetFigure* tf);
void dropNewFigure(TetGame* tetg);
TetFigure* turnTetFigure(TetGame* tetg);
void calculateTet(TetGame* tetg);  // просчет одного такта

#endif