// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <wpi/raw_ostream.h>
#include <frc2/command/InstantCommand.h>
#include <frc/Errors.h>
#include "Util.h"
#include "commands/DriveToPosition.h"

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

  GetDriveTrain().DriveInit();
  m_Shooter.ShooterInit();
}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();

  //Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(1);
  
  GetCOB().GetTable().GetEntry(COB_KEY_FLYWHEEL_SPEED).SetDouble(GetShooter().FlywheelSpeed());
  GetCOB().GetTable().GetEntry(COB_KEY_FOD).SetBoolean(GetDriveTrain().m_FOD);
  if (GetCOB().GetTable().GetEntry(COB_KEY_NAVX_RESET).GetBoolean(false) == true) {
    GetNavX().ZeroYaw();
    GetCOB().GetTable().GetEntry(COB_KEY_NAVX_RESET).SetBoolean(false);
  }
  GetCOB().GetTable().GetEntry(COB_KEY_ROBOT_ANGLE).SetDouble(GetNavX().GetYaw());
}

void Robot::AutonomousInit() {
  DebugOutF("Auto Init");
  GetDriveTrain().BreakMode(true);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(true);

  frc2::CommandScheduler::GetInstance().Schedule(new DriveToPos(.5, 0, 0));
  
}
void Robot::AutonomousPeriodic() {
  
}

void Robot::TeleopInit() {
  DebugOutF("Teleop Init");
  GetDriveTrain().BreakMode(true);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(true);

}
void Robot::TeleopPeriodic() {
  
}

void Robot::DisabledInit() {
  GetDriveTrain().BreakMode(false);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(false);
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
