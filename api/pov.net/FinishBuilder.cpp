#include "Stdafx.h"
#include "FinishBuilder.h"

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
		FinishBuilder ^FinishBuilder::Ambient(RGB color)
		{
			Add(gcnew povray::Materials::Finishes::Ambient(color));
			return this;
		}

		FinishBuilder ^FinishBuilder::Ambient(float value)
		{
			Add(gcnew povray::Materials::Finishes::Ambient(RGB(value, value, value)));
			return this;
		}

		FinishBuilder ^FinishBuilder::Brilliance(float amount)
		{
			Add(gcnew povray::Materials::Finishes::Brilliance(amount));
			return this;
		}

		FinishBuilder ^FinishBuilder::Brilliance(float amount, float out)
		{
			Add(gcnew povray::Materials::Finishes::Brilliance(amount, out));
			return this;
		}

		FinishBuilder ^FinishBuilder::ConserveEnergy()
		{
			Add(gcnew povray::Materials::Finishes::ConserveEnergy(true));
			return this;
		}

		FinishBuilder ^FinishBuilder::Crand(float amount)
		{
			Add(gcnew povray::Materials::Finishes::Crand(true));
			return this;
		}

		FinishBuilder ^FinishBuilder::Diffuse(bool albedo, float amount, float backsideIllumination)
		{
			Add(gcnew povray::Materials::Finishes::Diffuse(albedo, amount, backsideIllumination));
			return this;
		}

		FinishBuilder ^FinishBuilder::Diffuse(float amount)
		{
			Add(gcnew povray::Materials::Finishes::Diffuse(amount));
			return this;
		}

		FinishBuilder ^FinishBuilder::Emission(RGB color)
		{
			Add(gcnew povray::Materials::Finishes::Emission(color));
			return this;
		}

		FinishBuilder ^FinishBuilder::Irid(float amount, float thickness, float turbulence)
		{
			Add(gcnew povray::Materials::Finishes::Irid(amount, thickness, turbulence));
			return this;
		}

		FinishBuilder ^FinishBuilder::Metallic(float amount)
		{
			Add(gcnew povray::Materials::Finishes::Metallic(amount));
			return this;
		}

		FinishBuilder ^FinishBuilder::Phong(bool albedo, float amount, float size)
		{
			Add(gcnew povray::Materials::Finishes::Phong(albedo, amount, size));
			return this;
		}

		FinishBuilder ^FinishBuilder::Phong(float amount)
		{
			Add(gcnew povray::Materials::Finishes::Phong(amount));
			return this;
		}

		FinishBuilder ^FinishBuilder::Reflection(RGB color)
		{
			Add(gcnew povray::Materials::Finishes::Reflection(color));
			return this;
		}

		FinishBuilder ^FinishBuilder::Reflection(RGB minColor, RGB maxColor, float falloff)
		{
			Add(gcnew povray::Materials::Finishes::Reflection(minColor, maxColor, falloff));
			return this;
		}

		FinishBuilder ^FinishBuilder::Roughness(float amount)
		{
			Add(gcnew povray::Materials::Finishes::Roughness(amount));
			return this;
		}

		FinishBuilder ^FinishBuilder::Specular(bool albedo, float amount)
		{
			Add(gcnew povray::Materials::Finishes::Specular(albedo, amount));
			return this;
		}

		FinishBuilder ^FinishBuilder::Specular(float amount)
		{
			Add(gcnew povray::Materials::Finishes::Specular(amount));
			return this;
		}

		FinishBuilder ^FinishBuilder::SubSurface(RGB translucency, RGB anisotropy)
		{
			Add(gcnew povray::Materials::Finishes::SubSurface(translucency, anisotropy));
			return this;
		}
	}
}