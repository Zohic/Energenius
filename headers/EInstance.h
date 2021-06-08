#pragma once

#include <string>

#include "specialHeader.h"

#include "DirectGraphics.h"
#include "TimeManager.h"


#define IDI_ICON1 101


struct EInstance
{
	
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	static LRESULT CALLBACK WndProcBridgePhase1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProcBridgePhase2ActualBridge(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	InputManager input;
	TimeManager timer;
	DirectGraphics dxg;

	bool allReady;
	const std::string ClassName = "energyWindow";
	const std::string BeginWindowName = "energenius";
	
	WNDCLASSEX wndClass = { 0 };
	HWND hWnd;
	int windowWidth=640, windowHeight=480;
	
	int outCode;
	void Set(int w, int h);
	void Begin(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	
	virtual void OnBegin();
	virtual void FrameWork();
	int framesToUpdate=1;
	//void SetFPS(int fps);
	

private :
	int curUpdateFrames;
};
