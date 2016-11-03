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

namespace povclr
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

		SceneObject ^Translate(Vector3 ^vector)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povclr::Translate(vector));

			return this;
		}

		SceneObject ^Rotate(Vector3 ^vector)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povclr::Rotate(vector));

			return this;
		}

		SceneObject ^Scale(Vector3 ^vector)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povclr::Scale(vector));

			return this;
		}

		SceneObject ^Translate(double x, double y, double z)
		{
			auto vector = gcnew Vector3(x, y, z);
			Translate(vector);
			return this;
		}

		SceneObject ^Rotate(double x, double y, double z)
		{
			auto vector = gcnew Vector3(x, y, z);
			Rotate(vector);
			return this;
		}

		SceneObject ^Scale(double x, double y, double z)
		{
			auto vector = gcnew Vector3(x, y, z);
			Scale(vector);
			return this;
		}

		SceneObject ^Scale(double value)
		{
			auto vector = gcnew Vector3(value, value, value);
			Scale(vector);
			return this;
		}

		SceneObject ^Transform(Matrix ^matrix)
		{
			if (Transformations == nullptr)
			{
				Transformations = gcnew List<Transformation^>();
			}
			Transformations->Add(gcnew povclr::MatrixTransformation(matrix));

			return this;
		}
	};
}