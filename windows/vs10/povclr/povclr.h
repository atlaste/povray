#pragma once

#include "core/scene/scenedata.h"
#include "vm/fnpovfpu.h"
#include "povms/povmsid.h"


using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class PovSettings
	{
	public:
		PovSettings() :
			WarningLevel(0),
			SplitUnions(false),
			RemoveBounds(true),
			BoundingMethod(2),
			OutputAlpha(false),
			BspMaxDepth(0),
			BspObjectISectCost(0.0f),
			BspBaseAccessCost(0.0f),
			BspChildAccessCost(0.0f),
			BspMissChance(0.0f)
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
	};

	public ref class Context : IDisposable
	{
	internal:
		Context(PovSettings^ settings)
		{
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

		~Context()
		{
			this->!Context();
		}

	protected:
		!Context()
		{
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

	public ref class Scene : public List<SceneObject^>
	{
	public:
		Scene()
		{
			Settings = gcnew PovSettings();
		}

		property PovSettings^ Settings;

		void Render()
		{
			Context ^renderContext = nullptr;
			try
			{
				renderContext = gcnew Context(Settings);

				for each(SceneObject^ item in this)
				{
					item->Render(renderContext);
				}
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
		Vector3(double v1, double v2, double v3) :
			X(v1), Y(v2), Z(v3)
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
			pov::Camera obj;
			obj.Location = Location->ToVector();
			obj.Look_At = LookAt->ToVector();
			// etc.

			context->sceneData->cameras.push_back(obj);
		}
	};

	public ref class LightSource : public SceneObject
	{
	public:
		LightSource(Vector3^ location, Vector3^ pointsAt) :
			Location(location), PointsAt(pointsAt)
		{}

		Vector3^ Location;
		Vector3^ PointsAt;

		virtual void Render(Context^ context) 
		{
			auto obj = new pov::LightSource();
			obj->Center = Location->ToVector();
			obj->Points_At = Location->ToVector();

			context->sceneData->lightSources.push_back(obj);
		}
	};
}
