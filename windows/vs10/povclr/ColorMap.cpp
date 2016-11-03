#include "Stdafx.h"
#include "ColorMap.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"
#include "core/material/pattern.h"
#include "core/material/normal.h"

namespace povclr
{
	template<>
	pov::ColourBlendMapPtr ColorMapDetail::Render<pov::ColourBlendMap>(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, double blendGamma)
	{
		// Simple code to eliminate duplicate entries:
		int p = 0;
		for (int c = 1; c < tempList.size(); c++)
		{
			if (memcmp(&(tempList[p]),
					   &(tempList[c]), sizeof(pov::ColourBlendMapEntry)) == 0)
				p--;

			tempList[++p] = tempList[c];
		}
		p++;
		tempList.resize(p);

		// Create result map:
		auto result = pov::ColourBlendMapPtr(new pov::ColourBlendMap());
		result->Set(tempList);
		result->blendMode = blendMode;

		pov::GammaCurvePtr blendGammaObj;

		if (double::IsNaN(blendGamma))
		{
			blendGammaObj = pov::PowerLawGammaCurve::GetByDecodingGamma(2.5f);
		}
		else
		{
			double value = blendGamma;
			if (value <= 0.0)
			{
				value = 0.0;
			}
			else if ((value < 1.0) && (context->sceneData->gammaMode != pov::kPOVList_GammaMode_None))
			{
				// (we're not warning about below-1.0-values in non-gamma-corrected scenes, as it is likely to make sense there)
				// context->Warning("Suspicious value specified for gamma in color map.");
			}
			blendGammaObj = pov::PowerLawGammaCurve::GetByDecodingGamma(value);
		}
		result->blendGamma = pov::GammaCurvePtr(pov::TranscodingGammaCurve::Get(context->sceneData->workingGamma, blendGammaObj));

		return result;
	}

	template<>
	pov::GenericPigmentBlendMapPtr ColorMapDetail::Render<pov::GenericPigmentBlendMap>(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, double blendGamma)
	{
		return ColorMapDetail::Render<pov::ColourBlendMap>(context, tempList, blendMode, blendGamma);
	}

	template<>
	pov::PigmentBlendMapPtr ColorMapDetail::Render<pov::PigmentBlendMap>(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, double blendGamma)
	{
		context->Error("Internal Error: Parse_Colour_Map called for non-colour blend map");
		return pov::PigmentBlendMapPtr();
	}

	template<>
	std::shared_ptr<pov::GenericNormalBlendMap> ColorMapDetail::Render<pov::GenericNormalBlendMap>(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, double blendGamma)
	{
		context->Error("Internal Error: Parse_Colour_Map called for non-colour blend map");
		return pov::GenericNormalBlendMapPtr();
	}

	template<>
	pov::SlopeBlendMapPtr ColorMapDetail::Render<pov::SlopeBlendMap>(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, double blendGamma)
	{
		context->Error("Internal Error: Parse_Colour_Map called for non-colour blend map");
		return pov::SlopeBlendMapPtr();
	}

	template<>
	pov::NormalBlendMapPtr ColorMapDetail::Render<pov::NormalBlendMap>(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, double blendGamma)
	{
		context->Error("Internal Error: Parse_Colour_Map called for non-colour blend map");
		return pov::NormalBlendMapPtr();
	}

	template<>
	pov::TextureBlendMapPtr ColorMapDetail::Render<pov::TextureBlendMap>(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, double blendGamma)
	{
		context->Error("Internal Error: Parse_Colour_Map called for non-colour blend map");
		return pov::TextureBlendMapPtr();
	}
}