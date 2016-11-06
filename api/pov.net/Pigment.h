#pragma once

#include "Context.h"
#include "Pigment.h"
#include "Color.h"
#include "Pattern.h"
#include "IMapType.h"
#include "PlainPattern.h"

#include "core/material/pigment.h"
#include "core/material/pattern.h"

namespace povray
{
	namespace Materials
	{
		public ref class Pigment : public IMapType
		{
		internal:
			void Render(Context^ context, pov::PIGMENT* pigment)
			{
				Pattern->RenderPigmentBlendMap(context, pigment);
			}

		public:
			Pigment(RGBFT color) :
				Pattern(gcnew Patterns::PlainPattern(color))
			{}

			Pigment(ITargetType<Pigment^> ^pattern) :
				Pattern((Materials::Pattern^) pattern)
			{}

			Pattern^ Pattern;
		};
	}
}