#pragma once

#include "Math.h"
#include "Context.h"

#include "core/math/vector.h"
#include "core/math/matrix.h"

namespace povclr
{
	public ref class Transformation abstract
	{
	internal:
		virtual void Render(Context^ context, pov::ObjectBase* obj, pov::TRANSFORM *localTrans) = 0;
	};

	public ref class Rotate : public Transformation
	{
	internal:
		virtual void Render(Context^ context, pov::ObjectBase* obj, pov::TRANSFORM *localTrans) override
		{
			pov::Compute_Rotation_Transform(localTrans, Rotation.ToVector());
			pov::Rotate_Object(obj, Rotation.ToVector(), localTrans);
		}

	public:
		Rotate(Vector3 rotation) :
			Rotation(rotation)
		{}

		Vector3 Rotation;
	};

	public ref class Translate : public Transformation
	{
	internal:
		virtual void Render(Context^ context, pov::ObjectBase* obj, pov::TRANSFORM *localTrans) override
		{
			pov::Compute_Translation_Transform(localTrans, Translation.ToVector());
			pov::Translate_Object(obj, Translation.ToVector(), localTrans);
		}

	public:
		Translate(Vector3 translation) :
			Translation(translation)
		{}

		Vector3 Translation;
	};

	public ref class Scale : public Transformation
	{
	internal:
		virtual void Render(Context^ context, pov::ObjectBase* obj, pov::TRANSFORM *localTrans) override
		{
			pov::Compute_Scaling_Transform(localTrans, Scaling.ToVector());
			pov::Scale_Object(obj, Scaling.ToVector(), localTrans);
		}

	public:
		Scale(Vector3 scaling) :
			Scaling(scaling)
		{}

		Vector3 Scaling;
	};

	public ref class MatrixTransformation : public Transformation
	{
	internal:
		virtual void Render(Context^ context, pov::ObjectBase* obj, pov::TRANSFORM *localTrans) override
		{
			pov::MATRIX matrix;
			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; ++x)
				{
					matrix[x][y] = Matrix->Get(x, y);
				}
			}
			pov::Compute_Matrix_Transform(localTrans, matrix);
			pov::Transform_Object(obj, localTrans);
		}

	public:
		MatrixTransformation(Matrix ^matrix) :
			Matrix(matrix)
		{}

		Matrix ^Matrix;
	};
}