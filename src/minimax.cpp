#include "minimax.h"
#include <vector>
#include <iostream>

void getPossibleMoves(BoardState state, std::vector<uint8_t>& moves)
{
	for (uint8_t pos = 0; pos < 3*3; pos++)
	{
		uint8_t row = pos / 3;
		uint8_t col = pos % 3;
		if (state.getTile(row, col) == empty)
		{
			moves.push_back(pos);
		}
	}
}

int minimax(BoardState state, bool isX, bool myTurn, uint8_t* retMove)
{
	// Evaluate points if board is in a complete state
	switch (state.isOver())
	{
		case xVictory:
			return isX?10:-10;
		case oVictory:
			return isX?-10:10;
		case tie:
			return 0;
	}
	
	// Determine all possible moves from this state
	std::vector<uint8_t> possibleMoves;
	std::vector<int> scores;
	
	getPossibleMoves(state, possibleMoves);
	
	// The move made will be an x if I am x and it is my turn, or if I am o and it is the other player's turn.
	// Therefore, isX == myTurn is equivalent to whether the move made will be an x.
	Tile newTile = (isX==myTurn)?xTile:oTile;
	
	// Check the minimax score of every possible move and record it
	for (uint8_t moveIndex = 0; moveIndex < possibleMoves.size(); moveIndex++)
	{
		uint8_t curMove = possibleMoves[moveIndex];
		BoardState newState(state);
		newState.setTile(curMove / 3, curMove % 3, newTile);
		scores.push_back(minimax(newState, isX, !myTurn, nullptr));
	}
	
	// Maximize if my turn, minimize if opponent's turn
	if (myTurn)
	{
		int maxScore = scores[0];
		uint8_t maxIndex = 0;
		for (uint8_t index = 0; index < scores.size(); index++)
		{
			if (scores[index] > maxScore)
			{
				maxScore = scores[index];
				maxIndex = index;
			}
		}
		if (retMove != nullptr)
		{
			*retMove = possibleMoves[maxIndex];
		}
		return maxScore;
	}
	else
	{
		int minScore = scores[0];
		for (uint8_t index = 0; index < scores.size(); index++)
		{
			if (scores[index] < minScore)
			{
				minScore = scores[index];
			}
		}
		return minScore;
	}
}


uint8_t decideMove(BoardState state, bool isX)
{
	uint8_t maxMove;
	minimax(state, isX, true, &maxMove);
	return maxMove;
}