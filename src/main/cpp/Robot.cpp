// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <wpi/raw_ostream.h>
#include <frc/Errors.h>
#include "Util.h"

#include <subsystems/DriveTrain.h>

Robot* Robot::s_Instance = nullptr;

Robot::Robot() {
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

  m_DriveTrain.DriveInit();
  m_Shooter.ShooterInit();
}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
}

void Robot::AutonomousInit() {
  DebugOutF("Auto Init");
  m_DriveTrain.BreakMode(true);
}
void Robot::AutonomousPeriodic() {
  
}

void Robot::TeleopInit() {
  DebugOutF("Teleop Init");
  m_DriveTrain.BreakMode(true);
}
void Robot::TeleopPeriodic() {
  m_DriveTrain.CartesianDrive(-m_Joystick.GetRawAxis(1), m_Joystick.GetRawAxis(0), m_Joystick.GetRawAxis(2), GetNavX()->GetYaw());
}

void Robot::DisabledInit() {
  m_DriveTrain.BreakMode(false);
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
