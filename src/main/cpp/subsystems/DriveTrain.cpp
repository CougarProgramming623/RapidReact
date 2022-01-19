#include "subsystems/DriveTrain.h"
#include "Robot.h"
#include "ID.h"
#include <ctre/phoenix/motorcontrol/ControlMode.h>

using ctre::phoenix::motorcontrol::ControlMode;


void DriveForward() {
    //Robot::GetRobot()->m_DriveTrain.BaseDrive(.15);
    DebugOutF("Forward");
}

void StopDrive() {
    //Robot::GetRobot()->m_DriveTrain.BaseDrive(0);
    DebugOutF("Stop");
}

DriveTrain::DriveTrain():
    m_FrontLeft(DRIVE_FRONT_LEFT),
    m_FrontRight(DRIVE_FRONT_RIGHT),
    m_BackLeft(DRIVE_BACK_LEFT),
    m_BackRight(DRIVE_BACK_RIGHT),
    m_DriveButton(Robot::GetRobot()->GetButtonBoard(), 1)
{
    
};


void DriveTrain::DriveInit(){
    m_DriveButton.WhileHeld(DriveForward, {});
    m_DriveButton.WhenReleased(StopDrive, {});
}

void DriveTrain::BaseDrive(double power){
    m_FrontLeft.Set(ControlMode::PercentOutput, power);
    m_FrontRight.Set(ControlMode::PercentOutput, power);
    m_BackLeft.Set(ControlMode::PercentOutput, power);
    m_BackRight.Set(ControlMode::PercentOutput, power);
}