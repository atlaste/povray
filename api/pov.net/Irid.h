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
			public ref class Irid : public Finish
			{
			internal:
				virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
				{
					if (Thickness != 0.0 && !float::IsNaN(Thickness))
					{
						texture->Irid_Film_Thickness = float(Thickness);
					}
					if (Turbulence != 0.0 && !float::IsNaN(Turbulence))
					{
						texture->Irid_Turb = float(Turbulence);
					}

					texture->Irid = float(Amount);

					return FinishAdjustments::None;
				}

			public:
				Irid(float amount, float thickness, float turbulence) :
					Amount(amount),
					Thickness(thickness),
					Turbulence(turbulence)
				{}

				float Amount;
				float Thickness;
				float Turbulence;
			};
		}
	}
}