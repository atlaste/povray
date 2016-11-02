#include "Stdafx.h"
#include "CSGObject.h"

namespace povclr
{
	void CSGObject::RenderDetail(Context^ context, pov::ObjectBase* obj)
	{
		// Add the properties and flags to the object:

		// parser.cpp:7626 and further contains the original code:
		if (UVMapping)
		{
			Set_Flag(obj, UV_FLAG);
		}

		if (PhotonDensity > 0.0)
		{
			Set_Flag(obj, PH_TARGET_FLAG);
			obj->Ph_Density = PhotonDensity;
			CheckPassThru(context, obj, PH_RFR_ON_FLAG);
		}
		else
		{
			Clear_Flag(obj, PH_TARGET_FLAG);
		}

		if (!double::IsNaN(Refraction))
		{
			if (Refraction > 0.0)
			{
				Set_Flag(obj, PH_RFR_ON_FLAG);
				Clear_Flag(obj, PH_RFR_OFF_FLAG);
				CheckPassThru(context, obj, PH_RFR_ON_FLAG);
			}
			else
			{
				Clear_Flag(obj, PH_RFR_ON_FLAG);
				Set_Flag(obj, PH_RFR_OFF_FLAG);
			}
		}

		if (!double::IsNaN(Reflection))
		{
			if (Reflection > 0.0)
			{
				Set_Flag(obj, PH_RFL_ON_FLAG);
				Clear_Flag(obj, PH_RFL_OFF_FLAG);
			}
			else
			{
				Clear_Flag(obj, PH_RFL_ON_FLAG);
				Set_Flag(obj, PH_RFL_OFF_FLAG);
			}
		}

		if (PassThrough > 0.0)
		{
			Set_Flag(obj, PH_PASSTHRU_FLAG);
			CheckPassThru(context, obj, PH_PASSTHRU_FLAG);
		}
		else
		{
			Clear_Flag(obj, PH_PASSTHRU_FLAG);
		}

		// TODO FIXME: Transformations...

		if (Surface)
		{
			obj->Type |= TEXTURED_OBJECT;
			obj->Texture = pov::Copy_Textures(context->defaultTexture);
			Surface->RenderDetail(context, obj->Texture);
		}

		if (InteriorTexture)
		{
			obj->Type |= TEXTURED_OBJECT;
			obj->Interior_Texture = pov::Copy_Textures(context->defaultTexture);
			InteriorTexture->RenderDetail(context, obj->Interior_Texture);
		}

		if (Inverse)
		{
			if (obj->Type & PATCH_OBJECT)
			{
				context->Warning("Cannot invert a patch object.");
			}

			obj->Invert();
		}

		// ... and so on (TODO FIXME parser.cpp:7890 and the rest...)

		// Bounding box calculations:
		if (!obj->Bound.empty())
		{
			// Get bounding objects bounding box.

			auto min = pov::Vector3d(-BOUND_HUGE);
			auto max = pov::Vector3d(BOUND_HUGE);

			for (std::vector<pov::ObjectPtr>::iterator Sib = obj->Bound.begin();
				 Sib != obj->Bound.end();
				 Sib++)
			{
				if (!Test_Flag((*Sib), INVERTED_FLAG))
				{
					min[0] = std::max(min[0], double((*Sib)->BBox.lowerLeft[0]));
					min[1] = std::max(min[1], double((*Sib)->BBox.lowerLeft[1]));
					min[2] = std::max(min[2], double((*Sib)->BBox.lowerLeft[2]));
					max[0] = std::min(max[0], double((*Sib)->BBox.lowerLeft[0] + (*Sib)->BBox.size[0]));
					max[1] = std::min(max[1], double((*Sib)->BBox.lowerLeft[1] + (*Sib)->BBox.size[1]));
					max[2] = std::min(max[2], double((*Sib)->BBox.lowerLeft[2] + (*Sib)->BBox.size[2]));
				}
			}

			pov::BoundingBox BBox;
			BBox.lowerLeft = pov::BBoxVector3d(min);
			BBox.size = pov::BBoxVector3d(max - min);

			// Update bounding box if necessary.
			//
			// TODO - Area is probably a better measure to decide which box is better.
			// TODO - Doesn't this mechanism prevent users from reliably overriding broken default boxes?
			auto v1 = BBox.size[0] * BBox.size[1] * BBox.size[2];
			auto v2 = obj->BBox.size[0] * obj->BBox.size[1] * obj->BBox.size[2];

			if (v1 < v2)
			{
				obj->BBox = BBox;
			}
		}

		// Assign clipping objects' bounding box to object if object's bounding box is larger.

		if (!obj->Clip.empty())
		{
			/* Get clipping objects bounding box. */

			auto min = pov::Vector3d(-BOUND_HUGE);
			auto max = pov::Vector3d(BOUND_HUGE);

			for (std::vector<pov::ObjectPtr>::iterator Sib = obj->Clip.begin();
				 Sib != obj->Clip.end();
				 Sib++)
			{
				if (!Test_Flag((*Sib), INVERTED_FLAG))
				{
					min[0] = std::max(min[0], double((*Sib)->BBox.lowerLeft[0]));
					min[1] = std::max(min[1], double((*Sib)->BBox.lowerLeft[1]));
					min[2] = std::max(min[2], double((*Sib)->BBox.lowerLeft[2]));
					max[0] = std::min(max[0], double((*Sib)->BBox.lowerLeft[0] + (*Sib)->BBox.size[0]));
					max[1] = std::min(max[1], double((*Sib)->BBox.lowerLeft[1] + (*Sib)->BBox.size[1]));
					max[2] = std::min(max[2], double((*Sib)->BBox.lowerLeft[2] + (*Sib)->BBox.size[2]));
				}
			}

			pov::BoundingBox BBox;
			BBox.lowerLeft = pov::BBoxVector3d(min);
			BBox.size = pov::BBoxVector3d(max - min);

			/* Get bounding boxes' volumes. */

			// TODO - Area is probably a better measure to decide which box is better.
			auto v1 = BBox.size[0] * BBox.size[1] * BBox.size[2];
			auto v2 = obj->BBox.size[0] * obj->BBox.size[1] * obj->BBox.size[2];

			if (v1 < v2)
			{
				obj->BBox = BBox;
			}
		}

		if ((obj->Texture == NULL) && (obj->Interior_Texture != NULL))
		{
			context->Error("Interior texture requires an exterior texture.");
		}

		// Scene post-processing and linking:

		SceneObject::PostProcess(context, obj, nullptr);
		SceneObject::LinkToFrame(context, obj);
	}
}