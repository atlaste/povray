#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/plane.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class Plane : Shape
		{
		public:
			Vector3 Normal;
			float Distance;

			Plane(Vector3 normal, float distance) :
				Normal(normal),
				Distance(distance)
			{}

		internal:
			virtual pov::ObjectBase* Render2(Context^ context) override
			{
				auto obj = new pov::Plane();
				obj->Normal_Vector = Normal.ToVector();
				obj->Distance = -Distance;

				auto len = obj->Normal_Vector.length();
				if (len < EPSILON)
				{
					context->Error("Degenerate plane normal.");
				}
				obj->Normal_Vector /= len;

				obj->Compute_BBox();

				return Shape::RenderDetail(context, obj);
			}
		};
	}
}