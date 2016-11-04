#pragma once

#include "Context.h"
#include "Pattern.h"
#include "Color.h"
#include "IMapType.h"
#include "Pigment.h"
#include "Normal.h"
#include "Texture.h"
#include "BlendMapBase.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"
#include "core/material/pattern.h"
#include "core/material/normal.h"

namespace povclr
{
	public ref class TriangularBase abstract : public Pattern
	{
	internal:
		template <typename MapType, typename PatternType>
		void Render(Context^ context, PatternType* container)
		{
			container->Type = pov::GENERIC_INTEGER_PATTERN;
			auto pat = new pov::TriangularPattern();
			container->pattern = pov::PatternPtr(pat);
		}
	};

	generic <typename BlendType>
	where BlendType : IMapType
	public ref class Triangular : public TriangularBase, public ITargetType<BlendType>
	{
	internal:
		virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) override
		{
			Render<pov::GenericPigmentBlendMap, pov::PIGMENT>(context, pigment);
			pigment->Blend_Map = BlendMapBase::CreateBlendMap<pov::PigmentBlendMap>(context, pov::kBlendMapType_Pigment, Blend1, Blend2, Blend3, Blend4, Blend5, Blend6);
		}

		virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) override
		{
			Render<pov::GenericNormalBlendMap, pov::TNORMAL>(context, normal);
			normal->Delta = 0.02;
			normal->Blend_Map = BlendMapBase::CreateBlendMap<pov::NormalBlendMap>(context, pov::kBlendMapType_Normal, Blend1, Blend2, Blend3, Blend4, Blend5, Blend6);
		}

		virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) override
		{
			Render<pov::TextureBlendMap, pov::TEXTURE>(context, texture);
			texture->Blend_Map = BlendMapBase::CreateBlendMap<pov::TextureBlendMap>(context, pov::kBlendMapType_Texture, Blend1, Blend2, Blend3, Blend4, Blend5, Blend6);
		}

	public:
		Triangular(BlendType blend1, BlendType blend2, BlendType blend3, BlendType blend4, BlendType blend5, BlendType blend6) :
			Blend1(blend1),
			Blend2(blend2),
			Blend3(blend3),
			Blend4(blend4),
			Blend5(blend5),
			Blend6(blend6)
		{}

		BlendType Blend1;
		BlendType Blend2;
		BlendType Blend3;
		BlendType Blend4;
		BlendType Blend5;
		BlendType Blend6;
	};
}