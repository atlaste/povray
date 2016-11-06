#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povray
{
	namespace Materials
	{
		namespace Finishes
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
				Crand(float amount) :
					Amount(amount)
				{}

				float Amount;
			};
		}
	}
}