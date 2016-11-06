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
			public ref class Spiral1 : public Pattern, public ITargetType<Pigment^>, public ITargetType<Normal^>, public ITargetType<Texture^>
			{
			internal:
				template <typename MapType, typename PatternType>
				void Render(Context^ context, PatternType* container)
				{
					container->Type = pov::GENERIC_PATTERN;
					auto pat = new pov::Spiral1Pattern();
					container->pattern = pov::PatternPtr(pat);
					pat->arms = NumberOfArms;
					pat->waveType = pov::kWaveType_Triangle;
				}

				virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) override
				{
					Render<pov::GenericPigmentBlendMap, pov::PIGMENT>(context, pigment);
				}

				virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) override
				{
					Render<pov::GenericNormalBlendMap, pov::TNORMAL>(context, normal);
					normal->Amount = BumpSize;
				}

				virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) override
				{
					Render<pov::TextureBlendMap, pov::TEXTURE>(context, texture);
				}

			public:
				Spiral1(float numberOfArms) :
					NumberOfArms(numberOfArms),
					BumpSize(1)
				{}

				Spiral1(float numberOfArms, float bumpSize) :
					NumberOfArms(numberOfArms),
					BumpSize(bumpSize)
				{}

				float NumberOfArms;
				float BumpSize;
			};
		}
	}
}