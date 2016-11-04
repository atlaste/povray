#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Settings
	{
	public:
		Settings(int width, int height) :
			WarningLevel(9),
			SplitUnions(false),
			RemoveBounds(true),
			BoundingMethod(0),
			OutputAlpha(false),
			BspMaxDepth(0),
			BspObjectISectCost(0.0f),
			BspBaseAccessCost(0.0f),
			BspChildAccessCost(0.0f),
			BspMissChance(0.0f),
			TracingMethod(0),
			JitterScale(1.0f),
			Jitter(false),
			AAThreshold(0.3f),
			AADepth(3),
			AAGammaValue(1.0f),
			Width(width),
			Height(height)
		{}

		int WarningLevel;
		bool SplitUnions;
		bool RemoveBounds;
		int BoundingMethod;
		bool OutputAlpha;
		int BspMaxDepth;
		float BspObjectISectCost;
		float BspBaseAccessCost;
		float BspChildAccessCost;
		float BspMissChance;

		int Quality = 9;

		int TracingMethod = 0;
		float JitterScale = 1.0f;
		bool Jitter = false;
		float AAThreshold = 0.3f;
		int AADepth = 3;
		float AAGammaValue = 1.0f;

		bool RadiosityEnabled = false;

		int Width;
		int Height;
	};
}