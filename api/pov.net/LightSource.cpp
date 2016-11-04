#include "Stdafx.h"
#include "LightSource.h"

namespace povclr
{
	void LightSource::Render(Context^ context)
	{
		auto obj = new pov::LightSource();
		obj->Center = Location.ToVector();
		Color.RenderDetail(obj->colour);

		// [SdB] Most of this code is actually from the parser... only god knows why logic about objects ended up there...

		obj->Direction = obj->Points_At - obj->Center;
		auto len = obj->Direction.length();
		if (len > EPSILON)
		{
			obj->Direction /= len;
		}

		if (obj->Circular)
		{
			if ((obj->Area_Size1 <= 1) || (obj->Area_Size2 <= 1))
			{
				throw gcnew Exception("Circular area light must have more than 1 point per axis");
			}
		}

		// check some properties of the orient light sources
		if (obj->Orient)
		{
			if (!obj->Circular)
			{
				obj->Circular = true;
				context->Warning("Orient can only be used with circular area lights. This area light is now circular.");
			}

			auto len1 = obj->Axis1.length();
			auto len2 = obj->Axis2.length();

			if (fabs(len1 - len2) > EPSILON)
			{
				context->Warning("When using orient, the two axes of the area light must be of equal length.\nOnly the length of the first axis will be used.");

				// the equalization is actually done in the lighting code, since only the length of
				// Axis1 will be used

			}

			if (obj->Area_Size1 != obj->Area_Size2)
			{
				context->Warning("When using orient, the two sample sizes for the area light should be equal.");
			}
		}

		// Make sure that circular light sources are larger than 1 by x [ENB 9/97]
		if (obj->Circular)
		{
			if ((obj->Area_Size1 <= 1) || (obj->Area_Size2 <= 1))
			{
				context->Warning("Circular area lights must have more than one sample along each axis.");
				obj->Circular = false;
			}
		}

		RenderTransformations(context, obj);

		SceneObject::PostProcess(context, obj, nullptr);
		SceneObject::LinkToFrame(context, obj);
	}
}