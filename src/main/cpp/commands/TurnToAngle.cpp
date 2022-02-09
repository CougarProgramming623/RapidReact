#include "commands/TurnToAngle.h"
#include "Util.h"
#include "Robot.h"

#define FOR_ALL_MOTORS(operation) m_DriveTrain.GetFrontR()operation;\
                                  m_DriveTrain.GetFrontL()operation;\
                                  m_DriveTrain.GetBackR()operation;\
                                  m_DriveTrain.GetBackL()operation; 
                                  
#define FOR_LEFT_MOTORS(operation) m_DriveTrain.GetFrontL()operation;\
                                   m_DriveTrain.GetBackL()operation; 

#define FOR_RIGHT_MOTORS(operation) m_DriveTrain.GetFrontR()operation;\
                                    m_DriveTrain.GetBackR()operation;

using ctre::phoenix::motorcontrol::ControlMode;

    const int kGEARBOX_RATIO = 12;
    const int kTICKS_PER_ROTATION = 2048;     
    const double kMETERS_PER_ROTATION = (8.0 * 2.54) * 3.1415926 / 100.0;     //(!) FINISH (!)
    const double kCOUNT_THRESHOLD = 500;      //How close to the exact number the encoders need to be (!) Should be tested (!)
    const double kDERIVATIVE_THRESHOLD = .1;  //(!) Test (!)
    const double kTIME_THRESHOLD = 1;         //(!) TEST (!)

    TurnToAngle::TurnToAngle(double angle){
        m_Angle = angle;




        AddRequirements(&Robot::GetRobot()->GetDriveTrain());
    }

    void TurnToAngle::Initialize(){
        DebugOutF("TurnToAngle Initialize");

        FOR_ALL_MOTORS(.Set(ControlMode::PercentOutput, 0))


        m_FinalTicks[0] = m_DriveTrain.GetFrontL().GetSelectedSensorPosition() + m_RotTicks;
        m_FinalTicks[1] = m_DriveTrain.GetFrontR().GetSelectedSensorPosition() + m_RotTicks;
        m_FinalTicks[2] = m_DriveTrain.GetBackL().GetSelectedSensorPosition()  + m_RotTicks;
        m_FinalTicks[3] = m_DriveTrain.GetBackR().GetSelectedSensorPosition()  + m_RotTicks;

        m_InitialTicks[0] = m_DriveTrain.GetFrontL().GetSelectedSensorPosition();
        m_InitialTicks[1] = m_DriveTrain.GetFrontR().GetSelectedSensorPosition();
        m_InitialTicks[2] = m_DriveTrain.GetBackL().GetSelectedSensorPosition();
        m_InitialTicks[3] = m_DriveTrain.GetBackR().GetSelectedSensorPosition();

        m_DriveTrain.BreakMode(true);
        m_DriveTrain.UsePostionPID();

        FOR_ALL_MOTORS(.ConfigPeakOutputForward(0.3, 0))
        FOR_ALL_MOTORS(.ConfigPeakOutputReverse(-0.3, 0))

        FOR_ALL_MOTORS(.ConfigClosedloopRamp(.06, 0))

        m_Clock.Start();



    }

    void TurnToAngle::Execute(){

    }

    bool TurnToAngle::IsFinished(){
        return  
            (abs(m_DriveTrain.GetFrontL().GetSelectedSensorPosition() - (m_FinalTicks[0])) <= kCOUNT_THRESHOLD &&
             abs(m_DriveTrain.GetFrontR().GetSelectedSensorPosition() - (m_FinalTicks[1])) <= kCOUNT_THRESHOLD &&
             abs(m_DriveTrain.GetBackL().GetSelectedSensorPosition()  - (m_FinalTicks[2])) <= kCOUNT_THRESHOLD &&
             abs(m_DriveTrain.GetBackR().GetSelectedSensorPosition()  - (m_FinalTicks[3])) <= kCOUNT_THRESHOLD) ||
            (m_Clock.Get() >= (units::time::second_t) kTIME_THRESHOLD &&
            (abs(m_DriveTrain.GetFrontL().GetErrorDerivative()) <= kDERIVATIVE_THRESHOLD ||
             abs(m_DriveTrain.GetFrontR().GetErrorDerivative()) <= kDERIVATIVE_THRESHOLD ||
             abs(m_DriveTrain.GetBackL().GetErrorDerivative())  <= kDERIVATIVE_THRESHOLD ||
             abs(m_DriveTrain.GetBackR().GetErrorDerivative())  <= kDERIVATIVE_THRESHOLD ));

    }

    void TurnToAngle::End(bool end){

    }