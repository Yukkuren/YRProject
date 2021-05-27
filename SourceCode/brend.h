#pragma once

#include<d3d11.h>

#define BLENDMAX (8)

//-----------------------------------------------------------------
//			ブレンドクラス
//-----------------------------------------------------------------

class Blend
{
private:
	Blend()
	{
	}
	~Blend()
	{

	}
public:
	ID3D11BlendState* blendstate[BLENDMAX] = { nullptr };

	enum
	{
		NONE,		//ブレンドなし
		ALPHA,		//アルファブレンド
		ADD,		//加算
		SUBTRACT,	//減算
		REPLACE,	//
		MULTIPLY,	//乗算

	};

	void Init()
	{
		for (int i = 0; i < BLENDMAX; i++)
		{
			blendstate[i] = nullptr;
		}
	}

	void Uninit()
	{
		for (int i = 0; i < BLENDMAX; i++)
		{
			blendstate[i]->Release();
		}
	}

	static Blend &getInstance()
	{
		static Blend instance;
		return instance;
	}
};

#define YRBlend		(Blend::getInstance())
