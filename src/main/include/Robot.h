// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc/AddressableLED.h>
#include <frc2/command/PrintCommand.h>

#include <frc/Joystick.h>
#include "subsystems/DriveTrain.h"

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

  void GetDriveTrain();
  void SetDriveTrain();

  inline static Robot* GetRobot(){
    return s_Instance;
  }

  inline frc::Joystick* GetButtonBoard(){
    return &m_ButtonBoard;
  }


  DriveTrain m_DriveTrain;

  private:
  
  static Robot* s_Instance;
  frc::AddressableLED m_LED{9};
  std::array<frc::AddressableLED::LEDData, 140> m_ledBuffer;

  
	frc::Joystick m_DriverJoystick = frc::Joystick(0);
	frc::Joystick m_ButtonBoard = frc::Joystick(1);

};
