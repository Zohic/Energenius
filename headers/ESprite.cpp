#include "ESprite.h"


ESprite::ESprite()
{
	width = 0;
	height = 0;
	texture = nullptr;
}


ESprite::ESprite(ETexture* tex)
{
	width = tex->width;
	height = tex->height;
	texture = tex;
}

void ESprite::GetRender(ETexture& output, bool useG)
{
	if (!changed)
		output = lastTexture.GetCopy();

	if (TransformQueue.size() == 0)
		output = texture->GetCopy();
	Mat33 completeMatrix;
	if (TransformQueue.size() == 1)
		completeMatrix = TransformQueue[0];
	else
	{
		completeMatrix = Mat33::Identity();
		for (int i = 0; i < TransformQueue.size(); i++)
		{
			completeMatrix = completeMatrix * TransformQueue[i];
		}
	}

	begin = IVec2d(0, 0);
	end = IVec2d(width, height);
	IVec2d newCoord;
	//----------------------LEFT-TOP-------------------------------------------
	newCoord = completeMatrix * IVec2d(0, 0);
	begin.x = newCoord.x;	    begin.y = newCoord.y;
	end.x = newCoord.x;	    end.y = newCoord.y;
	//----------------------RIGHT-BOT------------------------------------------
	newCoord = completeMatrix * IVec2d(texture->width, texture->height);
	begin.x = min(begin.x, newCoord.x); begin.y = min(begin.y, newCoord.y);
	end.x = max(end.x, newCoord.x); end.y = max(end.y, newCoord.y);
	//----------------------RIGHT-TOP------------------------------------------
	newCoord = completeMatrix * IVec2d(texture->width, 0);
	begin.x = min(begin.x, newCoord.x); begin.y = min(begin.y, newCoord.y);
	end.x = max(end.x, newCoord.x);  end.y = max(end.y, newCoord.y);
	//----------------------LEFT-BOT-------------------------------------------
	newCoord = completeMatrix * IVec2d(0, texture->height);
	begin.x = min(begin.x, newCoord.x); begin.y = min(begin.y, newCoord.y);
	end.x = max(end.x, newCoord.x); end.y = max(end.y, newCoord.y);
	//-------------------------------------------------------------------------

	width = end.x - begin.x;
	height = end.y - begin.y;

	if (width * height > 0)
	{
		output = ETexture(width, height);
		Mat33 inv = completeMatrix.GetInverse();
		if (!useG)
		{
			for (int x = 0; x < width; x++)
				for (int y = 0; y < height; y++)
				{
					newCoord = inv * IVec2d(begin.x + x, begin.y + y);
					output.PutPixel(x, y, texture->GetPixel(newCoord.x + 0.5f, newCoord.y + 0.5f));
				}
			lastTexture = output.GetCopy();
		}
		else
		{
			bool put = EAcceleration::ComputeTextureMat3(
				begin,
				IVec2d(width,height),
				IVec2d(texture->width,texture->height),
				output.buffer.get(),
				texture->buffer.get(), 
				inv);
			lastTexture = output.GetCopy();
		}
	}
}

void ESprite::AddMatrix(Mat33 mat)
{
	TransformQueue.push_back(mat);
	changed = true;
}

void ESprite::ChangeMatrix(int ind, Mat33 mat)
{
	TransformQueue[ind] = mat;
	changed = true;
}