#include "servo.h"

#include <pca9685.h>
#include <wiringPi.h>

#include <iostream>
#include <cstdlib>

#define PIN_BASE  300
#define I2C_ADDR  0x40
// PWM Pulse Frequency (Hz)
#define PWM_FREQ  50
// PWM Pulse Period (us)
#define PWM_PER   (1000000/PWM_FREQ)
// Servo minimum pulse width (us)
#define MIN_PULSE 900
// Servo maximum pulse width (us)
#define MAX_PULSE 2100
// Servo centering pulse width (us)
#define CEN_PULSE ((MIN_PULSE+MAX_PULSE)/2)
// Servo angle difference at min/max pulse (degrees)
#define MAX_ANGLE 64.0f
// Maximum ticks
#define MAX_TICKS 4095

int fd = -1;

void servoInit()
{
	fd = pca9685Setup(PIN_BASE, I2C_ADDR, PWM_FREQ);
	if (fd < 0)
	{
		std::cerr << "Error initializing servos" << std::endl;
		std::exit(1);
	}
	pca9685PWMReset(fd);
}

void setServo(uint8_t channel, float angle)
{
	// Pulse width (us)
	float pulseWidth = (angle / MAX_ANGLE) * (MAX_PULSE - CEN_PULSE);
	// PWM ticks
	uint16_t ticks = (uint16_t) ((pulseWidth / PWM_PER) * MAX_TICKS);
	pwmWrite(PIN_BASE + channel, ticks);
}