#pragma once
#include "EColor.h"
#include <memory>
#include <sstream>

struct ETexture
{
	ETexture();
	ETexture(int iwidth, int iheight);
	ETexture(const ETexture* source);

	std::unique_ptr<EColor> buffer;

	unsigned int width;
	unsigned int height;
	unsigned int length;

	void Fill(EColor clr);
	//void Fill(EColor clr, int w, int h);
	void Project(size_t dataPitch, unsigned char* const data, bool mod8) const;
	void PutPixel(int ix, int iy, EColor clr);
	ETexture GetCopy();
	EColor GetPixel(int ix, int iy) const;
	static ETexture FromFile(const std::wstring name);
	int LoadFromFile(const std::wstring name);

};