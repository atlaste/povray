#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povray
{
	public ref class Roughness : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			texture->Roughness = float(Amount == 0.0 ? 0.0 : (1.0 / Amount));

			return FinishAdjustments::None;
		}

	public:
		Roughness(float amount) :
			Amount(amount)
		{}

		float Amount;
	};
}