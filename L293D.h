#ifndef L293D_H
#define L293D_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
//#include "pins_arduino.h"

#include <inttypes.h>


#define CCW 0
#define CW 1
#define MAX_PWM 255
#define MIN_PWM 0

/****************************************/
/* LowLevel L293D Library				*/
/*										*/
/* Diese Library stellt rudimentaere	*/
/* Funktionen des L293D zur ver-		*/
/* fuegung fuer einen Motor !			*/
/*										*/
/****************************************/
class L293D {
public:
	/**
	* Konstruktor
	*
	* @param pwm	Pin fuer PWM
	* @param dirA	Pin fuer DIRA
	* @param dirB	Pin fuer DIRB
	*/
	L293D(int pwm, int dirA, int dirB);
	
	/**
	* Initialisierungsroutine, muss im 	
	* setup() aufgerufen werden
	* Mit dem Parameter kann man die Dreh-
	* richtung des Motors grundsaetzlich
	* aendern
	* @param reverse Default=False,CW = CW
	*				wenn TRUE dann CW = CCW
	*/
	void Init(bool reverse = false);
	
	/**
	* Motorgeschwindigkeit einstellen
	* Wert kann zwischen -255 und +255 liegen
	* Negative Werte Drehrichtung = CCW
	* Posotive Werte Drehrichtung = CW
	*
	* @param speed Wert zwischen -255 und +255
	*/
	void Speed(int speed);

	/**
	* Motorgeschwindigkeit einstellen
	* Wert kann zwischen 0 und +255 liegen
	*
	* @param speed Wert zwischen 0 und +255
	* @param dir CW oder CCW
	*/	
	void Speed(int speed, int dir);
	
	/**
	* Stoppt den Motor
	* Wenn die Bremse aktiviert werden
	* DIRA + DIRB auf HIGH gesetzt
	* Default = false
	*
	* @param withBreak Default=false, Bremse nutezn
	*/
	void Stop(bool withBreak = false);
	
private:
	int _pwm, _dirA, _dirB;
	bool _reverse;
};

#endif

