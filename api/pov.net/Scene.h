#pragma once

#include "SceneObject.h"
#include "Context.h"
#include "Settings.h"
#include "RenderTarget.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
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

		Scene(povray::Settings^ settings)
		{
			Settings = settings;
		}

		Scene()
		{
			Settings = gcnew povray::Settings(320,200);
		}

		property povray::Settings^ Settings;

		void Render(RenderTarget^ target);
	};
}