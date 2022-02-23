#include "subsystems/Climb.h"
#include "Robot.h"
#include <ctre/phoenix/motorcontrol/NeutralMode.h>

Climb::Climb() :
    m_PivotArm(CLIMBPIVOT),
    m_PullUpArm(CLIMBPULLUP)
{}

frc2::FunctionalCommand Climb::ManualClimb() {
    return frc2::FunctionalCommand(
        [&]{
            
        }, //On Init
        [&]{
            if (Climb::isMainUnocked() && Climb::isManualUnlocked()) {
                DebugOutF("Unlocked");
            }
        }, //On Excute
        [&](bool e){
            
        }, //On End
        [&]{return false;}, //Is Finished
        {this}
    );
}

void Climb::ClimbInit(){

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

bool isMainUnocked() { return Robot::GetRobot()->GetButtonBoard().GetRawButton(18); }
bool isManualUnlocked() { return Robot::GetRobot()->GetButtonBoard().GetRawButton(11); }