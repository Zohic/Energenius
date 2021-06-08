#pragma once
#include "ETexture.h"
#include "EMath.h"
#include <vector>
#include "EAcceleration.h"

struct ESprite
{
	ESprite();
	ESprite(ETexture* tex);
	int width;
	int height;
	ETexture* texture;
	ETexture lastTexture;
	std::vector<Mat33> TransformQueue;

	IVec2d begin;
	IVec2d end;
	bool changed = true;

	void GetRender(ETexture& output, bool useG);
	void AddMatrix(Mat33 mat);
	void ChangeMatrix(int ind, Mat33 mat);

};