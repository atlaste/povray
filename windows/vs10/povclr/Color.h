#pragma once

namespace povclr
{
	public ref class RGB
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
		RGB(double r, double g, double b) :
			Red(r),
			Green(g),
			Blue(b)
		{}

		double Red;
		double Green;
		double Blue;
	};

	public ref class RGBFT : public RGB
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
			RGB(r, g, b),
			Filter(f),
			Transparency(t)
		{}

		RGBFT(double r, double g, double b) :
			RGB(r, g, b),
			Filter(0),
			Transparency(0)
		{}

		double Filter;
		double Transparency;
	};
}