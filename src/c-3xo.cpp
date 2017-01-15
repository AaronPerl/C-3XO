#include <iostream>
#include "ik.h"
#include "servo.h"

int main(int argc, char* argv[])
{
	initRobot(150.0f, 140.0f);
	float x,y,z;
	setServo(0,0);
	while (std::cin >> x)// >> y >> z)
	{
		setServo(0,x);
		//moveTo(x,y,z);
	}
	return 0;
}
