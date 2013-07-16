
#include <osw_min.h>
#include <SR04.h>
#include <Logging.h>

#include "Constants.h"


SR04 *us[4];
volatile int usDistances[4];	// Liste der USSensor-Entfernungen



osw_task taskLog, taskUltraschall;
osw_delay taskDelay;

void setup() {

    Log.Init(4,38400L);
    Log.Info(CR"----------------------------------------------"CR);
    Log.Info("PROTOS II - Arduino Nano V1"CR);
    Log.Info(CR"----------------------------------------------"CR);

    //
    // SR04 initialisieren ---------------------------
    // 15ms timeout
    us[0] = new SR04(ECHO1,TRIG1,SR04_TIMEOUT);	// links
    us[1] = new SR04(ECHO2,TRIG2,SR04_TIMEOUT);	// vorne links
    us[2] = new SR04(ECHO3,TRIG3,SR04_TIMEOUT); // vorne rechts
    us[3] = new SR04(ECHO4,TRIG4,SR04_TIMEOUT); // rechts


    // EVENTs registrieren -------------------------
    /*    osw_evt_register(1, evtWhiteRing);
        osw_evt_register(2, evtBackward);
        osw_evt_register(3, evtTurnLeft);
        osw_evt_register(4, evtTurnRight);
    */
    // TASKs generieren ----------------------------
    taskUltraschall.taskCreate("TASK_Ultraschall",TASK_Ultraschall, OSW_NULL, 100);
	taskLog.taskCreate("TASK_Log", TASK_Log, OSW_NULL,100);
    taskUltraschall.activate();
}

void loop() {
   osw_tasks_go();
    CheckUltraschall();
    //delay(100);
}

void CheckUltraschall() {
    if (usDistances[0] <= MAX_DISTANCE_SIDE) {
        //setSpeed(-SLOW_SPEED,FULL_SPEED,100,2);
        //Log.Info("US-LEFT    %d"CR, usDistances[0]);
    }

    if (usDistances[1] <= MAX_DISTANCE_FRONT) {
        //Log.Info("US-FRONT-L %d"CR, usDistances[1]);
    }

    if (usDistances[2] <= MAX_DISTANCE_FRONT) {
        //Log.Info("US-FRONT-R %d"CR, usDistances[2]);
    }

    if (usDistances[3] <= MAX_DISTANCE_SIDE) {
        //Log.Info("US-RIGHT   %d"CR, usDistances[3]);
    }
}

void FlashLED(int led, int count) {
    digitalWrite(led,LOW);
    delay(50);
    digitalWrite(led,HIGH);
    delay(50);
}

// TASKs -----------------------------------------------
void *TASK_Ultraschall(void *_pData) {
    for (int x=0; x < 4; x++) {
        usDistances[x] = us[x]->Distance();
    }
	return OSW_NULL;
}

void *TASK_Log(void *_pData) {
    Log.Info("US [%d, %d, %d, %d]"CR, usDistances[0],usDistances[1],usDistances[2],usDistances[3]);
	return OSW_NULL;
}
