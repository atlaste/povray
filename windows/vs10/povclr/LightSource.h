#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class LightSource : public SceneObject
	{
	public:
		LightSource(Vector3 location, RGB color) :
			Location(location),
			Color(color)
		{}

		Vector3 Location;
		RGB Color;

		virtual void Render(Context^ context) override;
	};
}