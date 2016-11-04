#pragma once

namespace povray
{
	public interface class RenderTarget
	{
		void Initialize(int width, int height);
		void SetPixel(int x, int y, float red, float green, float blue, float alpha);
		void Finish();
	};
}