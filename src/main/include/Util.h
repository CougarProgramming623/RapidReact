#pragma once

#include <frc2/command/PrintCommand.h>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

inline void DebugOutF(const std::string_view message) {
	frc2::CommandScheduler::GetInstance().Schedule(new frc2::PrintCommand(message));
}
// this defines a lambda which when run returns the current button state of the raw button specified by ID. Is taken in by the frc::Button constuctor
#define BUTTON_L(id) [&] { return Robot::GetRobot()->GetButtonBoard().GetRawButton(id);}


template<typename T> class NTObj {
  public:
    void operator=(T t) {
      
    }
    operator T() {
		return convert();
    }
	T convert();
    NTObj(std::string_view key) {
		m_Entry = nt::NetworkTableInstance::GetDefault().GetEntry(key)
	}
    nt::NetworkTableEntry m_Entry;
};
template <typename T = int> class NTObj<int> {
	public:
	int convert() { return (int)(m_Entry.GetDouble()); }
};

template <typename T> NTObj<T> nt(std::string_view key) {
  return NTObj<T>(key);
}