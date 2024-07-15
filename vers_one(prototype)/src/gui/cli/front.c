#include "front.h"

// Инициализирует графический интерфейс игры с использованием библиотеки ncurses
void init_gui() {
  // Инициализирует окно терминала
  initscr();
  // Устанавливает курсор невидимым
  curs_set(0);
  // Включает цветовой режим
  start_color();
  // Создает несколько пар цветов для использования в игр
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_WHITE, COLOR_WHITE);
  init_pair(4, COLOR_WHITE, COLOR_RED);

  // Включает режим, чтобы ввод был немедленно доступен без ожидания нажатия
  // Enter.
  cbreak();
  // Отключает эхо-вывод введенных символов
  noecho();
  // Устанавливает режим ожидания без блокировки
  nodelay(stdscr, TRUE);
  // Включает прокрутку окна терминала
  scrollok(stdscr, TRUE);
}

// Отображает текущее состояние игры на экране
void printGame(GameInfo_t game, struct timespec sp_start,
               struct timespec sp_end) {
  // Рисует игровое поле
  print_field(game);
  // Отображает следующую фигуру
  print_next_figure(game);
  // Отображает информацию о текущем состоянии игры, такую как счет, рекорд,
  // уровень и скорость
  print_info(game);
  // Освобождает ресурсы графического интерфейса
  free_gui(game, 5, 20);
  // Обрабатывает задержку между кадрами
  handle_delay(sp_start, sp_end, game.speed);
  // Обновляет экран
  refresh();
}

// Отрисовывает игровое поле. Эта функция проходит по каждому блоку на поле и
// использует цвета для отображения пустых и занятых ячеек.
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

// Отображает следующую фигуру, которая должна появиться на игровом поле. Эта
// функция также использует цвета для визуализации фигуры.
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

// Отображает информацию о текущем состоянии игры, включая следующую фигуру,
// счет, рекорд, уровень и скорость.
void print_info(GameInfo_t game) {
  attron(COLOR_PAIR(4));
  mvwprintw(stdscr, 1, 16, "Next figure:");
  mvwprintw(stdscr, 11, 16, "Score: %d", game.score);
  mvwprintw(stdscr, 12, 16, "High score: %d", game.high_score);
  mvwprintw(stdscr, 15, 16, "Lvl: %d", game.level);
  mvwprintw(stdscr, 16, 16, "Speed: %d", game.speed);
  if (game.pause) mvwprintw(stdscr, 21, 2, "");  // Отвечает за старт и паузу
}

// Читает ввод пользователя из терминала и преобразует его в соответствующее
// действие игрока. Эта функция поддерживает команды для движения фигур, начала
// и паузы игры, а также выхода из игры.
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

// Обрабатывает задержку между кадрами игры, чтобы контролировать скорость
// падения фигур.
// Эта функция рассчитывает, сколько времени должно пройти между кадрами, исходя
// из текущей скорости игры, и использует nanosleep() для установки задержки,
// если необходимо.
void handle_delay(struct timespec sp_start, struct timespec sp_end,
                  int game_speed) {
  clock_gettime(CLOCK_MONOTONIC, &sp_end);
  struct timespec ts1, ts2 = {0, 0};
  if (sp_end.tv_sec - sp_start.tv_sec <= 0 &&
      (ts2.tv_nsec = (20000000 - game_speed * 1500000) -
                     (sp_end.tv_nsec - sp_start.tv_nsec)) > 0) {
    nanosleep(&ts2, &ts1);
  }
}