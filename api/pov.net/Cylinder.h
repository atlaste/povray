#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/cone.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class Cylinder : Shape
		{
		public:
			Vector3 Base;
			Vector3 Apex;
			float Radius;

			Cylinder(Vector3 base, Vector3 apex, float radius) :
				Base(base),
				Apex(apex),
				Radius(radius)
			{}

		internal:
			virtual pov::ObjectBase* Render2(Context^ context) override
			{
				auto obj = new pov::Cone();
				obj->base = Base.ToVector();
				obj->base_radius = Radius;
				obj->apex = Apex.ToVector();
				obj->apex_radius = Radius;

				obj->Compute_BBox();

				return Shape::RenderDetail(context, obj);
			}
		};
	}
}