

#pragma once

#define ROBOT_SABER
//#define ROBOT_GEORGE


#ifdef ROBOT_SABER

#define DRIVE_FRONT_LEFT 33
#define FLYWHEEL -1
#define FEEDER -1
#define FEEDER_BUTTON -1
#define FLYWHEEL_BUTTON -1

#endif

#ifdef ROBOT_GEORGE

#define DRIVE_FRONT_LEFT -77

#endif