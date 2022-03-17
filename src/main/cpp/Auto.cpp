#include "Auto.h"
#include "subsystems/Shooter.h"
#include "frc2/command/ParallelDeadlineGroup.h"
#include "Robot.h"

using ctre::phoenix::motorcontrol::ControlMode;

Auto::Auto(){

}

void Auto::AutoInit(){
    Robot::GetRobot()->GetNavX().ZeroYaw();
}

frc2::ParallelRaceGroup* Auto::DriveForward() {

    frc2::FunctionalCommand driving = frc2::FunctionalCommand([&] { //onInit

        }, [&]{ //onExecute
            Robot::GetRobot()->GetDriveTrain().CartesianDrive(-.4, 0, 0, 0, false);
        }, [&] (bool e) { //onEnd
            Robot::GetRobot()->GetDriveTrain().CartesianDrive(0, 0, 0, 0, false);
        }, [&] {//isFinished
            return false;
        }
    );
    driving.AddRequirements(&Robot::GetRobot()->GetDriveTrain());
    
    return new frc2::ParallelRaceGroup(frc2::WaitCommand(2_s), driving);
        
}

frc2::SequentialCommandGroup* DriveBackAndShoot(){
    frc2::ParallelRaceGroup drive = frc2::ParallelRaceGroup(
        frc2::WaitUntilCommand([&]{return Robot::GetRobot()->GetCOB().GetTable().GetEntry(COB_KEY_DISTANCE).GetDouble(0) > 505;}),
        frc2::FunctionalCommand([&] { //onInit

        }, [&]{ //onExecute
            Robot::GetRobot()->GetDriveTrain().CartesianDrive(-.4, 0, 0, 0, false);
        }, [&] (bool e) { //onEnd
            Robot::GetRobot()->GetDriveTrain().CartesianDrive(0, 0, 0, 0, false);
        }, [&] {//isFinished
            return false;
        })
    );
    return NULL;
}


frc2::SequentialCommandGroup* Auto::StandardFourBall(){
    return new frc2::SequentialCommandGroup(
        //Intake Down and spin
        TurnToAngle(180, 0.5),
        DriveToPos(1.3, 0, 0),
        frc2::WaitCommand(0.3_s),
        TurnToAngle(110, 0.5),
        TurnToAngle::TurnToTarget(),
        Robot::GetRobot()->GetShooter().ShootingCommand(),
        TurnToAngle([]{ return 165 - (double)Robot::GetRobot()->GetNavX().GetYaw();}, 0.5),
        DriveToPos(1.3, 0, 0),
        frc2::WaitCommand(2_s),
        TurnToAngle(150, 0.5),
        //Intake up
        TurnToAngle::TurnToTarget(),
        DriveToPos(1.3, 0, 0),
        TurnToAngle::TurnToTarget(),
        Robot::GetRobot()->GetShooter().ShootingCommand()
    );
}

frc2::SequentialCommandGroup* Auto::ShootAndDriveBack() {
    return new frc2::SequentialCommandGroup(
        frc2::ParallelRaceGroup(
            *Robot::GetRobot()->GetShooter().ScaleToDistanceCommand(),
            frc2::FunctionalCommand([&]{ //onInit
                }, [&]{//onExecute
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(ControlMode::PercentOutput, -1);
                }, [&](bool e){ //onEnd
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(ControlMode::PercentOutput, 0);
                }, [&]{return false;}, {}),
            frc2::WaitCommand(2_s)
        ),
        DriveToPos(2, 0, 0));
}

frc2::SequentialCommandGroup* Auto::TwoBallAuto() {
    frc2::SequentialCommandGroup* group = new frc2::SequentialCommandGroup();
    group->AddCommands(
        frc2::ParallelDeadlineGroup(
            DriveToPos(2.5, 0, 0),
            std::move(*Robot::GetRobot()->GetIntake().MoveDown()),
            std::move(*Robot::GetRobot()->GetIntake().Ingest())
        )
    );
    group->AddCommands(
        frc2::ParallelRaceGroup(
            DriveToPos(-2.3, 0, 0),
            std::move(*Robot::GetRobot()->GetIntake().MoveUp())
        ),
        TurnToAngle(-90, 0.2),
        frc2::WaitCommand(1_s),
        TurnToAngle::TurnToTarget(),
        frc2::ParallelDeadlineGroup(
            frc2::WaitCommand(10_s),
            std::move(*Robot::GetRobot()->GetShooter().ScaleToDistanceCommand()),
            frc2::SequentialCommandGroup(
                frc2::WaitCommand(2_s),
                frc2::FunctionalCommand([&]{ //onInit
                }, [&]{//onExecute
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(ControlMode::PercentOutput, -1);
                }, [&](bool e){ //onEnd
                    Robot::GetRobot()->GetShooter().GetFeeder().Set(ControlMode::PercentOutput, 0);
                }, [&]{return false;}, {})
            )
            
        )
    );
    return group;
}