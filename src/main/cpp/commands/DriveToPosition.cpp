#include "commands/DriveWithJoystick.h"
#include "Robot.h"

Drive::Drive(){
    AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

void Drive::Initialize(){
    DebugOutF("Initialize DriveToPosition Command");
}

void Drive::Execute(){
    Robot::GetRobot()->GetDriveTrain();
}