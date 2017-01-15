#include "tictactoe.h"

#include <iostream>

BoardState::BoardState(void)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = 0; j < 3; j++)
		{
			boardState[i][j] = empty;
		}
	}
}

Tile BoardState::getTile(uint8_t r, uint8_t c)
{
	return boardState[r][c];
}

void BoardState::setTile(uint8_t r, uint8_t c, Tile val)
{
	boardState[r][c] = val;
}

void BoardState::printLine(uint8_t lineNum)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		switch(boardState[lineNum][i])
		{
			case empty:
				std::cout << ' ';
				break;
			case xTile:
				std::cout << 'x';
				break;
			case oTile:
				std::cout << 'o';
				break;
		}
		if (i != 2)
		{
			std::cout << "|";
		}
	}
	std::cout << std::endl;
}

void BoardState::print(void)
{
	for (uint8_t i = 0; i < 3; i++)
	{
		printLine(i);
		if (i != 2)
		{
			std::cout << "-----" << std::endl;
		}
	}
}

WinState BoardState::winFromTile(Tile tile)
{
	if (tile == oTile)
	{
		return oVictory;
	}
	else if (tile == xTile)
	{
		return xVictory;
	}
	else
	{
		return none;
	}
}

WinState BoardState::isOver(void)
{
	// Row check
	for (uint8_t row = 0; row < 3; row++)
	{
		if (boardState[row][0] == boardState[row][1] &&
		    boardState[row][1] == boardState[row][2] &&
			boardState[row][2] != empty)
		{
			return winFromTile(boardState[row][0]);
		}
	}
	// Column check
	for (uint8_t col = 0; col < 3; col++)
	{
		if (boardState[0][col] == boardState[1][col] &&
		    boardState[1][col] == boardState[2][col] &&
			boardState[2][col] != empty)
		{
			return winFromTile(boardState[0][col]);
		}
	}
	// Diagonal check
	if (boardState[0][0] == boardState[1][1] &&
	    boardState[1][1] == boardState[2][2] &&
		boardState[2][2] != empty)
	{
		return winFromTile(boardState[0][0]);
	}
	if (boardState[0][2] == boardState[1][1] &&
	    boardState[1][1] == boardState[2][0] &&
		boardState[2][0] != empty)
	{
		return winFromTile(boardState[0][2]);
	}
	for (uint8_t row = 0; row < 3; row++)
	{
		for (uint8_t col = 0; col < 3; col++)
		{
			if (boardState[row][col] == empty) return none;
		}
	}
	return tie;
}