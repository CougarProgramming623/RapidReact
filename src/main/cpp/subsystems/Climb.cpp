#include "subsystems/Climb.h"
#include "Robot.h"
#include <ctre/phoenix/motorcontrol/NeutralMode.h>
#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include "Util.h"


using ctre::phoenix::motorcontrol::ControlMode;


Climb::Climb() :
    m_PivotArm(CLIMBPIVOT),
    m_PullUpArm(CLIMBPULLUP),
    m_AutoLock(BUTTON_L(11)),
    m_TeleLock(BUTTON_L(10)),
    m_MainLock(BUTTON_L(9))
{}


frc2::FunctionalCommand* Climb::ManualClimb() {
    return new frc2::FunctionalCommand(

        [&]{
            m_PullUpArm.Set(ControlMode::PercentOutput,0);
            m_PivotArm.Set(ControlMode::PercentOutput,0);
        }, //On Init
        [&]{
            double pullup = Robot::GetRobot()->GetButtonBoard().GetRawAxis(2);
            pullup = 0.1 > abs(pullup) ? 0 : pullup;
            //DebugOutF("pullup:" + std::to_string(pullup));
            //DebugOutF("Low limit: " + m_LowBound.Get());
            if(pullup > 0 && m_LowBound.Get() == 1){
                
                    m_PullUpArm.Set(ControlMode::PercentOutput, pullup);
                
            } else if (pullup < 0) {
                //DebugOutF("Current: " + std::to_string(m_PullUpArm.GetSelectedSensorPosition()) + " , min: " + std::to_string(m_MinSensorPosition));
                if(m_PullUpArm.GetSelectedSensorPosition() > m_MinSensorPosition){
                    m_PullUpArm.Set(ControlMode::PercentOutput, pullup);
                } else {
                    m_PullUpArm.Set(ControlMode::PercentOutput,0);
                }
            } else {
                m_PullUpArm.Set(ControlMode::PercentOutput,0);
            }

            // DebugOutF("current voltage on pot" + std::to_string(m_Analog.GetValue()));
            double pivot = Robot::GetRobot()->GetButtonBoard().GetRawAxis(3);
            // DebugOutF("setting pivot to " + std::to_string(pivot));
            pivot = 0.1 > abs(pivot) ? 0 : pivot;
            if(m_Analog.GetValue() > 1250) {
                m_PivotArm.Set(ControlMode::PercentOutput, pivot);
            } else if (pivot > 0) {
                m_PivotArm.Set(ControlMode::PercentOutput, pivot);
            } else {
                m_PivotArm.Set(ControlMode::PercentOutput,0);
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
    m_MinSensorPosition = m_PullUpArm.GetSelectedSensorPosition() - 281138;
    (m_MainLock && m_TeleLock).WhileActiveOnce(ManualClimb());
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