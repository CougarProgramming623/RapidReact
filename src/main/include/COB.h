#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#define COB_KEY_ENABLED "/COB/enabled"
#define COB_KEY_FOD "/COB/FOD"


class COB
{

public:

inline nt::NetworkTableInstance& GetTable() {return m_Table;}

private:
    nt::NetworkTableInstance m_Table = nt::NetworkTableInstance::GetDefault();
};