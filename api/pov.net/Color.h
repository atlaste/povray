#pragma once

#include "IMapType.h"

namespace povclr
{
	public value class RGB
	{
	internal:
		void RenderDetail(pov::MathColour& dst)
		{
			pov::RGBFTColour tmp;
			tmp.red() = Red;
			tmp.green() = Green;
			tmp.blue() = Blue;
			pov::TransColour tmp2 = pov::ToTransColour(tmp);
			dst = tmp2.colour();
		}
	public:
		RGB(float r, float g, float b) :
			Red(r),
			Green(g),
			Blue(b)
		{}

		float Red;
		float Green;
		float Blue;
	};

	public value class RGBFT : public IMapType
	{
	internal:
		void RenderDetail(pov::TransColour& dst)
		{
			pov::RGBFTColour tmp;
			tmp.red() = Red;
			tmp.green() = Green;
			tmp.blue() = Blue;
			tmp.filter() = Filter;
			tmp.transm() = Transparency;
			dst = ToTransColour(tmp);
		}

	public:
		RGBFT(float r, float g, float b, float f, float t) :
			Red(r),
			Green(g),
			Blue(b),
			Filter(f),
			Transparency(t)
		{}

		RGBFT(float r, float g, float b) :
			Red(r),
			Green(g),
			Blue(b),
			Filter(0),
			Transparency(0)
		{}

		float Red;
		float Green;
		float Blue;
		float Filter;
		float Transparency;
	};
}