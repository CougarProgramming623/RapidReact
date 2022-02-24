#include "subsystems/Shooter.h"
#include "Robot.h"
#include "Util.h"
#include "ID.h"
#include <frc2/command/InstantCommand.h>
#include <frc2/command/Command.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc2/command/WaitCommand.h>

#include <frc2/command/FunctionalCommand.h>
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"
#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <ctre/phoenix/motorcontrol/InvertType.h>

using ctre::phoenix::motorcontrol::ControlMode;

const double P = 0.25;
const double E = 0;
const double D = 0;
const double F = 0.05;
const double I = 0;

Shooter::Shooter() :
m_FeederButton(BUTTON_L(FEEDER_BUTTON)),
m_FlywheelToggleByDial(BUTTON_L(FLYWHEEL_BUTTON_BY_DIAL)),
m_FlywheelToggleByDistance(BUTTON_L(FLYWHEEL_BUTTON_BY_DISTANCE)),
m_ShootTime(BUTTON_L(16)),
m_FlywheelFront(FLYWHEEL_FRONT), //master
m_FlywheelBack(FLYWHEEL_BACK), //slave
m_Feeder(FEEDER),
m_ReadyShoot(BUTTON_L(READYSHOOT))
{}

void Shooter::ShooterInit(){
    DebugOutF("Shooter Init");
    FeederButton();
    FlywheelButton();
    ScaleToDistance();
    ShootTime();
    ShootOnReady();
    m_FlywheelBack.Set(ControlMode::Follower, FLYWHEEL_FRONT);
    m_FlywheelBack.SetInverted(ctre::phoenix::motorcontrol::InvertType::OpposeMaster);

}

void Shooter::ShootOnReady(){
    m_ReadyShoot.WhileHeld(frc2::FunctionalCommand(
        [&]{DebugOutF("Shooting when ready");}, //OnInit
        [&]{
            double setpoint = m_FlywheelFront.GetClosedLoopTarget();
            //DebugOutF(std::to_string(abs(m_FlywheelFront.GetSelectedSensorVelocity() - setpoint) * 600 / 2048));
            if (abs(m_FlywheelFront.GetSelectedSensorVelocity() - setpoint) * 600 / 2048 <= 70
                && m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048 >= 100
                && setpoint * 600 / 2048 >= 100){
                m_Feeder.Set(ControlMode::PercentOutput, 0.5);
                //DebugOutF("Shooting");
            } else {
                m_Feeder.Set(ControlMode::PercentOutput, 0);
                //DebugOutF("Not shooting");
            }
        },
        [&](bool e){m_Feeder.Set(ControlMode::PercentOutput, 0);},
        [&]{return false;}
    ));
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



   m_FlywheelToggleByDial.WhenHeld(frc2::FunctionalCommand( [&]{}, [&]{ //onExecute

            double dialSpeedRPM = ((int)((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1)  / 2 * 6001)); //rpm
            double targetSpeed = dialSpeedRPM / 600 * 2048; //ticks per second
            Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/flywheelSpeedSetpoint").SetDouble(dialSpeedRPM);

                m_FlywheelFront.Set(ControlMode::Velocity, targetSpeed);

        }, [&](bool e){ //onEnd

                m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
        }, [&]{ return false; }, {}
   )); //Functional Command End

        m_FlywheelToggleByDial.WhenReleased(frc2::InstantCommand( [&] { 
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
    }));
}

double Shooter::FlywheelRPM(){
    return m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048; //rpm
}
void Shooter::ShootTime(){ //button ID 13
    m_ShootTime.WhenPressed(frc2::ParallelRaceGroup(frc2::FunctionalCommand( [&]{}, [&]{ //onExecute
            double dialSpeed = ((int)((Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1)  / 2 * 6001)); //rpm
            double targetSpeed = dialSpeed / 600 * 2048; //ticks per second
            Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/flywheelSpeedSetpoint").SetDouble(dialSpeed);

                m_FlywheelFront.Set(ControlMode::Velocity, targetSpeed);

        }, [&](bool e){ //onEnd

                m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
        }, [&]{ return false; }, {}
   ), frc2::WaitCommand(5.0_s))); //cancels after 5 seconds

        m_FlywheelToggleByDial.WhenReleased(frc2::InstantCommand( [&] { 
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
    }));
}

void Shooter::ScaleToDistance(){
    m_FlywheelToggleByDistance.WhenHeld(frc2::FunctionalCommand([&] { //onInit

    }, [&]{ //onExecute

    double distance = Robot::GetRobot()->GetCOB().GetTable().GetEntry(COB_KEY_DISTANCE).GetDouble(0); //cm
    double RPM = distance * 4 + 70;

    double smoothRPM = runningAverage.Calculate(distance) * 4 + 70;

    
    Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/flywheelSpeedSetpoint").SetDouble(RPM);

    m_FlywheelFront.Set(ControlMode::Velocity, smoothRPM / 600 * 2048);

    
    }, [&] (bool e) { //onEnd
    m_FlywheelFront.Set(ControlMode::PercentOutput, 0);

    }, [&] {//isFinished
        return false;
    }, {}));
}