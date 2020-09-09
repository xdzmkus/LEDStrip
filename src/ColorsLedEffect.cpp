/*
* ColorsLedEffect.cpp
*
*/

#include "ColorsLedEffect.h"

ColorsLedEffect::ColorsLedEffect(CRGB leds[], uint16_t count, uint16_t Hz)
	: ILedEffect(leds, count, Hz), hue(0)
{
}

ColorsLedEffect::~ColorsLedEffect()
{
}

void ColorsLedEffect::reset()
{
	ILedEffect::reset();
	hue = 0;
}

bool ColorsLedEffect::paint()
{
	if (!ILedEffect::paint())
		return false;

	hue += 2;

	for (uint16_t i = 0; i < numLeds; i++)
	{
		ledLine[i] = CHSV(hue, 255, 255);
	}

	return true;
}
