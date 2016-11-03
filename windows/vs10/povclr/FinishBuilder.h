#pragma once

#include "Ambient.h"
#include "Brilliance.h"
#include "ConserveEnergy.h"
#include "Crand.h"
#include "Diffuse.h"
#include "Emission.h"
#include "Irid.h"
#include "Metallic.h"
#include "Phong.h"
#include "Reflection.h"
#include "Roughness.h"
#include "Specular.h"
#include "SubSurface.h"

namespace povclr
{
	ref class Ambient;
	ref class Brilliance;
	ref class ConserveEnergy;
	ref class Crand;
	ref class Diffuse;
	ref class Emission;
	ref class Irid;
	ref class Metallic;
	ref class Phong;
	ref class Reflection;
	ref class Roughness;
	ref class Specular;
	ref class SubSurface;

	public ref class FinishBuilder : public List<Finish^>
	{
	public:
		FinishBuilder ^Ambient(RGB^ color);

		FinishBuilder ^Brilliance(double amount);

		FinishBuilder ^Brilliance(double amount, double out);

		FinishBuilder ^ConserveEnergy();

		FinishBuilder ^Crand(double amount);

		FinishBuilder ^Diffuse(bool albedo, double amount, double backsideIllumination);

		FinishBuilder ^Diffuse(double amount);

		FinishBuilder ^Emission(RGB^ color);

		FinishBuilder ^Irid(double amount, double thickness, double turbulence);

		FinishBuilder ^Metallic(double amount);

		FinishBuilder ^Phong(bool albedo, double amount, double size);

		FinishBuilder ^Phong(double amount);

		FinishBuilder ^Reflection(RGB^ color);

		FinishBuilder ^Reflection(RGB^ minColor, RGB ^maxColor, double falloff);

		FinishBuilder ^Roughness(double amount);

		FinishBuilder ^Specular(bool albedo, double amount);

		FinishBuilder ^Specular(double amount);

		FinishBuilder ^SubSurface(RGB^ translucency, RGB ^anisotropy);
	};
}