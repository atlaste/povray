#pragma once

#include "Finish.h"
#include "Color.h"

#include "core/material/texture.h"

namespace povclr
{
	public ref class Emission : public Finish
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
		{
			Color->RenderDetail(texture->Emission);
			return FinishAdjustments::EmissionSet;
		}

	public:
		Emission(RGB^ color) :
			Color(color)
		{}

		RGB^ Color;
	};
}