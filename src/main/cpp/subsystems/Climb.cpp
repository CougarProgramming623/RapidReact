#include "subsystems/Climb.h"
#include "Robot.h"
#include <ctre/phoenix/motorcontrol/NeutralMode.h>
#include <ctre/phoenix/motorcontrol/ControlMode.h>

Climb::Climb() :
    m_PivotArm(CLIMBPIVOT),
    m_PullUpArm(CLIMBPULLUP)
{}

bool Climb::isMainUnlocked() { return true; }//return Robot::GetRobot()->GetButtonBoard().GetRawButton(18); }

bool Climb::isManualUnlocked() { return Robot::GetRobot()->GetButtonBoard().GetRawButton(11); }

frc2::FunctionalCommand Climb::ManualClimb() {
    return frc2::FunctionalCommand(
        [&]{
            
        }, //On Init
        [&]{
            if (Climb::isMainUnlocked()/* && Climb::isManualUnlocked()*/) { //DISABLED TEMPORARILY
                // m_PullUpArm.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, Robot::GetRobot()->GetJoystick().GetRawAxis(1));
                // if(Robot::GetRobot()->GetButtonBoard().GetRawButton(13)){
                //     m_PivotArm.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 1);
                // } else {
                //     m_PivotArm.Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
                // }
            }
        }, //On Excute
        [&](bool e){
            
        }, //On End
        [&]{return false;}, //Is Finished
        {this}
    );
}

void Climb::ClimbInit(){
    BreakMode(true);
    SetDefaultCommand(Climb::ManualClimb());
}

void Climb::BreakMode(bool on){
    if (on){
        m_PullUpArm.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
        m_PivotArm.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    } else {
        m_PullUpArm.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
        m_PivotArm.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
    }
} 