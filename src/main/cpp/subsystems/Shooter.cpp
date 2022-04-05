

#include "subsystems/Shooter.h"

#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <ctre/phoenix/motorcontrol/InvertType.h>
#include <frc2/command/Command.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc2/command/WaitCommand.h>

#include "ID.h"
#include "Robot.h"
#include "Util.h"
#include "ctre/phoenix/motorcontrol/can/TalonFX.h"

using ctre::phoenix::motorcontrol::ControlMode;
using ctre::phoenix::motorcontrol::DemandType;

Shooter::Shooter()
    : m_FlywheelFront(FLYWHEEL_FRONT),  // master
      m_FlywheelBack(FLYWHEEL_BACK),    // slave
      m_Feeder(FEEDER),

      m_feederButton(BUTTON_L(FEED_BUTTON)),
      m_speedDial(
          [&] { return Robot::GetRobot()->GetButtonBoard().GetRawAxis(0); }),
      m_shootSpeed(BUTTON_L(FLYWHEEL_BUTTON_BY_SPEED)),
      m_shootDistance(BUTTON_L(FLYWHEEL_BUTTON_BY_DISTANCE)),
      m_ShootButton(BUTTON_L(SHOOT_BUTTON)) {}

constexpr double kSaturationMax = 11;

void Shooter::ShooterInit() {
  DebugOutF("Shooter Init");
  // FeederButton();
  FlywheelButton();
  ShooterPeriotic();

  m_FlywheelBack.Set(ControlMode::Follower, FLYWHEEL_FRONT);
  m_FlywheelBack.SetInverted(
      ctre::phoenix::motorcontrol::InvertType::OpposeMaster);

  m_FlywheelFront.EnableVoltageCompensation(true);
  m_FlywheelFront.ConfigVoltageCompSaturation(kSaturationMax);

  const double kP = 0;  // 0.067627; //0.46437;  //0.087797;
  const double kD = 0;
  const double kI = 0;

  m_FlywheelFront.Config_kP(0, kP);
  m_FlywheelFront.Config_kD(0, kD);
  m_FlywheelFront.Config_kI(0, kI);
  m_FlywheelFront.Config_kF(0, 0);
}

void Shooter::SetRPM(double rpm) {
  const double kS = 0.48939;  // 0.52041;
  const double kV = 0.10902;  // 0.10875;
  // const double kA = 0.0048056; //0.0043097; // we don't use this so it's
  // commented out to stop the warning

  double ticksPer100ms = rpm / 600 * 2048;
  double rps = rpm / 60;

  double feedForwardVoltage = kS + kV * rps;

  m_FlywheelFront.Set(ControlMode::Velocity, ticksPer100ms,
                      DemandType::DemandType_ArbitraryFeedForward,
                      feedForwardVoltage / kSaturationMax);
  // m_FlywheelFront.Set(ControlMode::PercentOutput,
  // feedForwardVoltage/kSaturationMax);
  Robot::GetRobot()
      ->GetCOB()
      .GetTable()
      .GetEntry(COB_KEY_FLYWHEEL_SETPOINT)
      .SetDouble(rpm);
  Robot::GetRobot()
      ->GetCOB()
      .GetTable()
      .GetEntry("COB/RPMError")
      .SetDouble(rpm - FlywheelRPM());
}

void Shooter::ShooterPeriotic() {
  bool canSafeShoot =
      // (!Robot::GetRobot()->GetNavX().IsMoving())  &&
      std::abs(Robot::GetRobot()
                   ->GetCOB()
                   .GetTable()
                   .GetEntry(COB_KEY_LIME_LIGHT_TX)
                   .GetDouble(0)) < 1 &&
      std::abs(Robot::GetRobot()
                   ->GetCOB()
                   .GetTable()
                   .GetEntry(COB_KEY_LIME_LIGHT_TV)
                   .GetDouble(0)) > 0;

  if ((Robot::GetRobot()->GetButtonBoard().GetRawButton(SHOOT_BUTTON) &&
       canSafeShoot) ||
      Robot::GetRobot()->GetButtonBoard().GetRawButton(FEED_BUTTON)) {
    m_Feeder.Set(ControlMode::PercentOutput, -1);
  } else {
    m_Feeder.Set(ControlMode::PercentOutput, 0);
  }
}

frc2::FunctionalCommand Shooter::ShootOnReadyCommand() {
  return frc2::FunctionalCommand(
      [&] { DebugOutF("Shooting when ready"); },  // OnInit
      [&] {
        double setpoint = m_FlywheelFront.GetClosedLoopTarget();
        // DebugOutF(std::to_string(std::abs(m_FlywheelFront.GetSelectedSensorVelocity()
        // - setpoint) * 600 / 2048));
        if (std::abs(m_FlywheelFront.GetSelectedSensorVelocity() - setpoint) *
                    600 / 2048 <=
                70 &&
            m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048 >= 100 &&
            setpoint * 600 / 2048 >= 100) {
          m_Feeder.Set(ControlMode::PercentOutput, -0.5);
          // DebugOutF("Shooting");
        } else {
          m_Feeder.Set(ControlMode::PercentOutput, 0);
          // DebugOutF("Not shooting");
        }
      },
      [&](bool e) { m_Feeder.Set(ControlMode::PercentOutput, 0); },
      [&] { return false; });
}

void Shooter::FeederButton() {
  m_feederButton.WhenPressed(frc2::InstantCommand(
      [&] { m_Feeder.Set(ControlMode::PercentOutput, -1); }));

  m_feederButton.WhenReleased(frc2::InstantCommand(
      [&] { m_Feeder.Set(ControlMode::PercentOutput, 0); }));
}

void Shooter::FlywheelButton() {
  m_shootSpeed.WhenHeld(frc2::FunctionalCommand(
      [&] {},
      [&] {  // onExecute
        // m_FlywheelFront.Set(ControlMode::PercentOutput,
        // (Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) / 2);
        double dialPos =
            (Robot::GetRobot()->GetButtonBoard().GetRawAxis(0) + 1) / 2;
        SetRPM(dialPos * 6000);
      },
      [&](bool e) {  // onEnd
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);
      },
      [&] { return false; }, {}));

  m_shootDistance.WhenHeld(ScaleToDistanceCommand());
}

double Shooter::FlywheelRPM() {
  return m_FlywheelFront.GetSelectedSensorVelocity() * 600 / 2048;
}

frc2::InstantCommand Shoot() {
  return frc2::InstantCommand([&] {});
}

frc2::FunctionalCommand* Shooter::ScaleToDistanceCommand() {
  return new frc2::FunctionalCommand(
      [&] {  // onInit

      },
      [&] {  // onExecute
        double distance = Robot::GetRobot()
                              ->GetCOB()
                              .GetTable()
                              .GetEntry(COB_KEY_DISTANCE)
                              .GetDouble(0) /
                          2.54;  // cm
        double RPM = distance * 28.2 + 1991 +
                     (500 * Robot::GetRobot()->GetButtonBoard().GetRawAxis(1));

        // double smoothRPM = runningAverage.Calculate(distance) * 28.2 + 1831;

        Robot::GetRobot()
            ->GetCOB()
            .GetTable()
            .GetEntry("/COB/flywheelSpeedSetpoint")
            .SetDouble(RPM);

        SetRPM(RPM);

      },
      [&](bool e) {  // onEnd
        m_FlywheelFront.Set(ControlMode::PercentOutput, 0);

      },
      [&] {  // isFinished
        return false;
      });
}

frc2::ParallelRaceGroup Shooter::ShootingCommand() {
  return frc2::ParallelRaceGroup(*ScaleToDistanceCommand(),
                                 ShootOnReadyCommand(), frc2::WaitCommand(4_s));
}
