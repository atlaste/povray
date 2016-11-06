#pragma once

#include "Context.h"
#include "Pattern.h"
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
			public ref class Facets : public Pattern, public ITargetType<Normal^>
			{
			internal:
				template <typename MapType, typename PatternType>
				void Render(Context^ context, PatternType* container)
				{
					container->Type = pov::FACETS_PATTERN;
					auto pat = new pov::FacetsPattern();
					pat->facetsSize = this->SizeFactor;
					pat->facetsCoords = this->Coordinates;
					pat->facetsMetric = this->Metric;
					container->pattern = pov::PatternPtr(pat);
				}

				virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) override
				{
					Render<pov::GenericNormalBlendMap, pov::TNORMAL>(context, normal);
					normal->Blend_Map = BlendMapBase::CreateBlendMap<pov::NormalBlendMap>(context, pov::kBlendMapType_Normal, NormalMap);
				}

				virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) override
				{
					throw gcnew NotSupportedException("Facets pattern does not support pigments");
				}

				virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) override
				{
					throw gcnew NotSupportedException("Facets pattern does not support textures");
				}


			public:
				Facets(... array<Normal^>^ normalMap) :
					NormalMap(normalMap),
					Coordinates(0.0),
					SizeFactor(0.1),
					Metric(2)
				{}

				Facets(float scaleValue, float sizeFactor, ... array<Normal^>^ normalMap) :
					NormalMap(normalMap),
					Coordinates(scaleValue),
					SizeFactor(sizeFactor),
					Metric(2)
				{}

				Facets(float scaleValue, float sizeFactor, float metric, ... array<Normal^>^ normalMap) :
					NormalMap(normalMap),
					Coordinates(scaleValue),
					SizeFactor(sizeFactor),
					Metric(metric)
				{}

				array<Normal^>^ NormalMap;
				float Coordinates;
				float SizeFactor;
				float Metric;
			};
		}
	}
}