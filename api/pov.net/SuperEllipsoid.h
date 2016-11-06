#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/superellipsoid.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class SuperEllipsoid : Shape
		{
		public:
			Vector2 Power;

			SuperEllipsoid(Vector2 power) :
				Power(power)
			{}

		internal:
			virtual pov::ObjectBase* Render2(Context^ context) override
			{
				auto obj = new pov::Superellipsoid();

				auto v1 = Power.ToVector();

				obj->Power[0] = 2.0 / v1[0];
				obj->Power[1] = v1[0] / v1[1];
				obj->Power[2] = 2.0 / v1[1];

				obj->Compute_BBox();

				return Shape::RenderDetail(context, obj);
			}
		};
	}
}