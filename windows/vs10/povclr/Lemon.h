#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/lemon.h"
#include "base/messenger.h"
#include "base/textstream.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Lemon : CSGObject
	{
	public:
		Vector3 ^Base;
		double BaseRadius;
		Vector3 ^Apex;
		double ApexRadius;
		double InnerRadius;

		Lemon(Vector3 ^base, double baseRadius, Vector3 ^apex, double apexRadius, double innerRadius) :
			Base(base),
			BaseRadius(baseRadius),
			Apex(apex),
			ApexRadius(apexRadius),
			InnerRadius(innerRadius)
		{}

		virtual void Render(Context^ context) override;
	};
}