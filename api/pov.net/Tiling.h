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
			public enum class TilingPattern
			{
				Square = 1,
				Hexagon = 2,
				Triangle = 3,
				Lozenge = 4,
				Rhombus = 5,
				Rectangle = 6,
				OctaSquare = 7,
				SquareTriangle = 8,
				HexaTriangle = 9,
				SquareOffset = 10,
				SquareRectangle = 11,
				RectangleSquare = 12,
				SquareInternal = 13,
				SquareInternal5 = 14,
				Squarefloat = 15,
				HexaSquareTriangle = 16,
				HexaSquareTriangle6 = 17,
				RectanglePair = 18,
				HexaTriRight = 19,
				HexaTriLeft = 20,
				SquareTri = 21,
				DodecaTri = 22,
				DodecaHex = 23,
				DodecaHex5 = 24,
				Penrose1 = 25,
				Penrose2 = 26,
				Penrose3 = 27
			};

			public ref class Tiling : public Pattern, public ITargetType<Pigment^>, public ITargetType<Normal^>, public ITargetType<Texture^>
			{
			internal:
				template <typename MapType, typename PatternType>
				void Render(Context^ context, PatternType* container)
				{
					container->Type = pov::TILING_PATTERN;
					auto pat = new pov::TilingPattern();
					pat->tilingType = (int)Pattern;
					container->pattern = pov::PatternPtr(pat);
				}

				IMPLEMENT_DEFAULT_PATTERN_CODE

			public:
				Tiling(TilingPattern pattern) :
					Pattern(pattern)
				{}

				TilingPattern Pattern;
			};
		}
	}
}