#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
{
	public ref class Reflection : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			MinColor->RenderDetail(texture->Reflection_Min);
			MaxColor->RenderDetail(texture->Reflection_Max);
			texture->Reflection_Falloff = float(Falloff);
			texture->Reflection_Fresnel = Fresnel;
			texture->Reflect_Exp = float(Exponent);
			texture->Reflect_Metallic = float(Metallic);

			return FinishAdjustments::None;
		}

	public:
		Reflection(RGB^ color) :
			MinColor(color),
			MaxColor(color),
			Falloff(1),
			Fresnel(false),
			Exponent(1.0),
			Metallic(0.0)
		{}

		Reflection(RGB^ minColor, RGB maxColor, float falloff) :
			MinColor(minColor),
			MaxColor(maxColor),
			Falloff(falloff),
			Fresnel(false),
			Exponent(1.0),
			Metallic(0.0)
		{}

		RGB^ MinColor;
		RGB^ MaxColor;
		float Falloff;
		bool Fresnel;
		float Exponent;
		float Metallic;
	};
}