// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc/AddressableLED.h>
#include <frc2/command/PrintCommand.h>
#include "subsystems/Shooter.h"

class Robot : public frc::TimedRobot {
 public:

  Robot();

  void RobotInit() override;
  void RobotPeriodic() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void DisabledInit() override;
  void DisabledPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  static Robot* GetRobot(){ return s_Instance; }

  inline frc::Joystick& GetButtonBoard(){ return m_ButtonBoard; }
  inline frc::Joystick& GetJoystick() { return m_Joystick; }

  inline Shooter& GetShooter(){ return m_Shooter; }


  private:
  
  static Robot* s_Instance;
  frc::AddressableLED m_LED{9};
  std::array<frc::AddressableLED::LEDData, 140> m_ledBuffer;

  Shooter m_Shooter;

  frc::Joystick m_ButtonBoard = frc::Joystick(0);
  frc::Joystick m_Joystick = frc::Joystick(1);
  
  
};
