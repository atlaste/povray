#pragma once

#include "Context.h"
#include "Pattern.h"
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
	namespace Materials
	{
		namespace Patterns
		{
			public ref class Radial : public Pattern, public ITargetType<Pigment^>, public ITargetType<Normal^>, public ITargetType<Texture^>
			{
			internal:
				template <typename MapType, typename PatternType>
				void Render(Context^ context, PatternType* container)
				{
					container->Type = pov::GENERIC_PATTERN;
					auto pat = new pov::RadialPattern();
					container->pattern = pov::PatternPtr(pat);
				}

				IMPLEMENT_DEFAULT_PATTERN_CODE

			public:
				Radial()
				{}
			};
		}
	}
}