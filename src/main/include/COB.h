#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#define COB_KEY_ENABLED "/COB/enabled"
#define COB_KEY_FOD "/COB/FOD"
#define COB_KEY_IS_TELE "/COB/isTele"
#define COB_KEY_FLYWHEEL_SPEED "/COB/flywheelSpeed"
#define COB_KEY_MOTER_SPEED "/COB/moterSpeed"
#define COB_KEY_DISTANCE "/COB/distance"
#define COB_KEY_IS_RED "/FMSInfo/IsRedAlliance"
#define COB_KEY_LIME_LIGHT_TX "/limelight/tx" // [-27,27]
#define COB_KEY_LIME_LIGHT_TV "/limelight/tv"

#define LIMELIGHT_HEIGHT 71.755 //cm
#define TARGET_HEIGHT 264 //cm
#define LIMELIGHT_ANGLE  45 //degree
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