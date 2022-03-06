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
m_FlywheelFront(FLYWHEEL_FRONT), //master
m_FlywheelBack(FLYWHEEL_BACK), //slave
m_Feeder(FEEDER),

m_feederButton(BUTTON_L(FEEDER_BUTTON)),
m_speedDial([&] {return Robot::GetRobot()->GetButtonBoard().GetRawAxis(0);}),
m_shootSpeed(BUTTON_L(FLYWHEEL_BUTTON_BY_SPEED))
{}


void Shooter::ShooterInit(){
    DebugOutF("Shooter Init");
    FeederButton();
    FlywheelButton();
    
    m_FlywheelBack.Set(ControlMode::Follower, FLYWHEEL_FRONT);
    m_FlywheelBack.SetInverted(ctre::phoenix::motorcontrol::InvertType::OpposeMaster);
}


frc2::FunctionalCommand Shooter::ShootOnReadyCommand(){
    return frc2::FunctionalCommand(
        [&]{DebugOutF("Shooting when ready");}, //OnInit
        [&]{
            double setpoint = m_FlywheelFront.GetClosedLoopTarget();
            //DebugOutF(std::to_string(abs(m_FlywheelFront.GetSelectedSensorVelocity() - setpoint) * 600 / 2048));
            if (abs(m_FlywheelFront.GetSelectedSensorVelocity() - setpoint) * 600 / 2048 <= 70
                && m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048 >= 100
                && setpoint * 600 / 2048 >= 100){
                m_Feeder.Set(ControlMode::PercentOutput, -0.5);
                //DebugOutF("Shooting");
            } else {
                m_Feeder.Set(ControlMode::PercentOutput, 0);
                //DebugOutF("Not shooting");
            }
        },
        [&](bool e){m_Feeder.Set(ControlMode::PercentOutput, 0);},
        [&]{return false;}
    );
}


void Shooter::FeederButton(){
    m_feederButton.WhenPressed(frc2::InstantCommand( [&] { 
        m_Feeder.Set(ControlMode::PercentOutput, -1);
    }));

    m_feederButton.WhenReleased(frc2::InstantCommand( [&] { 
        m_Feeder.Set(ControlMode::PercentOutput, 0);
    }));
}
void Shooter::FlywheelButton(){
    m_shootSpeed.WhenHeld(frc2::FunctionalCommand( [&]{}, [&]{ //onExecute
        m_FlywheelFront.Set(ControlMode::PercentOutput, (Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) * 2);
    }, [&](bool e){ //onEnd
            m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
    }, [&]{ return false; }, {})); 

    m_shootSpeed.WhenReleased(frc2::InstantCommand( [&] { 
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
    }));
}

double Shooter::FlywheelRPM() {
    return m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048;
}


frc2::InstantCommand Shoot(){
    return frc2::InstantCommand([&]{});
}


frc2::FunctionalCommand Shooter::ScaleToDistanceCommand() {
    return frc2::FunctionalCommand([&] { //onInit

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
        }
    );
};

frc2::ParallelRaceGroup Shooter::ShootingCommand(){
    return frc2::ParallelRaceGroup(
        ScaleToDistanceCommand(),
        ShootOnReadyCommand(),
        frc2::WaitCommand(4_s)
    );
}