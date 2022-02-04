#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#define COB_KEY_ENABLED "/COB/enabled"
#define COB_KEY_FOD "/COB/FOD"
#define COB_KEY_FLYWHEEL_SPEED "/COB/flywheelSpeed"
#define COB_KEY_MOTER_SPEED "/COB/moterSpeed"
#define COB_KEY_DISTANCE "/COB/distance"

#define LIMELIGHT_HEIGHT 69
#define TARGET_HEIGHT 104
#define LIMELIGHT_ANGLE  5

class COB
{

public:

inline nt::NetworkTableInstance& GetTable() {return m_Table;}

private:
    nt::NetworkTableInstance m_Table = nt::NetworkTableInstance::GetDefault();
};