

#include "Auto.h"

#include "Robot.h"
#include "commands/LockOnTarget.h"
#include "frc2/command/ParallelDeadlineGroup.h"
#include "frc2/command/PrintCommand.h"
#include "subsystems/Shooter.h"
#include "commands/LockOnTarget.h"

using ctre::phoenix::motorcontrol::ControlMode;

Auto::Auto() {}

void Auto::AutoInit() {
  Robot::GetRobot()->GetNavX().ZeroYaw();
}



frc2::SequentialCommandGroup* Auto::TwoBallAuto() {
  frc2::SequentialCommandGroup* group = new frc2::SequentialCommandGroup();
  group->AddCommands(frc2::ParallelDeadlineGroup(
      DriveToPos(1.5, 0, 0),
      std::move(*Robot::GetRobot()->GetIntake().MoveDown()),
      frc2::SequentialCommandGroup(
          frc2::ParallelDeadlineGroup(
              frc2::WaitCommand(0.5_s),
              std::move(*Robot::GetRobot()->GetIntake().Eject())),
          std::move(*Robot::GetRobot()->GetIntake().Ingest()))));
  group->AddCommands(
      frc2::PrintCommand("Starting race group"),
      frc2::ParallelRaceGroup(
          DriveToPos(-1.3, 0, 0),
          std::move(*Robot::GetRobot()->GetIntake().MoveUp())),
      frc2::PrintCommand("Finished race group"),
      // TurnToAngle(-90, 0.2),
      frc2::PrintCommand("Finished turn"),
      // TurnToAngle::TurnToTarget(),
      frc2::ParallelRaceGroup(frc2::WaitCommand(2_s), LockOnTarget(true)),
      frc2::ParallelDeadlineGroup(
          frc2::WaitCommand(10_s),
          std::move(*Robot::GetRobot()->GetShooter().ScaleToDistanceCommand()),
          frc2::SequentialCommandGroup(
              frc2::WaitCommand(1_s),
              frc2::FunctionalCommand(
                  [&] {  // onInit
                  },
                  [&] {  // onExecute
                    DebugOutF("running feeder");
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(
                        ControlMode::PercentOutput, -1);
                  },
                  [&](bool e) {  // onEnd
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(
                        ControlMode::PercentOutput, 0);
                  },
                  [&] { return false; }, {}))

              ));
  return group;
}

frc2::SequentialCommandGroup* Auto::TwoAndPrep() {
  frc2::SequentialCommandGroup* group = new frc2::SequentialCommandGroup();
  group->AddCommands(frc2::ParallelDeadlineGroup(
      DriveToPos(2, 0, 0),
      std::move(*Robot::GetRobot()->GetIntake().MoveDown()),
      frc2::SequentialCommandGroup(
          frc2::ParallelDeadlineGroup(
              frc2::WaitCommand(0.5_s),
              std::move(*Robot::GetRobot()->GetIntake().Eject())),
          std::move(*Robot::GetRobot()->GetIntake().Ingest()))));
  group->AddCommands(
      frc2::ParallelRaceGroup(
          DriveToPos(-2, 0, 0),
          std::move(*Robot::GetRobot()->GetIntake().MoveUp())
      ),
      frc2::ParallelRaceGroup(frc2::WaitCommand(2_s), TurnToAngle(-30, .02)),
      frc2::ParallelDeadlineGroup(
          frc2::WaitCommand(4_s),
          std::move(*Robot::GetRobot()->GetShooter().ScaleToDistanceCommand()),
          frc2::SequentialCommandGroup(
              frc2::WaitCommand(2_s),
              frc2::FunctionalCommand(
                  [&] {  // onInit
                  },
                  [&] {  // onExecute
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(
                        ControlMode::PercentOutput, -1);
                  },
                  [&](bool e) {  // onEnd
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(
                        ControlMode::PercentOutput, 0);
                  },
                  [&] { return false; }, {}))
                  
              ),
      
      TurnToAngle(40, 0.2), 
      DriveToPos(4.5, 0, 0)
  );
  return group;
}

frc2::SequentialCommandGroup* Auto::Taxi() {}

frc2::SequentialCommandGroup* Auto::Nothing() {}