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
		public ref class Union : Shape
		{
		public:
			List<Shape^> ^ Objects = gcnew List<Shape^>();

			Union(... array<SceneObject^>^ args)
			{
				Objects = gcnew List<Shape^>();
				for each (SceneObject^ obj in args)
				{
					Add(obj);
				}
			}

			void Add(SceneObject^ obj)
			{
				auto sh = dynamic_cast<Shape^>(obj);
				if (!sh)
				{
					// TODO FIXME: Actually this is debatable... a sceneobject can probably be rendered to a CSG? Right?
					throw gcnew ArgumentException("CSG object must be a Shape.");
				}
				Objects->Add(sh);
			}

		internal:
			virtual pov::ObjectBase *Render2(Context^ context) override
			{
				if (Objects->Count < 2)
				{
					context->Warning("Should have at least 2 objects in a CSG Union");
				}

				auto obj = new pov::CSGUnion();
				bool lightSourceUnion = true;

				for each (auto shape in Objects)
				{
					auto child = shape->Render2(context);

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