#pragma once

#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <frc2/command/button/Button.h>
#include <frc2/command/InstantCommand.h>
#include "Util.h"
#include <frc2/command/Command.h>
#include <frc2/command/SubsystemBase.h>
#include "./commands/DriveWithJoystick.h"
#include "./Util.h"

using ctre::phoenix::motorcontrol::can::TalonFX;

class DriveTrain : public frc2::SubsystemBase {
public: 
    DriveTrain();
    void BaseDrive(double power);
    void DriveInit();
    void BreakMode(bool on);
    void CartesianDrive(double y, double x, double rotation, double angle, bool FOD);


    void UseVelocityPID();
    void UseMagicPID();
    void UsePostionPID();
    void SetPID(double E, double P, double I, double D, double F);
    

    void DriveToPosition(double x);


    inline TalonFX& GetFrontL(){return m_FrontLeft;}
    inline TalonFX& GetFrontR(){return m_FrontRight;}
    inline TalonFX& GetBackL(){return m_BackLeft;}
    inline TalonFX& GetBackR(){return m_BackRight;}

private:
    TalonFX m_FrontLeft;
    TalonFX m_BackLeft;
    TalonFX m_FrontRight;
    TalonFX m_BackRight;

    frc2::Button m_DriveButton;
    frc2::Button m_FODToggle;

};