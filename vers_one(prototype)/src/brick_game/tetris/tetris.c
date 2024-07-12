#include "tetris.h"

TetFiguresT* createTetFiguresT(int count, int figures_size,
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

TetField* createTetField(int width, int height) {  // Память для игрового поля
  TetField* tetf = (TetField*)malloc(sizeof(TetField));
  tetf->width = width;
  tetf->height = height;

  tetf->blocks = (TetBlock*)malloc(sizeof(TetBlock) * width * height);
  // Память об информации о блоках (объем=структура*шир*выс)

  for (int i = 0; i < width * height; i++) {
    tetf->blocks[i].b = 0;  // Если блоков нет
  }

  return tetf;
}

void freeTetField(TetField* tetf) {
  if (tetf) {
    if (tetf->blocks) {
      free(tetf->blocks);
    }
    free(tetf);
  }
}

TetGame* createTetGame(int field_width, int field_height, int figures_size,
                       int count, TetBlock* figures_template) {
  // Выделение памяти для основной структуры игры
  TetGame* tetg = (TetGame*)malloc(sizeof(TetGame));
  // Для поля
  tetg->field = createTetField(field_width, field_height);
  // для набора шаблонов
  tetg->figurest = createTetFiguresT(count, figures_size, figures_template);

  // Опред. зн. пер.
  tetg->level = 1;
  tetg->ticks = TET_TICKS_START;
  tetg->ticks_left = TET_TICKS_START;
  tetg->score = 0;
  tetg->playing = TET_PLAYING;

  return tetg;
}

void freeTetGame(TetGame* tetg) {
  if (tetg) {
    freeTetField(tetg->field);
    freeTetFiguresT(tetg->figurest);
    free(tetg);
  }
}

// Смещение фигуры вниз
void moveFigureDown(TetGame* tetg) { tetg->figure->y++; }

// Поворот фигуры
void moveFigureUp(TetGame* tetg) { tetg->figure->y--; }

// Движение фигуры влево
void moveFigureLeft(TetGame* tetg) { tetg->figure->x--; }

// Движение фигуры вправо
void moveFigureRight(TetGame* tetg) { tetg->figure->x++; }

// Функ. было ли столкновение?
int collisionTet(TetGame* tetg) {
  // для удобстава объяв. пер.: падающую фигуру и игровое поле
  TetFigure* t = tetg->figure;
  TetField* tf = tetg->field;

  // Если столкн. произошло только в рамках обл. рис. фигуры
  for (int i = 0; i < t->size; i++) {
    for (int j = 0; j < t->size; j++) {
      // Все двумерные констр. хранятся в одномерных масс., запиш. формулу
      // расчета по друмерным коорд i=y, j=x

      // Если блок фигур не пуст, то рассчит коорд на поле
      if (t->blocks[i * t->size + j].b != 0) {
        int fx = t->x + j;
        int fy = t->y + i;

        // Если в том же месте, на поле, есть не пустой блок, зн. произ.
        // столкновение
        if (tf->blocks[fy * tf->width + fx].b != 0) return 1;

        // Столкновение с границами поля
        if (fx < 0 || fx >= tf->width || fy < 0 || fy >= tf->height) return 1;
      }
    }
  }
  // Если нет столкновений
  return 0;
}

// Функция размещения фигуры после падения
void plantFigure(TetGame* tetg) {
  // Перем. на фигуру
  TetFigure* t = tetg->figure;
  for (int i = 0; i < t->size; i++) {
    for (int j = 0; j < t->size; j++) {
      // Если блок не нулевой, опред. коорд поля соотв. блоку и перенесем его
      // данные на поле
      if (t->blocks[i * t->size + j].b != 0) {
        int fx = t->x + j;
        int fy = t->y + i;
        tetg->field->blocks[fy * tetg->field->width + fx].b =
            t->blocks[i * t->size + j].b;
      }
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
void dropLineTet(int i, TetField* tfl) {
  // Если строка нулевая, то очищаем
  if (i == 0) {
    for (int j = 0; j < tfl->width; j++) tfl->blocks[j].b = 0;
  }  // Иначе, перенощу блоки верхней строки на строку текущую (индекс текущей и
     // вверхней строки: к и (к-1)). Процесс повторяется для к от текущей
     // позиции до первой, так реализуется сдвиг блоков поля
  else {
    for (int k = i; k > 0; k--) {
      for (int j = 0; j < tfl->width; j++)
        tfl->blocks[k * tfl->width + j].b =
            tfl->blocks[(k - 1) * tfl->width + j].b;
    }
  }
}

// Функция удаляющая стороки и подсчитывающая их кол-во
int eraseLinesTet(TetGame* tetg) {
  TetField* tfl = tetg->field;
  int count = 0;  // Кол-во удал. стр.

  for (int i = tfl->height - 1; i >= 0; i--) {
    // Удал. стр. нач. с последней, выполняется сдвиг строк вниз
    while (lineFilledTet(i, tfl)) {
      // Пока текущая строка заполнена, удал ее со сдвигом вниз и увел. кол-во
      // удал. стр на 1
      dropLineTet(i, tfl);
      count++;
    }
  }
  // Начисление очков на основе количества удаленных строк
  if (count > 0) {
    if (count == 1) {
      tetg->score += 99;
    } else if (count == 2) {
      tetg->score += 298;
    } else if (count == 3) {
      tetg->score += 697;
    } else if (count == 4) {
      tetg->score += 1496;
    }
  }
  save_max_score(tetg);
  update_level(tetg);
  return count;
}

// Опр. функцию создания и иниц. фигуры
TetFigure* createTetFigure(TetGame* tetg) {
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
void dropNewFigure(TetGame* tetg) {
  // Создание нов. фиг. в памяти
  TetFigure* t = createTetFigure(tetg);
  // Координаты: в центре и  в верху
  t->x = tetg->field->width / 2 - t->size / 2;
  t->y = 0;
  // Шаблон фигуры ранд. образом исходя из кол-ва шаблонов
  int fnum = rand() % 7;
  for (int i = 0; i < t->size; i++) {
    for (int j = 0; j < t->size; j++) {
      // Скоп. блоки шабл. в обл. фигуры. Номер шаблона опр. смещение в масс.
      // блоков в соотв. выбранного шаблона
      t->blocks[i * t->size + j].b =
          tetg->figurest->blocks[fnum * t->size * t->size + i * t->size + j].b;
    }
  }
  // Удаление страрой фигуры из памяти. На ее роль назначим только что созданную
  freeTetFigure(tetg->figure);
  tetg->figure = t;
}

// Функ поворота фигуры
TetFigure* turnTetFigure(TetGame* tetg) {
  // Созд. пуст. фигуру
  TetFigure* t = createTetFigure(tetg);
  // Объяв. указ на старую фигуру
  TetFigure* old = tetg->figure;
  // Коорд. фиг. при повороте не меняются
  t->x = old->x;
  t->y = old->y;

  // Поворот осущ. след обр: будет счит., что посл. столбец блоков старой
  // фигуры, это первая строка новой. Предпоследний столбец старой фигуры, это
  // вторая строка новой фигуры. и т.д.
  for (int i = 0; i < t->size; i++) {
    for (int j = 0; j < t->size; j++) {
      // Если раб. с j - элементом нов. фигуры, то ему будет соответствовать j -
      // элем. столбца старой (прямая зависимость)

      // Если раб. с i - строкой нов. фиг., то зн. блок. для нее должны брать с
      // i - столбца старой фиг., но с конца
      t->blocks[i * t->size + j].b =
          old->blocks[j * t->size + (t->size - 1 - i)].b;
    }
  }
  return t;
}

// Функ. для просчета одного такта игрового цикла
void calculateTet(TetGame* tetg) {
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
      // Проверка ситуации на столкновение (если столкновение произошло, то
      // вернем старую версию фигуры, новую удаляю. Иначе, удаляется старая)
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

  // Сохранение очков в record
  int high_score;
  FILE* file = fopen("./max_score.txt", "r");
  fscanf(file, "%d", &high_score);
  fclose(file);
  // Если сделать tetg->score, то убирается проблема записаных неточных значений
  // в функ. eraseLinesTet, но сохранение очков будет в score
  tetg->highscore = high_score;
}

void save_max_score(TetGame* tetg) {
  FILE* file;
  int high_score;
  file = fopen("max_score.txt", "r");
  fscanf(file, "%d", &high_score);
  fclose(file);

  if (tetg->score > high_score) {
    file = fopen("max_score.txt", "w");
    fprintf(file, "%d", tetg->score);
    fclose(file);
  }
}

void update_level(TetGame* tetg) {
  int new_level = tetg->score / NEW_LEVEL_SCORE + 1;
  if (new_level > MAX_LEVEL) {
    new_level = MAX_LEVEL;
  }
  tetg->level = new_level;
}
