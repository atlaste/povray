#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/quadric.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class Quadric : Shape
		{
		public:
			Vector3 SquareTerms;
			Vector3 MixedTerms;
			Vector3 Terms;
			float Constant;

			Quadric(Vector3 squareTerms, Vector3 mixedTerms, Vector3 terms, float constant) :
				SquareTerms(squareTerms),
				MixedTerms(mixedTerms),
				Terms(terms),
				Constant(constant)
			{}

			virtual void Render(Context^ context) override
			{
				auto obj = new pov::Quadric();

				auto min = pov::Vector3d(-BOUND_HUGE);
				auto max = pov::Vector3d(BOUND_HUGE);

				obj->Compute_BBox(min, max);

				Shape::RenderDetail(context, obj);
			}
		};
	}
}