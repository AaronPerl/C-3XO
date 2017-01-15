#include "ik.h"
#include "servo.h"

#include <cmath>
#include <stdio.h>

#define PI 3.141592f
#define DEG(angle) (angle * 180.0f / PI)

#define ANGLE1_OFFSET 4.2f

// Length of each bar in the arm
float armLen = 100.0f;
// Height offset of the first joint
float height = 30.0f;
// Movement speed (mm/s)
float rate = 20.0f;

// Most recent coordinates
float curX = 100.0f;
float curY =  50.f;

void initRobot(float robotArmLength, float firstArmHeight)
{
	servoInit();
	armLen = robotArmLength;
	height = firstArmHeight;
}

void moveTo(float x, float y, float z)
{
	float baseAngle = DEG(atan2(z, x));
	float radius    = sqrt(x*x + z*z);
	
	float h_min_y   = height - y;
	float r_over_h  = radius / h_min_y;
	float r_diag     = sqrt(radius * radius + h_min_y * h_min_y);
	
	float atanterm  = DEG(atan(r_over_h));
	float acosterm  = DEG(acos(r_diag/(2 * armLen)));
	
	float armAngle1 = 90 + ANGLE1_OFFSET - atanterm - acosterm;
	float armAngle2 = 45 - atanterm + acosterm;
	
	printf("Angles: %5.2f %5.2f %5.2f\n", baseAngle, armAngle1, armAngle2);
}

void moveAlongLine(float toX, float toY, float toZ)
{
	
}