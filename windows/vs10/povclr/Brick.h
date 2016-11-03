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
	public ref class BrickBase abstract : public Pattern
	{
	internal:
		template <typename MapType, typename PatternType>
		void Render(Context^ context, PatternType* container)
		{
			if (container->Type != pov::BRICK_PATTERN)
			{
				container->Type = pov::BRICK_PATTERN;
				auto pat = new pov::BrickPattern();
				pat->brickSize = pov::Vector3d(8.0, 3.0, 4.5);
				pat->mortar = 0.5 - EPSILON*2.0;
				container->pattern = pov::PatternPtr(pat);
			}

			// container->Blend_Map = Parse_Blend_List<MapType>(2, container->pattern->GetDefaultBlendMap());
		}
	};

	generic <typename BlendType>
	where BlendType : IMapType
	public ref class Brick : public BrickBase, public ITargetType<BlendType>
	{
	internal:
		virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) override
		{
			Render<pov::GenericPigmentBlendMap, pov::PIGMENT>(context, pigment);
			pigment->Blend_Map = BlendMapBase::CreateBlendMap<pov::PigmentBlendMap>(context, pov::kBlendMapType_Pigment, Blend1, Blend2);
		}

		virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) override
		{
			Render<pov::GenericNormalBlendMap, pov::TNORMAL>(context, normal);
			normal->Delta = 0.02;
			normal->Blend_Map = BlendMapBase::CreateBlendMap<pov::NormalBlendMap>(context, pov::kBlendMapType_Normal, Blend1, Blend2);
		}

		virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) override
		{
			Render<pov::TextureBlendMap, pov::TEXTURE>(context, texture);
			texture->Blend_Map = BlendMapBase::CreateBlendMap<pov::TextureBlendMap>(context, pov::kBlendMapType_Texture, Blend1, Blend2);
		}

	public:
		Brick(BlendType blend1, BlendType blend2) :
			Blend1(blend1), Blend2(blend2)
		{}

		BlendType Blend1;
		BlendType Blend2;
	};
}