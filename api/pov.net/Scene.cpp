#include "stdafx.h"
#include "Scene.h"

#include "core/render/tracepixel.h"
#include "core/scene/tracethreaddata.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	// Some helper classes we need here (but shouldn't be exposed to the outside world)

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
			return static_cast<unsigned int>(finite.size());
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

	// Scene initialization code:

	void Scene::InitializeSettings(std::shared_ptr<pov::SceneData>& sceneData)
	{
		auto quality = pov::QualityFlags(Settings->Quality);

		sceneData->radiositySettings.vainPretrace = true;
		sceneData->radiositySettings.radiosityEnabled = Settings->RadiosityEnabled && quality.radiosity;

		sceneData->photonSettings.photonsEnabled &= quality.photons;
		sceneData->useSubsurface &= quality.subsurface;

		// Fix camera? view.cpp:890
	}

	void Scene::InitializePhotons(std::shared_ptr<pov::SceneData>& sceneData)
	{
		// Basically the code we need here is found in:
		//
		// PhotonEstimationTask
		// PhotonStrategyTask
		// PhotonShootingTask
		// PhotonSortingTask

		throw gcnew NotImplementedException();
	}

	void Scene::InitializeRadiosity(std::shared_ptr<pov::SceneData>& sceneData)
	{
		// Basically we need the code from view.cpp:1014 and the RadiosityTask.

		throw gcnew NotImplementedException();
	}

	void Scene::InitializeBounding(std::shared_ptr<pov::SceneData>& sceneData)
	{
		// Initialize BSP tree:

		// - Split into finite and infinite objects
		BSPSceneObjects objects(sceneData->objects);

		// - Rebuild object list
		BSPProgressFunctor progress;
		sceneData->objects.clear();
		sceneData->objects.insert(sceneData->objects.end(), objects.finite.begin(), objects.finite.end());
		sceneData->objects.insert(sceneData->objects.end(), objects.infinite.begin(), objects.infinite.end());
		sceneData->numberOfFiniteObjects = static_cast<unsigned int>(objects.finite.size());
		sceneData->numberOfInfiniteObjects = static_cast<unsigned int>(objects.infinite.size()) - objects.numLights;
		sceneData->tree = new pov::BSPTree(sceneData->bspMaxDepth, sceneData->bspObjectIsectCost, sceneData->bspBaseAccessCost, sceneData->bspChildAccessCost, sceneData->bspMissChance);
		sceneData->tree->build(progress, objects,
							   sceneData->nodes, sceneData->splitNodes, sceneData->objectNodes, sceneData->emptyNodes,
							   sceneData->maxObjects, sceneData->averageObjects, sceneData->maxDepth, sceneData->averageDepth,
							   sceneData->aborts, sceneData->averageAborts, sceneData->averageAbortObjects, sceneData->inputFile);
	}

	void Scene::InitializeIndices(std::shared_ptr<pov::SceneData>& sceneData)
	{
		// post process atmospheric media
		for (vector<pov::Media>::iterator i(sceneData->atmosphere.begin()); i != sceneData->atmosphere.end(); i++)
		{
			i->PostProcess();
		}

		// post process global light sources
		for (size_t i = 0; i < sceneData->lightSources.size(); i++)
		{
			sceneData->lightSources[i]->index = i;
			sceneData->lightSources[i]->lightGroupLight = false;
		}

		// post process local light sources
		for (size_t i = 0; i < sceneData->lightGroupLightSources.size(); i++)
		{
			sceneData->lightGroupLightSources[i]->index = i;
			sceneData->lightGroupLightSources[i]->lightGroupLight = true;
		}
	}

	void Scene::InitializeScene(std::shared_ptr<pov::SceneData>& sceneData)
	{
		InitializeIndices(sceneData);
		InitializeBounding(sceneData);
		InitializeSettings(sceneData);

		if (sceneData->photonSettings.photonsEnabled)
		{
			InitializePhotons(sceneData);
		}

		if (sceneData->radiositySettings.radiosityEnabled)
		{
			InitializeRadiosity(sceneData);
		}
	}

	void Scene::Render(RenderTarget^ target)
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

			InitializeScene(sceneData);

			// Initialize the ray-tracer:
			pov::QualityFlags quality(Settings->Quality);
			pov::TraceThreadData threadData(sceneData);

			CooperateFunctor cooperate;

			pov::PhotonGatherer photonGatherer(&sceneData->mediaPhotonMap, sceneData->photonSettings);
			pov::MediaFunction media(&threadData, nullptr, &photonGatherer);

			pov::RadiosityCache radiosityCache(sceneData->radiositySettings);
			pov::RadiosityFunction radiosity(sceneData, &threadData,
											 sceneData->radiositySettings, radiosityCache, cooperate, true, renderContext->camera->Location);

			// Initialize the tracer
			pov::TracePixel trace(sceneData, renderContext->camera, &threadData,
								  sceneData->parsedMaxTraceLevel, sceneData->parsedAdcBailout, quality,
								  cooperate, media, radiosity);

			media.BindTrace(&trace);

			// If we're using AA, we need to do initialize the gamma curve:
			pov::GammaCurvePtr aaGammaCurve;
			if (Settings->AAGammaValue > 0)
			{
				aaGammaCurve = pov::PowerLawGammaCurve::GetByDecodingGamma(Settings->AAGammaValue);
			}
			if (sceneData->workingGamma)
			{
				aaGammaCurve = pov::TranscodingGammaCurve::Get(sceneData->workingGamma, aaGammaCurve);
			}

			// Render the image:
			int width = Settings->Width;
			int height = Settings->Height;

			target->Initialize(width, height);

			for (unsigned int y = 0; y < static_cast<unsigned int>(height); ++y)
			{
				for (unsigned int x = 0; x < static_cast<unsigned int>(width); ++x)
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

}