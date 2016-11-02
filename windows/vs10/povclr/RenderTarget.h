#pragma once

namespace povclr
{
	public interface class RenderTarget
	{
		void Initialize(int width, int height);
		void SetPixel(int x, int y, double red, double green, double blue, double alpha);
		void Finish();
	};
}