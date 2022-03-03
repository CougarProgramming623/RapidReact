#pragma once

#include <frc2/command/button/Button.h>
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"
#include "ctre/phoenix/motorcontrol/can/TalonSRX.h"
#include <frc/Joystick.h>
#include <frc/filter/LinearFilter.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/ParallelRaceGroup.h>


class Shooter {
    public:

        Shooter();
        void ShooterInit();
        void FeederButton();
        void FlywheelButton();
        double FlywheelRPM();
        void ShootTime();
        void ScaleToDistance();
        void ShootOnReady();
        frc2::FunctionalCommand ShootOnReadyCommand();
        frc2::FunctionalCommand ScaleToDistanceCommand();
        frc2::ParallelRaceGroup ShootingCommand();


    private:

        frc::LinearFilter<double> runningAverage = frc::LinearFilter<double>::MovingAverage(10);
        ctre::phoenix::motorcontrol::can::TalonFX m_FlywheelFront;
        ctre::phoenix::motorcontrol::can::TalonFX m_FlywheelBack;
        ctre::phoenix::motorcontrol::can::TalonFX m_Feeder;
        frc2::Button m_FeederButton;
        frc2::Button m_FlywheelToggleByDial;
        frc2::Button m_FlywheelToggleByDistance;
        frc2::Button m_ShootTime;
        frc2::Button m_ReadyShoot;


};