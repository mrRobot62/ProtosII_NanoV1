#include "L293D.h"

L293D::L293D(int pwm, int dirA, int dirB) {
	_pwm = pwm;
	_dirA = dirA;
	_dirB = dirB;
}

void L293D::Init(bool reverse) {
	pinMode(_pwm, OUTPUT);
	pinMode(_dirA, OUTPUT);
	pinMode(_dirB, OUTPUT);
	_reverse = reverse;
}

void L293D::Speed(int speed) {
	if (speed > 0) 
		Speed(speed,CW);
	else
		Speed(speed,CCW);
	
}

void L293D::Speed(int speed, int dir) {
	if (_reverse) dir = !dir;
	if (speed < 0) speed *= -1;
	speed = constrain(speed,MIN_PWM, MAX_PWM);
	switch(dir) {
		case CW:
			digitalWrite(_dirA, HIGH);
			digitalWrite(_dirB, LOW);
			break;
		case CCW:
			digitalWrite(_dirA, LOW);
			digitalWrite(_dirB, HIGH);
			break;
	}
	analogWrite(_pwm,speed);
}

void L293D::Stop(bool withBreak) {
	digitalWrite(_pwm,HIGH);
	digitalWrite(_dirA, withBreak);
	digitalWrite(_dirB, withBreak);
	analogWrite(_pwm,0);
}

