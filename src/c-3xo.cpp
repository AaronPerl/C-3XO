#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "ik.h"
#include "servo.h"
#include "tictactoe.h"
#include "minimax.h"
#include "tttcv.h"

int X_BUTTON_PIN = 4;
int O_BUTTON_PIN = 5;

float boxCoords[9][2] = {
	{275, 37},
	{275,  0},
	{275,-30},
	{250, 37},
	{250,  0},
	{250,-28},
	{215, 32},
	{215,  5},
	{215,-28}
};

int main(int argc, char* argv[])
{
	wiringPiSetup();
	pinMode(X_BUTTON_PIN, INPUT);
	pinMode(O_BUTTON_PIN, INPUT);
	pullUpDnControl(X_BUTTON_PIN, PUD_UP);
	pullUpDnControl(O_BUTTON_PIN, PUD_UP);
	initRobot(150.0f, 63.5f);
	int x,y,z;
	
	initCV();
	
	/*while (std::cin >> x >> y >> z)
	{
		if (x == 1337)
		{
			moveAlongCircle(vec3(220.0f,0,0),40);
		}
		else if (x == 9001)
		{
			drawBoard();
		}
		else if (x == 9002)
		{
			for (uint8_t i = 0; i < 9; i++)
			{
				moveAlongCircle(vec3(boxCoords[i][0], 0, boxCoords[i][1]), 10);
			}
		}
		else if (x == 9003)
		{
			for (uint8_t i = 0; i < 9; i++)
			{
				moveAlongX(vec3(boxCoords[i][0], 0, boxCoords[i][1]), 5);
			}
		}
		else
		{
			moveAlongLine(vec3(x,y,z));
		}
		//setServo(1,y);
		//setServo(2,z);
	}*/
	
	delay(2000);
	
	if (argc < 2)
		drawBoard();
	
	moveAlongLine(vec3(200,40,-50));
	
	int r,c;
	bool isX = true;
	bool playerIsX = true;
	
	std::cout << "Pick a side!" << std::endl;
	
	while (digitalRead(X_BUTTON_PIN) && digitalRead(O_BUTTON_PIN));
	
	if (!digitalRead(O_BUTTON_PIN))
	{
		playerIsX = false;
	}
	
	if (playerIsX)
	{
		std::cout << "The player will be playing as x!" << std::endl;
	}
	else
	{
		std::cout << "The player will be playing as o!" << std::endl;
	}
	
	delay(2000);
	
	bool moved = false;
	BoardState state;
	
	while (state.isOver() == none)//std::cin >> x)// >> y >> z)
	{
		if (isX == playerIsX)
		{	
			/*while (1)
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
			}*/
			std::cout << "Draw your symbol" << std::endl;
			while (digitalRead(X_BUTTON_PIN) && digitalRead(O_BUTTON_PIN));
		}
		else
		{
			std::cout << "My turn!" << std::endl;
			if (moved)
			{
				bool curState[9];
				checkBoxes(curState);
				uint8_t numDifferent = 0;
				uint8_t newBox = 0;
				for (uint8_t i = 0; i < 9; i++)
				{
					uint8_t row = i / 3;
					uint8_t col = i % 3;
					if (curState[i] && state.getTile(row, col) == empty)
					{
						newBox = i;
						numDifferent++;
					}
				}
				if (numDifferent != 1)
				{
					printf("%d boxes filled in!\n", numDifferent);
					exit(1);
				}
				if (isX)
				{
					state.setTile(newBox / 3, newBox % 3, oTile);
				}
				else
				{
					state.setTile(newBox / 3, newBox % 3, xTile);
				}
				std::cout << "State after drawing: " << std::endl;
				state.print();
			}
			std::cout << "Deciding move!" << std::endl;
			uint8_t move = decideMove(state, !playerIsX);
			uint8_t row = move / 3;
			uint8_t col = move % 3;
			vec3 boxPos(boxCoords[move][0], 50, boxCoords[move][1]);
			if (isX)
			{
				state.setTile(row, col, xTile);
				moveAlongX(boxPos, 7);
			}
			else
			{
				state.setTile(row, col, oTile);
				moveAlongCircle(boxPos, 10);
			}
			moveAlongLine(vec3(200,40,-50));
		}
		isX = !isX;
		state.print();
		moved = true;
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
	}
	std::cout << std::endl;
	getc(stdin);
	return 0;
}
