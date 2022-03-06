#include "Auto.h"
#include "subsystems/Shooter.h"
#include "Robot.h"

Auto::Auto(){

}

void Auto::AutoInit(){
    Robot::GetRobot()->GetNavX().ZeroYaw();
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