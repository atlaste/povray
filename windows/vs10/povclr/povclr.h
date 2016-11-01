#pragma once

#include "core/scene/scenedata.h"
#include "vm/fnpovfpu.h"
#include "povms/povmsid.h"
#include "core/render/tracepixel.h"
#include "core/scene/tracethreaddata.h"
#include "core/shape/sphere.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Settings
	{
	public:
		Settings() :
			WarningLevel(0),
			SplitUnions(false),
			RemoveBounds(true),
			BoundingMethod(2),
			OutputAlpha(false),
			BspMaxDepth(0),
			BspObjectISectCost(0.0f),
			BspBaseAccessCost(0.0f),
			BspChildAccessCost(0.0f),
			BspMissChance(0.0f),
			TracingMethod(0),
			JitterScale(1.0),
			Jitter(false),
			AAThreshold(0.3),
			AADepth(3),
			AAGammaValue(1.0),
			Width(320),
			Height(200)
		{}

		int WarningLevel;
		bool SplitUnions;
		bool RemoveBounds;
		int BoundingMethod;
		bool OutputAlpha;
		int BspMaxDepth;
		float BspObjectISectCost;
		float BspBaseAccessCost;
		float BspChildAccessCost;
		float BspMissChance;

		int Quality = 9;

		int TracingMethod = 0;
		double JitterScale = 1.0;
		bool Jitter = false;
		double AAThreshold = 0.3;
		int AADepth = 3;
		double AAGammaValue = 1.0;

		int Width;
		int Height;
	};

	public ref class Context : IDisposable
	{
	internal:
		Context(Settings^ settings)
		{
			camera = nullptr;

			// Initialize scene data:
			sceneData = new pov::SceneData(new pov::FunctionVM());
			sceneData->tree = NULL;

			sceneData->warningLevel = settings->WarningLevel;

			sceneData->defaultFileType = DEFAULT_OUTPUT_FORMAT;
			sceneData->clocklessAnimation = false;
			sceneData->splitUnions = settings->SplitUnions;
			sceneData->removeBounds = settings->RemoveBounds;
			sceneData->boundingMethod = settings->BoundingMethod;
			sceneData->outputAlpha = settings->OutputAlpha;

			if (!sceneData->outputAlpha)
			{
				// if we're not outputting an alpha channel, precompose the scene background against a black "background behind the background"
				// (NB: Here, background color is still at its default of <0,0,0,0,1> = full transparency; we're changing that to opaque black.)
				sceneData->backgroundColour.Clear();
			}

			// NB a value of '0' for any of the BSP parameters tells the BSP code to use its internal default
			sceneData->bspMaxDepth = settings->BspMaxDepth;
			sceneData->bspObjectIsectCost = settings->BspObjectISectCost;
			sceneData->bspBaseAccessCost = settings->BspBaseAccessCost;
			sceneData->bspChildAccessCost = settings->BspChildAccessCost;
			sceneData->bspMissChance = settings->BspMissChance;
			sceneData->realTimeRaytracing = false;

			// Defaults:
			defaultTexture = pov::Create_Texture();
			defaultTexture->Pigment = pov::Create_Pigment();
			defaultTexture->Tnormal = NULL;
			defaultTexture->Finish = pov::Create_Finish();
		}

		pov::SceneData* sceneData;
		pov::Camera* camera;

		// Defaults:
		pov::TEXTURE* defaultTexture;

		~Context()
		{
			this->!Context();
		}

	protected:
		!Context()
		{
			if (camera)
			{
				delete camera;
				camera = nullptr;
			}
			if (sceneData)
			{
				delete sceneData;
				sceneData = nullptr;
			}

			if (defaultTexture)
			{
				pov::Destroy_Textures(defaultTexture);
				defaultTexture = NULL;
			}
		}
	};

	public interface class SceneObject
	{
		void Render(Context^ context);
	};

	public interface class RenderTarget
	{
		void Initialize(int width, int height);
		void SetPixel(int x, int y, double red, double green, double blue, double alpha);
		void Finish();
	};

	struct CooperateFunctor : public pov::Trace::CooperateFunctor
	{
		virtual void operator()() {}
	};

	struct BSPProgressFunctor : public pov::BSPTree::Progress
	{
		virtual void operator()(unsigned int nodes) const
		{}
	};

	struct BSPSceneObjects : public pov::BSPTree::Objects
	{
		std::vector<pov::ObjectPtr> infinite;
		std::vector<pov::ObjectPtr> finite;
		unsigned int numLights;

		BSPSceneObjects(std::vector<pov::ObjectPtr>& objects)
		{
			numLights = 0;
			for (std::vector<pov::ObjectPtr>::iterator i(objects.begin()); i != objects.end(); i++)
			{
				if (Test_Flag((*i), INFINITE_FLAG))
				{
					infinite.push_back(*i);
					if (((*i)->Type & LIGHT_SOURCE_OBJECT) != 0)
						numLights++;
				}
				else
				{
					finite.push_back(*i);
				}
			}
		}

		virtual unsigned int size() const
		{
			return finite.size();
		}

		virtual float GetMin(unsigned int axis, unsigned int i) const
		{
			return finite[i]->BBox.lowerLeft[axis];
		}

		virtual float GetMax(unsigned int axis, unsigned int i) const
		{
			return (finite[i]->BBox.lowerLeft[axis] + finite[i]->BBox.size[axis]);
		}

		virtual ~BSPSceneObjects()
		{}
	};

	public ref class Scene : public List<SceneObject^>
	{
	public:
		static Scene()
		{
			pov::Initialize_Noise();
			pov::InitializePatternGenerators();
		}

		Scene()
		{
			Settings = gcnew povclr::Settings();
		}

		property povclr::Settings^ Settings;

		void Render(RenderTarget^ target)
		{

			Context ^renderContext = nullptr;

			try
			{
				// Initialize the scene data:
				renderContext = gcnew Context(Settings);

				// Initialize the scene objects:
				for each(SceneObject^ item in this)
				{
					item->Render(renderContext);
				}

				auto sceneData = std::shared_ptr<pov::SceneData>(renderContext->sceneData);
				renderContext->sceneData = nullptr;

				// Initialize BSP tree:

				// - Split into finite and infinite objects
				BSPSceneObjects objects(sceneData->objects);

				// - Rebuild object list
				BSPProgressFunctor progress;
				sceneData->objects.clear();
				sceneData->objects.insert(sceneData->objects.end(), objects.finite.begin(), objects.finite.end());
				sceneData->objects.insert(sceneData->objects.end(), objects.infinite.begin(), objects.infinite.end());
				sceneData->numberOfFiniteObjects = objects.finite.size();
				sceneData->numberOfInfiniteObjects = objects.infinite.size() - objects.numLights;
				sceneData->tree = new pov::BSPTree(sceneData->bspMaxDepth, sceneData->bspObjectIsectCost, sceneData->bspBaseAccessCost, sceneData->bspChildAccessCost, sceneData->bspMissChance);
				sceneData->tree->build(progress, objects,
									   sceneData->nodes, sceneData->splitNodes, sceneData->objectNodes, sceneData->emptyNodes,
									   sceneData->maxObjects, sceneData->averageObjects, sceneData->maxDepth, sceneData->averageDepth,
									   sceneData->aborts, sceneData->averageAborts, sceneData->averageAbortObjects, sceneData->inputFile);

				// Initialize the ray-tracer:
				pov::QualityFlags quality(Settings->Quality);
				pov::TraceThreadData threadData(sceneData);

				CooperateFunctor cooperate;

				pov::PhotonGatherer photonGatherer(&sceneData->mediaPhotonMap, sceneData->photonSettings);
				pov::MediaFunction media(&threadData, nullptr, &photonGatherer);

				pov::RadiosityCache radiosityCache(sceneData->radiositySettings);
				pov::RadiosityFunction radiosity(sceneData, &threadData,
												 sceneData->radiositySettings, radiosityCache, cooperate, true, renderContext->camera->Location);

				pov::TracePixel trace(sceneData, renderContext->camera, &threadData,
									  sceneData->parsedMaxTraceLevel, sceneData->parsedAdcBailout, quality,
									  cooperate, media, radiosity);

				media.BindTrace(&trace);

				// Render the image:
				auto width = Settings->Width;
				auto height = Settings->Height;

				target->Initialize(width, height);

				for (size_t y = 0; y < height; ++y)
				{
					for (size_t x = 0; x < width; ++x)
					{
						pov_base::RGBTColour col;
						trace(x + 0.5, y + 0.5, width, height, col);

						target->SetPixel(x, y, col.red(), col.green(), col.blue(), col.alpha());
					}
				}

				// Finish:
				target->Finish();
			}
			catch (pov_base::Exception& e)
			{
				if (renderContext)
				{
					delete renderContext;
				}

				throw gcnew Exception(gcnew String(e.what()));
			}
		}
	};

	public ref class Vector3
	{
	public:
		Vector3(double x, double y, double z) :
			X(x), Y(y), Z(z)
		{}

		double X;
		double Y;
		double Z;

	internal:
		pov::Vector3d ToVector() { return pov::Vector3d(X, Y, Z); }
	};


	public ref class Camera : public SceneObject
	{
	public:
		Camera(Vector3^ location, Vector3^ lookAt) :
			Location(location), LookAt(lookAt)
		{}

		Vector3^ Location;
		Vector3^ LookAt;

		virtual void Render(Context^ context)
		{
			if (context->camera)
			{
				throw gcnew Exception("You can only have one camera in a scene.");
			}

			context->camera = new pov::Camera();
			context->camera->Location = Location->ToVector();
			context->camera->Look_At = LookAt->ToVector();
			// etc.

			// context->sceneData->cameras.push_back(context->camera);
		}
	};

	public ref class LightSource : public SceneObject
	{
	public:
		LightSource(Vector3^ location) :
			Location(location)
		{}

		Vector3^ Location;

		virtual void Render(Context^ context)
		{
			auto obj = new pov::LightSource();
			obj->Center = Location->ToVector();

			context->sceneData->lightSources.push_back(obj);
		}
	};

	public ref class RGB
	{
	public:
		RGB(double r, double g, double b) :
			Red(r),
			Green(g),
			Blue(b)
		{}

		double Red;
		double Green;
		double Blue;
	};

	public ref class RGBFT : public RGB
	{
	internal:
		void RenderDetail(pov::TransColour& dst)
		{
			pov::RGBFTColour tmp;
			tmp.red() = Red;
			tmp.green() = Green;
			tmp.blue() = Blue;
			tmp.filter() = Filter;
			tmp.transm() = Transparency;
			dst = ToTransColour(tmp);
		}

	public:
		RGBFT(double r, double g, double b, double f, double t) :
			RGB(r, g, b),
			Filter(f),
			Transparency(t)
		{}

		double Filter;
		double Transparency;
	};

	public ref class Pigment // TODO FIXME : Pattern
	{
	internal:
		void RenderDetail(Context^ context, pov::PIGMENT* pigment)
		{
			Color->RenderDetail(pigment->colour);
		}

	public:
		Pigment(RGBFT ^color) :
			Color(color)
		{}

		RGBFT ^Color;
	};

	public ref class Normal
	{
	public:
		double Amount;
		double Delta;
	};

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

	public ref class Texture
	{
	internal:
		void RenderDetail(Context^ context, pov::TEXTURE* texture)
		{
			if (Next != nullptr)
			{
				texture->Next = pov::Copy_Textures(context->defaultTexture);
				Next->RenderDetail(context, texture->Next);
			}

			if (Pigment != nullptr)
			{
				if (texture->Pigment == nullptr)
				{
					texture->Pigment = pov::Copy_Pigment(context->defaultTexture->Pigment);
				}
				Pigment->RenderDetail(context, texture->Pigment);
			}
		}

	public:
		Texture ^Next;
		Pigment ^Pigment;
		Normal ^Tnormal;
		Finish ^Finish;
		List<Texture^> ^Materials; 
	};

	public ref class Interior
	{
	public:
		bool Hollow = false;
		double IOR;
		double Dispersion;
		double Caustics;
		double FadeDistance;
		double FadePower;
		RGB ^FadeColour;
		// TODO: vector<Media> media;
		// TODO: shared_ptr<SubsurfaceInterior> subsurface;
	};

	public ref class Matrix
	{
	private:
		array<double>^ data = gcnew array<double>(16);

		void Set(int x, int y, double value)
		{
			data[y * 4 + x] = value;
		}

		double Get(int x, int y)
		{
			return data[y * 4 + x];
		}

	public:
		Matrix()
		{ 
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (i == j)
					{
						Set(i, j, 1.0);
					}
					else
					{
						Set(i, j, 0.0);
					}
				}
			}
		}
	};

	public ref class Transformation
	{
	private:
		Matrix ^Trans;
		Matrix ^Inverse;

	public:
		Transformation()
		{
			Trans = gcnew Matrix();
			Inverse = gcnew Matrix();
		}
	};

	[Flags()]
	public enum class CSGFlags : int
	{
		None                 = 0x00000000,
		NoShadows            = 0x00000001, /* Object doesn't cast shadows            */
		Closed               = 0x00000002, /* Object is closed                       */
		Inverted             = 0x00000004, /* Object is inverted                     */
		Smoothed             = 0x00000008, /* Object is smoothed                     */
		Cylinder             = 0x00000010, /* Object is a cylinder                   */
		Degenerate           = 0x00000020, /* Object is degenerate                   */
		Sturm                = 0x00000040, /* Object should use sturmian root solver */
		Opaque               = 0x00000080, /* Object is opaque                       */
		MultiTexture         = 0x00000100, /* Object is multi-textured primitive     */
		Infinite             = 0x00000200, /* Object is infinite                     */
		Hierarchy            = 0x00000400, /* Object can have a bounding hierarchy   */
		Hollow               = 0x00000800, /* Object is hollow (atmosphere inside)   */
		HollowSet            = 0x00001000, /* Hollow explicitly set in scene file    */
		UV                   = 0x00002000, /* Object uses UV mapping                 */
		DoubleIlluminate     = 0x00004000, /* Illuminate both sides of the surface   */
		NoImage              = 0x00008000, /* Object doesn't catch camera rays     [ENB 9/97] */
		NoReflection         = 0x00010000, /* Object doesn't catch reflection rays [ENB 9/97] */
		NoGlobalLights       = 0x00020000, /* Object doesn't receive light from global lights */
		NoGlobalLightsExpl   = 0x00040000, /* Object doesn't receive light from global lights explicitly set in scene file */
		PhotonsTarget        = 0x00080000, /* this object is a photons target */
		PhotonsPassThrough   = 0x00100000, /* this is pass through object (i.e. it may let photons pass on their way to the target) */
		PhotonsReflectOn     = 0x00200000, /* this object explicitly reflects photons */
		PhotonsReflectOff    = 0x00400000, /* this object explicitly does not reflect photons */
		PhotonsRefractOn     = 0x00800000, /* this object explicitly refracts photons */
		PhotonsRefractOff    = 0x01000000, /* this object explicitly does not refract photons */
		IgnorePhotons        = 0x02000000, /* this object does not collect photons */
		IgnoreRadiosity      = 0x04000000, /* Object doesn't receive ambient light from radiosity */
		NoRadiosity          = 0x08000000, /* Object doesn't catch radiosity rays (i.e. is invisible to radiosity) */
		CutawayTextures      = 0x10000000  /* Object (or any of its parents) has cutaway_textures set */
	};

	public ref class CSGObject abstract : public SceneObject
	{
	internal:
		void RenderDetail(Context^ context, pov::ObjectBase* obj)
		{
			if (Surface)
			{
				obj->Texture = pov::Copy_Textures(context->defaultTexture);
				Surface->RenderDetail(context, obj->Texture);
			}
		}

	public:
		Texture ^Surface;
		Interior ^Interior;
		Texture ^InteriorTexture;
		Transformation ^Transform;
		double PhotonDensity;
		double RadiosityImportance;
		CSGFlags ^Flags;

		CSGObject() :
			Surface(nullptr),
			Interior(nullptr),
			InteriorTexture(nullptr),
			Transform(nullptr),
			PhotonDensity(0),
			RadiosityImportance(0),
			Flags(CSGFlags::None)
		{}

		virtual void Render(Context^ context) = 0;
	};

	public ref class Sphere : CSGObject
	{
	public:
		Vector3 ^Center;
		double Radius;

		Sphere(Vector3 ^center, double radius) :
			Center(center),
			Radius(radius)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Sphere();
			obj->Center = Center->ToVector();
			obj->Radius = Radius;

			context->sceneData->objects.push_back(obj);
		}
	};
}
