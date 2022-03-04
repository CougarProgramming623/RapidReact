#pragma once
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include <vector>
#include <frc/Timer.h>
#include "./Robot.h"

class DriveToPos : public frc2::CommandHelper<frc2::CommandBase, DriveToPos> {
public:

    DriveToPos(double x, double y, double a);

    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End(bool end) override;

private:


    int m_InitialTicks[4];
    int m_FinalTicks[4];
    
    frc::Timer m_Clock = frc::Timer();

    // const double HORIZONTAL_CALIBRATION = 36.0/31.0; 
	// const double VERTICAL_CALIBRATION = 36.0/43.0;

    int m_XTicks;
    int m_YTicks;
    double m_Angle;

    DriveTrain& m_DriveTrain = Robot::GetRobot()->GetDriveTrain();

};