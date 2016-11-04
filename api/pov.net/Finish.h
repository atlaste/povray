#pragma once

#include "core/material/texture.h"

namespace povray
{
	[Flags()]
	enum class FinishAdjustments
	{
		None = 0x0000,
		DiffuseAdjust = 0x0001,
		PhongAdjust = 0x0002,
		SpecularAdjust = 0x0004,
		AmbientSet = 0x0008,
		EmissionSet = 0x0010
	};

	public ref class Finish abstract
	{
	internal:
		virtual FinishAdjustments Render(pov::Finish_Struct* texture) = 0;
	public:
		Finish() {}
	};
}