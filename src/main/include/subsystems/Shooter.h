#pragma once

#include <frc2/command/button/Button.h>
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"
#include "ctre/phoenix/motorcontrol/can/TalonSRX.h"
#include <frc/Joystick.h>

class Shooter {
    public:

        Shooter();
        void ShooterInit();
        void FeederButton();
        void FlywheelButton();

    private:

        frc2::Button m_FeederButton;
        frc2::Button m_FlywheelToggle;
        frc2::Button m_FlywheelDial;

        ctre::phoenix::motorcontrol::can::TalonFX m_FlywheelFront;
        ctre::phoenix::motorcontrol::can::TalonFX m_FlywheelBack;
        ctre::phoenix::motorcontrol::can::TalonFX m_Feeder;
    
};