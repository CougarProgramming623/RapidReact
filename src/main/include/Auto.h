

#pragma once
#include <frc2/command/SequentialCommandGroup.h>

#include "commands/DriveToPosition.h"
#include "commands/TurnToAngle.h"
#include "frc2/command/WaitCommand.h"
#include "subsystems/DriveTrain.h"

class Auto {
 public:
  Auto();
  void AutoInit();

  frc2::SequentialCommandGroup* TwoBallAuto();
  frc2::SequentialCommandGroup* TwoAndPrep();

 private:
};
