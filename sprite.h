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
//			�������Ȃ���ǂݍ��ނ͖̂������������ߊ֐��ő�p
//-------------------------------------------------------------
//
//	�ESprite�N���X�̃|�C���^���I�u�W�F�N�g�Ɏ�������
//	�EInit�ȂǂőΉ�����load�֐����Ă�
//	�E�`��֐��őΉ�����Draw�֐����Ă�
//-------------------------------------------------------------

//�^�C�g���̃V�F�[�_�[�Ɏg�p����萔�\����
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
	NONE,		//�}�X�N�Ȃ�(�f�t�H���g)
	WRITE,		//�}�X�N�l��������
	INDRAW,		//�}�X�N�̈���ɕ`��
	OUTDRAW,	//�}�X�N�̈�O�ɕ`��
	FRAME,		//�}�X�N��K�p����`��

	END,
};


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

	//�摜��ǂݍ���(�摜���T�C�Y,�摜�c�T�C�Y)
	void LoadGraph(float sw, float sh)
	{
		this->sw = sw;
		this->sh = sh;
	}
	//�摜�����ǂݍ���(�摜���T�C�Y,�摜�c�T�C�Y,�摜��������,�摜�c������,���������摜�ꖇ�̉��T�C�Y,�c�T�C�Y)
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

	//�摜�ʏ�`��(�R���e�L�X�g�A�`��ʒuX�A�`��ʒuY)
	void DrawGraph(YRShader* shader, float x, float y, SpriteMask mask = SpriteMask::NONE)
	{
		render( shader,x - (sw / 2.0f), y - (sh / 2.0f), sw, sh, sx, sy, sw, sh, 0.0f, 1, 1, 1, 1,mask);
	}
	//�摜��]�`��(�R���e�L�X�g�A�`��ʒux,y�A��]�p�x�A�摜�g�嗦)
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

	//�摜��]�`��(�摜�`��T�C�Y����^)(�R���e�L�X�g�A�`��ʒux,y�A��]�p�x�A�摜�g�嗦)
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

	//�~�`�摜�`��(�摜�`��T�C�Y����^)(�R���e�L�X�g�A�`��ʒux,y�A��]�p�x�A�~�̔��a)
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

	//�����摜�`��(�R���e�L�X�g�A�摜�̔ԍ��A�`��ʒuX,Y�A��]�p�x�A�摜�g�嗦�A�J���[)
	void DrawRotaDivGraph(
		YRShader* shader,
		int number,
		float x, float y, float angle, float size, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//number:�摜�̔ԍ�
		//x,y:�`��ʒu
		//nx,ny:�`�悷��摜�̏c�A���̕�����
		//size:�`�掞�̊g�嗦(�ʏ�T�C�Y1.0f)

		//number�ɍ��킹�ĕ`��ʒu�����肷��
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

	//�A�j���[�V�����`��Ɏg�p����^�C�}�[�����Z�b�g����
	void TimerReset()
	{
		time = 0.0f;
	}

	//�A�j���[�V�������Z�b�g
	void AnimReset()
	{
		num = 0;
	}

	//���݂̃A�j���[�V�������Ōォ�ǂ�����Ԃ�
	bool AnimFinCheck()
	{
		return num >= max - 1 ? true : false;
	}

	//�����摜�A�j���[�V�����`��(�R���e�L�X�g�A�`��ʒuX,Y�A��]�p�x�A�摜�g�嗦�A�A�j���[�V�������x�A�J���[)
	void DrawRotaDivGraph(
		YRShader* shader,
		float x, float y, float angle, float size, float frame,float elapsed_speed, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:�`��ʒu
		//nx,ny:�`�悷��摜�̏c�A���̕�����
		//size:�`�掞�̊g�嗦(�ʏ�T�C�Y1.0f)

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
		//num�ɍ��킹�ĕ`��ʒu�����肷��
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


	//�����摜�A�j���[�V�����`��(�R���e�L�X�g�A�`��ʒuX,Y�A��]�p�x�A�摜�g�嗦�A�A�j���[�V�������x�A�J���[)
	//�A�j���[�V�������Ō�ɗ�����true��Ԃ�
	bool DrawRotaDivGraphOnec(
		YRShader* shader,
		float x, float y, float angle, float size, float frame, float elapsed_speed, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:�`��ʒu
		//nx,ny:�`�悷��摜�̏c�A���̕�����
		//size:�`�掞�̊g�嗦(�ʏ�T�C�Y1.0f)

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
		//num�ɍ��킹�ĕ`��ʒu�����肷��
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


	//�����摜�A�j���[�V�����t�Đ��`��(�R���e�L�X�g�A�`��ʒuX,Y�A��]�p�x�A�摜�g�嗦�A�A�j���[�V�������x�A�J���[)
	//�A�j���[�V�������擪�ɗ�����true��Ԃ�
	bool DrawRotaDivGraphReverse(
		YRShader* shader,
		float x, float y, float angle, float size, float frame, float elapsed_speed, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:�`��ʒu
		//nx,ny:�`�悷��摜�̏c�A���̕�����
		//size:�`�掞�̊g�嗦(�ʏ�T�C�Y1.0f)

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
		//num�ɍ��킹�ĕ`��ʒu�����肷��
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

	//�����摜�`��(�V�F�[�_�[�A�`��ʒuX,Y�A��]�p�x�A�摜�g�嗦�A�摜�̔ԍ��A�J���[)
	void DrawRotaDivGraph(
		YRShader* shader,
		float x, float y, float angle, float size,int num, SpriteMask mask = SpriteMask::NONE, DirectX::XMFLOAT4 color = { 1,1,1,1 })
	{
		//x,y:�`��ʒu
		//nx,ny:�`�悷��摜�̏c�A���̕�����
		//size:�`�掞�̊g�嗦(�ʏ�T�C�Y1.0f)

		//num�ɍ��킹�ĕ`��ʒu�����肷��
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

	//�`��͈͎w��`��(�V�F�[�_�[�A�`��ʒu(x,y)�A�`�悵������`�̍�����W(x,y)�A�T�C�Y(�w�肵�����ォ��ǂ��܂�))
	//���S�����[�Ȃ̂Œ���
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
	//�摜�����ǂݍ���(�摜���T�C�Y,�摜�c�T�C�Y,�摜��������,�摜�c������,���������摜�ꖇ�̉��T�C�Y,�c�T�C�Y)
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

