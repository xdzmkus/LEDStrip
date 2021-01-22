/*
* LEDLine.cpp
*
*/

#include "LEDLine.h"

#include "BouncingBallsLedEffect.h"
#include "BugsLedEffect.h"
#include "ColorsLedEffect.h"
#include "FlameLedEffect.h"
#include "FlashLedEffect.h"
#include "GlowwormLedEffect.h"
#include "RainbowLedEffect.h"
#include "SparklesLedEffect.h"
#include "ThreeColorLedEffect.h"

const uint8_t LEDLine::NUM_EFFECTS = 9;

const char* LEDLine::availableEffects[NUM_EFFECTS] =
{
	BouncingBallsLedEffect::name,
	BugsLedEffect::name,
	ColorsLedEffect::name,
	FlameLedEffect::name,
	FlashLedEffect::name,
	GlowwormLedEffect::name,
	RainbowLedEffect::name,
	SparklesLedEffect::name,
	ThreeColorLedEffect::name
};

LEDLine::LEDLine(CRGB leds[], uint16_t count, bool start)
	: leds(leds), numLeds(count), isOn(start)
{
}

LEDLine::~LEDLine()
{
	delete effect;
}

uint8_t LEDLine::getAllEffectsNumber() const
{
	return NUM_EFFECTS;
}

const char** LEDLine::getAllEffectsNames() const
{
	return availableEffects;
}

bool LEDLine::setEffectByName(const char* effectName)
{
	if (strcmp(BouncingBallsLedEffect::name, effectName) == 0) {
		delete effect; effect = new BouncingBallsLedEffect(leds, numLeds, 50);
	}
	else if (strcmp(BugsLedEffect::name, effectName) == 0) {
		delete effect; effect = new BugsLedEffect(leds, numLeds, 20);
	}
	else if (strcmp(ColorsLedEffect::name, effectName) == 0) {
		delete effect; effect = new ColorsLedEffect(leds, numLeds, 10);
	}
	else if (strcmp(FlameLedEffect::name, effectName) == 0) {
		delete effect; effect = new FlameLedEffect(leds, numLeds, 10);
	}
	else if (strcmp(FlashLedEffect::name, effectName) == 0) {
		delete effect; effect = new FlashLedEffect(leds, numLeds, 1);
	}
	else if (strcmp(GlowwormLedEffect::name, effectName) == 0) {
		delete effect; effect = new GlowwormLedEffect(leds, numLeds, 30);
	}
	else if (strcmp(RainbowLedEffect::name, effectName) == 0) {
		delete effect; effect = new RainbowLedEffect(leds, numLeds, 10);
	}
	else if (strcmp(SparklesLedEffect::name, effectName) == 0) {
		delete effect; effect = new SparklesLedEffect(leds, numLeds, 20);
	}
	else if (strcmp(ThreeColorLedEffect::name, effectName) == 0) {
		delete effect; effect = new ThreeColorLedEffect(leds, numLeds, 30, { CRGB::White, 1, CRGB::Red, 1, CRGB::White, 1 });
	}
	else {
		return false;
	}

	return true;
}

bool LEDLine::setEffectByIdx(uint8_t idx)
{
	if (idx >= getAllEffectsNumber())
	{
		return false;
	}

	return setEffectByName(getAllEffectsNames()[idx]);
}

bool LEDLine::setNextEffect()
{
	uint8_t nextEffectIdx = getEffectIdx() + 1;
	
	if (nextEffectIdx >= getAllEffectsNumber())
		nextEffectIdx = 0;

	return setEffectByIdx(nextEffectIdx);
}

const char* LEDLine::getEffectName() const
{
	return (effect != nullptr) ? static_cast<const char*>(*effect) : nullptr;
}

uint8_t LEDLine::getEffectIdx() const
{
	if (effect != nullptr)
	{
		for (uint8_t idx = 0; idx < getAllEffectsNumber(); idx++)
		{
			if (strcmp(getAllEffectsNames()[idx], static_cast<const char*>(*effect)) == 0)
			{
				return idx;
			}
		}
	}

	return getAllEffectsNumber();		// non-existing effect index
}

void LEDLine::pause()
{
	isOn = false;
	if (effect != nullptr) effect->stop();
}

void LEDLine::resume()
{
	isOn = true;
	if (effect != nullptr) effect->start();
}

bool LEDLine::refresh() const
{
	return (!isOn || effect == nullptr) ? false : effect->paint();
}

bool LEDLine::isRunning() const
{
	return isOn;
}