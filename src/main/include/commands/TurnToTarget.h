#pragma once
#pragma once
#include "TurnToAngle.h"

class TurnToTarget : public frc2::CommandHelper<frc2::CommandBase, TurnToTarget>{
public: 
    TurnToTarget();

    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End(bool end) override;
private:
    double m_TargetAngle;
};