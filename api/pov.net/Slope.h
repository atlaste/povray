#pragma once

#include "Context.h"
#include "Pattern.h"
#include "IMapType.h"
#include "Pigment.h"
#include "Normal.h"
#include "Texture.h"
#include "Math.h"

#include "core/material/texture.h"
#include "core/material/pigment.h"
#include "core/material/pattern.h"
#include "core/material/normal.h"

namespace povclr
{
	public ref class Slope : public Pattern, public ITargetType<Pigment^>, public ITargetType<Normal^>, public ITargetType<Texture^>
	{
	internal:
		template <typename MapType, typename PatternType>
		void Render(Context^ context, PatternType* container)
		{
			container->Type = pov::RIPPLES_PATTERN;
			auto pat = new pov::SlopePattern();
			container->pattern = pov::PatternPtr(pat);

			pat->slopeDirection = Direction.ToVector();
			pat->slopeModLow = LowSlope;
			pat->slopeModWidth = HighSlope - LowSlope;
			pat->altitudeDirection = Altitude.ToVector();
			pat->altitudeModLow = LowAltitude;
			pat->altitudeModWidth = HighAltitude - LowAltitude;

			pat->slopeLen = pat->slopeDirection.length();
			pat->altitudeLen = pat->altitudeDirection.length();

			if (pat->slopeLen != 0.0) { pat->slopeDirection.normalize(); }
			if (pat->altitudeLen != 0.0) { pat->altitudeDirection.normalize(); }

			auto totalLen = pat->slopeLen + pat->altitudeLen;

			if (totalLen != 0.0)
			{
				pat->slopeLen /= totalLen;
				pat->altitudeLen = 1.0 - pat->slopeLen;
			}
			else
			{
				context->Error("Zero length for both slope and gradient.");
			}

			//  Pre-process some stuff (look for shortcuts) for rendering speed
			pat->slopeAxis = pat->altitudeAxis = 0; 
			if (!pat->pointAt)
			{
				for (int i = 0; i <= 2; i++)
				{
					if (pat->slopeDirection[i] == 1.0) { pat->slopeAxis = i + 1;  break; }
					else if (pat->slopeDirection[i] == -1.0) { pat->slopeAxis = -(i + 1); break; }
				}
				for (int i = 0; i <= 2; i++)
				{
					if (pat->altitudeDirection[i] == 1.0) { pat->altitudeAxis = i + 1;  break; }
					else if (pat->altitudeDirection[i] == -1.0) { pat->altitudeAxis = -(i + 1); break; }
				}
			}
		}

		IMPLEMENT_DEFAULT_PATTERN_CODE

	public:
		Slope(Vector3 direction) :
			Direction(direction),
			HighSlope(0),
			LowSlope(1),
			Altitude(Vector3(0,0,0)),
			LowAltitude(0),
			HighAltitude(0)
		{}

		Slope(Vector3 direction, float highSlope, float lowSlope) :
			Direction(direction),
			HighSlope(highSlope),
			LowSlope(lowSlope),
			Altitude(Vector3(0, 0, 0)),
			LowAltitude(0),
			HighAltitude(0)
		{}

		Slope(Vector3 direction, float highSlope, float lowSlope, Vector3 altitude, float lowAltitude, float highAltitude) :
			Direction(direction),
			HighSlope(highSlope),
			LowSlope(lowSlope),
			Altitude(altitude),
			LowAltitude(lowAltitude),
			HighAltitude(highAltitude)
		{}

		Vector3 Direction;
		float HighSlope;
		float LowSlope;
		Vector3 Altitude;
		float LowAltitude;
		float HighAltitude;
	};
}