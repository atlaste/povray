#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/sphere.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	public ref class Sphere : CSGObject
	{
	public:
		Vector3 Center;
		float Radius;

		Sphere(Vector3 center, float radius) :
			Center(center),
			Radius(radius)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Sphere();
			obj->Center = Center.ToVector();
			obj->Radius = Radius;

			obj->Compute_BBox();

			CSGObject::RenderDetail(context, obj);
		}
	};
}