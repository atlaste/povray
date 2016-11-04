#pragma once

#include "Context.h"
#include "Pattern.h"
#include "ColorMap.h"
#include "IMapType.h"
#include "Pigment.h"
#include "Normal.h"
#include "Texture.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"
#include "core/material/pattern.h"
#include "core/material/normal.h"

namespace povray
{
	public ref class Agate : public Pattern, public ITargetType<Pigment^>, public ITargetType<Normal^>, public ITargetType<Texture^>
	{
	internal:
		template <typename MapType, typename PatternType>
		void Render(Context^ context, PatternType* container)
		{
			container->Type = pov::AGATE_PATTERN;
			auto pat = new pov::AgatePattern();
			pat->agateTurbScale = Turbulence;
			container->pattern = pov::PatternPtr(pat);
			CheckTurbulence(container->pattern->warps, container->pattern->HasSpecialTurbulenceHandling());

			container->Blend_Map = ColorMap->Render<MapType>(context);
		}

		IMPLEMENT_DEFAULT_PATTERN_CODE

	public:
		Agate() : 
			Turbulence(1.0)
		{}

		Agate(float turbulence, ColorMap^ colorMap) :
			Turbulence(turbulence),
			ColorMap(colorMap)
		{}

		float Turbulence;

		ColorMap^ ColorMap;
	};
}