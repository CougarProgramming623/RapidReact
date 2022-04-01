

#include "commands/TurnToAngle.h"

#include "Util.h"

#define FOR_ALL_MOTORS(operation)     \
  m_DriveTrain.GetFrontR() operation; \
  m_DriveTrain.GetFrontL() operation; \
  m_DriveTrain.GetBackR() operation;  \
  m_DriveTrain.GetBackL() operation;

#define FOR_LEFT_MOTORS(operation)    \
  m_DriveTrain.GetFrontL() operation; \
  m_DriveTrain.GetBackL() operation;

#define FOR_RIGHT_MOTORS(operation)   \
  m_DriveTrain.GetFrontR() operation; \
  m_DriveTrain.GetBackR() operation;

using ctre::phoenix::motorcontrol::ControlMode;

const int kGEARBOX_RATIO = 12;
const int kTICKS_PER_ROTATION = 2048;
const double kMETERS_PER_ROTATION =
    (8.0 * 2.54) * 3.1415926 / 100.0;  //(!) FINISH (!)
const double kCOUNT_THRESHOLD =
    50;  // How close to the exact number the encoders need to be (!) Should be
         // tested (!)
const double kDERIVATIVE_THRESHOLD = .04;  //(!) Test (!)
const double kTIME_THRESHOLD = 1;          //(!) TEST (!)

TurnToAngle::TurnToAngle(std::function<double()> angle, double speed) {
  m_Angle = angle;
  m_MaxSpeed = speed;
  AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

TurnToAngle::TurnToAngle(double angle, double speed) {
  m_Angle = [angle] { return angle; };
  m_MaxSpeed = speed;
  AddRequirements(&Robot::GetRobot()->GetDriveTrain());
}

void TurnToAngle::Initialize() {
  m_RotTicks = 2048.0 / 60 * kGEARBOX_RATIO * m_Angle() * 9 / 10 * 1.028;
  m_TargetAngle = m_Angle() + Robot::GetRobot()->GetRealYaw();
  FOR_ALL_MOTORS(.Set(ControlMode::PercentOutput, 0))

  DebugOutF("TurnToAngle Initialize starting at " +
            std::to_string(Robot::GetRobot()->GetRealYaw()) +
            " going to " + std::to_string(m_TargetAngle));

  m_FinalTicks[0] =
      m_DriveTrain.GetFrontL().GetSelectedSensorPosition() + m_RotTicks;
  m_FinalTicks[1] =
      m_DriveTrain.GetFrontR().GetSelectedSensorPosition() - m_RotTicks;
  m_FinalTicks[2] =
      m_DriveTrain.GetBackL().GetSelectedSensorPosition() + m_RotTicks;
  m_FinalTicks[3] =
      m_DriveTrain.GetBackR().GetSelectedSensorPosition() - m_RotTicks;

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

void TurnToAngle::Execute() {
  m_DriveTrain.GetFrontL().Set(ControlMode::Position, m_FinalTicks[0]);
  m_DriveTrain.GetFrontR().Set(ControlMode::Position, m_FinalTicks[1]);
  m_DriveTrain.GetBackL().Set(ControlMode::Position, m_FinalTicks[2]);
  m_DriveTrain.GetBackR().Set(ControlMode::Position, m_FinalTicks[3]);
  // m_DriveTrain.GetFrontL().Set(ControlMode::PercentOutput, m_MaxSpeed *
  // (m_FinalTicks[0] > m_InitialTicks[0]) ? 1 : -1);
  // m_DriveTrain.GetFrontR().Set(ControlMode::PercentOutput, m_MaxSpeed *
  // (m_FinalTicks[1] > m_InitialTicks[1]) ? 1 : -1);
  // m_DriveTrain.GetBackL().Set(ControlMode::PercentOutput, m_MaxSpeed *
  // (m_FinalTicks[2] > m_InitialTicks[2]) ? 1 : -1);
  // m_DriveTrain.GetBackR().Set(ControlMode::PercentOutput, m_MaxSpeed *
  // (m_FinalTicks[3] > m_InitialTicks[3]) ? 1 : -1);
}

bool TurnToAngle::IsFinished() {
  return
      // (std::abs(m_DriveTrain.GetFrontL().GetSelectedSensorPosition() -
      // (m_FinalTicks[0])) <= kCOUNT_THRESHOLD &&
      //  std::abs(m_DriveTrain.GetFrontR().GetSelectedSensorPosition() -
      //  (m_FinalTicks[1])) <= kCOUNT_THRESHOLD &&
      //  std::abs(m_DriveTrain.GetBackL().GetSelectedSensorPosition()  -
      //  (m_FinalTicks[2])) <= kCOUNT_THRESHOLD &&
      //  std::abs(m_DriveTrain.GetBackR().GetSelectedSensorPosition()  -
      //  (m_FinalTicks[3])) <= kCOUNT_THRESHOLD) || (m_Clock.Get() >=
      //  (units::time::second_t) kTIME_THRESHOLD &&
      // (std::abs(m_DriveTrain.GetFrontL().GetErrorDerivative()) <=
      // kDERIVATIVE_THRESHOLD ||
      //  std::abs(m_DriveTrain.GetFrontR().GetErrorDerivative()) <=
      //  kDERIVATIVE_THRESHOLD ||
      //  std::abs(m_DriveTrain.GetBackL().GetErrorDerivative())  <=
      //  kDERIVATIVE_THRESHOLD ||
      //  std::abs(m_DriveTrain.GetBackR().GetErrorDerivative())  <=
      //  kDERIVATIVE_THRESHOLD )) ||
      (std::abs(Robot::GetRobot()->GetRealYaw() - m_TargetAngle) <= 1);
}

void TurnToAngle::End(bool end) {
  DebugOutF("TurnToAngle Finished");
  FOR_ALL_MOTORS(.ConfigPeakOutputForward(1, 0))
  FOR_ALL_MOTORS(.ConfigPeakOutputReverse(-1, 0))
  m_DriveTrain.BreakMode(true);
  FOR_ALL_MOTORS(.Set(ControlMode::PercentOutput, 0))
}

TurnToAngle TurnToAngle::TurnToTarget() {
  return TurnToAngle([&] { return Target(); }, 0.2);
}

double TurnToAngle::Target() {
  return Robot::GetRobot()
      ->GetCOB()
      .GetTable()
      .GetEntry("/limelight/tx")
      .GetDouble(0);
}
