#pragma once

#include "Context.h"
#include "Pigment.h"
#include "Normal.h"
#include "Finish.h"

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
				Pigment->RenderDetail(context, texture->Pigment);
			}
		}

	public:
		Texture ^Next;
		Pigment ^Pigment;
		Normal ^Tnormal;
		Finish ^Finish;
		List<Texture^> ^Materials;
	};
}