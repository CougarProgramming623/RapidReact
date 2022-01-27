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

const double P = 0.14314;
const double E = 0;
const double D = 0;
const double F = 0.10747;
const double I = 0;

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
        Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/ifShooting").SetBoolean(true);

    }));

    m_FeederButton.WhenReleased(frc2::InstantCommand( [&] { 
        m_Feeder.Set(ControlMode::PercentOutput, 0);
        Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/ifShooting").SetBoolean(false);

    }));
}

void Shooter::FlywheelButton(){

    /*m_FlywheelToggle.WhenPressed(frc2::InstantCommand( [&] { 
        DebugOutF(std::to_string((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) / 2));
        m_FlywheelFront.Set(ControlMode::PercentOutput, ((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) / 2));
    }));
    */

   m_FlywheelToggle.WhenHeld(frc2::FunctionalCommand( [&]{}, [&]{ //onExecute

            double dialSpeed = ((int)((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1)  / 2 * 6001)); //rpm
            double targetSpeed = dialSpeed / 600 * 2048; //ticks per second
            Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/flywheelSpeedSetpoint").SetDouble(dialSpeed);

                m_FlywheelFront.Set(ControlMode::Velocity, targetSpeed);

        }, [&](bool e){ //onEnd

                m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
        }, [&]{ return false; }, {}
   ));

        m_FlywheelToggle.WhenReleased(frc2::InstantCommand( [&] { 
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
    }));
}

double Shooter::FlywheelSpeed(){
    return m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048; //rpm
}