#include "Stdafx.h"

#include "BlendMapBase.h"
#include "Color.h"
#include "Pigment.h"
#include "Normal.h"
#include "Texture.h"

#include "core/material/blendmap.h"

namespace povclr
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

		auto blendmap = pov::Create_Blend_Map<pov::TextureBlendMap>(type);
		blendmap->Set(result);
		return blendmap;
	}
}