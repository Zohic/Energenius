#include "EInstance.h"



struct ArrowGame : public EInstance
{
	ETexture LeftSpr, DownSpr, UpSpr, RightSpr, TestSpr;
	ETexture* sprs[4];
	int posX[4];
	int posY[4];
	int cenX, cenY;
	bool pressA[4];

	float timeToSpawn;
	float elTime = 0;
	int score = 0;
	int combo = 0;
	int totalScore = 0;

	struct Arrow
	{
		float spd;
		int side;
		float pos[2];
		Arrow(float ispd, int iside, int cenX, int cenY)
		{
			spd = ispd;
			side = iside;
			switch (iside)
			{
			case 0:
				pos[0] = cenX * 2;
				pos[1] = cenY;
				break;
			case 1:
				pos[0] = cenX - 16;
				pos[1] = -32;
				break;
			case 2:
				pos[0] = -32;
				pos[1] = cenY;
				break;
			case 3:
				pos[0] = cenX - 16;
				pos[1] = cenY * 2;
				break;

			}
		}
	};

	std::vector<Arrow> arrows;

	struct Salute
	{
		float pos[50];
		float spd[50];
		DirectGraphics* dxg;
		EColor clr;
		float lifeTime = 0;

		Salute(int iposX, int iposY, DirectGraphics* d)
		{
			clr = EColor(200 + rand() % 40, rand() % 80, rand() % 40);

			dxg = d;

			for (int i = 0; i < 50; i += 2)
			{
				pos[i] = iposX;
				pos[i + 1] = iposY;
				spd[i] = -45 + rand() % 90;
				spd[i + 1] = -30 + rand() % 50;
			}
		}

		void draw(float dt)
		{
			float prevX = pos[0];
			float prevY = pos[1];

			lifeTime += dt;
			for (int i = 0; i < 50; i += 2)
			{
				pos[i] += spd[i] * dt;
				pos[i + 1] += spd[i + 1] * dt;
				spd[i + 1] += 100 * dt;
				dxg->swapTexture.PutPixel(pos[i], pos[i + 1], clr);
				if (prevY < pos[i + 1] && fabsf(pos[i] - prevX) < 20)
					dxg->DrawLine(pos[i], pos[i + 1], prevX, prevY, clr);
				prevX = pos[i];
				prevY = pos[i + 1];
			}

		}
	};

	std::vector<Salute> salutes;

	void OnBegin() override
	{
		LeftSpr.LoadFromFile(L"arrows/leftArr.png");
		RightSpr.LoadFromFile(L"arrows/rightArr.png");
		UpSpr.LoadFromFile(L"arrows/upArr.png");
		DownSpr.LoadFromFile(L"arrows/downArr.png");
		TestSpr.LoadFromFile(L"corey.png");

		cenX = dxg.screenWidth / 2;
		cenY = dxg.screenHeight / 2;

		posX[0] = cenX + 16;
		posY[0] = cenY;
		sprs[0] = &RightSpr;

		posX[1] = cenX - 16;
		posY[1] = cenY - 32;
		sprs[1] = &UpSpr;

		posX[2] = cenX - 48;
		posY[2] = cenY;
		sprs[2] = &LeftSpr;

		posX[3] = cenX - 16;
		posY[3] = cenY + 32;
		sprs[3] = &DownSpr;

		timeToSpawn = 3;

	}

	void FrameWork() override
	{

		dxg.swapTexture.Clear(EColor(0, 0, 0, 0));

		pressA[0] = input.keyStates[sRIGHT].pressed;
		pressA[1] = input.keyStates[sUP].pressed;
		pressA[2] = input.keyStates[sLEFT].pressed;
		pressA[3] = input.keyStates[sDOWN].pressed;

		elTime += timer.GdeltaTime;

		if (elTime > timeToSpawn)
		{
			arrows.push_back(Arrow(100 + rand() % 250, rand() % 4, cenX, cenY));
			elTime = 0;
			timeToSpawn = (float)(50 + rand() % 1200) / 1000.0f;
		}

		for (int i = 0; i < arrows.size(); i++)
		{

			bool canD = true;

			switch (arrows[i].side)
			{
			case 0:
				arrows[i].pos[0] -= arrows[i].spd * timer.deltaTime;
				break;
			case 1:
				arrows[i].pos[1] += arrows[i].spd * timer.deltaTime;
				break;
			case 2:
				arrows[i].pos[0] += arrows[i].spd * timer.deltaTime;
				break;
			case 3:
				arrows[i].pos[1] -= arrows[i].spd * timer.deltaTime;
				break;

			}

			if (fabsf(arrows[i].pos[0] - posX[arrows[i].side]) <= 32
				&& fabsf(arrows[i].pos[1] - posY[arrows[i].side]) <= 32)
			{
				if (fabsf(arrows[i].pos[0] - posX[arrows[i].side]) - arrows[i].spd * timer.deltaTime < 1
					&& fabsf(arrows[i].pos[1] - posY[arrows[i].side]) - arrows[i].spd * timer.deltaTime < 1)
				{
					if (score > 0)
						score -= 1;

					combo = 0;

					arrows.erase(arrows.begin() + i);
					canD = false;
				}
				else if (pressA[arrows[i].side])
				{
					score += 1;
					totalScore += 1;
					combo += 1;
					salutes.push_back(Salute(posX[arrows[i].side] + 16, posY[arrows[i].side] + 16, &dxg));

					for (int j = 0; j < combo; j++)
					{
						salutes.push_back(Salute(rand() % dxg.screenWidth, rand() % dxg.screenHeight, &dxg));
					}

					arrows.erase(arrows.begin() + i);
					canD = false;
				}
			}

			if (canD)
				dxg.DrawTexture((int)arrows[i].pos[0], (int)arrows[i].pos[1], *sprs[arrows[i].side]);
		}




		for (int i = 0; i < 4; i++)
		{
			if (!pressA[i])
				dxg.DrawTexture(posX[i], posY[i], *sprs[i]);
		}

		for (int i = 0; i < salutes.size(); i++)
		{
			salutes[i].draw(timer.GdeltaTime);
			if (salutes[i].lifeTime > 3)
				salutes.erase(salutes.begin() + i);
		}

		if (input.keyStates[KB].down)
			for (int i = 0; i < 19000; i++)
			{
				dxg.DrawTexture(input.mouseX + rand() % 200, input.mouseY + rand() % 200, TestSpr);
				//dxg.DrawText(input.mouseX + rand() % 200, input.mouseY + rand() % 200,std::to_string(i), EColor(210,200,200));
			}

		dxg.DrawText(0, 0, "FPS" + std::to_string((int)floorf(timer.GgFPS)), EColor(220, 180, 20));
		dxg.DrawText(0, 30, "TIME" + std::to_string((int)floorf(ElapsedTime)), EColor(220, 180, 20));
		dxg.DrawText(0, 70, "SCORE" + std::to_string((int)floorf(score)), EColor(220, 180, 20));
		dxg.DrawText(0, 100, "AVG" + std::to_string((int)roundf((totalScore / (ElapsedTime - 3) * 100))), EColor(220, 180, 20));
		dxg.DrawText(dxg.screenWidth / 4 * 3, 120, "COMBO" + std::to_string((int)floorf(combo)), EColor(220, 180, 20));

	}


};


/*ArrowGame instance;

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