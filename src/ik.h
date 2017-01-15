#ifndef __IK_H__
#define __IK_H__

void initRobot(float robotArmLength, float firstArmHeight);
void moveTo(float x, float y, float z);
void moveAlongLine(float toX, float toY, float toZ);

#endif