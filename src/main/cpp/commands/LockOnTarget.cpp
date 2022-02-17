#include "commands/LockOnTarget.h"
#include "Robot.h"

LockOnTarget::LockOnTarget(){
    AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

void LockOnTarget::Initialize(){
    DebugOutF("Initialize Drive Command");
}

void LockOnTarget::Execute(){

    m_Angle = Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/tx").GetDouble(0);
    Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/currentCommand").SetString("LockOnTarget");

    Robot *r = Robot::GetRobot();
    r->GetDriveTrain().CartesianDrive(
        -r->GetJoystick().GetRawAxis(1), 
        r->GetJoystick().GetRawAxis(0), 
        m_Angle / 27 / 2, 
        r->GetNavX().GetYaw(),
        true
    );
}