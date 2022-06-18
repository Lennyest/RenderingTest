#pragma once

#include <iostream>
#include <thread>
#include <windows.h>

using std::wstring;

enum COLOR
{
	EBLACK = 0x0000,
	EDARK_BLUE = 0x0001,
	EDARK_GREEN = 0x0002,
	EDARK_CYAN = 0x0003,
	EDARK_RED = 0x0004,
	EDARK_MAGENTA = 0x0005,
	EDARK_YELLOW = 0x0006,
	EGREY = 0x0007,
	EDARK_GREY = 0x0008,
	EBLUE = 0x0009,
	EGREEN = 0x000A,
	ECYAN = 0x000B,
	ERED = 0x000C,
	EMAGENTA = 0x000D,
	EYELLOW = 0x000E,
	EWHITE = 0x000F,
	EBG_BLACK = 0x0000,
	EBG_DARK_BLUE = 0x0010,
	EBG_DARK_GREEN = 0x0020,
	EBG_DARK_CYAN = 0x0030,
	EBG_DARK_RED = 0x0040,
	EBG_DARK_MAGENTA = 0x0050,
	EBG_DARK_YELLOW = 0x0060,
	EBG_GREY = 0x0070,
	EBG_DARK_GREY = 0x0080,
	EBG_BLUE = 0x0090,
	EBG_GREEN = 0x00A0,
	EBG_CYAN = 0x00B0,
	EBG_RED = 0x00C0,
	EBG_MAGENTA = 0x00D0,
	EBG_YELLOW = 0x00E0,
	EBG_WHITE = 0x00F0,
};

class GameEngine
{
public:
	int nScreenWidth = 50;
	int nScreenHeight = 50;

	int nMousePosX = 0;
	int nMousePosY = 0;

	float fDeltaTime;
	
	wstring AppName = L"Game Engine";

	void ConstructConsole(int _width, int _height)
	{
		if (hConsole == INVALID_HANDLE_VALUE) return;

		nScreenHeight = _height;
		nScreenWidth = _width;

		sRectWindow = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(hConsole, TRUE, &sRectWindow);

		COORD coord = { (short)nScreenWidth, (short)nScreenHeight };
		if (!SetConsoleScreenBufferSize(hConsole, coord)) return;
		if (!SetConsoleActiveScreenBuffer(hConsole)) return;

		// Create the basic font
		CONSOLE_FONT_INFOEX ConsoleFont;
		ConsoleFont.cbSize = sizeof(ConsoleFont);
		ConsoleFont.nFont = 0;
		ConsoleFont.dwFontSize.X = 5;
		ConsoleFont.dwFontSize.Y = 5;
		ConsoleFont.FontFamily = FF_DONTCARE;
		ConsoleFont.FontWeight = FW_NORMAL;
		wcscpy_s(ConsoleFont.FaceName, L"Consolas");

		// Set the window size.
		sRectWindow = { 0, 0, (short)nScreenWidth - 1, (short)nScreenHeight - 1 };
		if (!SetConsoleWindowInfo(hConsole, TRUE, &sRectWindow)) return;

		// Enable mouse input.
		if (!SetConsoleMode(hConsole, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) return;

		// Add this later for a closing method.
		//SetConsoleCtrlHandler();
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);

		// Check for max size.
		CONSOLE_SCREEN_BUFFER_INFO cScreenB;
		if (!GetConsoleScreenBufferInfo(hConsole, &cScreenB)) return;
		if (nScreenHeight > cScreenB.dwMaximumWindowSize.Y || nScreenWidth > cScreenB.dwMaximumWindowSize.X) return;
	}

	GameEngine()
	{
		ScreenBuffer = new CHAR_INFO[nScreenWidth * nScreenHeight];
		memset(ScreenBuffer, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);


		// Grab the console.
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Initialize rendering
		Setup();
	}

	~GameEngine()
	{
		// Remove the buffer when we are done.
		delete[] ScreenBuffer;
	}

	void Draw(int x, int y, short col = 0x000F)
	{
		if (x >= 0 && x < nScreenWidth && y >= 0 && y < nScreenHeight)
		{
			ScreenBuffer[y * nScreenWidth + x].Attributes = col;
			ScreenBuffer[y * nScreenWidth + x].Char.UnicodeChar = 0x2588;
		}
	}
private:
	CHAR_INFO* ScreenBuffer;
	HANDLE hConsole;
	HANDLE hConsoleComing;
	SMALL_RECT sRectWindow;

	void Setup()
	{
		auto CurTime = std::chrono::system_clock::now();
		auto CheckTime = std::chrono::system_clock::now();

		while (true)
		{
				// Delta time / Elapsed Time
				CurTime = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = CheckTime - CurTime;
				CurTime = CheckTime;
				fDeltaTime = elapsedTime.count();

				wchar_t Title[256];
				swprintf_s(Title, 256, L"%s | FPS: %0.0f", AppName.c_str(), (1.0f / fDeltaTime) * -1);
				SetConsoleTitle(Title);
				WriteConsoleOutput(hConsole, ScreenBuffer, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &sRectWindow);
		}
	}
protected:
	static BOOL CloseHandler(DWORD evt)
	{
		return true;
	}
};