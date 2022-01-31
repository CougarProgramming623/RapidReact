#pragma once
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include <vector>

class DriveToPos : public frc2::CommandHelper<frc2::CommandBase, DriveToPos>{
public:

    DriveToPos(double x, double y, double a);

    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End();

private:

    int m_InitialTicks[4];

    int m_XTicks;
    int m_Yticks;
    double m_Angle;


};