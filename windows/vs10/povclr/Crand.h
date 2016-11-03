#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
{
	public ref class Crand : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Crand = float(Amount);
			return FinishAdjustments::None;
		}

	public:
		Crand(double amount) :
			Amount(amount)
		{}

		double Amount;
	};
}