

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

  frc2::SequentialCommandGroup* StandardFourBall();
  frc2::SequentialCommandGroup* CramAuto();
  frc2::ParallelRaceGroup* DriveForward();
  frc2::SequentialCommandGroup* DriveBackAndShoot();
  frc2::SequentialCommandGroup* ShootAndDriveBack();
  frc2::SequentialCommandGroup* TwoBallAuto();

 private:
};
