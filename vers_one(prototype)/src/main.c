#include "brick_game/tetris/tetris.h"
#include "gui/cli/front.h"

int main() {
  struct timespec sp_start, sp_end = {0, 0};
  srand(time(NULL));
  init_gui();
  init_game();

  while (tetg->state != GAMEOVER) {
    clock_gettime(CLOCK_MONOTONIC, &sp_start);
    user_input(get_action(), 0);

    GameInfo_t game_info = update_current_state();

    if (tetg->state == GAMEOVER) {
      free_gui(game_info, tetg->figurest->size, tetg->field->height);
      continue;
    } else
      printGame(game_info, sp_start, sp_end);
  };
  free_game(tetg);

  endwin();

  return 0;
}