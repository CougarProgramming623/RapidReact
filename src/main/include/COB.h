#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#define COB_KEY_ENABLED "/COB/enabled"
#define COB_KEY_FOD "/COB/FOD"
#define COB_KEY_FLYWHEEL_SPEED "/COB/flywheelSpeed"

#define COB_KEY_NAVX_RESET "/COB/navXReset"
#define COB_KEY_ROBOT_ANGLE "/COB/robotAngle"
#define COB_KEY_DRIVE_MODE "/COB/driveMode"

class COB
{

public:

inline nt::NetworkTableInstance& GetTable() {return m_Table;}

private:
    nt::NetworkTableInstance m_Table = nt::NetworkTableInstance::GetDefault();
};