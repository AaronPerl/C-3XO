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
float rate = 80.0f;

// returns time difference in ns
long diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return (temp.tv_sec * 1000000) + (temp.tv_nsec / 1000);
}

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
	z /= 1.10f;
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
	float duration = 0.0f;
	
	struct timespec start;
	struct timespec now;
	
	clock_gettime(CLOCK_REALTIME, &start);
	
	while (duration < travelTime)
	{
		clock_gettime(CLOCK_REALTIME, &now);
		long td = diff(start, now);
		duration = td / 1000000.0f;
		float t = duration / travelTime;
		vec3 newPos = (prev * (1-t)) + (to * t);
		moveTo(newPos);
		//std::cout << duration << std::endl;
		//std::cout << newPos.getX() << "," << newPos.getY() << "," << newPos.getZ() << std::endl;
	}
	moveTo(to);
	
	prev = to;
}

void moveAlongCircle(vec3 center, float radius)
{
	center = vec3(center.getX(), 12, center.getZ());
	vec3 circleStart = vec3(center.getX(), 60, center.getZ()) + vec3(radius,0,0);
	moveAlongLine(circleStart);
	
	for (uint16_t i = 0; i < 270; i++)
	{
		float angle = (i * 2.0f) * PI / 180.0f;
		vec3 offset = vec3(cos(angle),0,sin(angle)) * radius;
		moveAlongLine(center + offset);
	}
	
	moveAlongLine(vec3(center.getX(), 60, center.getZ() + radius));
}

void moveAlongX(vec3 center, float radius)
{
	center = vec3(center.getX(), 12, center.getZ());
	vec3 xStart = vec3(center.getX(), 60, center.getZ()) + vec3(radius,0,radius);
	moveAlongLine(xStart);
	moveAlongLine(center + vec3(radius,0,radius));
	moveAlongLine(center + vec3(-radius,0,-radius));
	moveAlongLine(center + vec3(-radius,30,-radius));
	moveAlongLine(center + vec3(radius,30,-radius));
	moveAlongLine(center + vec3(radius,0,-radius));
	moveAlongLine(center + vec3(-radius,0,radius));
	moveAlongLine(center + vec3(-radius,30,radius));
}

void drawBoard()
{
	//Lift
	moveAlongLine(vec3(205.0f, 50.0f, 15.0f));
	//Drop
	moveAlongLine(vec3(205.0f, 12.0f, 15.0f));
	//Draw line
	moveAlongLine(vec3(295.0f, 14.0f, 15.0f));
	//Lift
	moveAlongLine(vec3(295.0f, 50.0f, 15.0f));
	//Move
	moveAlongLine(vec3(295.0f, 50.0f, -15.0f));
	//Drop
	moveAlongLine(vec3(295.0f, 14.0f, -15.0f));
	//Draw line
	moveAlongLine(vec3(205.0f, 12.0f, -15.0f));
	//Lift
	moveAlongLine(vec3(205.0f, 50.0f, -15.0f));
	//Move
	moveAlongLine(vec3(235.0f, 50.0f, -45.0f));
	moveAlongLine(vec3(235.0f, 13.0f, -45.0f));
	moveAlongLine(vec3(235.0f, 13.0f, 45.0f));
	moveAlongLine(vec3(235.0f, 50.0f, 45.0f));
	
	moveAlongLine(vec3(265.0f, 50.0f, 45.0f));
	moveAlongLine(vec3(265.0f, 14.0f, 45.0f));
	moveAlongLine(vec3(265.0f, 14.0f, -45.0f));
	moveAlongLine(vec3(265.0f, 50.0f, -45.0f));
	moveAlongLine(vec3(250.0f, 50.0f, 0.0f));
}