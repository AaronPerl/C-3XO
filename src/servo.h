#ifndef __SERVO_H__
#define __SERVO_H__

#include <stdint.h>

void servoInit();
void setServo(uint8_t channel, float angle);

#endif