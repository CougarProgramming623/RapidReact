#include "subsystems/Intake.h"
#include "ID.h"
#include "Util.h"
#include "Robot.h"

#include <ctre/phoenix/motorcontrol/ControlMode.h>

#define standardUpDownSpeed 0.1 //standard moves up
#define standardInOutSpeed 0.1 //standard moves in


using ctre::phoenix::motorcontrol::ControlMode;

Intake::Intake() : 
	//buttons
	m_moveUp(BUTTON_L(INTAKE_BUTTON_UP)),
	m_moveDown(BUTTON_L(INTAKE_BUTTON_DOWN)),
	
	m_directionIn(BUTTON_L(INTAKE_BUTTON_IN)),
	m_directionOut(BUTTON_L(INTAKE_BUTTON_OUT)),

	//motors
	m_motorUpDown(INTAKE_UP_DOWN),
	m_motorInOut(INTAKE_IN_OUT)

{}

void Intake::IntakeInit() {
	setUpDownButtons();
	setInOutButtons();
}

void Intake::setUpDownButtons() {
	m_moveUp.WhileHeld([&]   { m_motorUpDown.Set(ControlMode::PercentOutput, -standardInOutSpeed); });
	m_moveDown.WhileHeld([&] { m_motorUpDown.Set(ControlMode::PercentOutput, standardInOutSpeed);  });
}

void Intake::setInOutButtons() {
	m_directionIn.WhileHeld([&]  { m_motorInOut.Set(ControlMode::PercentOutput, standardInOutSpeed);  });
	m_directionOut.WhileHeld([&] { m_motorInOut.Set(ControlMode::PercentOutput, -standardInOutSpeed); });
}

