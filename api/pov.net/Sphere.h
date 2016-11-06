#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/sphere.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class Sphere : Shape
		{
		public:
			Vector3 Center;
			float Radius;

			Sphere(Vector3 center, float radius) :
				Center(center),
				Radius(radius)
			{}

		internal:
			virtual pov::ObjectBase* Render2(Context^ context) override
			{
				auto obj = new pov::Sphere();
				obj->Center = Center.ToVector();
				obj->Radius = Radius;

				obj->Compute_BBox();

				return Shape::RenderDetail(context, obj);
			}
		};
	}
}