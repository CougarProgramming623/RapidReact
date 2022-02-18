#include "commands/Music.h"


Music::Music(std::string piece) {
    m_Piece = piece;
    AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

void Music::Initialize(){
    m_Orch.AddInstrument(Robot::GetRobot()->GetDriveTrain().GetFrontL());
    m_Orch.AddInstrument(Robot::GetRobot()->GetDriveTrain().GetFrontR());
    m_Orch.AddInstrument(Robot::GetRobot()->GetDriveTrain().GetBackL());
    m_Orch.AddInstrument(Robot::GetRobot()->GetDriveTrain().GetBackR());
    m_Orch.AddInstrument(Robot::GetRobot()->GetShooter().GetFlyBack());
    m_Orch.AddInstrument(Robot::GetRobot()->GetShooter().GetFlyFront());
    m_Orch.LoadMusic(m_Piece);
    m_Orch.Play();
}

void Music::Execute(){

}

bool Music::IsFinished(){
    return !m_Orch.IsPlaying();
}

void Music::End(bool e){}