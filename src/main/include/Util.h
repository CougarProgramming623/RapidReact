#pragma once

#include <frc2/command/PrintCommand.h>

inline void DebugOutF(const std::string_view message) {
	// frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}
// this defines a lambda which when run returns the current button state of the raw button specified by ID. Is taken in by the frc::Button constuctor
#define BUTTON_L(id) [&] { return Robot::GetRobot()->GetButtonBoard().GetRawButton(id);}

