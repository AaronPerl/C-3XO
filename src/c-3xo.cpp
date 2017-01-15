#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <cmath>
#include "ik.h"
#include "servo.h"
#include "tictactoe.h"
#include "minimax.h"
#include "tttcv.h"

int X_BUTTON_PIN = 4;
int O_BUTTON_PIN = 5;

vec3 home(200,40,-50);

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

	while (1)
	{
		std::cout << "Press any button to begin!" << std::endl;
		
		while (digitalRead(X_BUTTON_PIN) && digitalRead(O_BUTTON_PIN));
				
		drawBoard();
		
		moveAlongLine(home);
		
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
				std::cout << "Draw your symbol and press any button to continue" << std::endl;
				while (digitalRead(X_BUTTON_PIN) && digitalRead(O_BUTTON_PIN));
				
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
						printf("%d\n", newBox);
						numDifferent++;
					}
				}
				if (numDifferent != 1)
				{
					printf("%d boxes filled in!\n", numDifferent);
					continue;
				}
				if (isX)
				{
					state.setTile(newBox / 3, newBox % 3, xTile);
				}
				else
				{
					state.setTile(newBox / 3, newBox % 3, oTile);
				}
				std::cout << "State after drawing: " << std::endl;
				state.print();
			}
			else
			{
				std::cout << "My turn!" << std::endl;
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
				moveAlongLine(home);
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
				moveAlongLine(vec3(250,50,0));
				for (int i = 30; i < 330; i++)
				{
					float angle = 3.1415 * i / 180.0f;
					moveAlongLine(vec3(250+30*sin(angle), 15, 0 + 30*cos(angle)));
				}
				moveAlongLine(vec3(250,50,0));
				break;
			default:
				std::cout << "problematic!";
				break;
		}
		std::cout << std::endl;
		
		moveAlongLine(home);
	}
	return 0;
}
