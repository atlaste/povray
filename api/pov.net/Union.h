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

			Union(... array<Shape^>^ args)
			{
				Objects = gcnew List<Shape^>(args);
			}

			virtual void Render(Context^ context) override
			{
				// TODO FIXME

				throw gcnew NotImplementedException();
			}
		};
	}
}