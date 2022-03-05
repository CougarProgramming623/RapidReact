#pragma once

#include <frc2/command/button/Button.h>
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/SequentialCommandGroup.h>

//UP brings the intake to a stowed, vertical position
//DOWN brings the intake to a horizontal, ready-to-intake position
//INGEST spins the intake to bring balls INTO the robot
//EJECT spins the intake to push balls AWAY from the robot

class Intake {

	public:

	Intake();
	void IntakeInit();

	void bindUpDownButton();
	frc2::SequentialCommandGroup MoveUp();
	frc2::SequentialCommandGroup MoveDown();

	void bindIngestEjectButtons();
	frc2::InstantCommand Ingest();
	frc2::InstantCommand Eject();
	

	private:

	frc2::Button m_moveUpDownButton;

	frc2::Button m_directionIngest;
	frc2::Button m_directionEject;

	ctre::phoenix::motorcontrol::can::TalonSRX m_motorUpDown;
	ctre::phoenix::motorcontrol::can::TalonSRX m_motorInOut;
};