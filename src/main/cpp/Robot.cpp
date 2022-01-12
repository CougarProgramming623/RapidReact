// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <wpi/raw_ostream.h>
#include <frc/Errors.h>

Robot* Robot::s_Instance = nullptr;

Robot::Robot(){
  wpi::outs() << "Robot Cpnstruct\n";
  s_Instance = this;
}

void Robot::RobotInit() {
  wpi::outs() << "Robot Init\n";
  m_LED.SetLength(140);

  for (int i = 0; i < 140; i++)
  {
    m_ledBuffer[i].SetRGB(255, 255, 0);
  }
  
  m_LED.SetData(m_ledBuffer);
  m_LED.Start();
}

void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {
  wpi::outs() << "Auto Init\n";
}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  wpi::outs() << "Teleop Init\n";
}
void Robot::TeleopPeriodic() {}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
