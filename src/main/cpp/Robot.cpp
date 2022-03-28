

// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <frc/DriverStation.h>
#include <frc/Errors.h>
#include <frc/RobotBase.h>
#include <frc/RobotController.h>
#include <frc2/command/InstantCommand.h>
#include <wpi/raw_ostream.h>

#include <cmath>

#include "Auto.h"
#include "ID.h"
#include "Util.h"
#include "commands/LockOnTarget.h"

#define _USE_MATH_DEFINES
#include "commands/DriveToPosition.h"

#include <subsystems/DriveTrain.h>

#include "commands/TurnToAngle.h"
#include <frc2/command/SequentialCommandGroup.h>
#include <frc/Timer.h>

const int kSTRIP_1_START = 111;
const int kSTRIP_2_START = 192;

Robot* Robot::s_Instance = nullptr;

Robot::Robot()
    :

      m_TargetLock(
          [&] { return Robot::GetRobot()->GetJoystick().GetRawButton(1); }),

      m_LimeLightToggle(BUTTON_L(LIMELIGHT_TOGGLE))

{
  s_Instance = this;
}

void Robot::RobotInit() {
  DebugOutF("Robot Init");
  m_NumLED = 272;
  m_LED.SetLength(m_NumLED);

  for (int i = 0; i < m_NumLED; i++) {
    m_ledBuffer[i].SetRGB(255, 255, 0);
  }

  m_LED.SetData(m_ledBuffer);
  m_LED.Start();

  GetDriveTrain().DriveInit();
  GetClimb().ClimbInit();
  m_Shooter.ShooterInit();
  m_OI.Init();
  m_Intake.IntakeInit();

  if (GetCOB().GetTable().GetEntry(COB_KEY_IS_RED).GetBoolean(false)) {
    m_AllianceColor.red = 1;
    m_AllianceColor.blue = 0;
  } else {
    m_AllianceColor.blue = 1;
    m_AllianceColor.red = 0;
  }

  m_LimeLightToggle.WhenPressed([&] {
    DebugOutF("Limelight Toggle");
    if (GetCOB().GetTable().GetEntry("/limelight/ledMode").GetDouble(2) == 1)
      GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(0);
    else
      GetCOB().GetTable().GetEntry("/limelight/ledMode").SetDouble(1);
  });

  frc2::Button(BUTTON_L(4)).WhenPressed([&] { GetNavX().ZeroYaw(); });
}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();

  //LED Code
  if (GetCOB().GetTable().GetEntry(COB_KEY_IS_RED).GetBoolean(false)) {
    m_AllianceColor.red = 1;
    m_AllianceColor.blue = 0;
  } else {
    m_AllianceColor.blue = 1;
    m_AllianceColor.red = 0;
  }
  auto alliance = frc::DriverStation::GetAlliance();

  
  PaintSolid(m_ledBuffer, m_AllianceColor, 0, m_NumLED);

  if (alliance != frc::DriverStation::Alliance::kRed &&
        alliance != frc::DriverStation::Alliance::kBlue) {
    PaintSolid(m_ledBuffer, frc::Color::kPurple, 0, m_NumLED);
  } else if (std::abs(GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TX).GetDouble(0)) < 1 &&
             std::abs(GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TV).GetDouble(0)) > 0){
    PaintSolid(m_ledBuffer, frc::Color::kGreen, kSTRIP_1_START, m_NumLED); 
  } else if (std::abs(GetCOB().GetTable().GetEntry(COB_KEY_LIME_LIGHT_TV).GetDouble(0)) > 0){
    CanSee(m_AllianceColor, m_NumLED, m_ledBuffer);
  }

  if (frc::Timer::GetMatchTime().to<double>() <= 30 &&
      GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).GetBoolean(false)) {
    if (m_LEDIndex > m_NumLED - 1)
      m_LEDIndex = 0;
    Strobe(m_LEDIndex % 8 >= 4, m_AllianceColor, m_NumLED, m_ledBuffer);
    m_LEDIndex++;
  }

  m_LED.SetData(m_ledBuffer); 
  //LED Code end
  

  if (frc::RobotController::GetUserButton()) {
    if (Robot::GetRobot()
            ->GetCOB()
            .GetTable()
            .GetEntry("/limelight/ledMode")
            .GetDouble(2) == 1)
      Robot::GetRobot()
          ->GetCOB()
          .GetTable()
          .GetEntry("/limelight/ledMode")
          .SetDouble(0);
    else
      Robot::GetRobot()
          ->GetCOB()
          .GetTable()
          .GetEntry("/limelight/ledMode")
          .SetDouble(1);
  }

  PushDistance();

  GetCOB()
      .GetTable()
      .GetEntry(COB_KEY_FLYWHEEL_SPEED)
      .SetDouble(GetShooter().FlywheelRPM());
  // GetCOB().GetTable().GetEntry(COB_KEY_FLYWHEEL_RPM).SetDouble(GetShooter().FlywheelRPM());
  // GetCOB().GetTable().GetEntry(COB_KEY_FOD).SetBoolean(GetDriveTrain().m_FOD);
  if (GetCOB().GetTable().GetEntry(COB_KEY_NAVX_RESET).GetBoolean(false) ==
      true) {
    GetNavX().ZeroYaw();
    GetCOB().GetTable().GetEntry(COB_KEY_NAVX_RESET).SetBoolean(false);
  }
  GetCOB()
      .GetTable()
      .GetEntry(COB_KEY_ROBOT_ANGLE)
      .SetDouble(GetNavX().GetYaw());
  GetCOB()
      .GetTable()
      .GetEntry(COB_KEY_MATCH_TIME)
      .SetDouble(frc::Timer::GetMatchTime().to<double>());
  GetCOB()
      .GetTable()
      .GetEntry(COB_KEY_TICKS)
      .SetDouble(GetCOB().GetTable().GetEntry(COB_KEY_TICKS).GetDouble(0) + 1);
}

void Robot::AutonomousInit() {
  DebugOutF("Auto Init");
  GetDriveTrain().BreakMode(true);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(true);
  GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).SetBoolean(false);
  Auto* auto1 = new Auto();
  GetNavX().ZeroYaw();
  // m_Auto->DriveForward()->Schedule();
  m_Auto = auto1->TwoBallAuto();
  m_Auto->Schedule();
}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  DebugOutF("Teleop Init");
  m_Auto->Cancel();
  GetDriveTrain().BreakMode(true);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(true);
  GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).SetBoolean(true);
  m_TargetLock.WhenHeld(LockOnTarget());
}
void Robot::TeleopPeriodic() {
  m_Shooter.ShooterPeriotic();
}

void Robot::DisabledInit() {
  GetDriveTrain().BreakMode(false);
  GetCOB().GetTable().GetEntry(COB_KEY_ENABLED).SetBoolean(false);
  GetCOB().GetTable().GetEntry(COB_KEY_IS_TELE).SetBoolean(false);
}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}

void Robot::PushDistance() {
  double h = TARGET_HEIGHT - LIMELIGHT_HEIGHT;
  double angleFromGroundDeg =
      LIMELIGHT_ANGLE +
      GetCOB().GetTable().GetEntry("/limelight/ty").GetDouble(0);

  GetCOB()
      .GetTable()
      .GetEntry(COB_KEY_DISTANCE)
      .SetDouble(h / std::tan(angleFromGroundDeg * (M_PI / 180)));
}

bool Robot::InRange() {
  return GetCOB().GetTable().GetEntry(COB_KEY_DISTANCE).GetDouble(0) >= 500 &&
         GetCOB().GetTable().GetEntry(COB_KEY_DISTANCE).GetDouble(0) <= 530;
}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
