#include "tetris.h"

// New template figure
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