#include "Stdafx.h"

#include "BlendMapBase.h"
#include "Color.h"
#include "Pigment.h"
#include "Normal.h"
#include "Texture.h"

#include "core/material/blendmap.h"

// TODO: Due to the gcroot solution that's used here (we cannot mix templates and generics...), I also had to use 
// MoveNext and Current. It's not ideal, especially not since Dispose is currently not called. The best way this is probably
// solved is to wrap it in an auto_gcroot, which calls the d'tor when the enumerator goes out of scope. Note that this solution
// will work fine: it's very unlikely that the enumerator will need a dispose call anyways, and 'delete e' calls it explicitly,
// so the only case when stuff will go wrong is when an exception occurs halfway in the process.

namespace povray
{
	namespace Materials
	{
		namespace Patterns
		{
			template <>
			std::shared_ptr<pov::ColourBlendMap> BlendMapBaseHelper::CreateBlendMap<pov::ColourBlendMap>(gcroot<Context^> context, pov::BlendMapTypeId type, gcroot<IEnumerable<IMapType^>^> components)
			{
				pov::ColourBlendMap::Vector result;
				int count = 0;

				auto e = components->GetEnumerator();
				while (e->MoveNext())
				{
					pov::TransColour col;

					pov::BlendMapEntry<pov::ColourBlendMapData> item;
					item.value = count++;

					safe_cast<RGBFT^>(e->Current)->RenderDetail(item.Vals);

					result.push_back(std::move(item));
				}
				delete e;

				auto blendmap = pov::Create_Blend_Map<pov::ColourBlendMap>(type);
				blendmap->Set(result);
				return blendmap;
			}

			template <>
			std::shared_ptr<pov::PigmentBlendMap> BlendMapBaseHelper::CreateBlendMap<pov::PigmentBlendMap>(gcroot<Context^> context, pov::BlendMapTypeId type, gcroot<IEnumerable<IMapType^>^> components)
			{
				pov::PigmentBlendMap::Vector result;
				int count = 0;

				auto e = components->GetEnumerator();
				while (e->MoveNext())
				{
					pov::BlendMapEntry<pov::PigmentBlendMapData> item;
					item.value = count++;
					item.Vals = nullptr;

					safe_cast<Pigment^>(e->Current)->Render(context, item.Vals);

					result.push_back(std::move(item));
				}
				delete e;

				auto blendmap = pov::Create_Blend_Map<pov::PigmentBlendMap>(type);
				blendmap->Set(result);
				return blendmap;
			}

			template <>
			std::shared_ptr<pov::SlopeBlendMap> BlendMapBaseHelper::CreateBlendMap<pov::SlopeBlendMap>(gcroot<Context^> context, pov::BlendMapTypeId type, gcroot<IEnumerable<IMapType^>^> components)
			{
				throw gcnew NotImplementedException();
			}

			template <>
			std::shared_ptr<pov::NormalBlendMap> BlendMapBaseHelper::CreateBlendMap<pov::NormalBlendMap>(gcroot<Context^> context, pov::BlendMapTypeId type, gcroot<IEnumerable<IMapType^>^> components)
			{
				pov::NormalBlendMap::Vector result;
				int count = 0;

				auto e = components->GetEnumerator();
				while (e->MoveNext())
				{
					pov::BlendMapEntry<pov::NormalBlendMapData> item;
					item.value = count++;
					item.Vals = nullptr;

					safe_cast<Normal^>(e->Current)->Render(context, item.Vals);

					result.push_back(std::move(item));
				}
				delete e;

				auto blendmap = pov::Create_Blend_Map<pov::NormalBlendMap>(type);
				blendmap->Set(result);
				return blendmap;
			}

			template <>
			std::shared_ptr<pov::TextureBlendMap> BlendMapBaseHelper::CreateBlendMap<pov::TextureBlendMap>(gcroot<Context^> context, pov::BlendMapTypeId type, gcroot<IEnumerable<IMapType^>^> components)
			{
				pov::TextureBlendMap::Vector result;
				int count = 0;

				auto e = components->GetEnumerator();
				while (e->MoveNext())
				{
					pov::BlendMapEntry<pov::TexturePtr> item;
					item.value = count++;
					item.Vals = nullptr;

					safe_cast<Texture^>(e->Current)->RenderDetail(context, item.Vals);

					result.push_back(std::move(item));
				}
				delete e;

				auto blendmap = pov::Create_Blend_Map<pov::TextureBlendMap>(type);
				blendmap->Set(result);
				return blendmap;
			}
		}
	}
}