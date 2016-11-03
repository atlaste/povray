#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
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
		Brilliance(double amount) :
			Amount(amount),
			Out(1.0)
		{}

		Brilliance(double amount, double out) :
			Amount(amount),
			Out(out)
		{}

		double Amount;
		double Out;
	};
}