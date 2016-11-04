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

namespace povclr
{
	FinishBuilder ^FinishBuilder::Ambient(RGB^ color)
	{
		Add(gcnew povclr::Ambient(color));
		return this;
	}

	FinishBuilder ^FinishBuilder::Brilliance(float amount)
	{
		Add(gcnew povclr::Brilliance(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Brilliance(float amount, float out)
	{
		Add(gcnew povclr::Brilliance(amount, out));
		return this;
	}

	FinishBuilder ^FinishBuilder::ConserveEnergy()
	{
		Add(gcnew povclr::ConserveEnergy(true));
		return this;
	}

	FinishBuilder ^FinishBuilder::Crand(float amount)
	{
		Add(gcnew povclr::Crand(true));
		return this;
	}

	FinishBuilder ^FinishBuilder::Diffuse(bool albedo, float amount, float backsideIllumination)
	{
		Add(gcnew povclr::Diffuse(albedo, amount, backsideIllumination));
		return this;
	}

	FinishBuilder ^FinishBuilder::Diffuse(float amount)
	{
		Add(gcnew povclr::Diffuse(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Emission(RGB^ color)
	{
		Add(gcnew povclr::Emission(color));
		return this;
	}

	FinishBuilder ^FinishBuilder::Irid(float amount, float thickness, float turbulence)
	{
		Add(gcnew povclr::Irid(amount, thickness, turbulence));
		return this;
	}

	FinishBuilder ^FinishBuilder::Metallic(float amount)
	{
		Add(gcnew povclr::Metallic(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Phong(bool albedo, float amount, float size)
	{
		Add(gcnew povclr::Phong(albedo, amount, size));
		return this;
	}

	FinishBuilder ^FinishBuilder::Phong(float amount)
	{
		Add(gcnew povclr::Phong(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Reflection(RGB^ color)
	{
		Add(gcnew povclr::Reflection(color));
		return this;
	}

	FinishBuilder ^FinishBuilder::Reflection(RGB^ minColor, RGB maxColor, float falloff)
	{
		Add(gcnew povclr::Reflection(minColor, maxColor, falloff));
		return this;
	}

	FinishBuilder ^FinishBuilder::Roughness(float amount)
	{
		Add(gcnew povclr::Roughness(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Specular(bool albedo, float amount)
	{
		Add(gcnew povclr::Specular(albedo, amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::Specular(float amount)
	{
		Add(gcnew povclr::Specular(amount));
		return this;
	}

	FinishBuilder ^FinishBuilder::SubSurface(RGB^ translucency, RGB anisotropy)
	{
		Add(gcnew povclr::SubSurface(translucency, anisotropy));
		return this;
	}
}