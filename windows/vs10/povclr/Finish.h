#pragma once

#include "core/material/texture.h"

namespace povclr
{
	public ref class Finish
	{
	public:
		double Diffuse;
		double DiffuseBack;
		double Brilliance;
		double BrillianceOut;
		double BrillianceAdjust;
		double BrillianceAdjustRad;
		double Specular;
		double Roughness;
		double Phong;
		double PhongSize;
		double Irid;
		double IridFilmThickness;
		double IridTurbulence;
		double Crand;
		double Metallic;
		RGB ^Ambient;
		RGB ^Emission;
		RGB ^ReflectionMax;
		RGB ^ReflectionMin;
		RGB ^SubsurfaceTranslucency;
		RGB ^SubsurfaceAnisotropy;
		double ReflectionFalloff;
		bool ReflectionFresnel;
		bool Fresnel;
		double ReflectMetallic;
		int ConserveEnergy;
		bool UseSubsurface;
	};
}