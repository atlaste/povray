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
			public ref class SubSurface : public Finish
			{
			internal:
				virtual FinishAdjustments Render(pov::Finish_Struct* texture) override
				{
					Translucency.RenderDetail(texture->SubsurfaceTranslucency);
					Anisotropy.RenderDetail(texture->SubsurfaceAnisotropy);
					texture->UseSubsurface = true;

					return FinishAdjustments::None;
				}

			public:
				SubSurface(RGB translucency, RGB anisotropy) :
					Translucency(translucency),
					Anisotropy(anisotropy)
				{}

				RGB Translucency;
				RGB Anisotropy;
			};
		}
	}
}