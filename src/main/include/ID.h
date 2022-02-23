

#pragma once

#define ROBOT_SABER
//#define ROBOT_GEORGE


#ifdef ROBOT_SABER

#define DRIVE_FRONT_LEFT -1 //31
#define DRIVE_FRONT_RIGHT 32
#define DRIVE_BACK_LEFT 34
#define DRIVE_BACK_RIGHT 33
#define FLYWHEEL_FRONT 42
#define FLYWHEEL_BACK 41
#define FEEDER 3
#define FEEDER_BUTTON 2
#define FLYWHEEL_BUTTON_BY_DIAL 4
#define FLYWHEEL_BUTTON_BY_DISTANCE 5
#define FLYWHEEL_DIAL -1
#define SHOOTTIME 14
#define TESTBUTTON -1
#define READYSHOOT 3
#define CLIMBPULLUP 31 //Conflict with drive
#define CLIMBPIVOT 3

#endif

#ifdef ROBOT_GEORGE

#define DRIVE_FRONT_LEFT -1

#endif