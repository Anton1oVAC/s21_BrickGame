#ifndef FRONT_H
#define FRONT_H

#include "brick_game/tetris/tetris.h"

void init_gui();
void printGame(GameInfo_t game, struct timespec sp_start,
               struct timespec sp_end);
void print_field(GameInfo_t game);
void print_next_figure(GameInfo_t game);
void print_info(GameInfo_t game);
UserAction_t get_action();
void handle_delay(struct timespec sp_start, struct timespec sp_end,
                  int game_speed);

#endif