#include "tetris.h"

// Figures
TetBlock oFigure[5][5] = {{{0}, {0}, {0}, {0}, {0}},
                          {{0}, {1}, {1}, {0}, {0}},
                          {{0}, {1}, {1}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}}};
TetBlock iFigure[5][5] = {{{0}, {0}, {1}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}}};
TetBlock tFigure[5][5] = {{{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {1}, {1}, {1}, {0}},
                          {{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}}};
TetBlock sFigure[5][5] = {{{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {1}, {1}, {0}},
                          {{0}, {1}, {1}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}}};
TetBlock zFigure[5][5] = {{{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}},
                          {{0}, {1}, {1}, {0}, {0}},
                          {{0}, {0}, {1}, {1}, {0}},
                          {{0}, {0}, {0}, {0}, {0}}};
TetBlock jFigure[5][5] = {{{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {1}, {1}, {0}, {0}},
                          {{0}, {0}, {0}, {0}, {0}}};
TetBlock lFigure[5][5] = {{{0}, {0}, {0}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {0}, {1}, {0}, {0}},
                          {{0}, {0}, {1}, {1}, {0}},
                          {{0}, {0}, {0}, {0}, {0}}};

TetBlock **create_templates() {
  TetBlock **tet_templates = malloc(7 * sizeof(TetBlock *));
  tet_templates[0] = &iFigure[0][0];
  tet_templates[1] = &oFigure[0][0];
  tet_templates[2] = &tFigure[0][0];
  tet_templates[3] = &sFigure[0][0];
  tet_templates[4] = &zFigure[0][0];
  tet_templates[5] = &jFigure[0][0];
  tet_templates[6] = &lFigure[0][0];
  return tet_templates;
}

Game *tetg;

GameInfo_t update_current_state() {
  GameInfo_t game_info = {0};
  calculate(tetg);

  if (tetg->state != GAMEOVER) {
    if (game_info.field != NULL)
      free_print_field(game_info.field, tetg->field->height);
    game_info.field =
        create_print_field(tetg->field->width, tetg->field->height);

    if (game_info.next != NULL)
      free_next_block(game_info.next, tetg->figurest->size);

    game_info.next = create_next_block(tetg->figurest->size);

    game_info.score = tetg->score;
    game_info.high_score = tetg->high_score;
    game_info.level = tetg->level;
    game_info.speed = tetg->speed;
    game_info.pause = tetg->pause;
  }
  return game_info;
}

Game *create_game(int field_width, int field_height, int figures_size,
                  int count) {
  Game *tetg = (Game *)malloc(sizeof(Game));
  tetg->field = create_field(field_width, field_height);
  tetg->tet_templates = create_templates();
  tetg->figurest = create_figuresT(count, figures_size, tetg->tet_templates);

  tetg->score = 0;
  tetg->high_score = load_score();
  tetg->ticks = 30;
  tetg->ticks_left = 30;
  tetg->speed = 1;
  tetg->level = 1;

  tetg->pause = 1;
  tetg->state = INIT;

  tetg->next = rand() % tetg->figurest->count;

  return tetg;
}

void init_game() {
  tetg = create_game(10, 20, 5, 7);

  TetPlayer *player = (TetPlayer *)malloc(sizeof(TetPlayer));
  player->action = Start;
  tetg->player = player;
  drop_new_figure(tetg);
}

TetField *create_field(int width, int height) {
  TetField *tetf = (TetField *)malloc(sizeof(TetField));
  tetf->width = width;
  tetf->height = height;
  tetf->blocks = (TetBlock **)malloc(sizeof(TetBlock *) * height);
  for (int i = 0; i < height; i++) {
    tetf->blocks[i] = (TetBlock *)malloc(sizeof(TetBlock) * width);
    for (int j = 0; j < width; j++) {
      tetf->blocks[i][j].b = 0;
    }
  }

  return tetf;
}
void free_field(TetField *tetf) {
  if (tetf) {
    for (int i = 0; i < tetf->height; i++) free(tetf->blocks[i]);
    free(tetf->blocks);
    free(tetf);
  }
}

TetFiguresT *create_figuresT(int count, int figures_size,
                             TetBlock **figures_template) {
  TetFiguresT *tetft = (TetFiguresT *)malloc(sizeof(TetFiguresT));
  tetft->count = count;
  tetft->size = figures_size;
  tetft->blocks = figures_template;

  return tetft;
}
void free_figuresT(TetFiguresT *tetft) {
  if (tetft) free(tetft);
}

void free_figure(TetFigure *tf) {
  if (tf) {
    if (tf->blocks) {
      for (int i = 0; i < tf->size; i++) {
        if (tf->blocks[i]) {
          free(tf->blocks[i]);
        }
      }
      free(tf->blocks);
    }
    free(tf);
  }
}

// Освобождение памяти шаблонов
void free_templates(TetBlock **templates) {
  if (templates) free(templates);
}

// Создание фигуры
TetFigure *create_figure(Game *tetg) {
  TetFigure *figure = (TetFigure *)malloc(sizeof(TetFigure));
  figure->x = 0;
  figure->y = 0;
  figure->size = tetg->figurest->size;
  figure->blocks = (TetBlock **)malloc(sizeof(TetBlock *) * figure->size);
  for (int i = 0; i < figure->size; i++) {
    figure->blocks[i] = (TetBlock *)malloc(sizeof(TetBlock) * figure->size);
    for (int j = 0; j < figure->size; j++) {
      figure->blocks[i][j].b = 0;
    }
  }
  return figure;
}

// Создать и вывести фигуру
int **create_print_field(int width, int height) {
  int **print_field = (int **)malloc(height * sizeof(int *));
  for (int i = 0; i < height; i++) {
    print_field[i] = (int *)malloc(width * sizeof(int));
  }

  TetField *field = tetg->field;
  TetFigure *figure = tetg->figure;

  for (int i = 0; i < field->height; i++) {
    for (int j = 0; j < field->width; j++) {
      int sym = 0;
      if (field->blocks[i][j].b != 0)
        sym = 1;
      else {
        int x = j - figure->x;
        int y = i - figure->y;

        if (x >= 0 && x < figure->size && y >= 0 && y < figure->size)
          if (figure->blocks[y][x].b != 0) sym = 1;
      }
      print_field[i][j] = sym;
    }
  }
  return print_field;
}
// освобождение памяти
void free_print_field(int **print_field, int height) {
  if (print_field) {
    for (int i = 0; i < height; i++) {
      if (print_field[i]) free(print_field[i]);
    }
    free(print_field);
  }
}

// Создание следующей фигуры
int **create_next_block(int size) {
  int **next = (int **)malloc(size * sizeof(int *));
  for (int i = 0; i < size; i++) {
    next[i] = (int *)malloc(size * sizeof(int));
    for (int j = 0; j < tetg->figurest->size; j++) {
      next[i][j] =
          tetg->tet_templates[tetg->next][i * tetg->figurest->size + j].b;
    }
  }
  return next;
}
// Освобождение памяти новой фигуры
void free_next_block(int **next, int size) {
  if (next) {
    for (int i = 0; i < size; i++) free(next[i]);
    free(next);
  }
}

void free_gui(GameInfo_t game, int size, int height) {
  free_print_field(game.field, height);
  free_next_block(game.next, size);
}

void free_game(Game *tetg) {
  if (tetg) {
    if (tetg->figure != NULL) free_figure(tetg->figure);
    free_field(tetg->field);
    free_figuresT(tetg->figurest);
    free_templates(tetg->tet_templates);
    free(tetg->player);
    free(tetg);
  }
}

void user_input(UserAction_t action, bool hold) {
  if (!hold) {
    switch (action) {
      case Left:
        tetg->player->action = Left;
        break;
      case Right:
        tetg->player->action = Right;
        break;
      case Up:
        tetg->player->action = Up;
        break;
      case Down:
        tetg->player->action = Down;
        break;
      case Start:
        tetg->player->action = Start;
        break;
      case Pause:
        tetg->player->action = Pause;
        break;
      case Terminate:
        tetg->player->action = Terminate;
        break;
      default:
        tetg->player->action = Action;
        break;
    }
  }
}

void drop_new_figure(Game *tetg) {
  tetg->figure = create_figure(tetg);
  TetFigure *figure = create_figure(tetg);
  figure->x = tetg->field->width / 2 - figure->size / 2;
  figure->y = 0;
  int fnum = tetg->next;

  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      figure->blocks[i][j].b =
          tetg->figurest->blocks[fnum][i * figure->size + j].b;
  if (tetg->figure != NULL) free_figure(tetg->figure);
  tetg->figure = figure;

  fnum = rand() % tetg->figurest->count;
  tetg->next = fnum;
}

void calculate(Game *tetg) {
  if (tetg->ticks_left <= 0 && tetg->state != PAUSE && tetg->state != INIT)
    calculate_tet(tetg);
  if (tetg->state == GAMEOVER) return;

  switch (tetg->player->action) {
    case Right:
      if (tetg->pause) break;
      move_figure_right(tetg);
      if (collision(tetg)) move_figure_left(tetg);
      break;
    case Left:
      if (tetg->pause) break;
      move_figure_left(tetg);
      if (collision(tetg)) move_figure_right(tetg);
      break;
    case Down:
      if (tetg->pause) break;
      move_figure_down(tetg);
      if (collision(tetg)) move_figure_up(tetg);
      break;
    case Up: {
      if (tetg->pause) break;
      handle_rotation(tetg);
    } break;
    case Pause:
      if (tetg->pause) {
        tetg->pause = 0;
        tetg->state = MOVING;
      } else {
        tetg->pause = 1;
        tetg->state = PAUSE;
      }
      break;
    case Terminate:
      tetg->state = GAMEOVER;
      break;
    case Start:
      tetg->pause = 0;
      tetg->state = MOVING;
      break;
    default:
      break;
  }

  tetg->ticks_left--;
}

void calculate_tet(Game *tetg) {
  tetg->ticks_left = tetg->ticks;
  move_figure_down(tetg);
  tetg->state = MOVING;
  if (collision(tetg)) {
    move_figure_up(tetg);
    plant_figure(tetg);
    score(tetg);
    if (tetg->figure != NULL) free_figure(tetg->figure);
    drop_new_figure(tetg);
    tetg->state = DROP;
    if (collision(tetg)) {
      tetg->state = GAMEOVER;
    }
  }
}

void move_figure_down(Game *tetg) { tetg->figure->y++; }

void move_figure_up(Game *tetg) { tetg->figure->y--; }

void move_figure_right(Game *tetg) { tetg->figure->x++; }

void move_figure_left(Game *tetg) { tetg->figure->x--; }

int collision(Game *tetg) {
  TetFigure *figure = tetg->figure;
  TetField *field = tetg->field;

  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++) {
      if (figure->blocks[i][j].b != 0) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (fx < 0 || fx >= field->width || fy < 0 || fy >= field->height) {
          tetg->state = COLLISION;
          return 1;
        }
        if (field->blocks[fy][fx].b != 0) {
          tetg->state = COLLISION;
          return 1;
        }
      }
    }
  return 0;
}

int erase_lines(Game *tetg) {
  TetField *tfl = tetg->field;
  int count = 0;
  for (int i = tfl->height - 1; i >= 0; i--) {
    while (line_filled(i, tfl)) {
      drop_line(i, tfl);
      count++;
    }
  }
  return count;
}

int line_filled(int i, TetField *tfl) {
  for (int j = 0; j < tfl->width; j++)
    if (tfl->blocks[i][j].b == 0) return 0;
  return 1;
}

void drop_line(int i, TetField *tfl) {
  if (i == 0)
    for (int j = 0; j < tfl->width; j++) tfl->blocks[i][j].b = 0;
  else {
    for (int k = i; k > 0; k--)
      for (int j = 0; j < tfl->width; j++)
        tfl->blocks[k][j].b = tfl->blocks[k - 1][j].b;
  }
}

// поворот фигуры
TetFigure *turn_figure(Game *tetg) {
  TetFigure *figure = create_figure(tetg);
  TetFigure *old_figure = tetg->figure;
  figure->x = old_figure->x;
  figure->y = old_figure->y;
  int size = figure->size;

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      figure->blocks[i][j].b = old_figure->blocks[j][size - 1 - i].b;
  return figure;
}

void handle_rotation(Game *tetg) {
  TetFigure *t = turn_figure(tetg);
  TetFigure *told = tetg->figure;
  tetg->figure = t;
  if (collision(tetg)) {
    tetg->figure = told;
    free_figure(t);
  } else
    free_figure(told);
}

// Функция размещения фигуры после падения
void plant_figure(Game *tetg) {
  TetFigure *figure = tetg->figure;
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      if (figure->blocks[i][j].b != 0) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (fx >= 0 && fx < tetg->field->width && fy >= 0 &&
            fy < tetg->field->height) {
          tetg->field->blocks[fy][fx].b = figure->blocks[i][j].b;
        }
      }
}

void score(Game *tetg) {
  int erased_lines = erase_lines(tetg);
  switch (erased_lines) {
    case 0:
      break;
    case 1:
      tetg->score += 100;
      break;
    case 2:
      tetg->score += 300;
      break;
    case 3:
      tetg->score += 700;
      break;
    default:
      tetg->score += 1500;
      break;
  }
  if (tetg->score > tetg->high_score) {
    tetg->high_score = tetg->score;
    save_score(tetg->high_score);
  }

  int new_level = tetg->score / 600 + 1;
  if (new_level > tetg->level && new_level <= 10) {
    tetg->level = new_level;
    tetg->speed = new_level;
  }
}

void save_score(int high_score) {
  FILE *file = fopen("max_score.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

int load_score() {
  int high_score = 0;
  FILE *file = fopen("max_score.txt", "r");
  if (file != NULL) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}
