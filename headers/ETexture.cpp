#include <Windows.h>//for gdi plus, not special header because it is blocking gdi plus
#include "ETexture.h"

#include <gdiplus.h>
#include <sstream>

#pragma comment( lib,"gdiplus.lib" )

ETexture::ETexture()
{
	width = 640;
	height = 480;
	length = width * height;
}
ETexture::ETexture(int iwidth, int iheight)
{
	width = iwidth;
	height = iheight;
	buffer.reset(new EColor[(size_t)width * (size_t)height]);
	length = width * height;
}
ETexture::ETexture(const ETexture* source)
{
	width = source->width;
	height = source->height;
	buffer.reset(new EColor[(size_t)width * (size_t)height]);
	memcpy(buffer.get(), source->buffer.get(), sizeof(EColor) * width * height);
	length = width * height;
}


void ETexture::Fill(EColor clr)
{
	std::fill(buffer.get(), buffer.get()+width*height, clr);
	//memset(buffer.get(), (clr.data), sizeof(EColor::data)*width * height);
	//for (int i = 0; i < width * height; i++)
	//	buffer.get()[i] = clr;
}

/*void ETexture::Fill(EColor clr, int w, int h)
{
	std::fill(buffer.get(), buffer.get() + w * h, clr);
}*/

ETexture ETexture::GetCopy()
{
	ETexture copy;
	copy = ETexture(width, height);
	memcpy(copy.buffer.get(), buffer.get(), sizeof(EColor) * width * height);
	return copy;
}

void ETexture::Project(size_t dataPitch, unsigned char* const data, bool mod) const
{
	if (mod)
	{
		memcpy(data, buffer.get(), length*sizeof(EColor));
	}
	else
	{
		for (int ny = 0; ny < height; ny++)
			memcpy(&data[dataPitch * ny], &buffer.get()[width * ny], sizeof(EColor) * width);
	}
	
	
}

void ETexture::PutPixel(int ix, int iy, EColor clr)
{
	if (ix >= 0)
		if (iy >= 0)
			if (ix < width)
				if (iy < height)
				{
					buffer.get()[ix + width * iy] = clr;
				}
					
}

EColor ETexture::GetPixel(int ix, int iy) const
{
	if(ix >= 0)
	if (iy >= 0)
	if (ix < width)
	if (iy < height)
		return buffer.get()[ix + width * iy];
	return EColor((iy+width)%255, (2*iy+ix)%255 , (3*ix+height)%255 , 0);
}

ETexture ETexture::FromFile(const std::wstring name)
{
	
	Gdiplus::Bitmap bitmap(name.c_str());

	ETexture out = ETexture(bitmap.GetWidth(), bitmap.GetHeight());

	Gdiplus::Color clr;

	for (unsigned int cy = 0; cy < out.height; cy++)
	{
		for (unsigned int cx = 0; cx < out.width; cx++)
		{
			bitmap.GetPixel(cx, cy, &clr);
			out.buffer.get()[cy * out.width + cx].data = clr.GetValue();
		}
	}

	return out;
}

int ETexture::LoadFromFile(const std::wstring name)
{
	Gdiplus::Bitmap bitmap(name.c_str());

	int o = bitmap.GetLastStatus();

	if (o != Gdiplus::Status::Ok)
		return o;

	width = bitmap.GetWidth();
	height = bitmap.GetHeight();
	buffer.reset(new EColor[width * height]);
	Gdiplus::Color clr;

	for (unsigned int cy = 0; cy < height; cy++)
	{
		for (unsigned int cx = 0; cx < width; cx++)
		{
			bitmap.GetPixel(cx, cy, &clr);
			buffer.get()[cy * width + cx].data = clr.GetValue();
		}
	}
	return o;
}


