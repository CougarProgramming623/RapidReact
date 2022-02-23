#include "LEDFunction.h"

void CanSee(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 140> &ledBuffer){
    ledBuffer[LEDIndex].SetRGB(0, 255, 0);
    ledBuffer[(numLED + LEDIndex - tailLength) % numLED].SetLED(allianceColor);
    ledBuffer[numLED - LEDIndex].SetRGB(0, 255, 0);
    ledBuffer[(numLED + numLED - LEDIndex - tailLength) % numLED].SetLED(allianceColor);
}
 void EndGame(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 140> &ledBuffer){
    for(int i = 0; i < 7; i++){
      ledBuffer[(LEDIndex + (i * 10)) % numLED].SetRGB(255, 255, 255);
      ledBuffer[(numLED + LEDIndex - tailLength + (i * 10)) % numLED].SetLED(allianceColor);
    } 
}
 void LowBattery(frc::Color allianceColor, int numLED, int tailLength, int LEDIndex, std::array<frc::AddressableLED::LEDData, 140> &ledBuffer){
    ledBuffer[LEDIndex].SetRGB(255, 215, 0);
    ledBuffer[(numLED + LEDIndex - tailLength) % numLED].SetLED(allianceColor);
}