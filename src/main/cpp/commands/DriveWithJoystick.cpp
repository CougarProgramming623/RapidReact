#include "commands/DriveWithJoystick.h"
#include "Robot.h"

Drive::Drive(){
    AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

void Drive::Initialize(){
    DebugOutF("Initialize Drive Command");
}

void Drive::Execute(){
    Robot *r = Robot::GetRobot();
    r->GetDriveTrain().CartesianDrive(
        -r->GetJoystick().GetRawAxis(1), 
        r->GetJoystick().GetRawAxis(0), 
        r->GetJoystick().GetRawAxis(2), 
        r->GetNavX().GetYaw()
    );
}