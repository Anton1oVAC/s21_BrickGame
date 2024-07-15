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

Suite* test() {
  Suite* suite = suite_create("TEST");

  TCase* tcase_score = tcase_create("SCORE");
  tcase_add_test(tcase_score, test_score);
  suite_add_tcase(suite, tcase_score);

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