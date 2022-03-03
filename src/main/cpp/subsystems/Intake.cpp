#include "subsystems/Intake.h"
#include "ID.h"
#include "Util.h"
#include "Robot.h"

#include <ctre/phoenix/motorcontrol/SupplyCurrentLimitConfiguration.h>
#include <ctre/phoenix/motorcontrol/ControlMode.h>
#include <frc2/command/ParallelRaceGroup.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/WaitCommand.h>
#include <frc2/command/SequentialCommandGroup.h>


#define standardUpSpeed 0.75 
#define standardDownSpeed -0.25
#define standardInOutSpeed 1 //standard moves in
#define voltageThreshhold 1 //voltage to stop the UpDownMotor from moving further down


using ctre::phoenix::motorcontrol::ControlMode;

Intake::Intake() : 
	//buttons
	m_moveUpDownButton(BUTTON_L(INTAKE_BUTTON_UP_DOWN)),
	
	m_directionIn(BUTTON_L(INTAKE_BUTTON_IN)),
	m_directionOut(BUTTON_L(INTAKE_BUTTON_OUT)),

	//motors
	m_motorUpDown(INTAKE_UP_DOWN),
	m_motorInOut(INTAKE_IN_OUT)

{}

void Intake::IntakeInit() {
	setUpDownButton();
	setInOutButtons();
}

void Intake::setUpDownButton() {
	m_moveUpDownButton.WhileHeld([&] { m_motorUpDown.Set(ControlMode::PercentOutput, standardUpSpeed); });
	m_moveUpDownButton.WhenReleased([&] {
		frc2::SequentialCommandGroup( //blip motor down for X seconds
			frc2::InstantCommand([&] { m_motorUpDown.Set(ControlMode::PercentOutput, standardDownSpeed); }),
			frc2::InstantCommand([&]{DebugOutF("Before wait");}),
			frc2::WaitCommand(1.0_s),
			frc2::InstantCommand([&]{DebugOutF("After wait");}),
			frc2::InstantCommand([&] { m_motorUpDown.Set(ControlMode::PercentOutput, 0); })
		);
	});
}

void Intake::setInOutButtons() {
	m_directionIn.WhileHeld([&]  { m_motorInOut.Set(ControlMode::PercentOutput, standardInOutSpeed);
		DebugOutF(std::to_string(m_motorInOut.GetSelectedSensorPosition()));
	});
	m_directionOut.WhileHeld([&] { m_motorInOut.Set(ControlMode::PercentOutput, -standardInOutSpeed); });

	m_directionIn.WhenReleased([&]  { m_motorInOut.Set(ControlMode::PercentOutput, 0); });
	m_directionOut.WhenReleased([&] { m_motorInOut.Set(ControlMode::PercentOutput, 0); });
}

bool Intake::checkCurrentLimit() {
	//check if currentlimit is reached
	return false;
}