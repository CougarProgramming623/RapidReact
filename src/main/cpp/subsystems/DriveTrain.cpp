#include "subsystems/DriveTrain.h"
#include "Robot.h"
#include "ID.h"
#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <frc/drive/Vector2d.h>

const int kMAX_VELOCITY = 6380/60/10*2048;//RPM->Convert to RPS->Convert to RP100MS->Convert to TP100MS

using ctre::phoenix::motorcontrol::ControlMode;
using ctre::phoenix::motorcontrol::NeutralMode;

DriveTrain::DriveTrain():
    m_FrontLeft(DRIVE_FRONT_LEFT),
    m_FrontRight(DRIVE_FRONT_RIGHT),
    m_BackLeft(DRIVE_BACK_LEFT),
    m_BackRight(DRIVE_BACK_RIGHT),
    m_DriveButton(BUTTON_L(1)),
    m_FODToggle([&] {return Robot::GetRobot()->GetJoystick().GetRawButton(1);})
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

void Normalize(std::vector<double> wheelSpeeds) {
	double maxMagnitude = *std::max_element(wheelSpeeds.begin(), wheelSpeeds.end());

	if (maxMagnitude > 1) {
		for (size_t i = 0; i < wheelSpeeds.size(); i++) {
			wheelSpeeds[i] = wheelSpeeds[i] / maxMagnitude;
		}
	}
} //Normalize()

void DriveTrain::CartesianDrive(double y, double x, double rotation, double angle) {
	//source: WPILib
	//same code found in CartesianDrive in the WPI Library but adapted for being used in Velocity Mode
	frc::Vector2d input{x, y};
	std::vector<double> wheelSpeeds;


    for(int i = 0; i < 4; i++) {
        wheelSpeeds.push_back(0.0);
    }

    const int kFRONT_LEFT = 0;
    const int kFRONT_RIGHT = 1;
    const int kBACK_LEFT = 2;
    const int kBACK_RIGHT = 3;

    if (m_FOD){ input.Rotate(angle); }

    x = abs(x) <= 0.05f ? 0 : x;
	y = abs(y) <= 0.05f ? 0 : y;
	rotation = abs(rotation) <= 0.025f ? 0 : rotation;
	
	wheelSpeeds[kFRONT_LEFT] = input.y + input.x + rotation;
	wheelSpeeds[kFRONT_RIGHT] = input.y - input.x - rotation;
	wheelSpeeds[kBACK_LEFT] = input.y - input.x + rotation;
	wheelSpeeds[kBACK_RIGHT] = input.y + input.x - rotation;

	Normalize(wheelSpeeds);
        
    m_FrontLeft.Set(ControlMode::Velocity, wheelSpeeds[kFRONT_LEFT] * kMAX_VELOCITY );
    m_FrontRight.Set(ControlMode::Velocity, wheelSpeeds[kFRONT_RIGHT] * kMAX_VELOCITY);
    m_BackLeft.Set(ControlMode::Velocity, wheelSpeeds[kBACK_LEFT] * kMAX_VELOCITY);
    m_BackRight.Set(ControlMode::Velocity, wheelSpeeds[kBACK_RIGHT] * kMAX_VELOCITY);
    Robot::GetRobot()->GetCOB().GetTable().GetEntry(COB_KEY_MOTER_SPEED).SetDouble(wheelSpeeds[kFRONT_LEFT]);
}

void DriveTrain::UseVelocityPID(){
    SetPID(50, 0.0, 0.0, 0.0, 0.05);
}

void DriveTrain::SetPID(double E, double P, double I, double D, double F){

    m_FrontLeft.ConfigAllowableClosedloopError(0.0, E, 0.0);
    m_FrontRight.ConfigAllowableClosedloopError(0.0, E, 0.0);
    m_BackLeft.ConfigAllowableClosedloopError(0.0, E, 0.0);
    m_BackRight.ConfigAllowableClosedloopError(0.0, E, 0.0);

    m_FrontLeft.Config_kP(0.0, P, 0.0);
    m_FrontRight.Config_kP(0.0, P, 0.0);
    m_BackLeft.Config_kP(0.0, P, 0.0);
    m_FrontRight.Config_kP(0.0, P, 0.0);

    m_FrontLeft.Config_kI(0.0, I, 0.0);
    m_FrontRight.Config_kI(0.0, I, 0.0);
    m_BackLeft.Config_kI(0.0, I, 0.0);
    m_FrontRight.Config_kI(0.0, I, 0.0);

    m_FrontLeft.Config_kD(0.0, D, 0.0); 
    m_FrontRight.Config_kD(0.0, D, 0.0);
    m_BackLeft.Config_kD(0.0, D, 0.0);
    m_FrontRight.Config_kD(0.0, D, 0.0);

    m_FrontLeft.Config_kF(0.0, F, 0.0);
    m_FrontRight.Config_kF(0.0, F, 0.0);
    m_BackLeft.Config_kF(0.0, F, 0.0);
    m_FrontRight.Config_kF(0.0, F, 0.0);
}

//void DriveTrain::DriveToPosition(double x){
  //  m_FrontLeft.
//}

void DriveTrain::DriveInit(){

    m_BackRight.SetInverted(true);
    m_FrontRight.SetInverted(true);



    m_DriveButton.WhileHeld(frc2::InstantCommand([&] {
        Robot::GetRobot()->GetDriveTrain().BaseDrive(.15);
        DebugOutF("Forward");
    }));
    m_DriveButton.WhenReleased(frc2::InstantCommand([&] {
        Robot::GetRobot()->GetDriveTrain().BaseDrive(0);
        DebugOutF("Stop");
    }));

    BreakMode(true);

    m_FODToggle.WhenPressed([&] {
        m_FOD = !m_FOD;
    });
    
}

void DriveTrain::BaseDrive(double power){
    m_FrontLeft.Set(ControlMode::PercentOutput, power);
    m_FrontRight.Set(ControlMode::PercentOutput, power);
    m_BackLeft.Set(ControlMode::PercentOutput, power);
    m_BackRight.Set(ControlMode::PercentOutput, power);
}