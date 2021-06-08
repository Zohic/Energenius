#include <string>
#include "EInstance.h"

#include <iostream>
#include <fstream>


struct Test :public EInstance
{

	struct Range
	{
		float min, max;
		Range()
		{
			min = 0;
			max = 1;
		}
		Range(float mn, float mx)
		{
			min = mn;
			max = mx;
		}
	};

	float rTo3(float a)
	{
		return floorf(a * 1000) / 1000;
	}

	int divides;
	float dt;
	std::vector<Range> ranges;
	std::vector<float> heights;
	Range mainRange;
	float sto;
	float pMax;

	bool cool;

	int beginRange = 70;
	float scale = 100;
	int hScale = 50;

	float avg;

	float lineStep = 0.001f;

	Vec2d lastPos;
	float lScale = 1;
	void OnBegin() override
	{
		std::fstream ins;
		ins.open("output.txt");

		cool = ins.is_open();

		ins >> mainRange.min;
		ins >> mainRange.max;
		ins >> divides;

		heights.resize(divides);

		for (int i = 0; i < divides; i++)
		{
			ranges.push_back(Range());
			ins >> ranges[i].min;
			ins >> ranges[i].max;
			ins >> heights[i];
		}

		ins >> sto;
		ins >> pMax;
		ins >> avg;

		lastPos = Vec2d(beginRange, 0);

	}
	void FrameWork() override
	{
		dxg.swapTexture.Clear(EColor(30, 30, 50));

		dxg.DrawLine(40, 0, 40, 440, EColor(255, 255, 255));
		dxg.DrawLine(40, 440, 640, 440, EColor(255, 255, 255));

		float step = (mainRange.max - mainRange.min) / divides;

		for (int i = 0; i < divides+1; i++)
		{
			std::string val = std::to_string(rTo3(mainRange.min + step * i));
			val.resize(4);
			dxg.DrawText(beginRange + step * i * scale*divides, 445, val, EColor(255, 255, 255));

			float hStep = 0.5;

			val = std::to_string(rTo3(hStep * i));
			val.resize(4);

			dxg.DrawText(0 ,440 - hStep*i*hScale , val, EColor(255,255,255));

			if (i < divides)
				dxg.FillRect(beginRange + step * i * scale*divides+3, 440 - heights[i] * hScale, step * scale*divides-3, heights[i] * hScale, EColor(144, 144, 0));
			
		}
	

		for (float i = mainRange.min; i < mainRange.max; i += 0.001f)
		{
			float smx = beginRange + (i - mainRange.min)*scale*divides;
			float curMinusAvg = i - avg;
			float smy = pMax * expf(-(curMinusAvg * curMinusAvg) / (2 * sto * sto));
			
			dxg.swapTexture.PutPixel(smx, 440-smy*hScale, EColor(255, 0, 0));
		}
		float curMinusAvg = avg - avg;
		float smy = pMax * expf(-(curMinusAvg * curMinusAvg) / (2 * sto * sto));
		float smx = beginRange + (avg-mainRange.min) * scale*divides;
		dxg.DrawLine(smx, 440-smy*hScale , smx,440, EColor(255, 255, 255));


	}
};

/*Test instance;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{

	instance.Begin(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return instance.outCode;
}*/