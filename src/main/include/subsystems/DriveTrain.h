#pragma once

#include <ctre/phoenix/motorcontrol/can/TalonFX.h>

class DriveTrain {
public: 
    DriveTrain::DriveTrain();


private:
    ctre::phoenix::motorcontrol::can::TalonFX m_FrontLeft;
    ctre::phoenix::motorcontrol::can::TalonFX m_BackLeft;
    ctre::phoenix::motorcontrol::can::TalonFX m_FrontRight;
    ctre::phoenix::motorcontrol::can::TalonFX m_BackRight;
};