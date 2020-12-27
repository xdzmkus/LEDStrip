#if defined(ESP32) || defined(ESP8266)
#define LED_PIN D3  // D1 leds pin (connected to D5 on my NodeMCU1.0 !!!)
#define BTN_PIN D6  // D6 button pin
#else
#define LED_PIN 9   // leds pin
#define BTN_PIN 10  // button pin
#endif

#include <Denel_Button.h>

Denel_Button btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

#include <EEPROM.h>
#define EEPROM_ADDRESS_EFFECT 0
#define EEPROM_EFFECT_LENGTH 15

char EFFECT_NAME[EEPROM_EFFECT_LENGTH + 1];

#include <FastLED.h>
#define NUM_LEDS 256
#define CURRENT_LIMIT 8000
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 20

uint16_t brightness = MAX_BRIGHTNESS/2;

CRGB leds[NUM_LEDS];

#include "LEDLine.h"

LEDLine ledLine(leds, NUM_LEDS);

void handleButtonEvent(const Denel_Button* button, BUTTON_EVENT eventType)
{
	switch (eventType)
	{
	case BUTTON_EVENT::Clicked:
		ledLine.setNextEffect();
		Serial.print(F("EFFECT: ")); Serial.println(ledLine.getEffectName());
		break;
	case BUTTON_EVENT::DoubleClicked:
		save();
		ledLine.pause();
		break;
	case BUTTON_EVENT::RepeatClicked:
		brightness += MIN_BRIGHTNESS;
		if (brightness > MAX_BRIGHTNESS + MIN_BRIGHTNESS*2) brightness = 0;
		FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
		Serial.print(F("BRIGHTNESS: ")); Serial.println(brightness);
		break;
	case BUTTON_EVENT::LongPressed:
		ledLine.pause();
		FastLED.clear(true);
		Serial.println(F("OFF"));
		break;
	default:
		break;
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

	btn.setEventHandler(handleButtonEvent);

	load();
}

void loop()
{
	btn.check();

	if (ledLine.paint())
	{
		FastLED.show();
	}
}

void save()
{
	strncpy(EFFECT_NAME, (ledLine.getEffectName() == nullptr || !ledLine.isRunning()) ? "OFF" : ledLine.getEffectName(), EEPROM_EFFECT_LENGTH);

#if defined(ESP32) || defined(ESP8266)
	EEPROM.begin(EEPROM_EFFECT_LENGTH + 1);
#endif

	for (uint8_t i = 0; i < EEPROM_EFFECT_LENGTH + 1; i++)
	{
		EEPROM.write(EEPROM_ADDRESS_EFFECT + i, EFFECT_NAME[i]);
	}
#if defined(ESP32) || defined(ESP8266)
	EEPROM.commit();
#endif

	Serial.print(F("SAVED: ")); Serial.println(EFFECT_NAME);
}

void load()
{
#if defined(ESP32) || defined(ESP8266)
	EEPROM.begin(EEPROM_EFFECT_LENGTH);
#endif

	for (int i = 0; i < EEPROM_EFFECT_LENGTH; i++)
	{
		EFFECT_NAME[i] = EEPROM.read(EEPROM_ADDRESS_EFFECT + i);
	}

	EFFECT_NAME[EEPROM_EFFECT_LENGTH] = '\0';

	ledLine.setEffectByName(EFFECT_NAME);

	Serial.print(F("LOADED: ")); Serial.println(EFFECT_NAME);
}