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
	FinishBuilder ^FinishBuilder::Ambient(RGB^ color)
	{
		Add(gcnew povray::Ambient(color));
		return this;
	}

	FinishBuilder ^FinishBuilder::Brilliance(float amount)
	{
		Add(gcnew povray::Brilliance(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Brilliance(float amount, float out)
	{
		Add(gcnew povray::Brilliance(amount, out));
		return this;
	}

	FinishBuilder ^FinishBuilder::ConserveEnergy()
	{
		Add(gcnew povray::ConserveEnergy(true));
		return this;
	}

	FinishBuilder ^FinishBuilder::Crand(float amount)
	{
		Add(gcnew povray::Crand(true));
		return this;
	}

	FinishBuilder ^FinishBuilder::Diffuse(bool albedo, float amount, float backsideIllumination)
	{
		Add(gcnew povray::Diffuse(albedo, amount, backsideIllumination));
		return this;
	}

	FinishBuilder ^FinishBuilder::Diffuse(float amount)
	{
		Add(gcnew povray::Diffuse(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Emission(RGB^ color)
	{
		Add(gcnew povray::Emission(color));
		return this;
	}

	FinishBuilder ^FinishBuilder::Irid(float amount, float thickness, float turbulence)
	{
		Add(gcnew povray::Irid(amount, thickness, turbulence));
		return this;
	}

	FinishBuilder ^FinishBuilder::Metallic(float amount)
	{
		Add(gcnew povray::Metallic(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Phong(bool albedo, float amount, float size)
	{
		Add(gcnew povray::Phong(albedo, amount, size));
		return this;
	}

	FinishBuilder ^FinishBuilder::Phong(float amount)
	{
		Add(gcnew povray::Phong(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Reflection(RGB^ color)
	{
		Add(gcnew povray::Reflection(color));
		return this;
	}

	FinishBuilder ^FinishBuilder::Reflection(RGB^ minColor, RGB maxColor, float falloff)
	{
		Add(gcnew povray::Reflection(minColor, maxColor, falloff));
		return this;
	}

	FinishBuilder ^FinishBuilder::Roughness(float amount)
	{
		Add(gcnew povray::Roughness(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Specular(bool albedo, float amount)
	{
		Add(gcnew povray::Specular(albedo, amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Specular(float amount)
	{
		Add(gcnew povray::Specular(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::SubSurface(RGB^ translucency, RGB anisotropy)
	{
		Add(gcnew povray::SubSurface(translucency, anisotropy));
		return this;
	}
}