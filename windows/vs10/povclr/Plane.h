#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/plane.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Plane : CSGObject
	{
	public:
		Vector3 ^Normal;
		double Distance;

		Plane(Vector3 ^normal, double distance) :
			Normal(normal),
			Distance(distance)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Plane();
			obj->Normal_Vector = Normal->ToVector();
			obj->Distance = -Distance;

			auto len = obj->Normal_Vector.length();
			if (len < EPSILON)
			{
				context->Error("Degenerate plane normal.");
			}
			obj->Normal_Vector /= len;

			obj->Compute_BBox();

			CSGObject::RenderDetail(context, obj);
		}
	};
}