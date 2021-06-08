#pragma once

#include <vector>

#include "specialHeader.h"

#include "ETexture.h"
#include "InputManager.h"
#include "GDIPlusInstance.h"
#include <d3d11.h>
#include "EMath.h"
#include "ESprite.h"
#include <thread>

struct DirectGraphics
{
	~DirectGraphics();
	void Init(HWND hWnd, int w, int h);

	InputManager* inputManagerPtr;



	void ProcessFrame();
	void ClearBuffer(float red, float green, float blue);

	void DrawLine(int x1, int y1, int x2, int y2, EColor clr);
	//void DrawLine(int x1, int y1, int x2, int y2, EColor clr, ETexture target);
	void DrawVector(Vec2d beg, Vec2d vec, EColor clr = EColor(255, 255, 255));

	void DrawCircle(int x, int y, float r, EColor clr);
	void DrawCircle(Vec2d p, float r, EColor clr);
	void FillRect(int x, int y, int w, int h, EColor clr);
	void FillRect(IVec2d p, IVec2d s, EColor clr);

	bool Mod4Pitch;

	bool TextureOnScreen(int dx, int dy, ETexture tex);
	bool DrawTextureOffset(int dx, int dy, ETexture *tex, bool eh);
	bool DrawSprite(ESprite* spr);
	bool DrawSpriteOffset(int dx, int dy, ESprite* spr, bool a);


	void DrawText(int dx, int dy, std::string text, EColor clr);

	int screenWidth;
	int screenHeight;


	//RowPitch is data's width in bytes
	//DepthPitch is size if data in bytes
	D3D11_MAPPED_SUBRESOURCE dxMainTextureBuffer;

	ETexture swapTexture;
	ETexture fontTexture;
	

private:

	GDIPlusInstance gdiInst;//for loading images

	ID3D11Device* devicePtr = nullptr;
	IDXGISwapChain* swapChainPtr = nullptr;
	ID3D11DeviceContext* deviceContextPtr = nullptr;
	ID3D11RenderTargetView* targetViewPtr = nullptr;
	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11Texture2D* dxMainTexture;

	ID3DBlob* blob;

	ID3D11ShaderResourceView* dxShaderResourceView;
	ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;
	ID3D11Buffer* vertexBuffer;
	ID3D11InputLayout* inputLayout;
	ID3D11SamplerState* samplerState;
	




};
