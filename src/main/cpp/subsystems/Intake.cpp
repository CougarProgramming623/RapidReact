#include "subsystems/Intake.h"
#include "ID.h"
#include "Util.h"
#include "Robot.h"

#include <ctre/phoenix/motorcontrol/SupplyCurrentLimitConfiguration.h>
#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc2/command/WaitCommand.h>


#define standardUpSpeed 1
#define standardDownSpeed -0.25
#define standardInOutSpeed 1 //standard moves in
#define voltageThreshhold 1 //voltage to stop the UpDownMotor from moving further down


using ctre::phoenix::motorcontrol::ControlMode;

Intake::Intake() : 
	//buttons
	m_moveUpDownButton([&] { return !(Robot::GetRobot()->GetButtonBoard().GetRawButton(6));}),
	
	m_directionIngest(BUTTON_L(INTAKE_BUTTON_INGEST)),
	m_directionEject(BUTTON_L(INTAKE_BUTTON_EJECT)),

	//motors
	m_motorUpDown(INTAKE_UP_DOWN),
	m_motorInOut(INTAKE_INGEST_EJECT)

{}

void Intake::IntakeInit() {
	bindUpDownButton();
	bindIngestEjectButtons();
}

void Intake::bindUpDownButton() {
	m_moveUpDownButton.WhenReleased(MoveDown());
	m_moveUpDownButton.WhileHeld(MoveUp());
}

frc2::SequentialCommandGroup Intake::MoveUp() { return frc2::SequentialCommandGroup(
	frc2::ParallelRaceGroup(frc2::FunctionalCommand([&]{
			m_motorUpDown.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
		}, [&] {//onExecute
			m_motorUpDown.Set(ControlMode::PercentOutput, standardUpSpeed);
		}, [&] (bool e) {}, [&] { return false; }, {}), frc2::WaitCommand(0.65_s)),
	frc2::FunctionalCommand([&]{}, [&] { m_motorUpDown.Set(ControlMode::PercentOutput, .25); }, [&](bool e){}, [&] {return false;}, {})
);}

frc2::SequentialCommandGroup Intake::MoveDown() { return frc2::SequentialCommandGroup( //blip motor down for X seconds
	frc2::ParallelRaceGroup(
		frc2::FunctionalCommand([&] {//onInit
				m_motorUpDown.SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
			}, [&]{//onExecute
				m_motorUpDown.Set(ControlMode::PercentOutput, standardDownSpeed); 
			}, [&](bool e){}, [&]{ return false; }, {}),
		frc2::WaitCommand(0.65_s)
	),
	frc2::InstantCommand([&] { m_motorUpDown.Set(ControlMode::PercentOutput, 0.2); })
);}

void Intake::bindIngestEjectButtons() {
	m_directionIngest.WhenPressed(Ingest());
	m_directionEject.WhenPressed(Eject());

	m_directionIngest.WhenReleased([&]  { m_motorInOut.Set(ControlMode::PercentOutput, 0); });
	m_directionEject.WhenReleased([&] { m_motorInOut.Set(ControlMode::PercentOutput, 0); });
}

frc2::InstantCommand Intake::Ingest() { return frc2::InstantCommand([&] { m_motorInOut.Set(ControlMode::PercentOutput, standardInOutSpeed); }); }
frc2::InstantCommand Intake::Eject() { return frc2::InstantCommand([&] {m_motorInOut.Set(ControlMode::PercentOutput, -standardInOutSpeed); }); }