#pragma once

#include "Settings.h"

#include "core/scene/scenedata.h"
#include "core/material/pigment.h"
#include "vm/fnpovfpu.h"
#include "povms/povmsid.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
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

			// Initialize atmosphere:
			sceneData->atmosphereIOR = 1.0;
			sceneData->atmosphereDispersion = 1.0;
			sceneData->fog = NULL;
			sceneData->rainbow = NULL;
			sceneData->skysphere = NULL;

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

		// Messages:
		List<String^> ^ warnings = gcnew List<String^>();

		void Warning(const char* str)
		{
			// TODO FIXME: Use something better than this like events in the settings.
			warnings->Add(gcnew String(str));
		}

		void Error(const char* str)
		{
			// TODO FIXME: Use something better than this like events in the settings.
			warnings->Add(gcnew String(str));
			throw gcnew Exception(gcnew String(str));
		}

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
}