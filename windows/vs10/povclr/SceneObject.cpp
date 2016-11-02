#include "Stdafx.h"
#include "SceneObject.h"

namespace povclr
{
	void SceneObject::CheckPassThru(Context^ context, pov::ObjectPtr o, int flag)
	{
		if (Test_Flag(o, PH_PASSTHRU_FLAG) &&
			Test_Flag(o, PH_TARGET_FLAG) &&
			!Test_Flag(o, PH_RFR_OFF_FLAG))
		{
			switch (flag)
			{
				case PH_PASSTHRU_FLAG:
					context->Warning("Cannot use pass_through with refraction & target.\nTurning off refraction.");
					Set_Flag(o, PH_RFR_OFF_FLAG);
					Clear_Flag(o, PH_RFR_ON_FLAG);
					break;

				case PH_TARGET_FLAG:
					if (Test_Flag(o, PH_RFR_ON_FLAG))
					{
						context->Warning("Cannot use pass_through with refraction & target.\nTurning off pass_through.");
						Clear_Flag(o, PH_PASSTHRU_FLAG);
					}
					else
					{
						context->Warning("Cannot use pass_through with refraction & target.\nTurning off refraction.");
						Set_Flag(o, PH_RFR_OFF_FLAG);
						Clear_Flag(o, PH_RFR_ON_FLAG);
					}
					break;

				case PH_RFR_ON_FLAG:
					context->Warning("Cannot use pass_through with refraction & target.\nTurning off pass_through.");
					Clear_Flag(o, PH_PASSTHRU_FLAG);
					break;
			}
		}
	}

	void SceneObject::PostProcess(Context^ context, pov::ObjectBase* obj, pov::ObjectBase* parent)
	{
		if (obj == NULL)
		{
			return;
		}

		if (obj->Type & LT_SRC_UNION_OBJECT)
		{
			for (vector<pov::ObjectPtr>::iterator Sib = (dynamic_cast<pov::CSG *>(obj))->children.begin();
				 Sib != (dynamic_cast<pov::CSG *>(obj))->children.end();
				 Sib++)
			{
				PostProcess(context, *Sib, obj);
			}
			return;
		}

		// Promote texture etc. from parent to children.

		if (parent != NULL)
		{
			if (obj->Texture == NULL)
			{
				obj->Texture = pov::Copy_Texture_Pointer(parent->Texture);
				// NK 1998 copy uv_mapping flag if and only if we copy the texture
				if (Test_Flag(parent, UV_FLAG))
					Set_Flag(obj, UV_FLAG);
			}
			if (obj->Interior_Texture == NULL)
			{
				obj->Interior_Texture = pov::Copy_Texture_Pointer(parent->Interior_Texture);
				if (Test_Flag(parent, UV_FLAG))
					Set_Flag(obj, UV_FLAG);
			}
			if (obj->interior == NULL)
			{
				obj->interior = parent->interior;
			}

			if (Test_Flag(parent, NO_REFLECTION_FLAG))
			{
				Set_Flag(obj, NO_REFLECTION_FLAG);
			}
			if (Test_Flag(parent, NO_RADIOSITY_FLAG))
			{
				Set_Flag(obj, NO_RADIOSITY_FLAG);
			}
			if (Test_Flag(parent, NO_IMAGE_FLAG))
			{
				Set_Flag(obj, NO_IMAGE_FLAG);
			}
			if (Test_Flag(parent, NO_SHADOW_FLAG))
			{
				Set_Flag(obj, NO_SHADOW_FLAG);
			}
			if (Test_Flag(parent, CUTAWAY_TEXTURES_FLAG))
			{
				Set_Flag(obj, CUTAWAY_TEXTURES_FLAG);
			}

			// NK phmap
			// promote photon mapping flags to child
			if (Test_Flag(parent, PH_TARGET_FLAG))
			{
				Set_Flag(obj, PH_TARGET_FLAG);
				obj->Ph_Density = parent->Ph_Density;
				CheckPassThru(context, obj, PH_TARGET_FLAG);
			}

			// promote object-specific radiosity settings to child
			if (!obj->RadiosityImportance.isSet())
				obj->RadiosityImportance = parent->RadiosityImportance(context->sceneData->radiositySettings.defaultImportance);

			if (Test_Flag(parent, PH_PASSTHRU_FLAG))
			{
				Set_Flag(obj, PH_PASSTHRU_FLAG);
				CheckPassThru(context, obj, PH_PASSTHRU_FLAG);
			}

			if (Test_Flag(parent, PH_RFL_ON_FLAG))
			{
				Set_Flag(obj, PH_RFL_ON_FLAG);
				Clear_Flag(obj, PH_RFL_OFF_FLAG);
			}
			else if (Test_Flag(parent, PH_RFL_OFF_FLAG))
			{
				Set_Flag(obj, PH_RFL_OFF_FLAG);
				Clear_Flag(obj, PH_RFL_ON_FLAG);
			}

			if (Test_Flag(parent, PH_RFR_ON_FLAG))
			{
				Set_Flag(obj, PH_RFR_ON_FLAG);
				Clear_Flag(obj, PH_RFR_OFF_FLAG);
				CheckPassThru(context, obj, PH_RFR_ON_FLAG);
			}
			else if (Test_Flag(parent, PH_RFR_OFF_FLAG))
			{
				Set_Flag(obj, PH_RFR_OFF_FLAG);
				Clear_Flag(obj, PH_RFR_ON_FLAG);
			}

			if (Test_Flag(parent, PH_IGNORE_PHOTONS_FLAG))
			{
				Set_Flag(obj, PH_IGNORE_PHOTONS_FLAG);
			}
		}

		if (obj->interior != NULL)
			obj->interior->PostProcess();

		if ((obj->Texture == NULL) &&
			!(obj->Type & TEXTURED_OBJECT) &&
			!(obj->Type & LIGHT_SOURCE_OBJECT))
		{
			if (parent)
			{
				if ((dynamic_cast<pov::CSGIntersection *>(parent) == NULL) ||
					!Test_Flag(parent, CUTAWAY_TEXTURES_FLAG))
				{
					obj->Texture = pov::Copy_Textures(context->defaultTexture);
				}
			}
			else
				obj->Texture = pov::Copy_Textures(context->defaultTexture);
		}

		if (!(obj->Type & LIGHT_GROUP_OBJECT) &&
			!(obj->Type & LIGHT_GROUP_LIGHT_OBJECT))
		{
			Post_Textures(obj->Texture);  //moved cey 6/97

			if (obj->Interior_Texture)
			{
				Post_Textures(obj->Interior_Texture);
			}
		}

		if (obj->Type & LIGHT_SOURCE_OBJECT)
		{
			DBL len1, len2;
			pov::LightSource *Light = dynamic_cast<pov::LightSource *>(obj);

			// check some properties of the orient light sources
			if (Light->Orient)
			{
				if (!Light->Circular)
				{
					Light->Circular = true;
					context->Warning("Orient can only be used with circular area lights. This area light is now circular.");
				}

				len1 = Light->Axis1.length();
				len2 = Light->Axis2.length();

				if (fabs(len1 - len2) > EPSILON)
				{
					context->Warning("When using orient, the two axes of the area light must be of equal length.\nOnly the length of the first axis will be used.");

					// the equalization is actually done in the lighting code, since only the length of
					// Axis1 will be used

				}

				if (Light->Area_Size1 != Light->Area_Size2)
				{
					context->Warning("When using orient, the two sample sizes for the area light should be equal.");
				}
			}

			// Make sure that circular light sources are larger than 1 by x [ENB 9/97]
			if (Light->Circular)
			{
				if ((Light->Area_Size1 <= 1) || (Light->Area_Size2 <= 1))
				{
					context->Warning("Circular area lights must have more than one sample along each axis.");
					Light->Circular = false;
				}
			}
		}

		if (obj->Type & LIGHT_SOURCE_OBJECT)
		{
			// post-process the light source
			auto light = dynamic_cast<pov::LightSource*>(obj);
			if (light->Projected_Through_Object != NULL)
			{
				if (light->Projected_Through_Object->interior != NULL)
				{
					light->Projected_Through_Object->interior.reset();
					context->Warning("Projected through objects can not have interior, interior removed.");
				}
				if (light->Projected_Through_Object->Texture != NULL)
				{
					Destroy_Textures(light->Projected_Through_Object->Texture);
					light->Projected_Through_Object->Texture = NULL;
					context->Warning("Projected through objects can not have texture, texture removed.");
				}
			}

			// only global light sources are in Frame.Light_Sources list [trf]
			if (!(obj->Type & LIGHT_GROUP_LIGHT_OBJECT))
			{
				// add this light to the frame's list of global light sources
				context->sceneData->lightSources.push_back(reinterpret_cast<pov::LightSource *>(obj));
			}
			else
			{
				// Put it into the frame's list of light-group lights
				context->sceneData->lightGroupLightSources.push_back(reinterpret_cast<pov::LightSource *>(obj));
			}
		}
		else
		{
			// post-process the object

			// If there is no interior create one.

			if (obj->interior == NULL)
			{
				obj->interior = pov::InteriorPtr(new pov::Interior());
			}

			// Promote hollow flag to interior.

			obj->interior->hollow = (Test_Flag(obj, HOLLOW_FLAG) != false);

			// Promote finish's IOR to interior IOR.

			if (obj->Texture != NULL)
			{
				if (obj->Texture->Type == pov::PLAIN_PATTERN)
				{
					auto finish = obj->Texture->Finish;
					if (finish != NULL)
					{
						if (finish->Temp_IOR >= 0.0)
						{
							obj->interior->IOR = finish->Temp_IOR;
							obj->interior->Dispersion = finish->Temp_Dispersion;
						}
						if (finish->Temp_Caustics >= 0.0)
						{
							obj->interior->Caustics = finish->Temp_Caustics;
						}

						obj->interior->Old_Refract = finish->Temp_Refract;
					}
				}
			}

			// If there is no IOR specified use the atmopshere ior.

			if (obj->interior->IOR == 0.0)
			{
				obj->interior->IOR = context->sceneData->atmosphereIOR;
				obj->interior->Dispersion = context->sceneData->atmosphereDispersion;
			}

			// If object has subsurface light transport enabled, precompute some necessary information
			/* if(!obj->Texture->Finish->SubsurfaceTranslucency.IsZero()) */
			if (context->sceneData->useSubsurface)
			{
				obj->interior->subsurface = shared_ptr<pov::SubsurfaceInterior>(new pov::SubsurfaceInterior(obj->interior->IOR));
			}
		}

		if (dynamic_cast<pov::CSG*>(obj))
		{
			for (vector<pov::ObjectPtr>::iterator Sib = (dynamic_cast<pov::CSG *>(obj))->children.begin();
				 Sib != (dynamic_cast<pov::CSG *>(obj))->children.end();
				 Sib++)
			{
				PostProcess(context, *Sib, obj);
			}
		}

		// Test wether the object is finite or infinite. [DB 9/94]
		// CJC TODO FIXME: see if this can be improved, and/or if it is appropriate for all bounding systems

		double volume = obj->BBox.size[0] * obj->BBox.size[1] * obj->BBox.size[2];

		if (volume > HUGE_VAL)
		{
			Set_Flag(obj, INFINITE_FLAG);
		}

		// Test if the object is opaque or not. [DB 8/94]

		if ((dynamic_cast<pov::Blob *>(obj) == NULL) && // FIXME
			(dynamic_cast<pov::Mesh *>(obj) == NULL) && // FIXME
			(Test_Opacity(obj->Texture)) &&
			((obj->Interior_Texture == NULL) ||
			 Test_Opacity(obj->Interior_Texture)))
		{
			Set_Flag(obj, OPAQUE_FLAG);
		}
		else
		{
			// Objects with multiple textures have to be handled separately.

			if (dynamic_cast<pov::Blob *>(obj) != NULL) // FIXME
				(dynamic_cast<pov::Blob *>(obj))->Test_Blob_Opacity();

			if (dynamic_cast<pov::Mesh *>(obj) != NULL) // FIXME
				(dynamic_cast<pov::Mesh *>(obj))->Test_Mesh_Opacity();
		}
	}

	void SceneObject::LinkToFrame(Context^ context, pov::ObjectPtr obj)
	{
		if (obj == NULL)
			return;

		/* Remove bounding object if object is cheap to intersect. [DB 8/94]  */

		if ((obj->Bound.empty() == false) && (context->sceneData->removeBounds == true))
		{
			if ((dynamic_cast<pov::CSGUnion *>(obj) == NULL) && // FIXME
				(dynamic_cast<pov::CSGIntersection *>(obj) == NULL) && // FIXME
				(dynamic_cast<pov::CSGMerge *>(obj) == NULL) && // FIXME
				(dynamic_cast<pov::Poly *>(obj) == NULL) && // FIXME
				(dynamic_cast<pov::TrueType *>(obj) == NULL) && // FIXME
				((dynamic_cast<pov::Quadric *>(obj) == NULL) || (dynamic_cast<pov::Quadric *>(obj)->Automatic_Bounds)))
			{
				/* Destroy only, if bounding object is not used as clipping object. */

				if (obj->Bound != obj->Clip)
					pov::Destroy_Object(obj->Bound);
				obj->Bound.clear();
				context->Warning("Unnecessary bounding object removed.");
			}
		}

		/*
		* [CJC 8/01]
		*
		* if all children of a union have the no_shadow flag, then the union should
		* have it as well.
		*/
		if (dynamic_cast<pov::CSGUnion *>(obj) != NULL && dynamic_cast<pov::CSGMerge *>(obj) == NULL)
		{
			vector<pov::ObjectPtr>::iterator This_Sib = (dynamic_cast<pov::CSG *>(obj))->children.begin();
			while (This_Sib != (dynamic_cast<pov::CSG *>(obj))->children.end())
			{
				if ((dynamic_cast<pov::LightSource *>(*This_Sib) == NULL) && !Test_Flag((*This_Sib), NO_SHADOW_FLAG)) // FIXME
					break;
				This_Sib++;
			}
			if (This_Sib == (dynamic_cast<pov::CSG *>(obj))->children.end())
				Set_Flag(obj, NO_SHADOW_FLAG);
		}

		// Link the object to the frame if it's not a CSG union object,
		// if it's clipped or if bounding slabs aren't used.
		// TODO FIXME - check if bound is used
		if ((obj->Clip.empty() == false) || (dynamic_cast<pov::CSGUnion *>(obj) == NULL) || (dynamic_cast<pov::CSGMerge *>(obj) != NULL))
		{
			context->sceneData->objects.push_back(obj);
			return;
		}

		/*
		* [DB 8/94]
		*
		* The object is a CSG union object. It will be split if all siblings are
		* finite, i.e. the volume of the bounding box doesn't exceed a threshold.
		*/

		/* NK phmap - added code so union is not split up if it is
		flagged for hi-density photon mapping...
		maybe we SHOULD split it anyways... do speed tests later */
		if ((reinterpret_cast<pov::CSGUnion *>(obj))->do_split == false)
		{
			context->sceneData->objects.push_back(obj);
			return;
		}

		if (!obj->Bound.empty())
		{
			/* Test if all children are finite. */
			bool finite = true;
			DBL Volume;
			for (vector<pov::ObjectPtr>::iterator This_Sib = (dynamic_cast<pov::CSG *>(obj))->children.begin();
				 This_Sib != (dynamic_cast<pov::CSG *>(obj))->children.end();
				 This_Sib++)
			{
				BOUNDS_VOLUME(Volume, (*This_Sib)->BBox);
				if (Volume > BOUND_HUGE)
				{
					finite = false;
					break;
				}
			}

			/*
			* If the union has infinite children or splitting is not used and
			* the object is not a light group link the union to the frame.
			*/

			if (((finite == false) || (context->sceneData->splitUnions == false)) && ((obj->Type & LIGHT_GROUP_OBJECT) != LIGHT_GROUP_OBJECT))
			{
				if (finite)
					context->Warning("CSG union unnecessarily bounded.");
				context->sceneData->objects.push_back(obj);
				return;
			}

			context->Warning("Bounded CSG union split.");
		}

		// Link all children of a union to the frame.
		for (vector<pov::ObjectPtr>::iterator This_Sib = (dynamic_cast<pov::CSG *>(obj))->children.begin();
			 This_Sib != (dynamic_cast<pov::CSG *>(obj))->children.end();
			 This_Sib++)
		{
			// Child is no longer inside a CSG object.
			(*This_Sib)->Type &= ~IS_CHILD_OBJECT;
			LinkToFrame(context, *This_Sib);
		}

		(dynamic_cast<pov::CSG *>(obj))->children.clear();
		Destroy_Object(obj);
	}
}