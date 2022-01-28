#pragma once

#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <frc2/command/button/Button.h>
#include <frc2/command/InstantCommand.h>
#include "Util.h"
#include <frc2/command/Command.h>
#include <frc2/command/SubsystemBase.h>
#include "./commands/DriveWithJoystick.h"
#include "./Util.h"

using ctre::phoenix::motorcontrol::can::TalonFX;

class DriveTrain : public frc2::SubsystemBase {
public: 
    DriveTrain();
    void BaseDrive(double power);
    void DriveInit();
    void BreakMode(bool on);
    void CartesianDrive(double y, double x, double rotation, double angle);

    bool m_FOD = true;

private:
    TalonFX m_FrontLeft;
    TalonFX m_BackLeft;
    TalonFX m_FrontRight;
    TalonFX m_BackRight;

    frc2::Button m_DriveButton;
    frc2::Button m_FODToggle;
};