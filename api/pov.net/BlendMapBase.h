#pragma once

#include "Context.h"
#include "IMapType.h"

using namespace System::Collections::Generic;

namespace povray
{
	struct BlendMapBaseHelper
	{
		template <typename MapType>
		static std::shared_ptr<MapType> CreateBlendMap(gcroot<Context^> context, pov::BlendMapTypeId type, gcroot<IEnumerable<IMapType^>^> components);
	};

	public ref class BlendMapBase
	{
	internal:
		template <typename MapType>
		static std::shared_ptr<MapType> CreateBlendMap(Context^ context, pov::BlendMapTypeId type, IEnumerable<IMapType^>^ components)
		{
			return BlendMapBaseHelper::CreateBlendMap<MapType>(context, type, components);
		}
		
		template <typename MapType>
		static std::shared_ptr<MapType> CreateBlendMap(Context^ context, pov::BlendMapTypeId type, ... array<IMapType^>^ components)
		{
			return BlendMapBaseHelper::CreateBlendMap<MapType>(context, type, (IEnumerable<IMapType^>^)components);
		}
	};
}