#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/torus.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public enum class SpindleMode
	{
		Difference,
		Intersection,
		Merge,
		Union
	};

	public ref class Torus : CSGObject
	{
	public:
		double MajorRadius;
		double MinorRadius;
		SpindleMode Spindle;

		Torus(double majorRadius, double minorRadius) :
			MajorRadius(majorRadius), MinorRadius(minorRadius), Spindle(SpindleMode::Union)
		{}

		Torus(double majorRadius, double minorRadius, SpindleMode spindleMode) :
			MajorRadius(majorRadius), MinorRadius(minorRadius), Spindle(spindleMode)
		{}

		virtual void Render(Context^ context) override
		{
			bool invert = false;
			pov::SpindleTorus::SpindleMode spindleMode = pov::SpindleTorus::UnionSpindle;
			bool spindleModeSet = false;

			auto majorRadius = MajorRadius;
			if (majorRadius < 0)
			{
				context->Warning("Negative torus major radius has the effect of inverting the object; if this is intentional, use the 'inverse' keyword instead.");
				majorRadius = -majorRadius;
				invert = !invert;
			}

			auto minorRadius = MinorRadius;
			if (minorRadius < 0)
			{
				context->Warning("Negative torus minor radius has the effect of inverting the object; if this is intentional, use the 'inverse' keyword instead.");
				minorRadius = -minorRadius;
				invert = !invert;
			}

			bool spindleTorus = (majorRadius < minorRadius);

			switch (Spindle)
			{
				case SpindleMode::Difference: spindleMode = pov::SpindleTorus::DifferenceSpindle; break;
				case SpindleMode::Merge: spindleMode = pov::SpindleTorus::MergeSpindle; break;
				case SpindleMode::Intersection: spindleMode = pov::SpindleTorus::IntersectionSpindle; break;
				default:
				case SpindleMode::Union: spindleMode = pov::SpindleTorus::UnionSpindle; break;
			}

			if (spindleTorus)
			{
				auto obj = new pov::SpindleTorus();
				obj->mSpindleMode = spindleMode;
				
				obj->MajorRadius = majorRadius;
				obj->MinorRadius = minorRadius;

				obj->Compute_BBox();

				CSGObject::RenderDetail(context, obj);

			}
			else
			{
				auto obj = new pov::Torus();

				obj->MajorRadius = majorRadius;
				obj->MinorRadius = minorRadius;

				obj->Compute_BBox();

				CSGObject::RenderDetail(context, obj);
			}


			// TODO FIXME: Not sure if this is needed; RenderDetail already handles inversion.
			//
			// if (invert)
			// {
			// 	obj = obj->Invert();
			// }

		}
	};
}