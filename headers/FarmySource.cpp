#include <string>

#include "EInstance.h"


struct Farmy :public EInstance
{

	ETexture mapSpr;
	ETexture mapDraw;
	float scY = 1.0f;
	float scX = 1.0f;
	int okx = 0;

	void OnBegin() override
	{
		mapSpr.LoadFromFile(L"FarmySprites/Farm.png");
	}

	void FrameWork() override
	{

		dxg.swapTexture.Clear(EColor(0, 0, 0));

		if (input.keyStates[sUP].down)
			scY -= 0.03f;
		if (input.keyStates[sDOWN].down)
			scY += 0.03f;

		if (input.keyStates[sLEFT].down)
			scX -= 0.03f;
		if (input.keyStates[sRIGHT].down)
			scX += 0.03f;
		
	

		dxg.ScaleTextureAMP(&mapSpr, &mapDraw, scX, scY);

		dxg.DrawTexture(input.mouseX, input.mouseY, mapDraw);

		Concurrency::accelerator acr = dxg.mainAcc;
		std::wstring info = acr.description;
		std::string infoStr(info.begin(), info.end());

		dxg.DrawText(30, 30 , infoStr, EColor(255, 255, 255));

		dxg.DrawText(30, 50, std::to_string(timer.gFPS), EColor(255, 255, 255));

		dxg.DrawText(30, 70, std::to_string(scX), EColor(255, 255, 255));


		//dxg.DrawText(30, 30, "HELLO, HOW ARE YOU.", EColor(255,10,10));

		//dxg.DrawText(10, 10, std::to_string((int)floorf(timer.gFPS)), EColor(233, 233, 233));
		//dxg.DrawText(30, 30, std::to_string((int)mapDraw.buffer), EColor(233, 233, 233));

	}

};


Farmy game;


/*int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{

	game.Begin(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return game.outCode;
}*/