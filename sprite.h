#pragma once

#include <windows.h>
#include<d3d11.h>
#include<directxmath.h>
#include <wrl.h>
#include<map>
#include<string>
#include<vector>
//#include "framework.h"

//-------------------------------------------------------------
//			生成しながら読み込むのは無理だったため関数で代用
//-------------------------------------------------------------
//
//	・Spriteクラスのポインタをオブジェクトに持たせる
//	・Initなどで対応するload関数を呼ぶ
//	・描画関数で対応したDraw関数を呼ぶ
//-------------------------------------------------------------

struct Sprite_div
{
	float nx;						//分割した画像の始点X
	float ny;						//分割した画像の始点Y
};

class Sprite
{
public:
	std::vector<Sprite_div>div;		//分割画像座標保存用変数
	int max;						//分割最大数
	float sx = 0.0f;				//画像始点X(右上)
	float sy = 0.0f;				//画像始点Y(右上)
	float sw;						//画像横サイズ
	float sh;						//画像縦サイズ
	float nsx;						//分割した一枚の大きさX
	float nsy;						//分割した一枚の大きさY
	float numX;						//画像横分割数
	float numY;						//画像縦分割数
	int	num = 0;
	int time = 0;
public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vert;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rastersize;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler;
	D3D11_TEXTURE2D_DESC								texture2d_desc;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthstate;
	//ID3D11BlendState *blendstate;
	//vertex vertics[4];

	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcord;
	};


	void render(
		//UNIT.03
		float dx, float dy,				//Coordinate of sprite's left-top corner in screen space
		float dw, float dh,				//Size of sprite in screen space
		float sx, float sy,				//Coordinate of sprite's left-top corner in texture space
		float sw, float sh,				//Sizeof sprite in texture space
		float angle,					//Raotation angle (Rotation centre is sprite's center),Unit is degree
		float r,float g,float b,	//Color of sprite's each vertices
		float a
	);
	//画像を読み込む(画像横サイズ,画像縦サイズ)
	void LoadGraph(float sw, float sh)
	{
		this->sw = sw;
		this->sh = sh;
	}
	//画像分割読み込み(画像横サイズ,画像縦サイズ,画像横分割数,画像縦分割数,分割した画像一枚の横サイズ,縦サイズ)
	void LoadDivGraph(float sw, float sh, int numX, int numY, int n_x, int n_y)
	{
		this->sw = sw;
		this->sh = sh;
		max = numX * numY;
		div.resize(max);
		int v = 0;
		for (int i = 0; i < max; i++)
		{
			div[i].ny = static_cast<float>(i / numX)* n_y;
			div[i].nx = static_cast<float>(v)* n_x;
			v++;
			if (v >= numX)
			{
				v = 0;
			}
		}
		this->numX = static_cast<float>(numX);
		this->numY = static_cast<float>(numY);
		nsx = static_cast<float>(n_x);
		nsy = static_cast<float>(n_y);
	}

	//画像通常描画(コンテキスト、描画位置X、描画位置Y)
	void DrawGraph(float x, float y)
	{
		render( x - (sw / 2.0f), y - (sh / 2.0f), sw, sh, sx, sy, sw, sh, 0.0f, 1, 1, 1, 1);
	}
	//画像回転描画(コンテキスト、描画位置x,y、回転角度、画像拡大率)
	void DrawRotaGraph(float x, float y, float angle, float size, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		render(
			x - (sw / 2 * size),
			y - (sh / 2 * size),
			sw * size, sh * size,
			0,
			0,
			sw,
			sh,
			angle,
			color.x,
			color.y,
			color.z,
			color.w);
	}

	//画像回転描画(画像描画サイズ決定型)(コンテキスト、描画位置x,y、回転角度、画像拡大率)
	void DrawRotaGraph(
		float x, float y, float angle,
		DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		render(
			x - (size.x / 2.0f),
			y - (size.y / 2.0f),
			size.x, size.y,
			0,
			0,
			sw,
			sh,
			angle,
			color.x,
			color.y,
			color.z,
			color.w);
	}
	//円形画像描画(画像描画サイズ決定型)(コンテキスト、描画位置x,y、回転角度、円の半径)
	void DrawCircleGraph(
		float x, float y, float angle,
		float size, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		render(
			x - size,
			y - size,
			size * 2.0f, size * 2.0f,
			0,
			0,
			sw,
			sh,
			angle,
			color.x,
			color.y,
			color.z,
			color.w);
	}

	//分割画像描画(コンテキスト、画像の番号、描画位置X,Y、回転角度、画像拡大率、カラー)
	void DrawRotaDivGraph(
		int number,
		float x, float y, float angle, float size, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//number:画像の番号
		//x,y:描画位置
		//nx,ny:描画する画像の縦、横の分割数
		//size:描画時の拡大率(通常サイズ1.0f)

		//numberに合わせて描画位置を決定する
		render(
			x - (nsx / 2 * size),
			y - (nsy / 2 * size),
			nsx * size,
			nsx * size,
			div[number].nx,
			div[number].ny,
			nsx,
			nsy,
			angle,
			color.x,
			color.y,
			color.z,
			color.w
		);

	}

	//分割画像アニメーション描画(コンテキスト、描画位置X,Y、回転角度、画像拡大率、アニメーション速度、カラー)
	void DrawRotaDivGraph(
		float x, float y, float angle, float size, int frame, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:描画位置
		//nx,ny:描画する画像の縦、横の分割数
		//size:描画時の拡大率(通常サイズ1.0f)

		/*static int num = 0;
		static int time = 0;*/
		time++;
		if (time > frame)
		{
			num++;
			time = 0;
			if (num >= max)
			{
				num = 0;
			}
		}
		//numに合わせて描画位置を決定する
		render(
			x - (nsx / 2 * size),
			y - (nsy / 2 * size),
			nsx * size,
			nsx * size,
			div[num].nx,
			div[num].ny,
			nsx,
			nsy,
			angle,
			color.x,
			color.y,
			color.z,
			color.w
		);

	}
	Sprite(const wchar_t* wchar/*Texture file name*/);
	Sprite(const wchar_t* wchar, float sw, float sh);
	Sprite(const wchar_t* wchar, float sw, float sh, int numX, int numY, int n_x, int n_y);
	void Init(const wchar_t* whar);
	~Sprite();
	
	
};

namespace TextureALL
{
	static std::map < std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > tex;
	HRESULT TextureLoad(ID3D11Device *device, const wchar_t* wchar, D3D11_TEXTURE2D_DESC *texture2d_desc, ID3D11ShaderResourceView **pShader);
	HRESULT create_vertex_file(ID3D11Device *device, const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input);
	HRESULT CreatePixel_files(ID3D11Device *device, const char *ps_file, ID3D11PixelShader **pixel);

}

