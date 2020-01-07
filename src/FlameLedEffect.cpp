/*
* FlameLedEffect.cpp
*
*/


#include "FlameLedEffect.h"

FlameLedEffect::FlameLedEffect(CRGB leds[], uint16_t count, bool reversed)
	: ILedEffect(leds, count), fireReversed(reversed)
{
	heatMap = new uint8_t[count];
	gPal = HeatColors_p;
}

FlameLedEffect::~FlameLedEffect()
{
	delete[] heatMap;
}

void FlameLedEffect::refresh()
{
	// SPARKING: What chance (out of 255) is there that a new spark will be lit?
	// Higher chance = more roaring fire.  Lower chance = more flickery fire.
	// Default 120, suggested range 50-200.
	const uint8_t SPARKING = 120;

	random16_add_entropy(random16());

	// Step 1.  Cool down every cell a little
	for (uint16_t i = 0; i < numLeds; i++)
	{
		heatMap[i] = qsub8(heatMap[i], random8(0, 1024/numLeds + 2));
	}

	// Step 2.  heatMap from each cell drifts 'up' and diffuses a little
	for (uint16_t k = numLeds - 1; k > 2; k--)
	{
		heatMap[k] = (heatMap[k - 1] + heatMap[k - 2] + heatMap[k - 3]) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heatMap near the bottom
	if (random8() < SPARKING)
	{
		uint8_t y = random8(8);
		heatMap[y] = qadd8(heatMap[y], random8(160, 255));
	}

	// Step 4.  Map from heatMap cells to LED colors
	for (uint16_t j = 0; j < numLeds; j++)
	{
		// Scale the heatMap value from 0-255 down to 0-240
		// for best results with color palettes.
		uint8_t colorindex = scale8(heatMap[j], 240);
		CRGB color = ColorFromPalette(gPal, colorindex);
		uint16_t pixelnumber = fireReversed ? (numLeds - 1) - j : j;
		ledStrip[pixelnumber] = color;
	}

}