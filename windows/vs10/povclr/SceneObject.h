#pragma once

#include "Context.h"
#include "Settings.h"

#include "core/shape/blob.h"
#include "core/shape/mesh.h"
#include "core/shape/csg.h"
#include "core/shape/quadric.h"
#include "core/shape/polynomial.h"
#include "core/lighting/subsurface.h"
#include "core/material/pattern.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class SceneObject abstract
	{
	internal:
		void CheckPassThru(Context^ context, pov::ObjectPtr o, int flag);
		void PostProcess(Context^ context, pov::ObjectBase* obj, pov::ObjectBase* parent);
		void LinkToFrame(Context^ context, pov::ObjectPtr obj);

	public:
		virtual void Render(Context^ context) = 0;
	};
}