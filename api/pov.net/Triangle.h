#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/triangle.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	public ref class Triangle : CSGObject
	{
	public:
		Vector3 P1;
		Vector3 P2;
		Vector3 P3;

		Triangle(Vector3 p1, Vector3 p2, Vector3 p3) :
			P1(p1), P2(p2), P3(p3)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Triangle();

			obj->P1 = P1.ToVector();
			obj->P2 = P2.ToVector();
			obj->P3 = P3.ToVector();

			if (!obj->Compute_Triangle())
			{
				context->Warning("Degenerate triangle.");
			}

			CSGObject::RenderDetail(context, obj);
		}
	};
}