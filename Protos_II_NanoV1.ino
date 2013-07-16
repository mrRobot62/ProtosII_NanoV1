///////////////////////////////////////////////////////////////////////////
//
//    ____  ____  ____ __________ _____    ________   _   __
//   / __ \/ __ \/ __ /_  __/ __ / ___/   /  _/  _/  / | / ____ _____  ____
//  / /_/ / /_/ / / / // / / / / \__ \    / / / /   /  |/ / __ `/ __ \/ __ \
// / ____/ _, _/ /_/ // / / /_/ ___/ /  _/ /_/ /   / /|  / /_/ / / / / /_/ /
///_/   /_/ |_|\____//_/  \____/____/  /___/___/  /_/ |_/\__,_/_/ /_/\____/
//
///////////////////////////////////////////////////////////////////////////

#include <os_wrap.h>
#include <Logging.h>
#include <SR04.h>
#include "Motoren.h"
#include "Constants.h"

osw_task taskCNY70, taskUltraschall, taskMotor, taskIdle, taskPattern;
osw_priority_q priQ;
osw_delay taskDelay;
volatile bool isWhite[2] = {false,false};

Motoren antrieb = Motoren(PWMA, PWMB, DIR1A, DIR2A, DIR1B, DIR2B);
SR04* us[4];
int distances[4];

int leds[4];


#define MAX_NUM_PRI (3)

typedef struct {
	int A,B;
	word delay;
	char prio;
} T_SPEED;

volatile int x;


void setup()
{
	Log.Init(4,38400);

	Log.Info("______ ______ _______ _______ _______ _______      _______ _______      _______   "CR);
	Log.Info("|   __ |   __ |       |_     _|       |     __|    |_     _|_     _|    |    |  .---.-.-----.-----."CR);
	Log.Info("|    __|      |   -   | |   | |   -   |__     |     _|   |_ _|   |_     |       |  _  |     |  _  |"CR);
	Log.Info("|___|  |___|__|_______| |___| |_______|_______|    |_______|_______|    |__|____|___._|__|__|_____|"CR);

	if (priQ.priQcreate("SPEED_Q", sizeof(T_SPEED), 10, MAX_NUM_PRI) != OSW_OK) {
		Log.Error("SPEED_Q create failed"CR);
	}

	taskIdle.taskCreate("taskIdle", TASK_Idle, OSW_OK, 250);
	taskPattern.taskCreate("taskPattern", TASK_Pattern, OSW_OK, 2500);
	taskCNY70.taskCreate("taskCNY70", TASK_CNY70, OSW_OK, 100);
	taskUltraschall.taskCreate("taskUltraschall", TASK_Ultraschall, OSW_OK, 250);
	taskMotor.taskCreate("taskMotor", TASK_Motor, OSW_OK, 200);

	// CNY70 initialisieren ------------------------
	pinMode(CNY0, INPUT);
	pinMode(CNY1, INPUT);
	// Pullup einschalten
	digitalWrite(CNY0, HIGH);
	digitalWrite(CNY1, HIGH);

	attachInterrupt(0,ISR_INT0,FALLING);
	attachInterrupt(1,ISR_INT1,FALLING);

	//-----------------------------------------------
	// Ultraschall ----------------------------------
	us[0] = new SR04(ECHO1, TRIG1, SR04_TIMEOUT);
	us[1] = new SR04(ECHO2, TRIG2, SR04_TIMEOUT);
	us[2] = new SR04(ECHO3, TRIG3, SR04_TIMEOUT);
	us[3] = new SR04(ECHO4, TRIG4, SR04_TIMEOUT);

	osw_list_tasks();
	taskCNY70.activate();
	taskUltraschall.activate();
	
	isWhite[0] = isWhite[1] = false;
}

void loop()
{
	randomSeed(millis());
	osw_tasks_go();
}


//******************************************************************
// TASKS
//******************************************************************

void* TASK_CNY70(void* _pData)
{
	int result;
	T_SPEED currentSpeed;
	bool isTmpWhite[2];
	//
	// den Interrupt deaktivieren
	detachInterrupt(0);
	detachInterrupt(1);
	// task deaktivieren um doppelte Aufrufe zu vermeiden
	taskCNY70.deactivate();



	//
	// Die eigentliche Tabelle wird kopiert, somit wird vermieden
	// das während der Task abgearbeitet wird, sich die Werte
	// in der Tabelle ändern und es zu unerwünschten Effekten
	// kommt
	isTmpWhite[0] = isWhite[0];
	isTmpWhite[1] = isWhite[1];

	if (!isTmpWhite[0] && !isTmpWhite[1]) {
		taskCNY70.activate();
		attachInterrupt(0,ISR_INT0,FALLING);
		attachInterrupt(1,ISR_INT1,FALLING);
		return OSW_NULL;
	}

	taskPattern.deactivate();


	//
	// rückwärts
	currentSpeed.prio = 3;
	currentSpeed.A = -255;
	currentSpeed.B = -255;
	currentSpeed.delay = 200;
	result = priQ.priQsend(3, (char*)&currentSpeed, sizeof(currentSpeed), OSW_PRIQ_BUMP_LOW_HEAD_FULL);

	// links & rechts
	if (isTmpWhite[0] && isTmpWhite[1])  {
		// zufällig links oder rechts drehen lassen
		isTmpWhite[random(2)]=false;
	}

	// links weiss
	if (isTmpWhite[0])	{
		currentSpeed.A = 255;
		currentSpeed.B = -255;
	}

	// rechts weiss
	if (isTmpWhite[1])  {
		currentSpeed.A = -255;
		currentSpeed.B = 255;
	}

	currentSpeed.delay = 200;
	result = priQ.priQsend(3, (char*)&currentSpeed, sizeof(currentSpeed), OSW_PRIQ_BUMP_LOW_HEAD_FULL);
	// vorwärts
	currentSpeed.A = 255;
	currentSpeed.B = 255;
	currentSpeed.delay = 0;
	result = priQ.priQsend(currentSpeed.prio, (char*)&currentSpeed, sizeof(currentSpeed), OSW_PRIQ_BUMP_LOW_HEAD_FULL);
	Log.Info("=== TASK_CNY70"CR);

	// Tabelle resetten
	isWhite[0] = isWhite[1] = false;

	taskCNY70.activate();
	attachInterrupt(0,ISR_INT0,FALLING);
	attachInterrupt(1,ISR_INT1,FALLING);
	return OSW_NULL;
}

void* TASK_Ultraschall(void* _pData)
{
	T_SPEED currentSpeed;

	// um Interferenzen zu vermeiden wird erst links, vorne rechts, rechts, vorne links
	// gemessen
	distances[0] = us[0]->Distance();	// links
	distances[2] = us[2]->Distance();	// vRechts
	distances[3] = us[3]->Distance();	// rechts
	distances[1] = us[1]->Distance();	// vLinks



	if ( (distances[0] == 999 || distances[0] > MAX_DISTANCE_SIDE) &&
	(distances[1] == 999 || distances[1] > MAX_DISTANCE_SIDE) &&
	(distances[2] == 999 || distances[2] > MAX_DISTANCE_SIDE) &&
	(distances[3] == 999 || distances[3] > MAX_DISTANCE_SIDE)) {
		return OSW_NULL;
	}


	currentSpeed.prio = 2;
	currentSpeed.A = 220;
	currentSpeed.B = 250;
	currentSpeed.delay = 50;
	int result = priQ.priQsend(currentSpeed.prio, (char*)&currentSpeed, sizeof(currentSpeed), OSW_PRIQ_BUMP_LOW_HEAD_FULL);

	Log.Info("### TASK_Ultraschall"CR);
	taskPattern.deactivate();
	return OSW_NULL;
}

void* TASK_Idle(void* _pData)
{
	T_SPEED currentSpeed;
	taskPattern.activate();
	currentSpeed.prio = 0;
	currentSpeed.A = 200;
	currentSpeed.B = 200;
	currentSpeed.delay = 0;
	int result = priQ.priQsend(currentSpeed.prio, (char*)&currentSpeed, sizeof(currentSpeed), OSW_PRIQ_BUMP_LOW_HEAD_FULL);

	//	Log.Info("TASK_Idle"CR);

	return OSW_NULL;
}

void* TASK_Pattern(void* _pData)
{
	T_SPEED currentSpeed;
	randomSeed(millis());
	byte pattern = random(6);
	currentSpeed.prio = 1;

	switch(pattern) {
		case 0:
		currentSpeed.A = 200;
		currentSpeed.B = 250;
		break;

		case 1:
		currentSpeed.A = 250;
		currentSpeed.B = 200;
		break;

		case 2:
		currentSpeed.A = 100;
		currentSpeed.B = 200;
		break;

		case 3:
		currentSpeed.A = 200;
		currentSpeed.B = 100;
		break;

		case 4:
		currentSpeed.A = -200;
		currentSpeed.B = 200;
		break;

		case 5:
		currentSpeed.A = 200;
		currentSpeed.B = -200;
		break;
	}

	currentSpeed.delay = 100;
	int result = priQ.priQsend(currentSpeed.prio, (char*)&currentSpeed, sizeof(currentSpeed), OSW_PRIQ_BUMP_LOW_HEAD_FULL);
	return OSW_NULL;
}

void* TASK_Motor(void* _pData)
{
	T_SPEED currentSpeed;
	static osw_delay delay;
	int wait = 1;
	int pri;

	while (1) {
		int result = priQ.priQreceive(&pri, (char*)&currentSpeed, sizeof(currentSpeed));

		if (result == OSW_SINGLE_THREAD) {
			break;    //return OSW_NULL;
		}

		//        Log.Info("MOTOR new speed [A:%d, B:%d, P:%d]"CR, currentSpeed.A, currentSpeed.B, currentSpeed.prio);
		antrieb.Speed(currentSpeed.A,currentSpeed.B);

		if (currentSpeed.delay > 0) {
			taskMotor.deactivate();
			//            Log.Info("DELAY %d", currentSpeed.delay);

			while (taskDelay.delay(currentSpeed.delay) != OSW_OK) {
				;
			}

			//            Log.Info(" finished"CR);
			taskMotor.activate();

		}

	}

	//Log.Info("*************************"CR);
	return OSW_NULL;
}

void ISR_INT0()
{
	isWhite[0] = true;
}

void ISR_INT1()
{
	isWhite[1] = true;
}