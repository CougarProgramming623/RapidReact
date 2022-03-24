

#include "OI.h"

#include <frc2/command/InstantCommand.h>

#include "COB.h"
#include "ID.h"
#include "Robot.h"
#include "Util.h"

OI::OI()
// m_limelightToggle(BUTTON_L(LIMELIGHT_TOGGLE))
{}

void OI::Init() {
  ToggleLimelight();
}

void OI::ToggleLimelight() {
  // m_limelightToggle.WhenPressed(frc2::InstantCommand([&]{
  // 	bool currentState =
  // Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").GetBoolean(true);
  // 	Robot::GetRobot()->GetCOB().GetTable().GetEntry("limelight/ledMode").SetBoolean(!currentState);
  // }));
}
