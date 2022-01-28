// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include <frc/AddressableLED.h>
#include <frc2/command/PrintCommand.h>
#include "subsystems/Shooter.h"

#include <frc/Joystick.h>
#include "subsystems/DriveTrain.h"
#include <AHRS.h>
#include <frc/SPI.h>
#include "COB.h"
#include <frc2/command/SubsystemBase.h>

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

  inline DriveTrain& GetDriveTrain(){return m_DriveTrain;}

  static Robot* GetRobot(){ return s_Instance; }

  inline frc::Joystick& GetButtonBoard(){ return m_ButtonBoard; }
  inline frc::Joystick& GetJoystick() { return m_Joystick; }

  inline Shooter& GetShooter(){ return m_Shooter; }

  inline AHRS& GetNavX() {return m_NavX; }

  inline COB& GetCOB() {return m_COB; }

  private:
  
  static Robot* s_Instance;
  frc::AddressableLED m_LED{9};
  std::array<frc::AddressableLED::LEDData, 140> m_ledBuffer;

  Shooter m_Shooter;

  frc::Joystick m_ButtonBoard = frc::Joystick(0);
  frc::Joystick m_Joystick = frc::Joystick(1);
  
  AHRS m_NavX {frc::SPI::Port::kMXP};

  DriveTrain m_DriveTrain;

  COB m_COB;
};
