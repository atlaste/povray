#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povray
{
	public ref class Specular : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Specular = float(Amount);

			return Albedo ? FinishAdjustments::SpecularAdjust : FinishAdjustments::None;
		}

	public:
		Specular(bool albedo, float amount) :
			Albedo(albedo),
			Amount(amount)
		{}

		Specular(float amount) :
			Albedo(false)
		{}

		bool Albedo;
		float Amount;
	};
}