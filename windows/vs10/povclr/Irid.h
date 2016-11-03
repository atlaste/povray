#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
{
	public ref class Irid : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			if (Thickness != 0.0 && !double::IsNaN(Thickness))
			{
				texture->Irid_Film_Thickness = float(Thickness);
			}
			if (Turbulence != 0.0 && !double::IsNaN(Turbulence))
			{
				texture->Irid_Turb = float(Turbulence);
			}

			texture->Irid = float(Amount);

			return FinishAdjustments::None;
		}

	public:
		Irid(double amount, double thickness, double turbulence) :
			Amount(amount),
			Thickness(thickness),
			Turbulence(turbulence)
		{}

		double Amount;
		double Thickness;
		double Turbulence;
	};
}