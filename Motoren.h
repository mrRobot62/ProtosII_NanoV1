#ifndef MOTOREN_H
#define MOTOREN_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
//#include "pins_arduino.h"

#include <inttypes.h>
#include "L293D.h"

class Motoren {
public:
	Motoren(int pwmA, int pwmB, int dir1A, int dir1B, int dir2A, int dir2B);
	
	void Init (int reverseA=false, int reverseB=false);
	
	void Speed(int speedA, int speedB);
	void Speed(int speedA, int speedB, int dirA, int dirB);
	void Speed(int speedA, int speedB, int milliseconds);
	
	void StopA(int withBreak=false);
	void StopB(int withBreak=false);
	void StopAll(int withBreak=false);
	
private:
	L293D *m1;
	L293D *m2;
};

#endif

