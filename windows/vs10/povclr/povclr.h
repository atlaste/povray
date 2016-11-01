#pragma once

#include "core/scene/scenedata.h"
#include "vm/fnpovfpu.h"
#include "povms/povmsid.h"
#include "core/render/tracepixel.h"
#include "core/scene/tracethreaddata.h"

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
		}

		pov::SceneData* sceneData;
		pov::Camera* camera;

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
		{
		}
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
		{
		}
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
}
