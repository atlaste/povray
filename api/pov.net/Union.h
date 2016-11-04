#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/csg.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	public ref class Union : CSGObject
	{
	public:
		List<CSGObject^> ^ Objects = gcnew List<CSGObject^>();

		Union(... array<CSGObject^>^ args)
		{
			Objects = gcnew List<CSGObject^>(args);
		}

		virtual void Render(Context^ context) override
		{
			// TODO FIXME

			throw gcnew NotImplementedException();
		}
	};
}