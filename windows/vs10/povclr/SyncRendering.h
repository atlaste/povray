#pragma once

#include "Scene.h"

using namespace System;
using namespace System::Drawing;

namespace povclr
{
	public ref class SyncRendering
	{
	private:
		Scene ^scene;
		Image ^result;

	public:
		SyncRendering(Scene ^scene)
		{
			this->scene = scene;
			this->result = nullptr;
		}

		bool Rendering()
		{
			return false;
		}

		double PercentCompleted()
		{
			return result == nullptr ? 0 : 100;
		}

		Image ^CurrentStatus()
		{
			return result;
		}

		void Render()
		{
			// Initialize here so rendering will reflect the correct values:
			result = nullptr;

			auto callback = gcnew RenderCallback();
			scene->Render(callback);

			result = callback->Image;

			delete callback;
		}
	};
}