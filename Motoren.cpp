#include "Motoren.h"

Motoren::Motoren(int pwmA, int pwmB, int dir1A, int dir2A, int dir1B, int dir2B){
	m1 = new L293D(pwmA, dir1A, dir2A);
	m2 = new L293D(pwmB, dir1B, dir2B);
}

void Motoren::Init (int reverseA, int reverseB) {
	m1->Init(reverseA);
	m2->Init(reverseB);
}

void Motoren::Speed(int speedA, int speedB) {
	m1->Speed(speedA);
	m2->Speed(speedB);
}

void Motoren::Speed(int speedA, int speedB, int dirA, int dirB) {
	m1->Speed(speedA,dirA);
	m2->Speed(speedB,dirB);
}

void Motoren::Speed(int speedA, int speedB, int milliseconds) {
	Speed(speedA,speedB);
	delay(milliseconds);
	StopAll(true);
}

void Motoren::StopA(int withBreak){
	m1->Stop(withBreak);
}

void Motoren::StopB(int withBreak){
	m2->Stop(withBreak);
}

void Motoren::StopAll(int withBreak){
	StopA(withBreak);
	StopB(withBreak);
}

