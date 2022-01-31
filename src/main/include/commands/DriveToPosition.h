#pragma once
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include <vector>

class DriveToPos : public frc2::CommandHelper<frc2::CommandBase, DriveToPos>{
public:

    explicit DriveToPos(int x, int y, double a);

    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End();

private:

    std::vector<int> m_InitialTicks;

    int m_XTicks;
    int m_Yticks;
    double m_Angle;


};