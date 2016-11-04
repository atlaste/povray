#pragma once

#include "Context.h"
#include "Settings.h"
#include "Transformation.h"

#include "core/shape/blob.h"
#include "core/shape/mesh.h"
#include "core/shape/csg.h"
#include "core/shape/quadric.h"
#include "core/shape/polynomial.h"
#include "core/lighting/subsurface.h"
#include "core/material/pattern.h"

using namespace System;
using namespace System::Collections::Generic;

namespace povray
{
	public ref class SceneObject abstract
	{
	internal:
		void CheckPassThru(Context^ context, pov::ObjectPtr o, int flag);
		void PostProcess(Context^ context, pov::ObjectBase* obj, pov::ObjectBase* parent);
		void LinkToFrame(Context^ context, pov::ObjectPtr obj);

	protected:
		void RenderTransformations(Context^ context, pov::ObjectBase* obj)
		{
			pov::TRANSFORM localTrans;
			if (Transformations != nullptr)
			{
				for each (Transformation^ trans in Transformations)
				{
					trans->Render(context, obj, &localTrans);
				}
			}
		}

	public:
		SceneObject() :
			Transformations(nullptr)
		{}

		List<Transformation^> ^Transformations;

		virtual void Render(Context^ context) = 0;

		SceneObject ^Translate(Vector3 vector)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povray::Translate(vector));

			return this;
		}

		SceneObject ^Rotate(Vector3 vector)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povray::Rotate(vector));

			return this;
		}

		SceneObject ^Scale(Vector3 vector)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povray::Scale(vector));

			return this;
		}

		SceneObject ^Translate(float x, float y, float z)
		{
			Vector3 vector(x, y, z);
			Translate(vector);
			return this;
		}

		SceneObject ^Rotate(float x, float y, float z)
		{
			Vector3 vector(x, y, z);
			Rotate(vector);
			return this;
		}

		SceneObject ^Scale(float x, float y, float z)
		{
			Vector3 vector(x, y, z);
			Scale(vector);
			return this;
		}

		SceneObject ^Scale(float value)
		{
			Vector3 vector(value, value, value);
			Scale(vector);
			return this;
		}

		SceneObject ^Transform(Matrix ^matrix)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povray::MatrixTransformation(matrix));

			return this;
		}
	};
}