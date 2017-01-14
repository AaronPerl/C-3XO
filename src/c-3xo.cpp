#include <iostream>
#include "servo.h"

int main(int argc, char* argv[])
{
	servoInit();
	int16_t x;
	while (std::cin >> x)
	{
		setServo(16, x);
	}
	return 0;
}
