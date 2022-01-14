#include "subsystems/Shooter.h"
#include "Robot.h"
#include "Util.h"
#include "ID.h"
#include <frc2/command/InstantCommand.h>
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"
#include <ctre/phoenix/motorcontrol/ControlMode.h>

using ctre::phoenix::motorcontrol::ControlMode;

Shooter::Shooter() :

m_Flywheel(FLYWHEEL),
m_Feeder(FEEDER),
m_FeederButton( [&] { return m_ButtonBoard.GetRawButton(-1);}),
m_FlywheelToggle( [&] { return m_ButtonBoard.GetRawButton(-1);})
{}

void Shooter::ShooterInit(){

    FeederButton();
    FlywheelButton();
}

void Shooter::FeederButton(){
    m_FeederButton.WhenPressed(frc2::InstantCommand( [&] { 
        m_Feeder.Set(ControlMode::Velocity, 0.1);
    }));

    m_FeederButton.WhenReleased(frc2::InstantCommand( [&] { 
        m_Feeder.Set(ControlMode::Velocity, 0);
    }));
}

void Shooter::FlywheelButton(){

    m_FlywheelToggle.WhenPressed(frc2::InstantCommand( [&] { 
        m_Flywheel.Set(ControlMode::Velocity, 0.1);
    }));

    m_FlywheelToggle.WhenReleased(frc2::InstantCommand( [&] { 
        m_Flywheel.Set(ControlMode::Velocity, 0);
    }));
}
