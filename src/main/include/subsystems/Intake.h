#pragma once

#include <frc2/command/button/Button.h>
#include <ctre/phoenix/motorcontrol/can/TalonSRX.h>


//UP brings the intake to a stowed, vertical position
//DOWN brings the intake to a horizontal, ready-to-intake position
//IN spins the intake to bring balls INTO the robot
//OUT spins the intake to push balls AWAY from the robot

class Intake {

	public:

	Intake();
	void IntakeInit();
	void setUpDownButtons();
	void setInOutButtons();

	private:

	frc2::Button m_moveUp;
	frc2::Button m_moveDown;

	frc2::Button m_directionIn;
	frc2::Button m_directionOut;

	ctre::phoenix::motorcontrol::can::TalonSRX m_motorUpDown;
	ctre::phoenix::motorcontrol::can::TalonSRX m_motorInOut;
};