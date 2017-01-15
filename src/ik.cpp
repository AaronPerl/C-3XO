#include "ik.h"
#include "servo.h"

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <time.h>

#define PI 3.141592f
#define DEG(angle) (angle * 180.0f / PI)

#define ANGLE1_OFFSET 4.2f

// Length of each bar in the arm
float armLen;
// Height offset of the first joint
float height;
// Movement speed (mm/s)
float rate = 20.0f;

// Most recent coordinates
vec3 prev(280.0f, 50.0f, 0.0f);

vec3::vec3(float newX, float newY, float newZ): x(newX), y(newY), z(newZ) {}

float vec3::magnitude() const
{
	return sqrt(x*x + y*y + z*z);
}

vec3 vec3::operator+(const vec3& other) const
{
	return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator-(const vec3& other) const
{
	return vec3(x - other.x, y - other.y, z - other.z);
}

vec3 vec3::operator-() const
{
	return vec3(-x, -y, -z);
}

vec3 vec3::operator*(float scalar) const
{
	return vec3(x * scalar, y * scalar, z * scalar);
}

vec3 vec3::unit() const
{
	float mag = magnitude();
	return vec3(x/mag, y/mag, z/mag);
}

void initRobot(float robotArmLength, float firstArmHeight)
{
	servoInit();
	armLen = robotArmLength;
	height = firstArmHeight;
	moveTo(prev);
}

void moveTo(vec3 pos)
{
	moveTo(pos.getX(), pos.getY(), pos.getZ());
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
	float armAngle2 = 90 - atanterm + acosterm;
	
	setServo(0,baseAngle);
	setServo(1,armAngle1);
	setServo(2,armAngle2);
	//printf("Angles: %5.2f %5.2f %5.2f\n", baseAngle, armAngle1, armAngle2);
}

void moveAlongLine(vec3 to)
{
	vec3 delta = to - prev;
	vec3 dir = delta.unit();
	float distance = delta.magnitude();
	
	float travelTime = (distance / rate);
	float startTime = millis() / 1000.0f;
	float duration = 0.0f;
	
	while (duration < travelTime)
	{
		float t = duration / travelTime;
		vec3 newPos = (prev * (1-t)) + (to * t);
		moveTo(newPos);
		std::cout << newPos.getX() << "," << newPos.getY() << "," << newPos.getZ() << std::endl;
		duration = (millis() / 1000.0f) - startTime;
	}
	moveTo(to);
	
	prev = to;
}