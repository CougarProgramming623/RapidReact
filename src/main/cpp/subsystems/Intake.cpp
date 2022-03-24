

#include "subsystems/Intake.h"

#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <ctre/phoenix/motorcontrol/SupplyCurrentLimitConfiguration.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc2/command/WaitCommand.h>

#include "ID.h"
#include "Robot.h"
#include "Util.h"

#define standardUpSpeed .4
#define standardDownSpeed -0.25
#define standardInOutSpeed 1  // standard moves in
#define voltageThreshhold \
  1  // voltage to stop the UpDownMotor from moving further down

using ctre::phoenix::motorcontrol::ControlMode;

Intake::Intake()
    :  // buttons
      m_moveUpDownButton([&] {
        return !(Robot::GetRobot()->GetButtonBoard().GetRawButton(6));
      }),

      m_directionIngest(BUTTON_L(INTAKE_BUTTON_INGEST)),
      m_directionEject(BUTTON_L(INTAKE_BUTTON_EJECT)),

      // motors
      m_motorUpDown(INTAKE_UP_DOWN),
      m_motorInOut(INTAKE_INGEST_EJECT)

{}

void Intake::IntakeInit() {
  bindUpDownButton();
  bindIngestEjectButtons();
}

void Intake::bindUpDownButton() {
  m_moveUpDownButton.WhenReleased(MoveDown());
  m_moveUpDownButton.WhenPressed(MoveUp());
}

frc2::SequentialCommandGroup* Intake::MoveUp() {
  auto command = new frc2::SequentialCommandGroup(
      frc2::ParallelRaceGroup(
          frc2::FunctionalCommand(
              [&] {
                m_motorUpDown.SetNeutralMode(
                    ctre::phoenix::motorcontrol::NeutralMode::Brake);
              },
              [&] {  // onExecute
                m_motorUpDown.Set(ControlMode::PercentOutput, -standardUpSpeed);
              },
              [&](bool e) {}, [&] { return false; }, {}),
          frc2::WaitCommand(2_s)),
      frc2::FunctionalCommand(
          [&] {}, [&] { m_motorUpDown.Set(ControlMode::PercentOutput, -.2); },
          [&](bool e) {}, [&] { return false; }, {}));
  command->AddRequirements(this);
  return command;
}

frc2::SequentialCommandGroup* Intake::MoveDown() {
  auto command =
      new frc2::SequentialCommandGroup(  // blip motor down for X seconds
          frc2::ParallelRaceGroup(
              frc2::FunctionalCommand(
                  [&] {  // onInit
                    m_motorUpDown.SetNeutralMode(
                        ctre::phoenix::motorcontrol::NeutralMode::Coast);
                  },
                  [&] {  // onExecute
                    m_motorUpDown.Set(ControlMode::PercentOutput,
                                      -standardDownSpeed);
                  },
                  [&](bool e) {}, [&] { return false; }, {}),
              frc2::WaitCommand(0.65_s)),
          frc2::InstantCommand(
              [&] { m_motorUpDown.Set(ControlMode::PercentOutput, 0); }));
  command->AddRequirements(this);
  return command;
}

void Intake::bindIngestEjectButtons() {
  m_directionIngest.WhileActiveOnce(*Ingest());
  m_directionEject.WhileActiveOnce(*Eject());
}

frc2::FunctionalCommand* Intake::Ingest() {
  return new frc2::FunctionalCommand(
      [&] {  // onInit
      },
      [&] {  // onExecute
        m_motorInOut.Set(ControlMode::PercentOutput, -standardInOutSpeed);
      },
      [&](bool e) { m_motorInOut.Set(ControlMode::PercentOutput, 0); },
      [&] { return false; },
      {

      });
}
frc2::FunctionalCommand* Intake::Eject() {
  return new frc2::FunctionalCommand(
      [&] {  // onInit
      },
      [&] {  // onExecute
        m_motorInOut.Set(ControlMode::PercentOutput, standardInOutSpeed);
      },
      [&](bool e) { m_motorInOut.Set(ControlMode::PercentOutput, 0); },
      [&] { return false; },
      {

      });
}
