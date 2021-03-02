#pragma once

#include<d3d11.h>

#define BLENDMAX (8)

//-----------------------------------------------------------------
//			�u�����h�N���X
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
		NONE,		//�u�����h�Ȃ�
		ALPHA,		//�A���t�@�u�����h
		ADD,		//���Z
		SUBTRACT,	//���Z
		REPLACE,	//
		MULTIPLY,	//��Z

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
