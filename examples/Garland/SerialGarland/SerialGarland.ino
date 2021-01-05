#define LED_PIN 9	// leds pin

#include <FastLED.h>
#define NUM_LEDS 8
#define CURRENT_LIMIT 500
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 20

uint16_t brightness = MAX_BRIGHTNESS/2;

CRGB leds[NUM_LEDS];

#include "LEDLine.h"

LEDLine ledLine(leds, NUM_LEDS);

#define NAME_EFFECT_LENGTH 15

char EFFECT_NAME[NAME_EFFECT_LENGTH + 1];

bool isNewEffect = false;

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent()
{
	static uint8_t receivedBytes = 0;

	while (Serial.available())
	{
		// get the new byte:
		char recvChar = (char)Serial.read();

		// if the incoming character is a newline, set a flag so the main loop can do something about it:
		if (recvChar == '\r' || recvChar == '\n')
		{
			if (receivedBytes == 0) continue; // ignore empty lines

			EFFECT_NAME[receivedBytes] = '\0';
			receivedBytes = 0;
			isNewEffect = true;
		}
		else
		{
			EFFECT_NAME[receivedBytes++] = recvChar;

			if (receivedBytes == NAME_EFFECT_LENGTH)
				receivedBytes = 0;
		}
	}
}

void setupLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
	FastLED.clear(true);
}

void setup()
{
	Serial.begin(115200);

	Serial.println(F("LEDLine EFFECTS:"));
	for (auto var : ledLine.availableEffects)
		Serial.println(var);

	setupLED();

	ledLine.setNextEffect();
}

void loop()
{
	if (isNewEffect)
	{
		Serial.print(F("NEW_EFFECT:"));	Serial.println(EFFECT_NAME);

		isNewEffect = false;
		if (ledLine.setEffectByName(EFFECT_NAME))
		{
			ledLine.resume();
		}
		else
		{
			Serial.println(F("BAD_EFFECT"));
		}
		Serial.flush();
	}

	if (ledLine.paint())
	{
		FastLED.show();
	}
}