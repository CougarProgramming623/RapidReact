

#include "commands/LockOnTarget.h"

#include "Robot.h"

LockOnTarget::LockOnTarget(bool noJoystick) {
  m_noJoystick = noJoystick;
  AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

void LockOnTarget::Initialize() {
  // DebugOutF("Initialize Drive Command");
}

void LockOnTarget::Execute() {
  m_Angle = Robot::GetRobot()
                ->GetCOB()
                .GetTable()
                .GetEntry("/limelight/tx")
                .GetDouble(0);
  Robot::GetRobot()
      ->GetCOB()
      .GetTable()
      .GetEntry("/COB/currentCommand")
      .SetString("LockOnTarget");

  Robot* r = Robot::GetRobot();
  
  double rotationPower = m_Angle / 27 / 2;

  if(!m_noJoystick){
    r->GetDriveTrain().CartesianDrive(
        -r->GetJoystick().GetRawAxis(1), r->GetJoystick().GetRawAxis(0),
        rotationPower, r->GetNavX().GetYaw(), true);
  } else {
    r->GetDriveTrain().CartesianDrive(0, 0,
        rotationPower, r->GetNavX().GetYaw(), true);
  }
}
