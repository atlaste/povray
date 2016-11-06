#pragma once

#include "Context.h"
#include "Settings.h"
#include "Shape.h"
#include "Math.h"

#include "core/shape/lemon.h"
#include "base/messenger.h"
#include "base/textstream.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	namespace Shapes
	{
		public ref class Lemon : Shape
		{
		public:
			Vector3 Base;
			float BaseRadius;
			Vector3 Apex;
			float ApexRadius;
			float InnerRadius;

			Lemon(Vector3 base, float baseRadius, Vector3 apex, float apexRadius, float innerRadius) :
				Base(base),
				BaseRadius(baseRadius),
				Apex(apex),
				ApexRadius(apexRadius),
				InnerRadius(innerRadius)
			{}

		internal:
			pov::ObjectBase* Render2(Context^ context) override;
		};
	}
}