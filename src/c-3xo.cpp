#include <iostream>
#include <stdio.h>
#include "ik.h"
#include "servo.h"
#include "tictactoe.h"
#include "minimax.h"

int main(int argc, char* argv[])
{
	initRobot(150.0f, 140.0f);
	float x,y,z;
	setServo(0,0);
	
	while (std::cin >> x)
	{
		setServo(0,x);
	}
	
	/*
	int r,c;
	bool isX = true;
	bool playerIsX;
	
	std::cout << "Pick a side! Type [x] or [o] and hit enter." << std::endl;
	char selection = '\0';
	
	do
	{
		std::cin >> selection;
	} while (selection != 'x' && selection != 'o');
	
	std::cout << "The player will be playing as: " << selection << std::endl;
	
	if (selection == 'x')
	{
		playerIsX = true;
	}
	else
	{
		playerIsX = false;
	}
	
	BoardState state;
	state.print();
	while (state.isOver() == none)//std::cin >> x)// >> y >> z)
	{
		if (isX == playerIsX)
		{	
			while (1)
			{
				std::cin >> r >> c;
				if (state.getTile(r,c) == empty)
				{
					if (isX)
					{
						state.setTile(r,c,xTile);
					}
					else
					{
						state.setTile(r,c,oTile);
					}
					break;
				}
				else
				{
					std::cout << "Please pick an empty tile!" << std::endl;
				}
			}
		}
		else
		{
			uint8_t move = decideMove(state, !playerIsX);
			uint8_t row = move / 3;
			uint8_t col = move % 3;
			if (isX)
			{
				state.setTile(row, col, xTile);
			}
			else
			{
				state.setTile(row, col, oTile);
			}
		}
		isX = !isX;
		state.print();

		//setServo(0,x);
		//moveTo(x,y,z);
	}
	std::cout << "The winner is: ";
	switch (state.isOver())
	{
		case xVictory:
			std::cout << "x!";
			break;
		case oVictory:
			std::cout << "o!";
			break;
		case tie:
			std::cout << "no one!";
			break;
		default:
			std::cout << "problematic!";
			break;
	}*/
	std::cout << std::endl;
	getc(stdin);
	return 0;
}
