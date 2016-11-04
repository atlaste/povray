#pragma once

#include "RenderTarget.h"

#include <cstdint>

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

namespace povclr
{
	public ref class RenderCallback : public RenderTarget, public IDisposable
	{
	private:
		Bitmap ^bmp;
		BitmapData ^bmpData;
		Object ^lockObject = gcnew Object();

		ref class Lock
		{
			Object^ obj;

		public:
			Lock(Object ^ obj) : obj(obj)
			{
				System::Threading::Monitor::Enter(obj);
			}
			~Lock()
			{
				System::Threading::Monitor::Exit(obj);
			}
		};

	public:
		RenderCallback()
		{
			bmp = nullptr;
			bmpData = nullptr;
		}

		int PixelsProcessed;
		int NumberPixels;

		virtual void Initialize(int width, int height) 
		{
			NumberPixels = width * height;
			PixelsProcessed = 0;

			Lock l(lockObject);
			{
				bmp = gcnew Bitmap(width, height, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
				bmpData = bmp->LockBits(Rectangle(0, 0, bmp->Width, bmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);
			}
		}

		virtual void SetPixel(int x, int y, double red, double green, double blue, double alpha) 
		{
			uint8_t* baseptr = (uint8_t*)(bmpData->Scan0.ToPointer());
			uint8_t* ptr = baseptr + (y * bmpData->Stride + x * (32 / 8));

			if (alpha < 0) { alpha = 0; }
			else if (alpha > 1) { alpha = 1; }
			if (red < 0) { red = 0; }
			else if (red > 1) { red = 1; }
			if (green < 0) { green = 0; }
			else if (green > 1) { green = 1; }
			if (blue < 0) { blue = 0; }
			else if (blue > 1) { blue = 1; }

			// Little endian...
			*ptr++ = (uint8_t)(blue * 255);
			*ptr++ = (uint8_t)(green * 255);
			*ptr++ = (uint8_t)(red * 255);
			*ptr++ = (uint8_t)(alpha * 255);

			System::Threading::Interlocked::Increment(PixelsProcessed);
		}

		virtual void Finish()
		{
			Lock l(lockObject);
			if (bmpData != nullptr)
			{
				bmp->UnlockBits(bmpData);
				bmpData = nullptr;
			}
		}

		!RenderCallback()
		{
			Finish();
		}

		~RenderCallback()
		{
			Finish();
		}

		property Image^ Image
		{
			System::Drawing::Image^ get()
			{
				if (bmpData == nullptr)
				{
					return bmp;
				}
				else
				{
					Lock l(lockObject);

					if (bmpData == nullptr)
					{
						return bmp;
					}
					else
					{
						Bitmap ^tmp = gcnew Bitmap(bmp->Width, bmp->Height, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
						BitmapData ^tmpData = tmp->LockBits(Rectangle(0, 0, tmp->Width, tmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);

						uint8_t* tmpStart = (uint8_t*)(tmpData->Scan0.ToPointer());
						uint8_t* srcStart = (uint8_t*)(bmpData->Scan0.ToPointer());
						
						int count = bmpData->Stride * tmp->Height;

						for (int i = 0; i < count; ++i)
						{
							tmpStart[i] = srcStart[i];
						}

						tmp->UnlockBits(tmpData);
						return tmp;
					}
				}
			}
		}
	};

}