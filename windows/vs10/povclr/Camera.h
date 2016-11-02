#pragma once

#include "Context.h"
#include "Settings.h"
#include "SceneObject.h"
#include "Math.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Camera : public SceneObject
	{
	public:
		Camera(Vector3^ location, Vector3^ lookAt) :
			Location(location), LookAt(lookAt)
		{}

		Vector3^ Location;
		Vector3^ LookAt;

		virtual void Render(Context^ context) override;
	};
}