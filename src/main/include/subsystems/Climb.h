#pragma once
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>
#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/WaitUntilCommand.h>
#include <frc2/command/FunctionalCommand.h>
#include "ID.h"

#include <frc2/command/button/Button.h>
#include <frc2/command/SubsystemBase.h>

class Climb : public frc2::SubsystemBase {
public:
    Climb();

    void ClimbInit();
    void BreakMode(bool on);

    frc2::FunctionalCommand ManualClimb();

    bool isMainUnlocked();
    bool isManualUnlocked();

    // boolean pastcurrentstep(n) {
    //     return current dial position >= n
    // }
    
    // seq(
    //     waituntil { pastcurrentstep(1) }
    //     FunctionaCommand([]w[]w[]w[]w][], {&this}),
    //     waituntil { pastcurrentstep(2) }
    // )

private:
    ctre::phoenix::motorcontrol::can::TalonSRX m_PivotArm;
    ctre::phoenix::motorcontrol::can::TalonFX m_PullUpArm;

    frc2::Button m_AutoLock;
};