#pragma once
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include <frc/Timer.h>


class TurnToAngle : public frc2::CommandHelper<frc2::CommandBase, TurnToAngle>{
    public:

    TurnToAngle(double angle);

    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End(bool end) override;

    private:

    double m_Angle;
    int m_RotTicks;

    int m_InitialTicks[4];
    int m_FinalTicks[4];
    
    frc::Timer m_Clock = frc::Timer();

    DriveTrain& m_DriveTrain = Robot::GetRobot()->GetDriveTrain();


};