#ifndef __MINIMAX_H__
#define __MINIMAX_H__

#include "tictactoe.h"

// Decides what move to make, returns the tile to place on in the format:
// 0 1 2
// 3 4 5
// 6 7 8
uint8_t decideMove(BoardState state, bool isX);

#endif