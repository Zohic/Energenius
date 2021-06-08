#include "energyLib v0.1/EInstance.h"
#include <iostream>
#include <DirectXMath.h>

class Hello :public EInstance
{
	ETexture hi;
	ETexture bye;
	ESprite spr;
	EAcceleration acc;
	float x = 0;
	float y = 0;
	float angle = 0;
	bool fy = false;
	int gotedFPS = 0;
	float sumFPS = 0;
	float avgFPS = 0;
	void OnBegin() override
	{
		hi.LoadFromFile(L"texture.png");
		bye = ETexture(&hi);
		spr = ESprite(&bye);
		spr.AddMatrix(Mat33::RotationMat2D(0));
		spr.AddMatrix(Mat33::ScaleMat2D(1 ,1));
		//concurrency::accelerator::set_default(acc.accelerators[0].get_device_path());
	}
	void FrameWork() override
	{
		
		dxg.swapTexture.Fill(EColor(100, 0, 20));
		float ms = 300;

		if (input.keyStates[KQ].down)
			angle -= EPI * timer.deltaTime;
		if (input.keyStates[KE].down)
		{
			angle += EPI * timer.deltaTime;
			
		}
			
			
		spr.ChangeMatrix(0, Mat33::RotationMat2D(angle));
		//spr.ChangeMatrix(1, Mat33::ScaleMat2D(1 + angle, 1 + angle));
		
		
		if (input.keyStates[KG].pressed)
			fy = !fy;

		if (input.keyStates[KA].down)
			x -= ms * timer.deltaTime;
		if (input.keyStates[KD].down)
			x += ms * timer.deltaTime;
		if (input.keyStates[KW].down)
			y -= ms * timer.deltaTime;
		if (input.keyStates[KS].down)
			y += ms * timer.deltaTime;

		
		
		dxg.DrawSpriteOffset(x, y, &spr, fy);


		dxg.DrawText(10, 10, std::to_string(timer.FPS), EColor(255,255, 255));
		dxg.DrawText(10, 30, std::to_string(spr.width) + " ; " + std::to_string(spr.height), EColor(255, 255, 255));
		dxg.DrawText(150, 10, std::to_string(avgFPS), EColor(255, 255, 255));

		if (gotedFPS < 100)
		{
			sumFPS += timer.FPS;
			gotedFPS += 1;
		}
		else
		{
			avgFPS = sumFPS / 100;
			sumFPS = 0;
			gotedFPS = 0;
		}

		if (fy)
		{
			dxg.DrawText(10, 50, "ACELAR", EColor(0, 0, 0));
		}else
			dxg.DrawText(10, 50, "CPU", EColor(0, 0, 0));

		/*for (int i = 0; i < acc.accelerators.size(); i++)
		{
			std::wstring a = acc.accelerators[i].description;
			std::string str(a.begin(), a.end());
			dxg.DrawText(80, 50 + 10 * i, str, EColor(0, 0, 0));
		}*/
		

	}
};

Hello instance;




int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	instance.Set(611, 305);
	instance.Begin(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	
	return instance.outCode;
}