#pragma once

#include "Context.h"
#include "Pigment.h"
#include "Color.h"

#include "core/material/pigment.h"
#include "core/material/pattern.h"

namespace povclr
{
	public ref class Pigment // TODO FIXME : Pattern
	{
	internal:
		void RenderDetail(Context^ context, pov::PIGMENT* pigment)
		{
			// Piece of code from parse_materials.cpp:1009 -> Parse_Pattern(...) - the plain color case.
			pigment->Type = pov::PLAIN_PATTERN;
			pigment->pattern = pov::PatternPtr(new pov::PlainPattern());
			Color->RenderDetail(pigment->colour);
		}

	public:
		Pigment(RGBFT ^color) :
			Color(color)
		{}

		RGBFT ^Color;
	};
}