#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/csg.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class Intersection : Shape
		{
		public:
			List<Shape^> ^ Objects = gcnew List<Shape^>();

			Intersection(... array<Shape^>^ args)
			{
				Objects = gcnew List<Shape^>(args);
			}

		internal:
			virtual pov::ObjectBase *Render2(Context^ context) override
			{
				if (Objects->Count < 2)
				{
					context->Warning("Should have at least 2 objects in a CSG Intersection");
				}

				auto obj = new pov::CSGIntersection(false);
				bool lightSourceUnion = true;

				for each (auto shape in Objects)
				{
					auto child = shape->Render2(context);

					if (child->Type & PATCH_OBJECT)
					{
						context->Warning("Patch objects not allowed in intersection.");
					}

					obj->Type |= (child->Type & CHILDREN_FLAGS);
					child->Type |= IS_CHILD_OBJECT;

					if (!(child->Type & LIGHT_SOURCE_OBJECT))
					{
						lightSourceUnion = false;
					}
					obj->children.push_back(child);
				}

				if (lightSourceUnion)
				{
					obj->Type |= LT_SRC_UNION_OBJECT;
				}

				obj->Compute_BBox();

				return Shape::RenderDetail(context, obj);
			}
		};
	}
}