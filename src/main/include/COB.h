#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#define COB_KEY_ENABLED "/COB/enabled"
#define COB_KEY_FOD "/COB/FOD"
#define COB_KEY_FLYWHEEL_RPM "/COB/flywheelRPM"
#define COB_KEY_MOTER_SPEED "/COB/moterSpeed"
#define COB_KEY_DISTANCE "/COB/distance"

#define LIMELIGHT_HEIGHT 69 //cm
#define TARGET_HEIGHT 239 //cm
#define LIMELIGHT_ANGLE  40 //degree
#define COB_KEY_NAVX_RESET "/COB/navXReset"
#define COB_KEY_ROBOT_ANGLE "/COB/robotAngle"
#define COB_KEY_DRIVE_MODE "/COB/driveMode"

#define COB_KEY_MATCH_TIME "/COB/matchTime"
#define COB_KEY_TICKS "/COB/ticks"


class COB
{

public:

inline nt::NetworkTableInstance& GetTable() {return m_Table;}

private:
    nt::NetworkTableInstance m_Table = nt::NetworkTableInstance::GetDefault();
};