#include "tetris.h"

Game* tetg;

TetBlock** create_templates() {
  TetBlock** tet_templates = malloc(7 * sizeof(TetBlock*));
  tet_templates[0] = &iFigure[0][0];
  tet_templates[1] = &oFigure[0][0];
  tet_templates[2] = &tFigure[0][0];
  tet_templates[3] = &sFigure[0][0];
  tet_templates[4] = &zFigure[0][0];
  tet_templates[5] = &jFigure[0][0];
  tet_templates[6] = &lFigure[0][0];
  return tet_templates;
}

TetFiguresT* create_figuresT(int count, int figures_size,
                             TetBlock* figures_template) {
  // Алгоритм инициализации набора шаблонов

  TetFiguresT* tetft = (TetFiguresT*)malloc(sizeof(TetFiguresT));
  // Память под шаблон фигур

  tetft->count = count;
  tetft->size = figures_size;
  tetft->blocks = figures_template;

  return tetft;
}

void freeTetFiguresT(TetFiguresT* tetft) {
  if (tetft) free(tetft);
}

TetField* create_field(int width, int height) {  // Память для игрового поля
  TetField* tetf = (TetField*)malloc(sizeof(TetField));
  tetf->width = width;
  tetf->height = height;

  tetf->blocks = (TetBlock**)malloc(sizeof(TetBlock*) * height);
  // Память об информации о блоках (объем=структура*шир*выс)
  for (int i = 0; i < height; i++) {
    tetf->blocks[i] = (TetBlock*)malloc(sizeof(TetBlock) * width);
    for (int j = 0; j < width; j++) {
      tetf->blocks[i][j].b = 0;
    }
  }

  return tetf;
}

void free_field(TetField* tetf) {
  if (tetf) {
    if (tetf->blocks) {
      free(tetf->blocks);
    }
    free(tetf);
  }
}

Game* create_game(int field_width, int field_height, int figures_size,
                  int count, TetBlock* figures_template) {
  // Выделение памяти для основной структуры игры
  Game* tetg = (Game*)malloc(sizeof(Game));
  // Для поля
  tetg->field = createTetField(field_width, field_height);
  tetg->tet_templates = create_templates();
  // для набора шаблонов
  tetg->figurest = createTetFiguresT(count, figures_size, figures_template);

  // Опред. зн. пер.
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

void free_game(Game* tetg) {
  if (tetg) {
    freeTetField(tetg->field);
    freeTetFiguresT(tetg->figurest);
    free(tetg);
  }
}

// Смещение фигуры вниз
void move_figur_down(Game* tetg) { tetg->figure->y++; }

// Поворот фигуры
void move_figure_up(Game* tetg) { tetg->figure->y--; }

// Движение фигуры вправо
void move_figure_right(Game* tetg) { tetg->figure->x++; }

// Движение фигуры влево
void move_figur_left(Game* tetg) { tetg->figure->x--; }

// Функ. было ли столкновение?
int collision(Game* tetg) {
  // для удобстава объяв. пер.: падающую фигуру и игровое поле
  TetFigure* figure = tetg->figure;
  TetField* field = tetg->field;

  // Если столкн. произошло только в рамках обл. рис. фигуры
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++) {
      // Все двумерные констр. хранятся в одномерных масс., запиш. формулу
      // расчета по друмерным коорд i=y, j=x

      // Если блок фигур не пуст, то рассчит коорд на поле
      if (figure->blocks[i][j].b != 0) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        // Если в том же месте, на поле, есть не пустой блок, зн. произ.
        // столкновение
        if (fx < 0 || fx >= field->width || fy < 0 || fy >= field->height) {
          tetg->state = COLLISION;
          return 1;
        }
        // Столкновение с границами поля
        if (field->blocks[fy][fx].b != 0) {
          tetg->state = COLLISION;
          return 1;
        }
      }
    }
  // Если нет столкновений
  return 0;
}

// Функция размещения фигуры после падения
void plant_figure(Game* tetg) {
  // Перем. на фигуру
  TetFigure* figure = tetg->figure;
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      // Если блок не нулевой, опред. коорд поля соотв. блоку и перенесем
      // его данные на поле
      if (figure->blocks[i][j].b != 0) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (fx >= 0 && fx < tetg->field->width && fy >= 0 &&
            fy < tetg->field->height) {
          tetg->field->blocks[fy][fx].b = figure->blocks[i][j].b;
        }
      }
}

// Функция размещения фигуры после падения
void plant_figure(Game* tetg) {
  // Перем. на фигуру
  TetFigure* figure = tetg->figure;
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      // Если блок не нулевой, опред. коорд поля соотв. блоку и перенесем
      // его данные на поле
      if (figure->blocks[i][j].b != 0) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (fx >= 0 && fx < tetg->field->width && fy >= 0 &&
            fy < tetg->field->height) {
          tetg->field->blocks[fy][fx].b = figure->blocks[i][j].b;
        }
      }
}

// Функция для проверки заполнения стороки
int lineFilledTet(int i, TetField* tfl) {
  for (int j = 0; j < tfl->width; j++) {
    if (tfl->blocks[i * tfl->width + j].b == 0) return 0;
  }
  return 1;
}

// Функция сдвига на одну строку
void drop_line(int i, TetField* tfl) {
  // Если строка нулевая, то очищаем
  if (i == 0)
    for (int j = 0; j < tfl->width; j++) tfl->blocks[i][j].b = 0;
  // Иначе, перенощу блоки верхней строки на строку текущую (индекс
  // текущей и вверхней строки: к и (к-1)). Процесс повторяется для к от
  // текущей позиции до первой, так реализуется сдвиг блоков поля
  else {
    for (int k = i; k > 0; k--)
      for (int j = 0; j < tfl->width; j++)
        tfl->blocks[k][j].b = tfl->blocks[k - 1][j].b;
  }
}

// Функция удаляющая стороки
int erase_lines(Game* tetg) {
  TetField* tfl = tetg->field;
  int count = 0;
  // Удал. стр. нач. с последней, выполняется сдвиг строк вниз
  for (int i = tfl->height - 1; i >= 0; i--) {
    // Пока текущая строка заполнена, удал ее со сдвигом вниз и увел.
    // кол-во удал. стр на 1
    while (lineFilled(i, tfl)) {
      drop_line(i, tfl);
      count++;
    }
  }
  return count;
}

// Функция подсчитывающая кол-во очков на основе удаления
void score(Game* tetg) {
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
  // Условие для рекорда
  if (tetg->score > tetg->high_score) {
    tetg->high_score = tetg->score;
    save_score(tetg->high_score);
  }

  // Условие для для повышения уровня каждые 600 очков
  int new_level = tetg->score / 600 + 1;
  if (new_level > tetg->level && new_level <= 10) {
    tetg->level = new_level;
    tetg->speed = new_level;
  }
}

// Опр. функцию создания и иниц. фигуры
TetFigure* createTetFigure(Game* tetg) {
  // Память для струк. описания фигуры
  TetFigure* t = (TetFigure*)malloc(sizeof(TetFigure));
  // Иниц. коорд нулями
  t->x = 0;
  t->y = 0;
  // Разм. обл. фигуры, что и у шабл.
  t->size = tetg->figurest->size;
  // Выдел. памяти для хранения блоков
  t->blocks = (TetBlock*)malloc(sizeof(TetBlock) * t->size * t->size);
  return t;
}

void freeTetFigure(TetFigure* tf) {
  // Снач. осв. память для блоков, после для структ и описан.
  if (tf) {
    if (tf->blocks) {
      free(tf->blocks);
    }
    free(tf);
  }
}

// Функция выброса новой фигуры
void dropNewFigure(Game* tetg) {
  // Создание нов. фиг. в памяти
  TetFigure* t = createTetFigure(tetg);
  // Координаты: в центре и  в верху
  t->x = tetg->field->width / 2 - t->size / 2;
  t->y = 0;
  // Шаблон фигуры ранд. образом исходя из кол-ва шаблонов
  int fnum = rand() % 7;
  for (int i = 0; i < t->size; i++) {
    for (int j = 0; j < t->size; j++) {
      // Скоп. блоки шабл. в обл. фигуры. Номер шаблона опр. смещение в
      // масс. блоков в соотв. выбранного шаблона
      t->blocks[i * t->size + j].b =
          tetg->figurest->blocks[fnum * t->size * t->size + i * t->size + j].b;
    }
  }
  // Удаление страрой фигуры из памяти. На ее роль назначим только что
  // созданную
  freeTetFigure(tetg->figure);
  tetg->figure = t;
}

// Функ поворота фигуры
TetFigure* turnTetFigure(Game* tetg) {
  // Созд. пуст. фигуру
  TetFigure* t = createTetFigure(tetg);
  // Объяв. указ на старую фигуру
  TetFigure* old = tetg->figure;
  // Коорд. фиг. при повороте не меняются
  t->x = old->x;
  t->y = old->y;

  // Поворот осущ. след обр: будет счит., что посл. столбец блоков старой
  // фигуры, это первая строка новой. Предпоследний столбец старой фигуры,
  // это вторая строка новой фигуры. и т.д.
  for (int i = 0; i < t->size; i++) {
    for (int j = 0; j < t->size; j++) {
      // Если раб. с j - элементом нов. фигуры, то ему будет
      // соответствовать j - элем. столбца старой (прямая зависимость)

      // Если раб. с i - строкой нов. фиг., то зн. блок. для нее должны
      // брать с i - столбца старой фиг., но с конца
      t->blocks[i * t->size + j].b =
          old->blocks[j * t->size + (t->size - 1 - i)].b;
    }
  }
  return t;
}

// Функ. для просчета одного такта игрового цикла
void calculate_tet(Game* tetg) {
  tetg->ticks_left = tetg->ticks;
  move_figur_down(tetg);
  tetg->state = MOVING;
  if (collision(tetg)) {
    move_figure_up(tetg);
    plant_figure(tetg);
    score(tetg);
    if (tetg->figure != NULL) freeFigure(tetg->figure);
    drop_new_figure(tetg);
    tetg->state = DROP;
    if (collision(tetg)) {
      tetg->state = GAMEOVER;
    }
  }
}
// Состояние игры и обработка действий
void calculate(Game* tetg) {
  if (tetg->ticks_left <= 0 && tetg->state != PAUSE && tetg->state != INIT)
    calculate_tet(tetg);
  if (tetg->state == GAMEOVER) return;

  // Обработка действий игрока
  switch (tetg->player->action) {
    case Right:
      if (tetg->pause) break;
      move_figure_right(tetg);
      if (collision(tetg)) move_figur_left(tetg);
      break;
    case Left:
      if (tetg->pause) break;
      move_figur_left(tetg);
      if (collision(tetg)) move_figure_right(tetg);
      break;
    case Down:
      if (tetg->pause) break;
      move_figur_down(tetg);
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

// Функ. для просчета одного такта игрового цикла
void calculateTet(Game* tetg) {
  if (tetg->ticks_left <= 0) {
    tetg->ticks_left = tetg->ticks;
    moveFigureDown(tetg);
    if (collisionTet(tetg)) {
      moveFigureUp(tetg);
      plantFigure(tetg);
      tetg->score += eraseLinesTet(tetg);
      dropNewFigure(tetg);
      if (collisionTet(tetg)) {
        tetg->playing = TET_GAMEOVER;
        return;
      }
    }
  }
  // Обработка действий игрока
  switch (tetg->player->action) {
    case TET_PLAYER_RIGHT:
      moveFigureRight(tetg);
      if (collisionTet(tetg)) moveFigureLeft(tetg);
      break;

    case TET_PLAYER_LEFT:
      moveFigureLeft(tetg);
      if (collisionTet(tetg)) moveFigureRight(tetg);
      break;

    case TET_PLAYER_DOWN:
      moveFigureDown(tetg);
      if (collisionTet(tetg)) moveFigureUp(tetg);
      break;

    // Для поворота фигуры
    case TET_PLAYER_UP: {
      // Переменная фигуры после поворота
      TetFigure* t = turnTetFigure(tetg);

      // Переменная для хранения старой версии фигуры
      TetFigure* old = tetg->figure;

      // Объявление развернутой версии фигуры, текущей
      tetg->figure = t;
      // Проверка ситуации на столкновение (если столкновение произошло,
      // то вернем старую версию фигуры, новую удаляю. Иначе, удаляется
      // старая)
      if (collisionTet(tetg)) {
        tetg->figure = old;
        freeTetFigure(t);
      } else {
        freeTetFigure(old);
      }
    } break;
    case TET_PLAYER_NOP:
    default:
      break;
  }
  tetg->ticks_left--;
}

void save_score(int high_score) {
  FILE* file = fopen("high_score.dat", "w");
  if (file != NULL) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

int load_score() {
  int high_score = 0;
  FILE* file = fopen("high_score.dat", "r");
  if (file != NULL) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}

void update_level(Game* tetg) {
  int new_level = tetg->score / NEW_LEVEL_SCORE + 1;
  if (new_level > MAX_LEVEL) {
    new_level = MAX_LEVEL;
  }
  tetg->level = new_level;
}
