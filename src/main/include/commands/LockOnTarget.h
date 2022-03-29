

#pragma once
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>

class LockOnTarget
    : public frc2::CommandHelper<frc2::CommandBase, LockOnTarget> {
 public:
  explicit LockOnTarget(bool);

  void Initialize() override;
  void Execute() override;

 private:
  double m_Angle;
  bool m_noJoystick;

};
