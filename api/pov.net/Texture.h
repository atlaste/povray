#pragma once

#include "Context.h"
#include "Pigment.h"
#include "Normal.h"
#include "Finish.h"
#include "FinishBuilder.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"

namespace povray
{
	namespace Materials
	{
		public ref class Texture : public IMapType
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

					Pigment->Render(context, texture->Pigment);
				}

				// TODO FIXME: TNormal is also a pattern
				// TODO FIXME: Next?
				// TODO FIXME: Materials

				// Apply finish:
				if (finish != nullptr)
				{
					for each (povray::Materials::Finish^ fin in finish)
					{
						fin->Render(texture->Finish);
					}
				}
			}

			FinishBuilder^ finish;

		public:
			Texture() :
				finish(nullptr)
			{}

			Texture ^Next;
			Pigment ^Pigment;
			Normal ^TNormal;

			property FinishBuilder ^Finish
			{
				FinishBuilder ^get()
				{
					if (finish == nullptr) { finish = gcnew FinishBuilder(); }
					return finish;
				}

				void set(FinishBuilder^ value)
				{
					finish = value;
				}
			}

			List<Texture^> ^Materials;
		};
	}
}