#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include<d3d11.h>
#include<memory.h>
#include"sprite.h"
#include"Model/Geometric_primitive.h"
//#include"GeometricPrimitive.h"

#include "misc.h"
#include "high_resolution_timer.h"
#include"static_mesh.h"
#include"Model/Skinned_mesh.h"
#include"camera.h"
#include "Scene/Scene.h"
#include"brend.h"
#include"../imgui/imgui.h"
#include"../imgui/imgui_impl_win32.h"
#include"../imgui/imgui_impl_dx11.h"
#include"YRMouse.h"
#include"Key.h"
#include"Font.h"
#include"framebuffer.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);


//ImGuiを完全に出さない場合はこの定義をコメントアウトする
//その場合、デバッグ表示に使用するメモリを自動的に確保しないようになる
#define		EXIST_IMGUI

//この関数でImGuiを描画するかどうかを決定する
bool Get_Use_ImGui();
bool Get_Debug_Draw();

//フェードアウト、インを行う際に使用
#define		FADE_MIX( x )		( x * 3.0f)

//シーンテーブル
enum class SCENE_TABLE : int
{
	SCENE_TITLE,
	SCENE_SELECT,
	SCENE_LOAD,
	SCENE_GAME,
	SCENE_TEST,


	SCENE_END,
};

//-----------------------------------------------------------------
//			フレームワーククラス
//-----------------------------------------------------------------
//・全てのシーンの情報を保存し、このクラスを経由して他のシーンの情報を参照することができる
//-----------------------------------------------------------------

class framework
{
public:
	CONST HWND hwnd;
	static CONST LONG SCREEN_WIDTH = 1920;
	static CONST LONG SCREEN_HEIGHT = 1080;

	std::unique_ptr<Sprite>							fade_img = nullptr;

	std::unique_ptr<Font> font = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth;

	framebuffer										framebuffer;

	static const int Rasterizer_Size = 4;
	enum
	{
		RS_CULL_BACK = 0,
		RS_CULL_FRONT,
		RS_WIRE,
		RS_NONE,
	};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state[Rasterizer_Size];

	static const int Depth_Size = 3;
	enum
	{
		DS_FALSE,
		DS_TRUE,
		DS_WRITE_FALSE
	};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthstencil_state[Depth_Size];



	//各シーンクラス
	SceneGame scenegame;
	SceneLoad sceneload;
	SceneTitle scenetitle;
	SceneSelect sceneselect;
	SceneTest	scenetest;

	SceneBase* scene = nullptr;		//現在のシーン
	SceneBase* Nscene = nullptr;	//次のシーン

	//シーンテーブルにシーンを設定している
	SceneBase* scene_tbl[static_cast<int>(SCENE_TABLE::SCENE_END)] = {
		&scenetitle,
		&sceneselect,
		&sceneload,
		&scenegame,
		&scenetest,
	};


	//シーン切り替え
	//設定したシーンが存在すれば切り替えて初期化を行う
	void SetScene(SCENE_TABLE next)
	{
		if (next < SCENE_TABLE::SCENE_END)
		{
			Nscene = scene_tbl[static_cast<int>(next)];
		}

		if (Nscene) {
			scene = Nscene;
			Nscene = nullptr;
		}
		scene->Init();
		//scene->Update(timer.time_interval());
	};

	void SetViewPort(float width,float height);

	bool CreateConstantBuffer(ID3D11Buffer** pCB, u_int size);

	static framework& getInstance(HWND hwnd = nullptr)
	{
		static framework instance(hwnd);
		return instance;
	}

	void Clear(DWORD color);

	framework(HWND hwnd) : hwnd(hwnd)
	{

	}
	~framework()
	{
		scene->UnInit();


		YRBlend.Uninit();
#ifdef EXIST_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif
	}
	int run()
	{
		MSG msg = {};

		if (!initialize()) return 0;
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
#ifdef EXIST_IMGUI
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
#endif
				timer.tick();
				calculate_frame_stats();
				pKeyState.CheckKeyState();
				update(timer.time_interval());
				render(timer.time_interval());
			}
		}
		return static_cast<int>(msg.wParam);
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
#ifdef EXIST_IMGUI

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		{
			return 1;
		}
#endif
		static int nWheelFraction = 0;	// 回転量の端数

		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case WM_ENTERSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
			timer.stop();
			break;
		case WM_EXITSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
			timer.start();
			break;
		case WM_ACTIVATE:
			// ウィンドウのアクティブ切り替え時に端数をリセット
			nWheelFraction = 0;
			break;
		case WM_MOUSEWHEEL:
			//マウスホイール回転
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
			// ノッチ数を求める
			int nNotch = zDelta / WHEEL_DELTA;

			// 端数を保存する
			nWheelFraction = zDelta % WHEEL_DELTA;

			if (nNotch > 0)
			{
				// ↑に回転（チルト）した
				pMouse.wheel_state++;
			}
			else if (nNotch < 0)
			{
				// ↓に回転（チルト）した
				pMouse.wheel_state--;
			}
		}
		return 0;
		case WM_MOUSEMOVE:
			//マウスを動かしたとき
			pMouse.pos.x = static_cast<float>(LOWORD(lparam));
			pMouse.pos.y = static_cast<float> (HIWORD(lparam));
			if (wparam & MK_LBUTTON)
			{
				pMouse.SetLeft(true);
			}
			else
			{
				pMouse.SetLeft(false);
			}
			if (wparam & MK_RBUTTON)
			{
				pMouse.SetRight(true);
			}
			else
			{
				pMouse.SetRight(false);
			}
			if (wparam & MK_MBUTTON)
			{
				pMouse.SetCursor(true);
			}
			else
			{
				pMouse.SetCursor(false);
			}
			return 0;
		case WM_LBUTTONDOWN:
			//マウス左ボタン押下
			pMouse.pos.x = static_cast<float>(LOWORD(lparam));
			pMouse.pos.y = static_cast<float> (HIWORD(lparam));
			pMouse.SetLeft(true);
			return 0;
		case WM_MBUTTONDOWN:
			//マウスホイール押下
			pMouse.pos.x = static_cast<float>(LOWORD(lparam));
			pMouse.pos.y = static_cast<float> (HIWORD(lparam));
			pMouse.SetCursor(true);
			return 0;
		case WM_RBUTTONDOWN:
			//マウス右ボタン押下
			pMouse.pos.x = static_cast<float>(LOWORD(lparam));
			pMouse.pos.y = static_cast<float> (HIWORD(lparam));
			pMouse.SetRight(true);
			return 0;
		case WM_LBUTTONUP:
			//マウス左ボタン離した
			pMouse.SetLeft(false);
			return 0;
		case WM_MBUTTONUP:
			//マウスホイール離した
			pMouse.SetCursor(false);
			return 0;
		case WM_RBUTTONUP:
			//マウス右ボタン離した
			pMouse.SetRight(false);
			return 0;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

	void BlendSet(int blendstate)
	{
		context->OMSetBlendState(YRBlend.blendstate[blendstate], NULL, 0xffffffff);
	}


private:
	bool initialize();
	void update(float elapsed_time);
	void render(float elapsed_time);

	bool CreateRasterizerState();
	bool CreateDepthStencilState();
private:
	high_resolution_timer timer;
	void calculate_frame_stats()
	{
		static int frames = 0;
		static float time_tlapsed = 0.0f;

		frames++;

		//FPSの表示
		if ((timer.time_stamp() - time_tlapsed) >= 1.0f)
		{
			float fps = static_cast<float>(frames); // fps = frameCnt / 1
			float mspf = 1000.0f / fps;
			std::ostringstream outs;
			outs.precision(6);
			outs << "Desastal";
			outs << "  FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
			SetWindowTextA(hwnd, outs.str().c_str());
			// Reset for next average.
			frames = 0;
			time_tlapsed += 1.0f;
		}
	}
};

#define FRAMEWORK		(framework::getInstance())
