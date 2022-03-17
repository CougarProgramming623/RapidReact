#include "commands/DriveWithJoystick.h"
#include "Robot.h"

Drive::Drive(){
    AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

void Drive::Initialize(){
    DebugOutF("Initialize Drive Command");
    Robot::GetRobot()->GetDriveTrain().UseVelocityPID();

}

double deadFix(double in) {
    if(abs(in) < 0.05) {
        return 0;
    }
    return in;
}

void Drive::Execute(){
    Robot::GetRobot()->GetCOB().GetTable().GetEntry("/COB/currentCommand").SetString("DriveWithJoystick");
    
    Robot *r = Robot::GetRobot();
        r->GetDriveTrain().CartesianDrive(
        deadFix(-r->GetJoystick().GetRawAxis(1)), 
        deadFix(r->GetJoystick().GetRawAxis(0)), 
        deadFix(r->GetJoystick().GetRawAxis(2)), 
        r->GetNavX().GetYaw(),
        true // TODO before push
    );

}