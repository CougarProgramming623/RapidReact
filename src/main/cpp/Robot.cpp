// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <frc/RobotController.h>
#include <wpi/raw_ostream.h>
#include <frc2/command/InstantCommand.h>
#include <frc/RobotBase.h>
#include <frc/DriverStation.h>
#include "commands/LockOnTarget.h"
#include <frc/Errors.h>
#include "Util.h"
#include "frc/timer.h"
#include "ID.h"

#include <math.h>
#define _USE_MATH_DEFINES
#include "commands/DriveToPosition.h"

#include <subsystems/DriveTrain.h>

#include "commands/TurnToAngle.h"
#include <frc2/command/SequentialCommandGroup.h>
#include <frc/Timer.h>


Robot* Robot::s_Instance = nullptr;

Robot::Robot() :

  m_TargetLock([&] { return Robot::GetRobot()->GetJoystick().GetRawButton(1); }),
  
  m_LimeLightToggle(BUTTON_L(14)) 

{
  s_Instance = this;
  
}

void Robot::RobotInit() {
  DebugOutF("Robot Init");
  

  m_NumLED = 70;

  m_LED.SetLength(140);

  for (int i = 0; i < 140; i++)
  {
    m_ledBuffer[i].SetRGB(255, 255, 0);
  }
  
  m_LED.SetData(m_ledBuffer);
  m_LED.Start();

  GetDriveTrain().DriveInit();
  m_Shooter.ShooterInit();
  m_OI.Init();

  if( GetCOB().GetTable().GetEntry(COB_KEY_IS_RED).GetBoolean(false)){
    m_AllianceColor.red = 1;
    m_AllianceColor.blue = 0;
  } else {
    m_AllianceColor.blue = 1;
    m_AllianceColor.red = 0;
  }
  

  m_LimeLightToggle.WhenPressed([&] {
    DebugOutF("Limelight Toggle");
    if(Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").GetDouble(2) == 1)
      Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(0);
    else
      Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(1); 
  });
}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
  

  if( GetCOB().GetTable().GetEntry(COB_KEY_IS_RED).GetBoolean(false)){
    m_AllianceColor.red = 1;
    m_AllianceColor.blue = 0;
  } else {
    m_AllianceColor.blue = 1;
    m_AllianceColor.red = 0;
  }

  if(frc::DriverStation::GetInstance().GetAlliance() != frc::DriverStation::Alliance::kRed && frc::DriverStation::GetInstance().GetAlliance() != frc::DriverStation::Alliance::kBlue){
    for (int i = 0; i < 140; i++)
      m_ledBuffer[i].SetRGB(255, 0, 255);
  } else if(abs(GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TX).GetDouble(0)) < 2 && GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TV).GetDouble(0) > 0){
    for (int i = 0; i < 140; i++)
      m_ledBuffer[i].SetRGB(0, 255, 0);
  } else if(GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TV).GetDouble(0) > 0){
    if(m_LEDIndex > m_NumLED - 1)
      m_LEDIndex = 0;
    CanSee(m_AllianceColor, m_NumLED, 10, m_LEDIndex, m_ledBuffer);
    m_LEDIndex++;
  } else if(frc::Timer::GetMatchTime().to<double>() <= 30 && GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).GetBoolean(false)){
    if(m_LEDIndex > m_NumLED - 1 )
      m_LEDIndex = 0;
    EndGame(m_AllianceColor, m_NumLED, 3, m_LEDIndex, m_ledBuffer);
    m_LEDIndex++;
  } else if((int) frc::RobotController::GetBatteryVoltage() < 10.5){
    if(m_LEDIndex > m_NumLED - 1)
      m_LEDIndex = 0;
    LowBattery(m_AllianceColor, m_NumLED, 10, m_LEDIndex, m_ledBuffer);
    m_LEDIndex++;
  } else { 
    for (int i = 0; i < 140; i++)
      m_ledBuffer[i].SetLED(m_AllianceColor);
  }
  m_LED.SetData(m_ledBuffer);

  

  if(frc::RobotController::GetUserButton()){
    if(Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").GetDouble(2) == 1)
      Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(0);
    else
      Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(1);  
  }
  

  PushDistance();
  
  GetCOB().GetTable().GetEntry(COB_KEY_FLYWHEEL_SPEED).SetDouble(GetShooter().FlywheelRPM());
  //GetCOB().GetTable().GetEntry(COB_KEY_FOD).SetBoolean(GetDriveTrain().m_FOD);
  if (GetCOB().GetTable().GetEntry(COB_KEY_NAVX_RESET).GetBoolean(false) == true) {
    GetNavX().ZeroYaw();
    GetCOB().GetTable().GetEntry(COB_KEY_NAVX_RESET).SetBoolean(false);
  }
  GetCOB().GetTable().GetEntry(COB_KEY_ROBOT_ANGLE).SetDouble(GetNavX().GetYaw());
  GetCOB().GetTable().GetEntry(COB_KEY_MATCH_TIME).SetDouble(frc::Timer::GetMatchTime().to<double>());

}

void Robot::AutonomousInit() {
  DebugOutF("Auto Init");
  GetDriveTrain().BreakMode(true);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(true);
  GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).SetBoolean(false);
}
void Robot::AutonomousPeriodic() {
  
}

void Robot::TeleopInit() {
  DebugOutF("Teleop Init");
  GetDriveTrain().BreakMode(true);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(true);
  GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).SetBoolean(true);
  m_TargetLock.WhenHeld(LockOnTarget());
}
void Robot::TeleopPeriodic() {
  
}

void Robot::DisabledInit() {
  GetDriveTrain().BreakMode(false);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(false);
  GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).SetBoolean(false);

}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

void Robot::PushDistance(){
  double h = TARGET_HEIGHT - LIMELIGHT_HEIGHT;
  double angleFromGroundDeg = LIMELIGHT_ANGLE + GetCOB().GetTable().GetEntry("/limelight/ty").GetDouble(0);

  GetCOB().GetTable().GetEntry(COB_KEY_DISTANCE).SetDouble(
    h / tan(angleFromGroundDeg * (M_PI / 180))
  );
}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
