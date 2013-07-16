/*****************************************
	CONSTANTS
*****************************************/

// - Motoren----------------------------
#define PWMA 5 	//PC5
#define PWMB 6 	//PC6

#define DIR1A 9		//PB1
#define DIR2A 10	//PB10

#define DIR1B 11	//PB3
#define DIR2B 12	//PB4

#define DEFAULT_SPEED_L 180
#define DEFAULT_SPEED_R 180
#define FULL_SPEED 255
#define HIGH_SPEED 230
#define FAST_SPEED 200
#define SLOW_SPEED 130

// -- CNY70 ----------------------------
#define CNY0 2		// INT0 PD2
#define CNY1 3		// INT1 PD3

// -- SR04 4x---------------------------
#define ECHO1 A7
#define TRIG1 A6
#define ECHO2 A5
#define TRIG2 A4
#define TRIG3 A3
#define ECHO3 A2
#define TRIG4 A1
#define ECHO4 A0
#define SR04_TIMEOUT 15000L

#define MAX_DISTANCE_SIDE 15	// 15cm
#define MAX_DISTANCE_FRONT 20	// 20cm

// -- DEBUG LED ------------------------
#define LED2 4
#define LED3 5