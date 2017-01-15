#ifndef __TICTACTOE_H__
#define __TICTACTOE_H__

#include <stdint.h>

enum Tile {empty, oTile, xTile};
enum WinState {none, oVictory, xVictory, tie};


class BoardState
{
private:
	Tile boardState[3][3];
	void printLine(uint8_t lineNum);
	WinState winFromTile(Tile tile);
public:
	BoardState(void);
	Tile getTile(uint8_t r, uint8_t c);
	void setTile(uint8_t r, uint8_t c, Tile val);
	void print(void);
	// Checks if the game is over
	WinState isOver(void);
};

#endif