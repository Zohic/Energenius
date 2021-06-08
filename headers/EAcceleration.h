#pragma once
#include <amp.h>
#include "EMath.h"
#include "EColor.h"

using namespace Concurrency;

struct ESprite;

struct EAcceleration
{

	EAcceleration();
	std::vector<accelerator> accelerators;

	static bool ComputeTextureMat3(IVec2d begin, IVec2d size, IVec2d osize, EColor* pix, EColor* orig, Mat33& mat);
	
};
#include "ESprite.h"
/*if (width * height > 0)
{
	output = ETexture(width, height);

	Mat33 inv = completeMatrix.GetInverse();
	
}*/
