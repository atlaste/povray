#pragma once

#include "Context.h"
#include "Pattern.h"
#include "ColorMap.h"
#include "Pigment.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"
#include "core/material/pattern.h"
#include "core/material/normal.h"

namespace povclr
{
	ref class Pigment;

	public ref class PlainPattern : public Pattern, public ITargetType<Pigment^>
	{
	internal:
		virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) override
		{
			pigment->Type = pov::PLAIN_PATTERN;
			pigment->pattern = pov::PatternPtr(new pov::PlainPattern());
			Color->RenderDetail(pigment->colour);
		}
		virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) override
		{
			throw gcnew NotSupportedException();
		}
		virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) override
		{
			throw gcnew NotSupportedException();
		}


	public:
		PlainPattern(RGBFT^ color) :
			Color(color)
		{}

		RGBFT^ Color;
	};
}