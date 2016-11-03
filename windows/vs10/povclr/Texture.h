#pragma once

#include "Context.h"
#include "Pigment.h"
#include "Normal.h"
#include "Finish.h"
#include "FinishBuilder.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"

namespace povclr
{
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

				Pigment->RenderPigmentBlendMap(context, texture->Pigment);
			}
			
			// TODO FIXME: TNormal is also a pattern
			// TODO FIXME: Next?
			// TODO FIXME: Materials

			// Apply finish:
			if (finish != nullptr)
			{
				for each (povclr::Finish^ fin in finish)
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
		Pattern ^Pigment;
		Normal ^TNormal;

		property FinishBuilder ^Finish
		{
			FinishBuilder ^get()
			{
				if (finish == nullptr) { finish = gcnew FinishBuilder(); }
				return finish;
			}
		}

		List<Texture^> ^Materials;
	};
}