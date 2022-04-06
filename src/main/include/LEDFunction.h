
#pragma once

#include <frc/AddressableLED.h>
#include <frc/util/Color.h>

#include "Util.h"

void CanSee(frc::Color allianceColor, int numLED,
            std::array<frc::AddressableLED::LEDData, 280>& ledBuffer);
void LowBattery(frc::Color allianceColor, int numLED, int tailLength,
                int LEDIndex,
                std::array<frc::AddressableLED::LEDData, 280>& ledBuffer);
void SetCorners(int tailLength,
                std::array<frc::AddressableLED::LEDData, 280>& ledBuffer, int R,
                int B, int G);
bool Strobe(bool white, frc::Color allianceColor, int numLED,
            std::array<frc::AddressableLED::LEDData, 280>& ledBuffer);
void PaintSolid(std::array<frc::AddressableLED::LEDData, 280>& ledBuffer,
                       frc::Color allianceColor, int start, int end);

