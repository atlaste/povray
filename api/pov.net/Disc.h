#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/disc.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class Disc : Shape
		{
		public:
			Vector3 Center;
			Vector3 Normal;
			float InnerRadius;
			float OuterRadius;

			Disc(Vector3 center, Vector3 normal, float outerRadius) :
				Center(center),
				Normal(normal),
				OuterRadius(outerRadius),
				InnerRadius(0)
			{}

			Disc(Vector3 center, Vector3 normal, float outerRadius, float innerRadius) :
				Center(center),
				Normal(normal),
				OuterRadius(outerRadius),
				InnerRadius(innerRadius)
			{}

		internal:
			virtual pov::ObjectBase* Render2(Context^ context) override
			{
				auto obj = new pov::Disc();
				obj->center = Center.ToVector();
				obj->normal = Normal.ToVector();
				obj->iradius2 = InnerRadius * InnerRadius;
				obj->oradius2 = OuterRadius * OuterRadius;

				obj->normal.normalize();

				auto tmpf = dot(obj->center, obj->normal);
				obj->d = -tmpf;

				obj->Compute_Disc();

				return Shape::RenderDetail(context, obj);
			}
		};
	}
}