#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
{
	public ref class Diffuse : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Diffuse = float(Amount);
			texture->DiffuseBack = float(BacksideIllumination);

			return Albedo ? FinishAdjustments::DiffuseAdjust : FinishAdjustments::None;
		}

	public:
		Diffuse(bool albedo, float amount, float backsideIllumination) :
			Albedo(albedo),
			Amount(amount),
			BacksideIllumination(backsideIllumination)
		{}

		Diffuse(float amount) :
			Albedo(false),
			Amount(amount),
			BacksideIllumination(0.0)
		{}

		bool Albedo;
		float Amount;
		float BacksideIllumination;
	};
}