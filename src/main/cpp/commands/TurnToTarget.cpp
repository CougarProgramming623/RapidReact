#include "commands/TurnToTarget.h"
#include "commands/TurnToAngle.h"

TurnToTarget::TurnToTarget(){

}

void TurnToTarget::Initialize(){
    m_TargetAngle = Robot::GetRobot()->GetCOB().GetTable().GetEntry("/limelight/tx").GetDouble(0);
}

void TurnToTarget::Execute(){
    TurnToAngle(m_TargetAngle, 0.07);
}

bool TurnToTarget::IsFinished(){
    return true;
}

void TurnToTarget::End(bool end){
    
}