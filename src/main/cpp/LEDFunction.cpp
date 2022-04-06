#include "LEDFunction.h"

const int kSTART_BL = 13;
const int kSTART_TL = 39;
const int kSTART_TR = 74;
const int kSTART_BR = 100;
const int kSTRIP_1_START = 111;
const int kSTRIP_2_START = 192;

void CanSee(frc::Color allianceColor, int numLED,
            std::array<frc::AddressableLED::LEDData, 280>& ledBuffer) {
  if(allianceColor.red == 1){
    for (int i = kSTRIP_1_START; i < 280; i += 3)
      ledBuffer[i].SetRGB(0, 255, 0);
  } else {
    for (int i = kSTRIP_1_START; i < 280; i += 3)
      ledBuffer[i].SetRGB(255, 255, 0);
  }
}

void LowBattery(frc::Color allianceColor, int numLED, int tailLength,
                int LEDIndex,
                std::array<frc::AddressableLED::LEDData, 280>& ledBuffer) {
  ledBuffer[LEDIndex % 111].SetRGB(255, 215, 0);
  ledBuffer[(111 + LEDIndex - tailLength) % 111].SetLED(allianceColor);
}

bool Strobe(bool white, frc::Color allianceColor, int numLED,
            std::array<frc::AddressableLED::LEDData, 280>& ledBuffer) {
  if (white) {
    for (int i = 6; i <= numLED - 1; i += 6) {
      ledBuffer[i].SetRGB(0, 0, 0);
      ledBuffer[i + 1].SetRGB(0, 0, 0);
      ledBuffer[i + 2].SetRGB(0, 0, 0);
    }
    return false;
  } else {
    for (int i = 0; i <= numLED - 1; i += 6) {
      ledBuffer[i + 3].SetRGB(0, 0, 0);
      ledBuffer[i + 4].SetRGB(0, 0, 0);
      ledBuffer[i + 5].SetRGB(0, 0, 0);
    }
    return true;
  }
}

void SetCorners(int tailLength,
                std::array<frc::AddressableLED::LEDData, 280>& ledBuffer, int R,
                int B, int G) {
  for (int i = kSTART_BL; i < kSTART_BL + 7; i++)
    ledBuffer[i].SetRGB(R, G, B);
  for (int i = kSTART_BR; i < kSTART_BR + 8; i++)
    ledBuffer[i].SetRGB(R, G, B);
  for (int i = kSTART_TL; i < kSTART_TL + 7; i++)
    ledBuffer[i].SetRGB(R, G, B);
  for (int i = kSTART_TR; i < kSTART_TR + 7; i++)
    ledBuffer[i].SetRGB(R, G, B);
}

void PaintSolid(std::array<frc::AddressableLED::LEDData, 280>& ledBuffer, 
                       frc::Color allianceColor, int start, int end){
  for (int i = start; i < end; i++)
    ledBuffer[i].SetLED(allianceColor);
}