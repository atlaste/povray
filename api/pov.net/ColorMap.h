#pragma once

#include "Color.h"
#include "Context.h"

#include "gcroot.h"

#include <memory>

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	// This struct is necessary because of a bug in C++/CLI: basically the thing doesn't understand template specializations that 
	// are implemented in the cpp file. This workaround does the trick:
	struct ColorMapDetail
	{
		template <typename MapType>
		static std::shared_ptr<MapType> Render(gcroot<Context^> context, std::vector<pov::ColourBlendMapEntry>& tempList, int blendMode, float blendGamma);
	};

	public enum class ColorMapBlendMode
	{
		LinearBlending = 0,
		GammaBlending = 1,
		LinearBlendingGammaBrightness = 2
	};

	public ref class ColorMapEntry
	{
	public:
		ColorMapEntry(float offset, RGBFT color) :
			Offset(offset),
			Color(color)
		{}

		float Offset;
		RGBFT^ Color;
	};
	
	public ref class ColorMap : public List<ColorMapEntry^>
	{
	internal:
		template <typename MapType>
		std::shared_ptr<MapType> Render(Context^ context)
		{
			std::vector<pov::ColourBlendMapEntry> tempList;
			for each (ColorMapEntry^ entry in this)
			{
				pov::ColourBlendMapEntry e;
				pov::TransColour col;
				entry->Color->RenderDetail(col);
				e.value = entry->Offset;
				e.Vals = col;
				tempList.push_back(e);
			}

			return ColorMapDetail::Render<MapType>(context, tempList, (int)BlendMode, BlendGamma);
		}

	public:
		ColorMap() : BlendMode(ColorMapBlendMode::LinearBlending),
			BlendGamma(float::NaN) 
		{}

		ColorMap(... array<ColorMapEntry^>^ args) : 
			List<ColorMapEntry^>(args),
			BlendMode(ColorMapBlendMode::LinearBlending),
			BlendGamma(float::NaN)
		{}

		ColorMapBlendMode BlendMode;
		float BlendGamma; // TODO FIXME: it's actually a bit more complex than a simple float.
	};
}