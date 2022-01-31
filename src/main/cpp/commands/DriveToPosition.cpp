#include "commands/DriveToPosition.h"
#include "Robot.h"

using ctre::phoenix::motorcontrol::ControlMode;

const int kTICKS_PER_ROTATION = 2048;  //(!) FINISH (!)
const double kMETERS_PER_ROTATION = (8.0 * 2.54) * 3.1415926 / 100.0;     //(!) FINISH (!)
const double kCOUNT_THREASHOLD = 204.8;  //How close to the exact number the encoders need to be (!) Should be tested (!)

DriveToPos::DriveToPos(double xx, double y, double a){
    double x = 0.5;
    m_XTicks = x / kMETERS_PER_ROTATION * kTICKS_PER_ROTATION;
    // m_XTicks = x / kTICKS_PER_ROTATION * kCM_PER_ROTATION;   
    // m_Yticks = y / kTICKS_PER_ROTATION * kCM_PER_ROTATION;
    DebugOutF("m_XTicks" + m_XTicks);
    m_Angle = a;


    AddRequirements(&Robot::GetRobot()->GetDriveTrain());

} //Haven't implemented angle and only uses m_XTicks

void DriveToPos::Initialize(){
    DebugOutF("Initialize DriveToPosition Command");
    DriveTrain& drivetrain = Robot::GetRobot()->GetDriveTrain();
    

    Robot::GetRobot()->GetDriveTrain().GetFrontL().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().Set(ControlMode::PercentOutput, 0);

    m_InitialTicks[0] = Robot::GetRobot()->GetDriveTrain().GetFrontL().GetSelectedSensorPosition();
    m_InitialTicks[1] = Robot::GetRobot()->GetDriveTrain().GetFrontR().GetSelectedSensorPosition();
    m_InitialTicks[2] = Robot::GetRobot()->GetDriveTrain().GetBackL().GetSelectedSensorPosition();
    m_InitialTicks[3] = Robot::GetRobot()->GetDriveTrain().GetBackR().GetSelectedSensorPosition();

    Robot::GetRobot()->GetDriveTrain().BreakMode(false);
    Robot::GetRobot()->GetDriveTrain().UseMagicPID();
}

bool DriveToPos::IsFinished(){
    return  
        abs(Robot::GetRobot()->GetDriveTrain().GetFrontL().GetSelectedSensorPosition() - (m_InitialTicks[0] + m_XTicks)) <= kCOUNT_THREASHOLD &&
        abs(Robot::GetRobot()->GetDriveTrain().GetFrontR().GetSelectedSensorPosition() - (m_InitialTicks[1] + m_XTicks)) <= kCOUNT_THREASHOLD &&
        abs(Robot::GetRobot()->GetDriveTrain().GetBackL().GetSelectedSensorPosition() - (m_InitialTicks[2] + m_XTicks)) <= kCOUNT_THREASHOLD &&
        abs(Robot::GetRobot()->GetDriveTrain().GetBackR().GetSelectedSensorPosition() - (m_InitialTicks[3] + m_XTicks)) <= kCOUNT_THREASHOLD;
}

void DriveToPos::Execute(){
    DebugOutF("Drive To Position Execute");
    Robot::GetRobot()->GetDriveTrain().GetFrontL().Set(ControlMode::PercentOutput, .5 );
	Robot::GetRobot()->GetDriveTrain().GetFrontR().Set(ControlMode::PercentOutput, .5 );
	Robot::GetRobot()->GetDriveTrain().GetBackL().Set(ControlMode::PercentOutput, .5 );
	Robot::GetRobot()->GetDriveTrain().GetBackR().Set(ControlMode::PercentOutput, .5 );
}

void DriveToPos::End(){
    DebugOutF("Drive To Position End");

    Robot::GetRobot()->GetDriveTrain().GetFrontL().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().Set(ControlMode::PercentOutput, 0);

   
}