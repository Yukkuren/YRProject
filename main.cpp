#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>
#define _CRTDBG_MAP_ALLOC						// mallocによるメモリリーク検出でCPPファイル名と行数出力指定
#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__)	// new によるメモリリーク検出でCPPファイル名と行数出力指定
#include <stdio.h>
#include <crtdbg.h>

#include "framework.h"

#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_win32.h"
#include "./imgui/imgui_impl_dx11.h"
#include <d3d11.h>
//別スレッド用インクルード
#include <roapi.h>
#include <windows.foundation.h>
#include "Resource.h"

#pragma comment(lib,"runtimeobject.lib")

LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	framework *f = reinterpret_cast<framework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return f ? f->handle_message(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtDumpMemoryLeaks();
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_crtBreakAlloc = 277571;
#else
#define DBG_NEW new
#endif

	HRESULT hr = Windows::Foundation::Initialize(RO_INIT_MULTITHREADED);
	if (FAILED(hr))
	{
		assert(!"multi threed error");
		return -1;
	}

	srand((unsigned int)time(NULL));

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = fnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("Desastal");
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, framework::SCREEN_WIDTH, framework::SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindow(_T("Desastal"), _T(""), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, -8, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	ShowWindow(hwnd, cmd_show);
	UpdateWindow(hwnd);

#ifdef EXIST_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
#endif

	framework& f = framework::getInstance(hwnd);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));
#ifdef EXIST_IMGUI
	ImGui_ImplWin32_Init(hwnd);
#endif
	return f.run();
}
