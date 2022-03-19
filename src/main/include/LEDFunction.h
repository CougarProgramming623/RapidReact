 #pragma once

 #include <frc/util/Color.h>
 #include <frc/AddressableLED.h>
 #include "Util.h"

 
void CanSee(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 190> &ledBuffer);
void EndGame(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 190> &ledBuffer);
void LowBattery(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 190> &ledBuffer);
void SetCorners(int tailLength, std::array<frc::AddressableLED::LEDData, 190> &ledBuffer, int R, int B, int G);
bool Strobe(bool white, frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 190> &ledBuffer);
