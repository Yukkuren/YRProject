#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include<d3d11.h>
#include<memory.h>
#include"sprite.h"
#include"Geometric_primitive.h"
#include"GeometricPrimitive.h"

#include "misc.h"
#include "high_resolution_timer.h"
#include"static_mesh.h"
#include"Skinned_mesh.h"
#include"camera.h"
#include "Scene.h"
#include"brend.h"
#include"./imgui/imgui.h"
#include"./imgui/imgui_impl_win32.h"
#include"./imgui/imgui_impl_dx11.h"
#include"YRMouse.h"
#include"Key.h"
#include"Font.h"
#include"framebuffer.h"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

//#define SPRITE_MAX	(1024)
#ifdef  _DEBUG
#endif //


//ImGui�����S�ɏo���Ȃ��ꍇ�͂��̒�`���R�����g�A�E�g����
#define		EXIST_IMGUI

//���̊֐���ImGui��`�悷�邩�ǂ��������肷��
bool Get_Use_ImGui();

#define		FADE_MIX( x )		( x * 3.0f)
//#define		ToRadian( x )		( x * ( PI / 180.0f ) )


enum SCENE_TABLE
{
	SCENE_TITLE,
	SCENE_SELECT,
	SCENE_LOAD,
	SCENE_GAME,
	SCENE_CLEAR,
	SCENE_OVER,
	SCENE_TEST,


	SCENE_END,
};

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

	//Microsoft::WRL::ComPtr<geometric_primitive>		cube;
	//std::unique_ptr<Skinned_mesh>					skin[10];
	//std::unique_ptr<Skinned_mesh>					skin;
	//std::unique_ptr<Skinned_mesh>					skin2;
	//MeshMotion										motion;
	//MeshMotion										motion2;
	//std::unique_ptr<Skinned_mesh>					ground;

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

	SceneGame scenegame;
	SceneLoad sceneload;
	SceneTitle scenetitle;
	SceneClear sceneclear;
	SceneOver sceneover;
	SceneSelect sceneselect;
	SceneTest	scenetest;
	//SceneBase scenetutorial;

	SceneBase* scene;
	SceneBase* Nscene;

	SceneBase* scene_tbl[SCENE_END] = {
		&scenetitle,
		&sceneselect,
		&sceneload,
		&scenegame,
		&sceneclear,
		&sceneover,
		&scenetest,
	};


	void SetScene(SCENE_TABLE next)
	{
		if (next < SCENE_END)
		{
			Nscene = scene_tbl[next];
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
		/*device->Release();
		context->Release();
		chain->Release();
		view->Release();
		depth->Release();*/
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
		static int nWheelFraction = 0;	// ��]�ʂ̒[��

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
			// �E�B���h�E�̃A�N�e�B�u�؂�ւ����ɒ[�������Z�b�g
			nWheelFraction = 0;
			break;
		case WM_MOUSEWHEEL:
			//�}�E�X�z�C�[����]
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
			// �m�b�`�������߂�
			int nNotch = zDelta / WHEEL_DELTA;

			// �[����ۑ�����
			nWheelFraction = zDelta % WHEEL_DELTA;

			if (nNotch > 0)
			{
				// ���ɉ�]�i�`���g�j����
				pMouse.wheel_state++;
			}
			else if (nNotch < 0)
			{
				// ���ɉ�]�i�`���g�j����
				pMouse.wheel_state--;
			}
		}
		return 0;
		case WM_MOUSEMOVE:
			//�}�E�X�𓮂������Ƃ�
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
			//�}�E�X���{�^������
			pMouse.pos.x = static_cast<float>(LOWORD(lparam));
			pMouse.pos.y = static_cast<float> (HIWORD(lparam));
			pMouse.SetLeft(true);
			return 0;
		case WM_MBUTTONDOWN:
			//�}�E�X�z�C�[������
			pMouse.pos.x = static_cast<float>(LOWORD(lparam));
			pMouse.pos.y = static_cast<float> (HIWORD(lparam));
			pMouse.SetCursor(true);
			return 0;
		case WM_RBUTTONDOWN:
			//�}�E�X�E�{�^������
			pMouse.pos.x = static_cast<float>(LOWORD(lparam));
			pMouse.pos.y = static_cast<float> (HIWORD(lparam));
			pMouse.SetRight(true);
			return 0;
		case WM_LBUTTONUP:
			//�}�E�X���{�^��������
			pMouse.SetLeft(false);
			return 0;
		case WM_MBUTTONUP:
			//�}�E�X�z�C�[��������
			pMouse.SetCursor(false);
			return 0;
		case WM_RBUTTONUP:
			//�}�E�X�E�{�^��������
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
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);

	bool CreateRasterizerState();
	bool CreateDepthStencilState();
private:
	high_resolution_timer timer;
	void calculate_frame_stats()
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.  These stats 
		// are appended to the window caption bar.
		static int frames = 0;
		static float time_tlapsed = 0.0f;

		frames++;

		// Compute averages over one second period.
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
