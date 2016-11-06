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
			public ref class ConserveEnergy : public Finish
			{
			internal:
				virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
				{
					texture->Conserve_Energy = Enable;
					return FinishAdjustments::None;
				}

			public:
				ConserveEnergy(bool enable) :
					Enable(enable)
				{}

				bool Enable;
			};
		}
	}
}