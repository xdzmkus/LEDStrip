/* 
* SparklesLedEffect.h
*
*/

#ifndef __SPARKLESLEDEFFECT_H__
#define __SPARKLESLEDEFFECT_H__

#include "ILedEffect.h"

class SparklesLedEffect : public ILedEffect
{
public:

	static const char* const name;

protected:

	const uint8_t SPARKLES_TRACK_STEP = 20;

public:
	SparklesLedEffect(CRGB leds[], uint16_t count, uint16_t Hz);
	~SparklesLedEffect();

	void reset() override;
	bool paint() override;

	operator const char* () const { return name; }

private:
	SparklesLedEffect(const SparklesLedEffect&) = delete;
	SparklesLedEffect& operator=(const SparklesLedEffect&) = delete;
};

#endif //__SPARKLESLEDEFFECT_H__
