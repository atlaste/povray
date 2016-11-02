#pragma once

#include "SceneObject.h"
#include "Context.h"
#include "Settings.h"
#include "RenderTarget.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Scene : public List<SceneObject^>
	{
	private:
		void InitializeSettings(std::shared_ptr<pov::SceneData>& sceneData);
		void InitializePhotons(std::shared_ptr<pov::SceneData>& sceneData);
		void InitializeRadiosity(std::shared_ptr<pov::SceneData>& sceneData);
		void InitializeBounding(std::shared_ptr<pov::SceneData>& sceneData);
		void InitializeIndices(std::shared_ptr<pov::SceneData>& sceneData);
		void InitializeScene(std::shared_ptr<pov::SceneData>& sceneData);

	public:
		static Scene()
		{
			pov::Initialize_Noise();
			pov::InitializePatternGenerators();
		}

		Scene(povclr::Settings^ settings)
		{
			Settings = settings;
		}

		Scene()
		{
			Settings = gcnew povclr::Settings(320,200);
		}

		property povclr::Settings^ Settings;

		void Render(RenderTarget^ target);
	};
}