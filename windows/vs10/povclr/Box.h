#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/box.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Box : CSGObject
	{
	public:
		Vector3 TopLeft;
		Vector3 BottomRight;

		Box(Vector3 topLeft, Vector3 bottomRight) :
			TopLeft(topLeft),
			BottomRight(bottomRight)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Box();
			obj->bounds[0] = TopLeft.ToVector();
			obj->bounds[1] = BottomRight.ToVector();

			obj->Compute_BBox();

			CSGObject::RenderDetail(context, obj);
		}
	};
}