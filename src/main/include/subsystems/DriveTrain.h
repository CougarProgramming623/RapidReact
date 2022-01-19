#pragma once

#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <frc2/command/button/JoystickButton.h>
#include "Util.h"

using ctre::phoenix::motorcontrol::can::TalonFX;

class DriveTrain {
public: 
    DriveTrain();
    void BaseDrive(double power);
    void DriveInit();
private:
    TalonFX m_FrontLeft;
    TalonFX m_BackLeft;
    TalonFX m_FrontRight;
    TalonFX m_BackRight;

    frc2::JoystickButton m_DriveButton;
};