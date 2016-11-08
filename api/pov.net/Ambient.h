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
			public ref class Ambient : public Finish
			{
			internal:
				virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
				{
					Color.RenderDetail(texture->Ambient);
					return FinishAdjustments::AmbientSet;
				}

			public:
				Ambient(RGB color) :
					Color(color)
				{}

				RGB Color;
			};
		}
	}
}