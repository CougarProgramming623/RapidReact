

#pragma once
#include <frc/Timer.h>
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>

#include "./subsystems/DriveTrain.h"
#include "Robot.h"

class TurnToAngle : public frc2::CommandHelper<frc2::CommandBase, TurnToAngle> {
 public:
  TurnToAngle(std::function<double()> angle, double speed);
  TurnToAngle(double angle, double speed);

  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End(bool end) override;

  static TurnToAngle TurnToTarget();
  static double Target();

 private:
  std::function<double()> m_Angle;
  int m_RotTicks;
  double m_TargetAngle;

  int m_InitialTicks[4];
  int m_FinalTicks[4];

  double m_MaxSpeed;

  frc::Timer m_Clock = frc::Timer();

  DriveTrain& m_DriveTrain = Robot::GetRobot()->GetDriveTrain();
};
