#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>
#include "ctre/phoenix/music/Orchestra.h"
#define MUSIC_PIRATES "../deploy/"
#include "./Robot.h"

class Music : public frc2::CommandHelper<frc2::CommandBase, Music> {
public:

    Music(std::string piece);

    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End(bool end) override;
    
    inline ctre::phoenix::music::Orchestra& GetOrch(){return m_Orch;}

private:
    ctre::phoenix::music::Orchestra m_Orch{};
    std::string m_Piece;
};