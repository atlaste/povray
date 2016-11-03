#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
{
	public ref class Metallic : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Metallic = float(Amount);
			return FinishAdjustments::None;
		}

	public:
		Metallic(double amount) :
			Amount(amount)
		{}

		double Amount;
	};
}