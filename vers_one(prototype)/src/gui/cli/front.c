#include "front.h"

void init_gui() {
  initscr();
  curs_set(0);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_WHITE, COLOR_WHITE);
  init_pair(4, COLOR_WHITE, COLOR_RED);

  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
}

void printGame(GameInfo_t game, struct timespec sp_start,
               struct timespec sp_end) {
  print_field(game);

  print_next_figure(game);
  print_info(game);
  free_gui(game, 5, 20);
  handle_delay(sp_start, sp_end, game.speed);
  refresh();
}

void print_field(GameInfo_t game) {
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      int sym = game.field[i][j] != 0 ? 2 : 1;
      attron(COLOR_PAIR(sym));
      mvaddch(i + 0, j, ' ');
      mvaddch(i + 0, j, ' ');
      attroff(COLOR_PAIR(sym));
    }
  }
}

void print_next_figure(GameInfo_t game) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      int sym = game.next[i][j] != 0 ? 2 : 0;
      attron(COLOR_PAIR(sym));
      mvaddch(i + 2, j * 2 + 18, ' ');
      mvaddch(i + 2, j * 2 + 19, ' ');
      attroff(COLOR_PAIR(sym));
    }
  }
}

void print_info(GameInfo_t game) {
  attron(COLOR_PAIR(4));
  mvwprintw(stdscr, 1, 16, "Next figure:");
  mvwprintw(stdscr, 11, 16, "Score: %d", game.score);
  mvwprintw(stdscr, 12, 16, "High score: %d", game.high_score);
  mvwprintw(stdscr, 15, 16, "Lvl: %d", game.level);
  mvwprintw(stdscr, 16, 16, "Speed: %d", game.speed);
  if (game.pause) mvwprintw(stdscr, 21, 2, "");  // Отвечает за старт и паузу
}

UserAction_t get_action() {
  int ch = getch();
  switch (ch) {
    case 'a':
      return Left;
    case 'd':
      return Right;
    case 'w':
      return Up;
    case 's':
      return Down;
    case 'r':
      return Start;
    case 'e':
      return Pause;
    case 'q':
      return Terminate;
    default:
      return Action;
  }
}

// Обработка задежки в игре
void handle_delay(struct timespec sp_start, struct timespec sp_end,
                  int game_speed) {
  clock_gettime(CLOCK_MONOTONIC, &sp_end);
  struct timespec ts1, ts2 = {0, 0};
  // первое усл.: просчет ситуаций и рисование заняли менее сек.
  // второе усл.: расчет задежки, и опр., стоит ли ее осущест.
  // 33млн - это общее время обр. тела цикла в нано сек.
  // Если все это время не было исчерано просчет. ситуаций и отобр., то стоит
  // сдел. паузу прод. в остав. промеж.
  if (sp_end.tv_sec - sp_start.tv_sec <= 0 &&
      (ts2.tv_nsec = (20000000 - game_speed * 1500000) -
                     (sp_end.tv_nsec - sp_start.tv_nsec)) > 0) {
    // Обесп.задежку парам.в структ.ts2.И в случае прерывания со стор.
    // системы вернет параметры не заверш. паузы в струк. ts1
    nanosleep(&ts2, &ts1);
  }
}