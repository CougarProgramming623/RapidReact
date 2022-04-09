

#pragma once
#include <ctre/phoenix/motorcontrol/can/TalonFX.h>
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>
#include <frc/AnalogInput.h>
#include <frc/DigitalInput.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/WaitUntilCommand.h>
#include <frc2/command/button/Button.h>

#include "ID.h"

class Climb : public frc2::SubsystemBase {
 public:
  Climb();

  void ClimbInit();
  void BreakMode(bool on);

  frc2::FunctionalCommand* ManualClimb();

  // boolean pastcurrentstep(n) {
  //     return current dial position >= n
  // }

  // seq(
  //     waituntil { pastcurrentstep(1) }
  //     FunctionaCommand([]w[]w[]w[]w][], {&this}),
  //     waituntil { pastcurrentstep(2) }
  // )

 private:
  ctre::phoenix::motorcontrol::can::TalonSRX m_PivotArm;
  ctre::phoenix::motorcontrol::can::TalonFX m_PullUpArm;

  frc::AnalogInput m_Analog{3};
  frc::AnalogInput m_AnalogUp{2}; //Change (!)
  frc::DigitalInput m_LowBound{0};

  frc2::Button m_AutoLock;
  frc2::Button m_TeleLock;
  frc2::Button m_MainLock;

  double m_MinSensorPosition;
};
