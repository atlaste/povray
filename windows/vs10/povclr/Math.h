#pragma once

#include "core/math/vector.h"

namespace povclr
{
	public value class Vector3
	{
	public:
		Vector3(float x, float y, float z) :
			X(x), Y(y), Z(z)
		{}

		float X;
		float Y;
		float Z;

	internal:
		pov::Vector3d ToVector() { return pov::Vector3d(X, Y, Z); }
	};

	public value class Vector2
	{
	public:
		Vector2(float x, float y) :
			X(x), Y(y)
		{}

		float X;
		float Y;

	internal:
		pov::Vector2d ToVector() { return pov::Vector2d(X, Y); }
	};

	public ref class Matrix
	{
	private:
		array<float>^ data = gcnew array<float>(16);

	public:
		Matrix()
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (i == j)
					{
						Set(i, j, 1.0);
					}
					else
					{
						Set(i, j, 0.0);
					}
				}
			}
		}

		void Set(int x, int y, float value)
		{
			data[y * 4 + x] = value;
		}

		float Get(int x, int y)
		{
			return data[y * 4 + x];
		}
	};
}