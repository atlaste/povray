#pragma once

#include "core/material/texture.h"
#include "core/material/normal.h"

namespace povray
{
	public ref class Normal : public IMapType
	{
	internal:
		void Render(Context^ context, pov::TNORMAL* normal)
		{
			Pattern->RenderNormalBlendMap(context, normal);
			throw gcnew NotImplementedException("TODO FIXME: Not implemented properly");
		}

	public:
		Normal(Pattern ^pattern) :
			Pattern(pattern)
		{}

		float Amount;
		float Delta;
		Pattern^ Pattern;
	};
}