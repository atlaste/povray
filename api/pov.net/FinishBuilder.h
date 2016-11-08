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

namespace povray
{
	namespace Materials
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
			FinishBuilder ^Ambient(RGB color);

			FinishBuilder ^Ambient(float value);

			FinishBuilder ^Brilliance(float amount);

			FinishBuilder ^Brilliance(float amount, float out);

			FinishBuilder ^ConserveEnergy();

			FinishBuilder ^Crand(float amount);

			FinishBuilder ^Diffuse(bool albedo, float amount, float backsideIllumination);

			FinishBuilder ^Diffuse(float amount);

			FinishBuilder ^Emission(RGB color);

			FinishBuilder ^Irid(float amount, float thickness, float turbulence);

			FinishBuilder ^Metallic(float amount);

			FinishBuilder ^Phong(bool albedo, float amount, float size);

			FinishBuilder ^Phong(float amount);

			FinishBuilder ^Reflection(RGB color);

			FinishBuilder ^Reflection(RGB minColor, RGB maxColor, float falloff);

			FinishBuilder ^Roughness(float amount);

			FinishBuilder ^Specular(bool albedo, float amount);

			FinishBuilder ^Specular(float amount);

			FinishBuilder ^SubSurface(RGB translucency, RGB anisotropy);
		};
	}
}