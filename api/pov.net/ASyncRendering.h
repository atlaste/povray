#pragma once

#include "RenderCallback.h"
#include "Scene.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Threading;

namespace povclr
{
	public ref class ASyncRendering
	{
	private:
		RenderCallback^ callback;
		Scene^ scene;
		Image^ result;

	public:
		ASyncRendering(Scene^ scene)
		{
			this->scene = scene;
			this->result = nullptr;
		}

		bool Rendering()
		{
			return callback != nullptr;
		}

		float PercentCompleted()
		{
			Thread::MemoryBarrier();
			RenderCallback ^tmp = callback;
			if (tmp == nullptr) { return 100.0; }
			else if (tmp->NumberPixels == 0) { return 0; }
			else { return 100.0 * (float)tmp->PixelsProcessed / (float)tmp->NumberPixels; }
		}

		Image ^CurrentStatus()
		{
			Thread::MemoryBarrier();
			RenderCallback ^callback = this->callback;
			return callback == nullptr ? result : callback->Image;
		}

		void RenderASync()
		{
			scene->Render(callback);

			result = callback->Image;

			delete callback;
			callback = nullptr;

			Thread::MemoryBarrier();
		}

		void Render()
		{
			// Initialize here so rendering will reflect the correct values:
			this->result = nullptr;

			callback = gcnew RenderCallback();
			Thread::MemoryBarrier();

			// Start the rendering thread:
			Thread ^t = gcnew Thread(gcnew ThreadStart(this, &ASyncRendering::RenderASync));
			t->IsBackground = true;
			t->Name = "POV-Ray render thread";
			t->Priority = ThreadPriority::Normal;

			t->Start();
		}
	};
}