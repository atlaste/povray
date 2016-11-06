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

namespace povray
{
	namespace Materials
	{
		namespace Patterns
		{
			public ref class CheckerBase abstract : public Pattern
			{
			internal:
				template <typename MapType, typename PatternType>
				void Render(Context^ context, PatternType* container)
				{
					container->Type = pov::GENERIC_INTEGER_PATTERN;
					container->pattern = pov::PatternPtr(new pov::CheckerPattern());
				}
			};

			generic <typename BlendType>
				where BlendType : IMapType
				public ref class Checker : public CheckerBase, public ITargetType<BlendType>
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
					Checker(BlendType blend1, BlendType blend2) :
						Blend1(blend1), Blend2(blend2)
					{}

					BlendType Blend1;
					BlendType Blend2;
				};
		}
	}
}