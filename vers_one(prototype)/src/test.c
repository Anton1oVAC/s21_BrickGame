#include <check.h>

#include "brick_game/tetris/tetris.h"

// ##### SCORE
START_TEST(test_score) {
  init_game();
  tetg->pause = 0;
  tetg->high_score = 0;
  tetg->score = 700;

  int line_fill = 18;
  for (int i = 0; i < tetg->field->width; i++) {
    tetg->field->blocks[line_fill][i].b = 1;
  }
  score(tetg);

  ck_assert_int_eq(tetg->score, 800);
  free_game(tetg);
}
END_TEST

// ####### MOVE_FIGURE_UP
START_TEST(test_up_one) {
  init_game();
  tetg->pause = 0;
  user_input(Up, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Up);
  ck_assert_ptr_nonnull(tetg->figure);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

START_TEST(test_up_two) {
  init_game();
  tetg->pause = 0;
  for (int i = 0; i < tetg->field->width; i++) {
    tetg->field->blocks[2][i].b = 1;
  }
  user_input(Up, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Up);
  ck_assert_ptr_nonnull(tetg->figure);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

// ###### PLANT_FIGURE
START_TEST(test_plant_figure) {
  init_game();
  tetg->pause = 0;
  plant_figure(tetg);
  ck_assert_ptr_nonnull(tetg->figure);
  free_game(tetg);
}
END_TEST

// ######## USER_INPUT
START_TEST(test_user_input_one) {
  init_game();
  tetg->pause = 1;
  user_input(Left, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Left);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

START_TEST(test_user_input_two) {
  init_game();
  tetg->pause = 0;
  user_input(Right, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Right);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

START_TEST(test_user_input_three) {
  init_game();
  tetg->pause = 0;
  user_input(Down, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Down);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

START_TEST(test_user_input_four) {
  init_game();
  tetg->pause = 0;
  user_input(Down, 0);
  for (int i = 0; i < tetg->field->width; i++) {
    tetg->field->blocks[1][i].b = 1;
  }
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Down);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

START_TEST(test_user_input_five) {
  init_game();
  user_input(Pause, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Pause);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

START_TEST(test_user_input_six) {
  init_game();
  user_input(Terminate, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Terminate);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

START_TEST(test_user_input_seven) {
  init_game();
  user_input(Start, 0);
  GameInfo_t game_info = update_current_state();
  ck_assert_int_eq(tetg->player->action, Start);
  free_gui(game_info, tetg->figurest->size, tetg->field->height);
  free_game(tetg);
}
END_TEST

// ######## CALCULATE_TET
START_TEST(test_calculate_one) {
  init_game();
  tetg->ticks = 1;
  tetg->ticks_left = 0;
  tetg->pause = 0;
  calculate_tet(tetg);
  ck_assert_int_eq(tetg->ticks_left, tetg->ticks);
  free_game(tetg);
}
END_TEST

START_TEST(test_calculate_two) {
  init_game();
  tetg->ticks_left = 0;
  tetg->pause = 1;
  for (int i = 0; i < tetg->field->width; i++) {
    tetg->field->blocks[1][i].b = 1;
  }
  calculate_tet(tetg);
  ck_assert_int_eq(tetg->ticks_left, tetg->ticks);
  free_game(tetg);
}
END_TEST

// ###### CREATE_GAME
START_TEST(test_create_game) {
  init_game();

  ck_assert_ptr_nonnull(tetg);
  ck_assert_ptr_nonnull(tetg->player);
  ck_assert_int_eq(tetg->player->action, Start);
  ck_assert_ptr_nonnull(tetg->figure);
  free_game(tetg);
}
END_TEST

// ######### ERASE_LINES
START_TEST(test_erase_lines) {
  init_game();
  int line_erase = 18;
  for (int i = 0; i < tetg->field->width; i++) {
    tetg->field->blocks[line_erase][i].b = 1;
  }
  int erased = erase_lines(tetg);
  ck_assert_int_eq(erased, 1);
  free_game(tetg);
}
END_TEST

Suite* test() {
  Suite* suite = suite_create("TEST");

  TCase* tcase_score = tcase_create("SCORE");
  tcase_add_test(tcase_score, test_score);
  suite_add_tcase(suite, tcase_score);

  TCase* tcase_up = tcase_create("MOVE_FIGURE_UP");
  tcase_add_test(tcase_up, test_up_one);
  tcase_add_test(tcase_up, test_up_two);
  suite_add_tcase(suite, tcase_up);

  TCase* tcase_palnt = tcase_create("PLANT_FIGURE");
  tcase_add_test(tcase_palnt, test_plant_figure);
  suite_add_tcase(suite, tcase_palnt);

  TCase* tcase_input = tcase_create("USER_INPUT");
  tcase_add_test(tcase_input, test_user_input_one);
  tcase_add_test(tcase_input, test_user_input_two);
  tcase_add_test(tcase_input, test_user_input_three);
  tcase_add_test(tcase_input, test_user_input_four);
  tcase_add_test(tcase_input, test_user_input_five);
  tcase_add_test(tcase_input, test_user_input_six);
  tcase_add_test(tcase_input, test_user_input_seven);
  suite_add_tcase(suite, tcase_input);

  TCase* tcase_calculate_tet = tcase_create("CALCULATE_TET");
  tcase_add_test(tcase_calculate_tet, test_calculate_one);
  tcase_add_test(tcase_calculate_tet, test_calculate_two);
  suite_add_tcase(suite, tcase_calculate_tet);

  TCase* tcase_create_game = tcase_create("CREATE_GAME");
  tcase_add_test(tcase_create_game, test_create_game);
  suite_add_tcase(suite, tcase_create_game);

  TCase* tcase_erase_lines = tcase_create("ERASE_LINES");
  tcase_add_test(tcase_erase_lines, test_erase_lines);
  suite_add_tcase(suite, tcase_erase_lines);

  return suite;
}

int main() {
  int count;
  Suite* s = test();
  SRunner* runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  count = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}