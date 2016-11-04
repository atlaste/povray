#pragma once

#include "Context.h"
#include "Pattern.h"
#include "Texture.h"
#include "ColorMap.h"
#include "BlendMapBase.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"
#include "core/material/pattern.h"
#include "core/material/normal.h"

namespace povray
{
	public ref class Cubic : public Pattern, public ITargetType<Texture^>
	{
	internal:
		virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) override
		{
			throw gcnew NotSupportedException();
		}
		virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) override
		{
			throw gcnew NotSupportedException();
		}
		virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) override
		{
			texture->Type = pov::GENERIC_INTEGER_PATTERN;
			texture->pattern = pov::PatternPtr(new pov::CubicPattern());
			texture->Blend_Map = BlendMapBase::CreateBlendMap<pov::TextureBlendMap>(context, pov::kBlendMapType_Texture, Items);
		}

	public:
		Cubic(array<Texture^>^ items) :
			Items(items)
		{
			if (Items->Length != 6)
			{
				throw gcnew ArgumentException("Number of faces of a cubic must be 6.");
			}
		}

		array<Texture^>^ Items;
	};
}