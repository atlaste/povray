#pragma once

#include "core/math/vector.h"

namespace povray
{
	public value class Vector3
	{
	public:
		Vector3(double v) :
			X((float)v), Y((float)v), Z((float)v)
		{}

		Vector3(double x, double y, double z) :
			X((float)x), Y((float)y), Z((float)z)
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
		Vector2(double v) :
			X((float)v), Y((float)v)
		{}
		
		Vector2(double x, double y) :
			X((float)x), Y((float)y)
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