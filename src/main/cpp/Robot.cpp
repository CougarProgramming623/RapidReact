// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <frc/RobotController.h>
#include <wpi/raw_ostream.h>
#include <frc2/command/InstantCommand.h>
#include <frc/Errors.h>
#include "Util.h"

#include <math.h>
#define _USE_MATH_DEFINES
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

  if( GetCOB().GetTable().GetEntry(COB_KEY_IS_RED)){
    m_AllianceColor.red = 1;
  } else {
    m_AllianceColor.blue = 1;
  }
}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();

  // LED color goes from green to red based on batery voltage
  // Full red below 10v full green above 12v
  // for (int i = 0; i < 140; i++)
  //   {
  //     int g = (int) sqrt((long double)       (((int) frc::RobotController::GetBatteryVoltage() - 10) * 255 * 255 / 765));
  //     int r = (int) sqrt((long double) (-1 * (((int) frc::RobotController::GetBatteryVoltage() - 12) * 255 * 255 / 765)));
  //     m_ledBuffer[i].SetRGB(r, g, 0);
  //   }

  // Flashes red if batery provides less than 10v
  // if((int) frc::RobotController::GetBatteryVoltage() < 13){
  //   bool on = false;
  //   if(on){
  //     for (int i = 0; i < 140; i++){
  //         m_ledBuffer[i].SetRGB(0, 0, 0);
  //       }
  //   } else {
  //     for (int i = 0; i < 140; i++){
  //         m_ledBuffer[i].SetRGB(255, 0, 0);
  //       }
  //   }
  //   on = !on;
  // }
  // if(GetCOB().GetTable().GetEntry(COB_KEY_DRIVE_MODE).GetString())
  //Has two lines charse eachother around if looking at the target
  //double limeLightTX = GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TX).GetDouble(0);

  //
  if(abs(GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TX).GetDouble(0)) < 2 && GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TV).GetDouble(0) > 0){
    for (int i = 0; i < 140; i++)
      m_ledBuffer[i].SetRGB(0, 255, 0);
    m_LED.SetData(m_ledBuffer);
  }else
  //Has two lines charse eachother around if it sees a target
  if(GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TV).GetDouble(0) > 0){
    //DebugOutF("On Target" + std::to_string(limeLightTX));
    int numLED = 70;
    int tailLength = 10;
    if(m_LEDIndex > numLED - 1)
      m_LEDIndex = 0;

    m_ledBuffer[m_LEDIndex].SetRGB(0, 255, 0);
    m_ledBuffer[(numLED + m_LEDIndex - tailLength) % numLED].SetLED(m_AllianceColor);
    m_ledBuffer[numLED - m_LEDIndex].SetRGB(0, 255, 0);
    m_ledBuffer[(numLED + numLED - m_LEDIndex - tailLength) % numLED].SetLED(m_AllianceColor);

    m_LEDIndex++;
    m_LED.SetData(m_ledBuffer);
  } else
  // Has a red and blue line go around if batery provides less than 10v
  if((int) frc::RobotController::GetBatteryVoltage() < 10){
    DebugOutF("Low Battery");
    int numLED = 70;
    int tailLength = 10;
    if(m_LEDIndex > numLED - 1)
      m_LEDIndex = 0;
    m_ledBuffer[m_LEDIndex].SetRGB(255, 215, 0);
    m_ledBuffer[(numLED + m_LEDIndex - tailLength) % numLED].SetLED(m_AllianceColor);
    m_LEDIndex++;
    m_LED.SetData(m_ledBuffer);
  } else { //Sets LED to m_AllianceColor if no other parterns are aplicable
    DebugOutF("LED Alliance Colors");
    for (int i = 0; i < 140; i++)
      m_ledBuffer[i].SetLED(m_AllianceColor);
    m_LED.SetData(m_ledBuffer);
  }

  //Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(1);

  PushDistance();
  
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

  frc2::CommandScheduler::GetInstance().Schedule(new DriveToPos(1, 0, 0));
  
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
