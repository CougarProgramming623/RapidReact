#include "subsystems/Shooter.h"
#include "Robot.h"
#include "Util.h"
#include "ID.h"
#include <frc2/command/InstantCommand.h>
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"
#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <ctre/phoenix/motorcontrol/InvertType.h>

using ctre::phoenix::motorcontrol::ControlMode;

Shooter::Shooter() :

m_FlywheelFront(FLYWHEEL_FRONT),
m_FlywheelBack(FLYWHEEL_BACK),
m_Feeder(FEEDER),
m_FeederButton( [&] { return Robot::GetRobot()->GetButtonBoard().GetRawButton(FEEDER_BUTTON);}),
m_FlywheelToggle( [&] { return Robot::GetRobot()->GetButtonBoard().GetRawButton(FLYWHEEL_BUTTON);})
{}

void Shooter::ShooterInit(){

    FeederButton();
    FlywheelButton();
    m_FlywheelBack.Set(ControlMode::Follower, FLYWHEEL_FRONT);
    m_FlywheelBack.SetInverted(ctre::phoenix::motorcontrol::InvertType::OpposeMaster);
}

void Shooter::FeederButton(){
    m_FeederButton.WhenPressed(frc2::InstantCommand( [&] { 
        m_Feeder.Set(ControlMode::PercentOutput, 0.5);
    }));

    m_FeederButton.WhenReleased(frc2::InstantCommand( [&] { 
        m_Feeder.Set(ControlMode::PercentOutput, 0);
    }));
}

void Shooter::FlywheelButton(){

    m_FlywheelToggle.WhenPressed(frc2::InstantCommand( [&] { 
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0.1);
    }));

    m_FlywheelToggle.WhenReleased(frc2::InstantCommand( [&] { 
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
    }));
}