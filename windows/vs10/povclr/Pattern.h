#pragma once

#include "Context.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"

namespace povclr
{
	public ref class Pattern abstract
	{
	internal:
		virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) = 0;
		virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) = 0;
		virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) = 0;

		pov::ClassicTurbulence *CheckTurbulence(pov::WarpList& warps, bool patternHandlesTurbulence)
		{
			pov::ClassicTurbulence* turb = NULL;

			if (!warps.empty())
			{
				turb = dynamic_cast<pov::ClassicTurbulence*>(warps.front());
			}

			if (!turb)
			{
				turb = new pov::ClassicTurbulence(patternHandlesTurbulence);
				warps.insert(warps.begin(), turb);
			}

			return turb;
		}

	public:
		Pattern() {}
	};
}

#define IMPLEMENT_DEFAULT_PATTERN_CODE \
virtual void RenderPigmentBlendMap(Context^ context, pov::PIGMENT* pigment) override { Render<pov::GenericPigmentBlendMap, pov::PIGMENT>(context, pigment); } \
virtual void RenderNormalBlendMap(Context^ context, pov::TNORMAL* normal) override { Render<pov::GenericNormalBlendMap, pov::TNORMAL>(context, normal); } \
virtual void RenderTextureBlendMap(Context^ context, pov::TEXTURE* texture) override { Render<pov::TextureBlendMap, pov::TEXTURE>(context, texture); } 
