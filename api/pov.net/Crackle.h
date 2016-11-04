#pragma once

#include "Context.h"
#include "Pattern.h"
#include "IMapType.h"
#include "Pigment.h"
#include "Normal.h"
#include "Texture.h"
#include "Math.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"
#include "core/material/pattern.h"
#include "core/material/normal.h"

namespace povclr
{
	public ref class Crackle : public Pattern, public ITargetType<Pigment^>, public ITargetType<Normal^>, public ITargetType<Texture^>
	{
	internal:
		template <typename MapType, typename PatternType>
		void Render(Context^ context, PatternType* container)
		{
			container->Type = pov::CRACKLE_PATTERN;
			auto pat = new pov::CracklePattern();
			pat->crackleIsSolid = 0;
			pat->crackleForm = Form.ToVector();
			pat->crackleMetric = Metric;
			pat->crackleOffset = Offset;
			container->pattern = pov::PatternPtr(pat);
		}

		IMPLEMENT_DEFAULT_PATTERN_CODE

	public:
		Crackle() :
			Form(Vector3(-1, 1, 0)),
			Metric(2),
			Offset(0)
		{}
		
		Crackle(Vector3 form) :
			Form(form),
			Metric(2),
			Offset(0)
		{}

		Crackle(Vector3 form, float metric, float offset) : 
			Form(form),
			Metric(metric),
			Offset(offset)
		{}

		Vector3 Form;
		float Metric;
		float Offset;
	};
}