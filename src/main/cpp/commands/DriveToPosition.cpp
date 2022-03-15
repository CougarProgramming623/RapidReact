#include "commands/DriveToPosition.h"
#include "Robot.h"

using ctre::phoenix::motorcontrol::ControlMode;

#define FOR_ALL_MOTORS(operation) m_DriveTrain.GetFrontR()operation;\
                                  m_DriveTrain.GetFrontL()operation;\
                                  m_DriveTrain.GetBackR()operation;\
                                  m_DriveTrain.GetBackL()operation; 

const int kGEARBOX_RATIO = 12;
const int kTICKS_PER_ROTATION = 2048;     //(!) FINISH (!)
const double kMETERS_PER_ROTATION = (8.0 * 2.54) * 3.1415926 / 100.0;     //(!) FINISH (!)
const double kCOUNT_THRESHOLD = 500;      //How close to the exact number the encoders need to be (!) Should be tested (!)
const double kDERIVATIVE_THRESHOLD = .1; //(!) Test (!)
const double kTIME_THRESHOLD = 1;         //(!) TEST (!)

DriveToPos::DriveToPos(double x, double y, double a){
    m_XTicks = x / kMETERS_PER_ROTATION * kTICKS_PER_ROTATION * kGEARBOX_RATIO;

    m_Angle = a;

    AddRequirements(&Robot::GetRobot()->GetDriveTrain());

} //Haven't implemented angle and only uses m_XTicks

void DriveToPos::Initialize(){
    DebugOutF("Initialize DriveToPosition Command");
        
    FOR_ALL_MOTORS(.Set(ControlMode::PercentOutput, 0))

    m_FinalTicks[0] = m_DriveTrain.GetFrontL().GetSelectedSensorPosition() + m_XTicks;
    m_FinalTicks[1] = m_DriveTrain.GetFrontR().GetSelectedSensorPosition() + m_XTicks;
    m_FinalTicks[2] = m_DriveTrain.GetBackL().GetSelectedSensorPosition() + m_XTicks;
    m_FinalTicks[3] = m_DriveTrain.GetBackR().GetSelectedSensorPosition() + m_XTicks;

    // m_XTicks = x / kTICKS_PER_ROTATION * kCM_PER_ROTATION;   
    // m_Yticks = y / kTICKS_PER_ROTATION * kCM_PER_ROTATION;
    DebugOutF("m_XTicks: " + std::to_string(m_XTicks));
    DebugOutF("Initial Position: " + std::to_string(m_DriveTrain.GetFrontL().GetSelectedSensorPosition()));

    m_InitialTicks[0] = m_DriveTrain.GetFrontL().GetSelectedSensorPosition();
    m_InitialTicks[1] = m_DriveTrain.GetFrontR().GetSelectedSensorPosition();
    m_InitialTicks[2] = m_DriveTrain.GetBackL().GetSelectedSensorPosition();
    m_InitialTicks[3] = m_DriveTrain.GetBackR().GetSelectedSensorPosition();

    m_DriveTrain.BreakMode(true);
    m_DriveTrain.UsePostionPID();

    FOR_ALL_MOTORS(.ConfigPeakOutputForward(0.3, 0))
    FOR_ALL_MOTORS(.ConfigPeakOutputReverse(-0.3, 0))

    FOR_ALL_MOTORS(.ConfigClosedloopRamp(.06, 0))

    m_Clock.Start();
}

void DriveToPos::Execute(){

    Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/currentCommand").SetString("DriveToPosition");

    m_DriveTrain.GetFrontL().Set(ControlMode::Position, m_InitialTicks[0] + m_XTicks);
	m_DriveTrain.GetFrontR().Set(ControlMode::Position, m_InitialTicks[1] + m_XTicks );
	m_DriveTrain.GetBackL().Set(ControlMode::Position, m_InitialTicks[2] + m_XTicks );
	m_DriveTrain.GetBackR().Set(ControlMode::Position, m_InitialTicks[3] + m_XTicks );
}

bool DriveToPos::IsFinished(){
    return  
       (abs(m_DriveTrain.GetFrontL().GetSelectedSensorPosition() - (m_InitialTicks[0] + m_XTicks)) <= kCOUNT_THRESHOLD &&
        abs(m_DriveTrain.GetFrontR().GetSelectedSensorPosition() - (m_InitialTicks[1] + m_XTicks)) <= kCOUNT_THRESHOLD &&
        abs(m_DriveTrain.GetBackL().GetSelectedSensorPosition()  - (m_InitialTicks[2] + m_XTicks)) <= kCOUNT_THRESHOLD &&
        abs(m_DriveTrain.GetBackR().GetSelectedSensorPosition()  - (m_InitialTicks[3] + m_XTicks)) <= kCOUNT_THRESHOLD) ||
       (m_Clock.Get() >= (units::time::second_t) kTIME_THRESHOLD &&
       (abs(m_DriveTrain.GetFrontL().GetErrorDerivative()) <= kDERIVATIVE_THRESHOLD ||
        abs(m_DriveTrain.GetFrontR().GetErrorDerivative()) <= kDERIVATIVE_THRESHOLD ||
        abs(m_DriveTrain.GetBackL().GetErrorDerivative())  <= kDERIVATIVE_THRESHOLD ||
        abs(m_DriveTrain.GetBackR().GetErrorDerivative())  <= kDERIVATIVE_THRESHOLD ));
}

void DriveToPos::End(bool end){
    DebugOutF("Drive To Position End");

    FOR_ALL_MOTORS(.Set(ControlMode::PercentOutput, 0))

    FOR_ALL_MOTORS(.ConfigPeakOutputForward(1, 0))
    FOR_ALL_MOTORS(.ConfigPeakOutputReverse(-1, 0))

    Robot::GetRobot()->GetDriveTrain().UseVelocityPID();
    DebugOutF("Final Position: " + std::to_string(m_DriveTrain.GetFrontL().GetSelectedSensorPosition()));
    m_DriveTrain.BreakMode(false);

}