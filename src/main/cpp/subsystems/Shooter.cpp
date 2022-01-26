#include "subsystems/Shooter.h"
#include "Robot.h"
#include "Util.h"
#include "ID.h"
#include <frc2/command/InstantCommand.h>
#include <frc2/command/FunctionalCommand.h>
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"
#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <ctre/phoenix/motorcontrol/InvertType.h>

using ctre::phoenix::motorcontrol::ControlMode;

Shooter::Shooter() :
m_FeederButton(BUTTON_L(FEEDER_BUTTON)),
m_FlywheelToggle(BUTTON_L(FLYWHEEL_BUTTON)),
m_FlywheelFront(FLYWHEEL_FRONT),
m_FlywheelBack(FLYWHEEL_BACK),
m_Feeder(FEEDER)
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

    /*m_FlywheelToggle.WhenPressed(frc2::InstantCommand( [&] { 
        DebugOutF(std::to_string((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) / 2));
        m_FlywheelFront.Set(ControlMode::PercentOutput, ((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) / 2));
    }));
    */

   m_FlywheelToggle.WhenHeld(frc2::FunctionalCommand( [&]{}, [&]{ //onExecute

            double dialSpeed = ((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) / 2);
            
            m_FlywheelFront.Set(ControlMode::PercentOutput, dialSpeed);

        }, [&](bool e){ //onEnd

                m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
        }, [&]{ return false; }, {}
   ));

    m_FlywheelToggle.WhenReleased(frc2::InstantCommand( [&] { 
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
    }));
}

double Shooter::FlywheelSpeed(){
    return m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048;
}