// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <wpi/raw_ostream.h>
#include <frc/Errors.h>
#include "Util.h"

#include <subsystems/Drive.h>

Robot* Robot::s_Instance = nullptr;

Robot::Robot(){
  s_Instance = this;
}

void Robot::RobotInit() {
  DebugOutF("Robot Init");
  m_LED.SetLength(140);

  for (int i = 0; i < 140; i++)
  {
    m_ledBuffer[i].SetRGB(255, 255, 0);
  }
  
  m_LED.SetData(m_ledBuffer);
  m_LED.Start();
}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
}

void Robot::AutonomousInit() {
  DebugOutF("Auto Init");
}
void Robot::AutonomousPeriodic() {
  
}

void Robot::TeleopInit() {
  DebugOutF("Teleop Init");
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
