//Board = Arduino Mini w/ ATmega328
#define __AVR_ATmega328P__
#define _VMDEBUG 1
#define ARDUINO 105
#define F_CPU 16000000L
#define __AVR__
extern "C" void __cxa_pure_virtual() {;}

//
//
void* TASK_CNY70(void* _pData);
void* TASK_Ultraschall(void* _pData);
void* TASK_Idle(void* _pData);
void* TASK_Pattern(void* _pData);
void* TASK_Motor(void* _pData);
void ISR_INT0();
void ISR_INT1();

#include "C:\Apps\Arduino-1.0.5-windows\hardware\arduino\variants\eightanaloginputs\pins_arduino.h" 
#include "C:\Apps\Arduino-1.0.5-windows\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Projekte\Dropbox\Robocup 2012\99_Bernd\Work\SumoRobot\Protos_II_NanoV1\Protos_II_NanoV1.ino"
#include "C:\Projekte\Dropbox\Robocup 2012\99_Bernd\Work\SumoRobot\Protos_II_NanoV1\Constants.h"
#include "C:\Projekte\Dropbox\Robocup 2012\99_Bernd\Work\SumoRobot\Protos_II_NanoV1\L293D.cpp"
#include "C:\Projekte\Dropbox\Robocup 2012\99_Bernd\Work\SumoRobot\Protos_II_NanoV1\L293D.h"
#include "C:\Projekte\Dropbox\Robocup 2012\99_Bernd\Work\SumoRobot\Protos_II_NanoV1\Motoren.cpp"
#include "C:\Projekte\Dropbox\Robocup 2012\99_Bernd\Work\SumoRobot\Protos_II_NanoV1\Motoren.h"
