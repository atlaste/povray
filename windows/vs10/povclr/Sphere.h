#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/sphere.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Sphere : CSGObject
	{
	public:
		Vector3 ^Center;
		double Radius;

		Sphere(Vector3 ^center, double radius) :
			Center(center),
			Radius(radius)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Sphere();
			obj->Center = Center->ToVector();
			obj->Radius = Radius;

			CSGObject::RenderDetail(context, obj);
		}
	};
}