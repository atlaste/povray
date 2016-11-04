#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/cone.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Cone : CSGObject
	{
	public:
		Vector3 Base;
		float BaseRadius;
		Vector3 Apex;
		float ApexRadius;

		Cone(Vector3 base, float baseRadius, Vector3 apex, float apexRadius) :
			Base(base),
			BaseRadius(baseRadius),
			Apex(apex),
			ApexRadius(apexRadius)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Cone();
			obj->base = Base.ToVector();
			obj->base_radius = BaseRadius;
			obj->apex = Apex.ToVector();
			obj->apex_radius = ApexRadius;

			obj->Compute_BBox();

			CSGObject::RenderDetail(context, obj);
		}
	};
}