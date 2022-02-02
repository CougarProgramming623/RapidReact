#include "commands/DriveToPosition.h"
#include "Robot.h"

using ctre::phoenix::motorcontrol::ControlMode;

const int kTICKS_PER_ROTATION = 2048;  //(!) FINISH (!)
const double kMETERS_PER_ROTATION = (8.0 * 2.54) * 3.1415926 / 100.0;     //(!) FINISH (!)
const double kCOUNT_THRESHOLD = 500;  //How close to the exact number the encoders need to be (!) Should be tested (!)

DriveToPos::DriveToPos(double xx, double y, double a){
    double x = 20;
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
    Robot::GetRobot()->GetDriveTrain().UsePostionPID();

    Robot::GetRobot()->GetDriveTrain().GetFrontL().ConfigPeakOutputForward(0.5, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().ConfigPeakOutputForward(0.5, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().ConfigPeakOutputForward(0.5, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().ConfigPeakOutputForward(0.5, 0);

    Robot::GetRobot()->GetDriveTrain().GetFrontL().ConfigPeakOutputReverse(-0.5, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().ConfigPeakOutputReverse(-0.5, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().ConfigPeakOutputReverse(-0.5, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().ConfigPeakOutputReverse(-0.5, 0);

    Robot::GetRobot()->GetDriveTrain().GetFrontL().ConfigClosedloopRamp(.02, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().ConfigClosedloopRamp(.02, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().ConfigClosedloopRamp(.02, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().ConfigClosedloopRamp(.02, 0);
}


bool DriveToPos::IsFinished(){
    return  
        abs(Robot::GetRobot()->GetDriveTrain().GetFrontL().GetSelectedSensorPosition() - (m_InitialTicks[0] + m_XTicks)) <= kCOUNT_THRESHOLD &&
        abs(Robot::GetRobot()->GetDriveTrain().GetFrontR().GetSelectedSensorPosition() - (m_InitialTicks[1] + m_XTicks)) <= kCOUNT_THRESHOLD &&
        abs(Robot::GetRobot()->GetDriveTrain().GetBackL().GetSelectedSensorPosition() - (m_InitialTicks[2] + m_XTicks)) <= kCOUNT_THRESHOLD &&
        abs(Robot::GetRobot()->GetDriveTrain().GetBackR().GetSelectedSensorPosition() - (m_InitialTicks[3] + m_XTicks)) <= kCOUNT_THRESHOLD;
}

void DriveToPos::Execute(){
    DebugOutF("Drive To Position Execute");
    Robot::GetRobot()->GetDriveTrain().GetFrontL().Set(ControlMode::Position, m_InitialTicks[0] + m_XTicks);
	Robot::GetRobot()->GetDriveTrain().GetFrontR().Set(ControlMode::Position, m_InitialTicks[1] + m_XTicks );
	Robot::GetRobot()->GetDriveTrain().GetBackL().Set(ControlMode::Position, m_InitialTicks[2] + m_XTicks );
	Robot::GetRobot()->GetDriveTrain().GetBackR().Set(ControlMode::Position, m_InitialTicks[3] + m_XTicks );
}

void DriveToPos::End(bool end){
    DebugOutF("Drive To Position End");

    Robot::GetRobot()->GetDriveTrain().GetFrontL().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().Set(ControlMode::PercentOutput, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().Set(ControlMode::PercentOutput, 0);

    Robot::GetRobot()->GetDriveTrain().GetFrontL().ConfigPeakOutputForward(1, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().ConfigPeakOutputForward(1, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().ConfigPeakOutputForward(1, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().ConfigPeakOutputForward(1, 0);

    Robot::GetRobot()->GetDriveTrain().GetFrontL().ConfigPeakOutputReverse(-1, 0);
    Robot::GetRobot()->GetDriveTrain().GetFrontR().ConfigPeakOutputReverse(-1, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackL().ConfigPeakOutputReverse(-1, 0);
    Robot::GetRobot()->GetDriveTrain().GetBackR().ConfigPeakOutputReverse(-1, 0);

    Robot::GetRobot()->GetDriveTrain().UseVelocityPID();

}