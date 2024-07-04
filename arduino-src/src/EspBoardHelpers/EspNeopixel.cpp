//#include <Arduino.h>
//#include "EspNeopixel.h"
//#include "FastLED.h"
//
//void EspNeopixel::BlinkLedRedThenOff()
//{
//    SetBuiltInLedRGB(255, 0, 0);
//
//    delay(500);
//
//    SetBuiltInLedRGB(0, 0, 0);
//
//    delay(500);
//}
//
//void EspNeopixel::FlashLedStartSequenceOnBoardStart()
//{
//    if (millis() > 10000) return;
//
//    if (!_firedOnce)
//    {
//        CFastLED::addLeds<SK6812, 48, GRB>(builtin_neopixel, 1).setCorrection(TypicalLEDStrip);
//
//        FastLED.setBrightness(10);
//
//        _firedOnce = true;
//    }
//
//    if (millis() > 1 && millis() < 333)
//        SetBuiltInLedRGB(0, 0, 255);
//
//    if (millis() > 333 && millis() < 666)
//        SetBuiltInLedRGB(0, 255, 0);
//
//    if (millis() > 666 && millis() < 999)
//        SetBuiltInLedRGB(0, 0, 255);
//
//    if (millis() > 999 && millis() < 1333)
//        SetBuiltInLedRGB(0, 255, 0);
//
//    if (millis() > 1333 && millis() < 1666)
//        SetBuiltInLedRGB(0, 0, 255);
//
//    if (millis() > 1666 && millis() < 1999)
//        SetBuiltInLedRGB(0, 255, 0);
//
//    if (millis() > 1999 && millis() < 3000)
//        SetBuiltInLedRGB(0, 0, 255);
//
//    if (millis() > 3000 && millis() < 4000)
//        SetBuiltInLedRGB(0, 0, 0);
//}
//
//void EspNeopixel::SetBuiltInLedRGB(unsigned char red, unsigned char green, unsigned char blue)
//{
//    SetBuiltInLedARGB(10, red, green, blue);
//}
//
//void EspNeopixel::SetBuiltInLedARGB(unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue)
//{
//    FastLED.setBrightness(alpha);
//
//    builtin_neopixel[0] = CRGB(red, green, blue);
//    FastLED.show();
//}
//
//
