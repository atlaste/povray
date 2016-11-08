#pragma once

#include "IMapType.h"

namespace povray
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
		RGB(double v) :
			Red((float)v),
			Green((float)v),
			Blue((float)v)
		{}

		RGB(double r, double g, double b) :
			Red((float)r),
			Green((float)g),
			Blue((float)b)
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
		RGBFT(double r, double g, double b, double f, double t) :
			Red((float)r),
			Green((float)g),
			Blue((float)b),
			Filter((float)f),
			Transparency((float)t)
		{}

		RGBFT(double r, double g, double b) :
			Red((float)r),
			Green((float)g),
			Blue((float)b),
			Filter(0),
			Transparency(0)
		{}

		RGBFT(double r, double g, double b, double f) :
			Red((float)r),
			Green((float)g),
			Blue((float)b),
			Filter(f),
			Transparency(0)
		{}

		RGBFT(double v) :
			Red((float)v),
			Green((float)v),
			Blue((float)v),
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