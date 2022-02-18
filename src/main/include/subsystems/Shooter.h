#pragma once

#include <frc2/command/button/Button.h>
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"
#include "ctre/phoenix/motorcontrol/can/TalonSRX.h"
#include <frc/Joystick.h>
#include <frc/filter/LinearFilter.h>

class Shooter {
    public:

        Shooter();
        void ShooterInit();
        void FeederButton();
        void FlywheelButton();
        double FlywheelSpeed();
        void ShootTime();
        void ScaleToDistance();
        void ShootOnReady();

        inline ctre::phoenix::motorcontrol::can::TalonFX& GetFlyBack() {return m_FlywheelBack;}
        inline ctre::phoenix::motorcontrol::can::TalonFX& GetFlyFront() {return m_FlywheelFront;}

    private:

        frc::LinearFilter<double> runningAverage = frc::LinearFilter<double>::MovingAverage(10);

        frc2::Button m_FeederButton;
        frc2::Button m_FlywheelToggleByDial;
        frc2::Button m_FlywheelToggleByDistance;
        frc2::Button m_FlywheelDial;
        frc2::Button m_ShootTime;
        frc2::Button m_ReadyShoot;

        ctre::phoenix::motorcontrol::can::TalonFX m_FlywheelFront;
        ctre::phoenix::motorcontrol::can::TalonFX m_FlywheelBack;
        ctre::phoenix::motorcontrol::can::TalonFX m_Feeder;
};