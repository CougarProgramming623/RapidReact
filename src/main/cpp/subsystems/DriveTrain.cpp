#include "subsystems/DriveTrain.h"
#include "Robot.h"
#include "ID.h"
#include <ctre/phoenix/motorcontrol/ControlMode.h>

using ctre::phoenix::motorcontrol::ControlMode;
using ctre::phoenix::motorcontrol::NeutralMode;

DriveTrain::DriveTrain():
    m_FrontLeft(DRIVE_FRONT_LEFT),
    m_FrontRight(DRIVE_FRONT_RIGHT),
    m_BackLeft(DRIVE_BACK_LEFT),
    m_BackRight(DRIVE_BACK_RIGHT),
    m_DriveButton(BUTTON_L(2))
{
    
};
void DriveTrain::BreakMode(bool on){
    if (on){
        m_FrontRight.SetNeutralMode(NeutralMode::Brake);
        m_FrontLeft.SetNeutralMode(NeutralMode::Brake);
        m_BackRight.SetNeutralMode(NeutralMode::Brake);
        m_BackLeft.SetNeutralMode(NeutralMode::Brake);
    } else {
        m_FrontRight.SetNeutralMode(NeutralMode::Coast);
        m_FrontLeft.SetNeutralMode(NeutralMode::Coast);
        m_BackRight.SetNeutralMode(NeutralMode::Coast);
        m_BackLeft.SetNeutralMode(NeutralMode::Coast);
    }
} 


void DriveTrain::DriveInit(){

    m_BackRight.SetInverted(true);
    m_FrontRight.SetInverted(true);



    m_DriveButton.WhileHeld(frc2::InstantCommand([&] {
        Robot::GetRobot()->m_DriveTrain.BaseDrive(.15);
        DebugOutF("Forward");
    }));
    m_DriveButton.WhenReleased(frc2::InstantCommand([&] {
        Robot::GetRobot()->m_DriveTrain.BaseDrive(0);
        DebugOutF("Stop");
    }));

    BreakMode(true);
}

void DriveTrain::BaseDrive(double power){
    m_FrontLeft.Set(ControlMode::PercentOutput, power);
    m_FrontRight.Set(ControlMode::PercentOutput, power);
    m_BackLeft.Set(ControlMode::PercentOutput, power);
    m_BackRight.Set(ControlMode::PercentOutput, power);
}