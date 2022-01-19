

#pragma once

#define ROBOT_SABER
//#define ROBOT_GEORGE


#ifdef ROBOT_SABER

#define DRIVE_FRONT_LEFT 31
#define DRIVE_FRONT_RIGHT 32
#define DRIVE_BACK_LEFT 34
#define DRIVE_BACK_RIGHT 33

#endif

#ifdef ROBOT_GEORGE

#define DRIVE_FRONT_LEFT -1

#endif