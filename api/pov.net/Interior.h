#pragma once

#include "Color.h"

#include "core/material/texture.h"
#include "core/material/normal.h"

namespace povclr
{
	public ref class Interior
	{
	public:
		bool Hollow = false;
		float IOR;
		float Dispersion;
		float Caustics;
		float FadeDistance;
		float FadePower;
		RGB FadeColour;
		// TODO: vector<Media> media;
		// TODO: shared_ptr<SubsurfaceInterior> subsurface;
	};
}