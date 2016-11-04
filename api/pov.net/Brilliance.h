#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povray
{
	public ref class Brilliance : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Brilliance = Amount;
			texture->BrillianceOut = Out;
			return FinishAdjustments::None;
		}

	public:
		Brilliance(float amount) :
			Amount(amount),
			Out(1.0)
		{}

		Brilliance(float amount, float out) :
			Amount(amount),
			Out(out)
		{}

		float Amount;
		float Out;
	};
}