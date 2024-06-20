#include "brick_game/tetris/tetris.h"
#include "gui/cli/front.h"

TetBlock tet_templates[] = {
    0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Проц. вывод. игровую ситуац. на экр.
void printTetGame(TetGame* tetg) {
  // Перем. для удобст. доступа к данным: поле игры и фигура
  TetField* tf = tetg->field;
  TetFigure* t = tetg->figure;

  for (int i = 0; i < tf->height; i++) {
    for (int j = 0; j < tf->width; j++) {
      // Корект. кода с замен. мет. печ. на присв. зн. перем. Так можно понять,
      // какой блок печатать
      int sym = 1;
      // Если текущий блок поля не пуст.
      if (tf->blocks[i * tf->width + j].b != 0)
        // Вывод его
        sym = 2;
      // Иначе, след. опр. распол. ли блок фиг. по коорд.
      else {
        int x = j - t->x;
        int y = i - t->y;
        // В лог. конст. в случ. ложности одного из первых высказ. остальные не
        // расчит. и нет опасности выхода за границу массива блоков. Запись
        // проверки условий в два этапа
        if (x >= 0 && x < t->size && y >= 0 && y < t->size)
          if (t->blocks[y * t->size + x].b != 0) sym = 2;
      }
      attron(COLOR_PAIR(sym));
      // printf("%d", sym);
      // Вывод пробела с коор.
      mvaddch(i, j, ' ');
      attroff(COLOR_PAIR(sym));
    }
  }
  // Принтф не гар. мгнов. очищ. появл. символов на экране. Для мнгнов. отображ.
  // необходимо очистить соответ. буфер
  //   fflush(stdout);  // ???
}

void str() {
  // объяв. структ. для фикс. моментов времени
  struct timespec sp_start, sp_end, ts1, ts2 = {0, 0};

  // Инициализация генератора случайных фигур
  srand(time(0));

  // Библиотека ncurses
  // Смена режима инфы в терм
  initscr();
  // Работа со светом
  start_color();
  // Цвет для поля
  init_pair(1, COLOR_WHITE, COLOR_RED);
  // Цвет для фигуры
  init_pair(2, COLOR_WHITE, COLOR_WHITE);

  // Процедуры влияющие на проблемы
  // Влияет на строковый буфер и принуж. к немедл. выводу символа нажатой
  // клавиши
  cbreak();
  // Отключает вывод символа нажатой клавиши на экран
  noecho();
  // Откл. задежку функ. getch
  nodelay(stdscr, TRUE);
  // Откл. возм. скролинга
  scrollok(stdscr, TRUE);

  TetGame* tetg = createTetGame(10, 20, 5, 7, tet_templates);

  // Структура для действий игрока
  TetPlayer player;
  player.action = TET_PLAYER_NOP;
  tetg->player = &player;
  dropNewFigure(tetg);

  // Игровой цикл действует пока игра не завершена
  while (tetg->playing != TET_GAMEOVER) {
    // Фикс. момента времени
    clock_gettime(CLOCK_MONOTONIC, &sp_start);

    // Реакция на нажатие клавиш
    int ch = getch();
    switch (ch) {
      case 'w':
        player.action = TET_PLAYER_UP;
        break;
      case 's':
        player.action = TET_PLAYER_DOWN;
        break;
      case 'a':
        player.action = TET_PLAYER_LEFT;
        break;
      case 'd':
        player.action = TET_PLAYER_RIGHT;
        break;
      case 'e':
        player.action = TET_PLAYER_START;  // Не реализована
        break;
      case 'q':
        player.action = TET_PLAYER_PAUSE;  // Не реализована
        break;
      case 't':
        player.action = TET_PLAYER_TERMINATE;  // Не реализована
        break;
      default:
        player.action = TET_PLAYER_NOP;
        break;
    }

    // Расч. одного такта
    calculateTet(tetg);
    // вывод проц. вывод. игр. ситуац. на экран
    printTetGame(tetg);

    attron(COLOR_PAIR(1));
    mvprintw(2, 12, "Score: %d", tetg->score);
    mvprintw(4, 12, "Record: %d", tetg->highscore);
    // Убрать курсор за экранwa
    move(tetg->field->height + 1, tetg->field->width + 1);
    attroff(COLOR_PAIR(1));

    clock_gettime(CLOCK_MONOTONIC, &sp_end);

    // первое усл.: просчет ситуаций и рисование заняли менее сек.
    // второе усл.: расчет задежки, и опр., стоит ли ее осущест.
    // 33млн - это общее время обр. тела цикла в нано сек.
    // Если все это время не было исчерано просчет. ситуаций и отобр., то стоит
    // сдел. паузу прод. в остав. промеж.
    if (sp_end.tv_sec - sp_start.tv_sec <= 0 &&
        ((ts2.tv_nsec = 33000000) - (sp_end.tv_nsec - sp_start.tv_nsec)) > 0)

      // Обесп. задежку парам. в структ. ts2. И в случае прерывания со стор.
      // системы вернет параметры не заверш. паузы в струк. ts1
      nanosleep(&ts2, &ts1);
  }
  // Освобождение памяти знаним. осн. структ.
  freeTetGame(tetg);
  // верн. исх. вид окна терм. и не нарушить его внутр. структуру
  endwin();
}
