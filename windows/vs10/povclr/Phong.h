#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
{
	public ref class Phong : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Phong = float(this->Amount);
			if (!double::IsNaN(Size))
			{
				texture->Phong_Size = float(this->Size);
			}

			return Albedo ? FinishAdjustments::PhongAdjust : FinishAdjustments::None;
		}

	public:
		Phong(bool albedo, double amount, double size) :
			Albedo(albedo),
			Amount(amount),
			Size(size)
		{}

		Phong(double amount) :
			Albedo(false),
			Amount(amount),
			Size(double::NaN)
		{}

		bool Albedo;
		double Amount;
		double Size;
	};
}