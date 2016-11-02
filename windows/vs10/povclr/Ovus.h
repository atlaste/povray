#pragma once

#include "Context.h"
#include "Settings.h"
#include "CSGObject.h"
#include "Math.h"

#include "core/shape/ovus.h"
#include "core/shape/sphere.h"
#include "base/messenger.h"
#include "base/textstream.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povclr
{
	public ref class Ovus : CSGObject
	{
	public:
		double TopRadius;
		double BottomRadius;

		Ovus(double topRadius, double bottomRadius) : 
			TopRadius(topRadius),
			BottomRadius(bottomRadius)
		{}

		virtual void Render(Context^ context) override
		{
			auto obj = new pov::Ovus();
			
			if ((obj->TopRadius < 0) || (obj->BottomRadius < 0))
			{
				context->Error("Both Ovus radii must be positive");
			}

			if (obj->TopRadius < 2.0 * obj->BottomRadius)
			{
				if (obj->BottomRadius > obj->TopRadius)
				{
					obj->ConnectingRadius = 2.0 * obj->BottomRadius;
					obj->VerticalPosition = 2.0 * obj->TopRadius - obj->BottomRadius - (obj->TopRadius * obj->TopRadius / (2.0 * obj->BottomRadius));
					obj->HorizontalPosition = sqrt((obj->BottomRadius)*(obj->BottomRadius) - (obj->VerticalPosition)*(obj->VerticalPosition));
					obj->BottomVertical = -obj->VerticalPosition;
					auto distance = obj->ConnectingRadius - obj->TopRadius;
					obj->TopVertical = ((obj->BottomRadius - obj->VerticalPosition) * obj->TopRadius / distance) + obj->BottomRadius;
				}
				else
				{
					obj->ConnectingRadius = 2.0 * obj->TopRadius;
					obj->VerticalPosition = -2.0 * obj->TopRadius + obj->BottomRadius + (1.5 * obj->TopRadius * obj->TopRadius / obj->BottomRadius);
					obj->HorizontalPosition = sqrt((obj->TopRadius)*(obj->TopRadius) - ((obj->VerticalPosition - obj->BottomRadius) * (obj->VerticalPosition - obj->BottomRadius)));
					obj->TopVertical = 2.0 * obj->BottomRadius - obj->VerticalPosition;
					auto distance = obj->ConnectingRadius - obj->BottomRadius;
					obj->BottomVertical = -obj->VerticalPosition * obj->BottomRadius / distance;
				}

				obj->Compute_BBox();
			}
			else
			{
				context->Warning("Ovus second radius should be less than twice first radius\nSubstituing a sphere to ovus as it would be identical & simpler");

				pov::Sphere *replacement = new pov::Sphere();
				replacement->Center[0] = 0;
				replacement->Center[1] = obj->BottomRadius;
				replacement->Center[2] = 0;
				replacement->Radius = obj->TopRadius;
				delete obj;

				replacement->Compute_BBox();
			}

			CSGObject::RenderDetail(context, obj);
		}
	};
}