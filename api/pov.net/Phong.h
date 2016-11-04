#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povray
{
	public ref class Phong : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Phong = float(this->Amount);
			if (!float::IsNaN(Size))
			{
				texture->Phong_Size = float(this->Size);
			}

			return Albedo ? FinishAdjustments::PhongAdjust : FinishAdjustments::None;
		}

	public:
		Phong(bool albedo, float amount, float size) :
			Albedo(albedo),
			Amount(amount),
			Size(size)
		{}

		Phong(float amount) :
			Albedo(false),
			Amount(amount),
			Size(float::NaN)
		{}

		bool Albedo;
		float Amount;
		float Size;
	};
}