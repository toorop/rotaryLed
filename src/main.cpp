/*
Libs:
Lib fast led: https://github.com/FastLED/FastLED
Rotary encoder: https://github.com/igorantolic/ai-esp32-rotary-encoder
*/

#include "AiEsp32RotaryEncoder.h"
#include <Arduino.h>
#include <FastLED.h>

// header
void setColor(uint32_t);
void SetColorByHue(uint8_t);
void RotaryLoop();
void RotaryBtnOnClick();

// rotary 12,14,27
#define ROTARY_CLK 12
#define ROTARY_DT 14
#define ROTARY_SW 27
#define ROTARY_VCC -1

// Led
#define NUM_LEDS 60
#define BRIGHTNESS 15
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LED_PIN 15

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// do not edit after
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
  ACTION_BRIGHTNESS = 0,
  ACTION_HUE = 1
} action;

// encoder
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_CLK, ROTARY_DT, ROTARY_SW, ROTARY_VCC);

// Define the array of leds
CRGB leds[NUM_LEDS];

// debounce vars
const unsigned long debounceTime = 100;
unsigned long dbtime;

// action onClic
action btnAction = ACTION_BRIGHTNESS;

void setup()
{
  Serial.begin(115200);
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  SetColorByHue(0);

  // rotary
  dbtime = millis();
  rotaryEncoder.begin();
  rotaryEncoder.setup([] { rotaryEncoder.readEncoder_ISR(); });
  rotaryEncoder.setBoundaries(0, 100, true); // on set de 0 a 100 ensuite en fonction de l'action on ferra la conversion
}

void loop()
{
  RotaryLoop();
}

// set color bye hue (0 -> 255)
void SetColorByHue(uint8_t hue)
{
  fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));
  FastLED.show();
}

void RotaryBtnOnClick()
{
  Serial.println("clic");
  if (btnAction == ACTION_BRIGHTNESS)
    btnAction = ACTION_HUE;
  else
    btnAction = ACTION_BRIGHTNESS;
  Serial.println(btnAction);
}

void RotaryLoop()
{
  // clic
  if (rotaryEncoder.currentButtonState() == BUT_RELEASED)
  {
    if (millis() - dbtime > debounceTime)
    {
      RotaryBtnOnClick();
    }
    dbtime = millis();
  }

  // rotate ?
  int16_t encoderDelta = rotaryEncoder.encoderChanged();
  if (encoderDelta == 0)
    return;

  int16_t rotaryValue = rotaryEncoder.readEncoder();
  Serial.println(rotaryValue);
  if (btnAction == ACTION_BRIGHTNESS)
  {
    if (rotaryValue < ) rotaryValue = 0;
    FastLED.setBrightness(rotaryValue);
    FastLED.show();

  }
  else
  {
    uint8_t hue = (int)(rotaryValue * 2.55);
    Serial.println(hue);
    SetColorByHue(hue);
  }
}
