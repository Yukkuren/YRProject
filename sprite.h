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
//			�������Ȃ���ǂݍ��ނ͖̂������������ߊ֐��ő�p
//-------------------------------------------------------------
//
//	�ESprite�N���X�̃|�C���^���I�u�W�F�N�g�Ɏ�������
//	�EInit�ȂǂőΉ�����load�֐����Ă�
//	�E�`��֐��őΉ�����Draw�֐����Ă�
//-------------------------------------------------------------

struct Sprite_div
{
	float nx;						//���������摜�̎n�_X
	float ny;						//���������摜�̎n�_Y
};

class Sprite
{
public:
	std::vector<Sprite_div>div;		//�����摜���W�ۑ��p�ϐ�
	int max;						//�����ő吔
	float sx = 0.0f;				//�摜�n�_X(�E��)
	float sy = 0.0f;				//�摜�n�_Y(�E��)
	float sw;						//�摜���T�C�Y
	float sh;						//�摜�c�T�C�Y
	float nsx;						//���������ꖇ�̑傫��X
	float nsy;						//���������ꖇ�̑傫��Y
	float numX;						//�摜��������
	float numY;						//�摜�c������
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
	//�摜��ǂݍ���(�摜���T�C�Y,�摜�c�T�C�Y)
	void LoadGraph(float sw, float sh)
	{
		this->sw = sw;
		this->sh = sh;
	}
	//�摜�����ǂݍ���(�摜���T�C�Y,�摜�c�T�C�Y,�摜��������,�摜�c������,���������摜�ꖇ�̉��T�C�Y,�c�T�C�Y)
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

	//�摜�ʏ�`��(�R���e�L�X�g�A�`��ʒuX�A�`��ʒuY)
	void DrawGraph(float x, float y)
	{
		render( x - (sw / 2.0f), y - (sh / 2.0f), sw, sh, sx, sy, sw, sh, 0.0f, 1, 1, 1, 1);
	}
	//�摜��]�`��(�R���e�L�X�g�A�`��ʒux,y�A��]�p�x�A�摜�g�嗦)
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

	//�摜��]�`��(�摜�`��T�C�Y����^)(�R���e�L�X�g�A�`��ʒux,y�A��]�p�x�A�摜�g�嗦)
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
	//�~�`�摜�`��(�摜�`��T�C�Y����^)(�R���e�L�X�g�A�`��ʒux,y�A��]�p�x�A�~�̔��a)
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

	//�����摜�`��(�R���e�L�X�g�A�摜�̔ԍ��A�`��ʒuX,Y�A��]�p�x�A�摜�g�嗦�A�J���[)
	void DrawRotaDivGraph(
		int number,
		float x, float y, float angle, float size, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//number:�摜�̔ԍ�
		//x,y:�`��ʒu
		//nx,ny:�`�悷��摜�̏c�A���̕�����
		//size:�`�掞�̊g�嗦(�ʏ�T�C�Y1.0f)

		//number�ɍ��킹�ĕ`��ʒu�����肷��
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

	//�����摜�A�j���[�V�����`��(�R���e�L�X�g�A�`��ʒuX,Y�A��]�p�x�A�摜�g�嗦�A�A�j���[�V�������x�A�J���[)
	void DrawRotaDivGraph(
		float x, float y, float angle, float size, int frame, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:�`��ʒu
		//nx,ny:�`�悷��摜�̏c�A���̕�����
		//size:�`�掞�̊g�嗦(�ʏ�T�C�Y1.0f)

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
		//num�ɍ��킹�ĕ`��ʒu�����肷��
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

