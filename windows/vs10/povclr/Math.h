#pragma once

#include "core/math/vector.h"

namespace povclr
{
	public ref class Vector3
	{
	public:
		Vector3(double x, double y, double z) :
			X(x), Y(y), Z(z)
		{}

		double X;
		double Y;
		double Z;

	internal:
		pov::Vector3d ToVector() { return pov::Vector3d(X, Y, Z); }
	};

	public ref class Vector2
	{
	public:
		Vector2(double x, double y) :
			X(x), Y(y)
		{}

		double X;
		double Y;

	internal:
		pov::Vector2d ToVector() { return pov::Vector2d(X, Y); }
	};

	public ref class Matrix
	{
	private:
		array<double>^ data = gcnew array<double>(16);

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

		void Set(int x, int y, double value)
		{
			data[y * 4 + x] = value;
		}

		double Get(int x, int y)
		{
			return data[y * 4 + x];
		}
	};

}