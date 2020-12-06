#pragma once

#include <windows.h>
#include<directxmath.h>
#include "YRShader.h"
#include<map>
#include<string>
#include "Texture.h"
#include "Sampler.h"
//#include "framework.h"

//-------------------------------------------------------------
//			生成しながら読み込むのは無理だったため関数で代用
//-------------------------------------------------------------
//
//	・Spriteクラスのポインタをオブジェクトに持たせる
//	・Initなどで対応するload関数を呼ぶ
//	・描画関数で対応したDraw関数を呼ぶ
//-------------------------------------------------------------

//タイトルのシェーダーに使用する定数構造体
struct Title_CBuffer
{
public:
	DirectX::XMFLOAT3		Resolution;
	float					iTime;
	float					brightness;
	float					ray_brightness;
	float					gamma;
	float					spot_brightness;
	float					ray_density;
	float					curvature;
	float					red;
	float					green;
	float					blue;
	DirectX::XMFLOAT4		material_color;
	float					dummy1;
	float					dummy2;
	float					dummy3;
};

enum class SpriteMask
{
	NONE,		//マスクなし(デフォルト)
	WRITE,		//マスク値書き込み
	INDRAW,		//マスク領域内に描画
	OUTDRAW,	//マスク領域外に描画
	FRAME,		//マスクを適用する描画

	END,
};


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
	float time = 0;
	//std::shared_ptr<Texture> texture = nullptr;
public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vert;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rastersize;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler;
	D3D11_TEXTURE2D_DESC								texture2d_desc;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthstate[static_cast<int>(SpriteMask::END)];
	//ID3D11BlendState *blendstate;
	//vertex vertics[4];

	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcord;
	};

	struct vertex_tex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
		DirectX::XMFLOAT4 Color;
	};

	void render(
		YRShader	*shader,
		float dx, float dy,				//Coordinate of sprite's left-top corner in screen space
		float dw, float dh,				//Size of sprite in screen space
		float sx, float sy,				//Coordinate of sprite's left-top corner in texture space
		float sw, float sh,				//Sizeof sprite in texture space
		float angle,					//Raotation angle (Rotation centre is sprite's center),Unit is degree
		float r,float g,float b,		//Color of sprite's each vertices
		float a,
		SpriteMask mask = SpriteMask::NONE
	);

	void render(
		YRShader*	shader,
		Texture*	 tex,
		float	dx, float	dy,
		float	dw, float	dh,
		float	sx, float	sy,
		float	sw, float	sh,
		float		angle,
		float		alpha
	);

	void render(
		YRShader* shader,
		Texture* tex0,
		Title_CBuffer cbuffer_param,
		Sampler* sampler_clamp,
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
		float	dx, float	dy,
		float	dw, float	dh,
		float	sx, float	sy,
		float	sw, float	sh,
		float		angle,
		float		alpha
	);

	void render(
		YRShader* shader,
		Texture* tex,
		ID3D11ShaderResourceView*const* srv,
		int		srv_size,
		float	dx, float	dy,
		float	dw, float	dh,
		float	sx, float	sy,
		float	sw, float	sh,
		float		angle,
		float		alpha
	);

	//画像を読み込む(画像横サイズ,画像縦サイズ)
	void LoadGraph(float sw, float sh)
	{
		this->sw = sw;
		this->sh = sh;
	}
	//画像分割読み込み(画像横サイズ,画像縦サイズ,画像横分割数,画像縦分割数,分割した画像一枚の横サイズ,縦サイズ)
	void LoadDivGraph(float sw, float sh, int numX, int numY, float n_x, float n_y, int max_num = 0)
	{
		this->sw = sw;
		this->sh = sh;
		if (max_num != 0)
		{
			max = max_num;
		}
		else
		{
			max = numX * numY;
		}
		div.resize(max);
		int v = 0;
		for (int i = 0; i < max; i++)
		{
			div[i].ny = static_cast<float>(i / numX) * n_y;
			div[i].nx = static_cast<float>(v) * n_x;
			v++;
			if (v >= numX)
			{
				v = 0;
			}
		}
		this->numX = static_cast<float>(numX);
		this->numY = static_cast<float>(numY);
		nsx = n_x;
		nsy = n_y;
	}

	//画像通常描画(コンテキスト、描画位置X、描画位置Y)
	void DrawGraph(YRShader* shader, float x, float y, SpriteMask mask = SpriteMask::NONE)
	{
		render( shader,x - (sw / 2.0f), y - (sh / 2.0f), sw, sh, sx, sy, sw, sh, 0.0f, 1, 1, 1, 1,mask);
	}
	//画像回転描画(コンテキスト、描画位置x,y、回転角度、画像拡大率)
	void DrawRotaGraph(YRShader* shader, float x, float y, float angle, float size, bool reverse=false, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		float ssx = 0.0f;
		float ssw = sw;
		if (reverse)
		{
			ssx = sw;
			ssw = -sw;
		}

		render(
			shader,
			x - (sw / 2 * size),
			y - (sh / 2 * size),
			sw * size, sh * size,
			ssx,
			0.0f,
			ssw,
			sh,
			angle,
			color.x,
			color.y,
			color.z,
			color.w,
			mask);
	}

	//画像回転描画(画像描画サイズ決定型)(コンテキスト、描画位置x,y、回転角度、画像拡大率)
	void DrawRotaGraph(
		YRShader* shader,
		float x, float y, float angle,
		DirectX::XMFLOAT2 size, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		render(
			shader,
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
			color.w,
			mask);
	}


	void DrawRotaGraph(
		YRShader* shader,
		float x, float y, float angle,
		DirectX::XMFLOAT2 size,bool reverse, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		float ssx = 0.0f;
		float ssy = 0.0f;
		float ssw = sw;
		float ssh = sh;
		if (reverse)
		{
			ssx = sw;
			ssy = sh;
			ssw = -sw;
			ssh = -sh;
		}

		render(
			shader,
			x - (size.x / 2.0f),
			y - (size.y / 2.0f),
			size.x, size.y,
			ssx,
			ssy,
			ssw,
			ssh,
			angle,
			color.x,
			color.y,
			color.z,
			color.w,
			mask);
	}

	//円形画像描画(画像描画サイズ決定型)(コンテキスト、描画位置x,y、回転角度、円の半径)
	void DrawCircleGraph(
		YRShader* shader,
		float x, float y, float angle,
		float size, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		render(
			shader,
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
		YRShader* shader,
		int number,
		float x, float y, float angle, float size, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//number:画像の番号
		//x,y:描画位置
		//nx,ny:描画する画像の縦、横の分割数
		//size:描画時の拡大率(通常サイズ1.0f)

		//numberに合わせて描画位置を決定する
		render(
			shader,
			x - (nsx / 2 * size),
			y - (nsy / 2 * size),
			nsx * size,
			nsy * size,
			div[number].nx,
			div[number].ny,
			nsx,
			nsy,
			angle,
			color.x,
			color.y,
			color.z,
			color.w,
			mask
		);

	}

	//アニメーション描画に使用するタイマーをリセットする
	void TimerReset()
	{
		time = 0.0f;
	}

	//アニメーションリセット
	void AnimReset()
	{
		num = 0;
	}

	//現在のアニメーションが最後かどうかを返す
	bool AnimFinCheck()
	{
		return num >= max - 1 ? true : false;
	}

	//分割画像アニメーション描画(コンテキスト、描画位置X,Y、回転角度、画像拡大率、アニメーション速度、カラー)
	void DrawRotaDivGraph(
		YRShader* shader,
		float x, float y, float angle, float size, float frame,float elapsed_speed, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:描画位置
		//nx,ny:描画する画像の縦、横の分割数
		//size:描画時の拡大率(通常サイズ1.0f)

		/*static int num = 0;
		static int time = 0;*/
		time += elapsed_speed;
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
			shader,
			x - (nsx / 2 * size),
			y - (nsy / 2 * size),
			nsx * size,
			nsy * size,
			div[num].nx,
			div[num].ny,
			nsx,
			nsy,
			angle,
			color.x,
			color.y,
			color.z,
			color.w,
			mask
		);

	}


	//分割画像アニメーション描画(コンテキスト、描画位置X,Y、回転角度、画像拡大率、アニメーション速度、カラー)
	//アニメーションが最後に来たらtrueを返す
	bool DrawRotaDivGraphOnec(
		YRShader* shader,
		float x, float y, float angle, float size, float frame, float elapsed_speed, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:描画位置
		//nx,ny:描画する画像の縦、横の分割数
		//size:描画時の拡大率(通常サイズ1.0f)

		/*static int num = 0;
		static int time = 0;*/
		time += elapsed_speed;
		if (time > frame)
		{
			if (num < max - 1)
			{
				num++;
			}
			time = 0;
		}
		//numに合わせて描画位置を決定する
		render(
			shader,
			x - (nsx / 2 * size),
			y - (nsy / 2 * size),
			nsx * size,
			nsy * size,
			div[num].nx,
			div[num].ny,
			nsx,
			nsy,
			angle,
			color.x,
			color.y,
			color.z,
			color.w,
			mask
		);

		if (num >= max - 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	//分割画像アニメーション逆再生描画(コンテキスト、描画位置X,Y、回転角度、画像拡大率、アニメーション速度、カラー)
	//アニメーションが先頭に来たらtrueを返す
	bool DrawRotaDivGraphReverse(
		YRShader* shader,
		float x, float y, float angle, float size, float frame, float elapsed_speed, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:描画位置
		//nx,ny:描画する画像の縦、横の分割数
		//size:描画時の拡大率(通常サイズ1.0f)

		/*static int num = 0;
		static int time = 0;*/
		time += elapsed_speed;
		if (time > frame)
		{
			if (num > 0)
			{
				num--;
			}
			time = 0.0f;
		}
		//numに合わせて描画位置を決定する
		render(
			shader,
			x - (nsx / 2 * size),
			y - (nsy / 2 * size),
			nsx * size,
			nsy * size,
			div[num].nx,
			div[num].ny,
			nsx,
			nsy,
			angle,
			color.x,
			color.y,
			color.z,
			color.w,
			mask
		);
		if (num == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	//分割画像描画(シェーダー、描画位置X,Y、回転角度、画像拡大率、画像の番号、カラー)
	void DrawRotaDivGraph(
		YRShader* shader,
		float x, float y, float angle, float size,int num, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:描画位置
		//nx,ny:描画する画像の縦、横の分割数
		//size:描画時の拡大率(通常サイズ1.0f)

		//numに合わせて描画位置を決定する
		render(
			shader,
			x - (nsx / 2 * size),
			y - (nsy / 2 * size),
			nsx * size,
			nsy * size,
			div[num].nx,
			div[num].ny,
			nsx,
			nsy,
			angle,
			color.x,
			color.y,
			color.z,
			color.w,
			mask
		);

	}

	//描画範囲指定描画(シェーダー、描画位置(x,y)、描画したい矩形の左上座標(x,y)、サイズ(指定した左上からどこまで))
	//中心が左端なので注意
	void DrawRectGraph(YRShader* shader, float x, float y, float srcX, float srcY, float width, float height, bool reverse = false, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		float ssx = srcX;
		float ssw = width;
		if (reverse)
		{
			ssx = srcX;
			ssw = -width;
		}
		render(shader, x, y, width, height, ssx, srcY, ssw, height, 0.0f, color.x, color.y, color.z, color.w,mask);
	}

	void DrawExtendGraph(YRShader* shader, float x, float y, float x2, float y2, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })
	{
		float width = x2 - x;
		float height = y2 - y;
		render(shader, x, y, width, height, sx, sy, sw, sh, 0.0f, color.x, color.y, color.z, color.w,mask);
	}

	void DrawExtendSetGraph(YRShader* shader, float x, float y, float x2, float y2, float sx, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })
	{
		float width = x2 - x;
		float height = y2 - y;
		render(shader, x, y, width, height, sx, sy, sw, sh, 0.0f, color.x, color.y, color.z, color.w, mask);
	}


	void DrawExtendAnimGraph(YRShader* shader, float x, float y, float x2, float y2, float speed, float elapsed_time, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })
	{
		sx += (speed * elapsed_time);
		float width = x2 - x;
		float height = y2 - y;
		if (sx < -sw)
		{
			sx = 0.0f;
		}
		if (sx > sw)
		{
			sx = 0.0f;
		}
		render(shader, x, y, width, height, sx, sy, sw, sh, 0.0f, color.x, color.y, color.z, color.w, mask);
	}

	Sprite();
	Sprite(const wchar_t* wchar/*Texture file name*/);
	Sprite(const wchar_t* wchar, float sw, float sh);
	//画像分割読み込み(画像横サイズ,画像縦サイズ,画像横分割数,画像縦分割数,分割した画像一枚の横サイズ,縦サイズ)
	Sprite(
		const wchar_t* wchar,
		float sw,
		float sh,
		int numX,
		int numY,
		float n_x,
		float n_y,
		int max_num = 0);
	void Init(const wchar_t* whar);
	~Sprite();
};

namespace TextureALL
{
	static std::map < std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > tex;
	HRESULT TextureLoad(ID3D11Device *device, const wchar_t* wchar, D3D11_TEXTURE2D_DESC *texture2d_desc, ID3D11ShaderResourceView **pShader);
	/*HRESULT create_vertex_file(ID3D11Device *device, const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input);
	HRESULT CreatePixel_files(ID3D11Device *device, const char *ps_file, ID3D11PixelShader **pixel);*/

}

