#include "LEDFunction.h"

const int kSTART_BL = 13;
const int kSTART_TL = 39;
const int kSTART_TR = 74;
const int kSTART_BR = 100;

void CanSee(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 125> &ledBuffer){
    ledBuffer[LEDIndex].SetRGB(0, 255, 0);
    ledBuffer[(numLED + LEDIndex - tailLength) % numLED].SetLED(allianceColor);
    ledBuffer[numLED - LEDIndex].SetRGB(0, 255, 0);
    ledBuffer[(numLED + numLED - LEDIndex - tailLength) % numLED].SetLED(allianceColor);
}
void EndGame(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 125> &ledBuffer){
  for(int i = 0; i < 7; i++){
    ledBuffer[(LEDIndex + (i * 10)) % numLED].SetRGB(255, 255, 255);
    ledBuffer[(numLED + LEDIndex - tailLength + (i * 10)) % numLED].SetLED(allianceColor);
  } 
}
void LowBattery(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 125> &ledBuffer){
  ledBuffer[LEDIndex].SetRGB(255, 215, 0);
  ledBuffer[(numLED + LEDIndex - tailLength) % numLED].SetLED(allianceColor);
}
void SetCorners(int tailLength, std::array<frc::AddressableLED::LEDData, 125> &ledBuffer, int R, int B, int G){
  for(int i = kSTART_BL; i < kSTART_BL + 7; i++)
    ledBuffer[i].SetRGB(R, G, B);
  for(int i = kSTART_BR; i < kSTART_BR + 8; i++)
    ledBuffer[i].SetRGB(R, G, B);
  for(int i = kSTART_TL; i < kSTART_TL + 7; i++)
    ledBuffer[i].SetRGB(R, G, B);
  for(int i = kSTART_TR; i < kSTART_TR + 7; i++)
    ledBuffer[i].SetRGB(R, G, B);
}


