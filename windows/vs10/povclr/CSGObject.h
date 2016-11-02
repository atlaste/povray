#pragma once

#include "Context.h"
#include "Settings.h"
#include "SceneObject.h"
#include "Math.h"
#include "Texture.h"
#include "Interior.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	[Flags()]
	public enum class CSGFlags : int
	{
		None = 0x00000000,
		NoShadows = 0x00000001, /* Object doesn't cast shadows            */
		Closed = 0x00000002, /* Object is closed                       */
		Inverted = 0x00000004, /* Object is inverted                     */
		Smoothed = 0x00000008, /* Object is smoothed                     */
		Cylinder = 0x00000010, /* Object is a cylinder                   */
		Degenerate = 0x00000020, /* Object is degenerate                   */
		Sturm = 0x00000040, /* Object should use sturmian root solver */
		Opaque = 0x00000080, /* Object is opaque                       */
		MultiTexture = 0x00000100, /* Object is multi-textured primitive     */
		Infinite = 0x00000200, /* Object is infinite                     */
		Hierarchy = 0x00000400, /* Object can have a bounding hierarchy   */
		Hollow = 0x00000800, /* Object is hollow (atmosphere inside)   */
		HollowSet = 0x00001000, /* Hollow explicitly set in scene file    */
		UV = 0x00002000, /* Object uses UV mapping                 */
		DoubleIlluminate = 0x00004000, /* Illuminate both sides of the surface   */
		NoImage = 0x00008000, /* Object doesn't catch camera rays     [ENB 9/97] */
		NoReflection = 0x00010000, /* Object doesn't catch reflection rays [ENB 9/97] */
		NoGlobalLights = 0x00020000, /* Object doesn't receive light from global lights */
		NoGlobalLightsExpl = 0x00040000, /* Object doesn't receive light from global lights explicitly set in scene file */
		PhotonsTarget = 0x00080000, /* this object is a photons target */
		PhotonsPassThrough = 0x00100000, /* this is pass through object (i.e. it may let photons pass on their way to the target) */
		PhotonsReflectOn = 0x00200000, /* this object explicitly reflects photons */
		PhotonsReflectOff = 0x00400000, /* this object explicitly does not reflect photons */
		PhotonsRefractOn = 0x00800000, /* this object explicitly refracts photons */
		PhotonsRefractOff = 0x01000000, /* this object explicitly does not refract photons */
		IgnorePhotons = 0x02000000, /* this object does not collect photons */
		IgnoreRadiosity = 0x04000000, /* Object doesn't receive ambient light from radiosity */
		NoRadiosity = 0x08000000, /* Object doesn't catch radiosity rays (i.e. is invisible to radiosity) */
		CutawayTextures = 0x10000000  /* Object (or any of its parents) has cutaway_textures set */
	};

	public ref class CSGObject abstract : public SceneObject
	{
	internal:
		void RenderDetail(Context^ context, pov::ObjectBase* obj);

	public:
		bool UVMapping;
		bool Inverse;
		double Refraction;
		double Reflection;
		double PassThrough;
		Texture ^Surface;
		Interior ^Interior;
		Texture ^InteriorTexture;
		Transformation ^Transform;
		double PhotonDensity;
		double RadiosityImportance;
		CSGFlags ^Flags;

		CSGObject() :
			UVMapping(false),
			Refraction(double::NaN),
			Reflection(double::NaN),
			PassThrough(0),
			Surface(nullptr),
			Interior(nullptr),
			InteriorTexture(nullptr),
			Transform(nullptr),
			PhotonDensity(0),
			RadiosityImportance(0),
			Flags(CSGFlags::None)
		{}

		virtual void Render(Context^ context) override = 0;
	};
}