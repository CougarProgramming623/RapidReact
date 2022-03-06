#pragma once
#include <frc2/command/SequentialCommandGroup.h>
#include "frc2/command/WaitCommand.h"
#include "subsystems/DriveTrain.h"
#include "commands/DriveToPosition.h"
#include "commands/TurnToAngle.h"

class Auto {
public:
    Auto();
    void AutoInit();

    frc2::SequentialCommandGroup* StandardFourBall();

private:


};