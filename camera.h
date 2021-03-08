#pragma once

#include <DirectXMath.h>
#include "YRShader.h"
//#include "YR_VectorMaster.h"

//-----------------------------------------------------------------
//			�J�����N���X
//-----------------------------------------------------------------
//�ESet�֐��Œl���Z�b�g������Active�Ōv�Z���s��
//-----------------------------------------------------------------

class Camera
{
private:
	DirectX::XMFLOAT4X4		view =
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f };		//����ϊ��s��
	DirectX::XMFLOAT4X4		projection =
	{
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f
	};	//���e�ϊ��s��
	DirectX::XMFLOAT3		eye = { 0.0f,0.0f,0.0f };	//���_
	DirectX::XMFLOAT3		focus = { 0.0f,0.0f,0.0f };		//�����_
	DirectX::XMFLOAT3		up = { 0.0f,0.0f,0.0f };			//��x�N�g��
	float					fov = 0.0f;		//��p
	float					aspect = 0.0f;		//�A�X�y�N�g��
	float					nearZ = 0.0f;		//�j�A�N���b�v�ʂ܂ł̋���
	float					farZ = 0.0f;		//�t�@�[�N���b�v�ʂ܂ł̋���

	DirectX::XMFLOAT2		start_Pos = { 0.0f,0.0f };	//�J�����ړ��J�n���ɃJ�[�\���̈ʒu��ۑ�����ϐ�

	DirectX::XMFLOAT3		at = { 0.0f,0.0f,0.0f };

	Camera() {};
public:

	float viblate_timer = 0.0f;

	int damage_pl_num = 0;

	DirectX::XMFLOAT3		targetPos = { 0.0f,0.0f,0.0f };		//�����_

	enum class CAMERA_STATE :int
	{
		MAIN = 0,
		PLAYER1P,
		PLAYER2P,
		ZOOM_CAMERA,
		END_GAME,
	};
	CAMERA_STATE camera_state = CAMERA_STATE::MAIN;

	enum  class Request : int
	{
		NONE,
		HOLD,		//�J����������(�����̃X�e�[�g�ɂ���)
		RELEASE,	//�J�����𗣂�(�X�e�[�g��MAIN�ɑ����ɖ߂�)
		WEAKEN,		//�J�������������߂�(�X�e�[�g�͖߂����A�ʒu����������MAIN�ɖ߂�)
		ZOOM,		//�J�������Y�[��������
	};
private:
	Request		camera_Request = Request::NONE;
	int			request_player = 0;
public:

	void RequestCamera(Request request, int now_player);
	void RequestCamera(int damage_player);
	const Request& GetRequest()const;
	const int& GetRequestPlayer()const;

	//�s��X�V
	void Active();

	//setter
	void SetPerspective(float fov, float aspect,
		float nearZ, float farZ);
	void SetEye(const DirectX::XMFLOAT3& eye);
	void SetFocus(const DirectX::XMFLOAT3& focus);
	void SetUp(const DirectX::XMFLOAT3& up);
	void SetFov(const float fov);

	void CameraMove(YRShader* shader);

	//�w�肵���ʒu�܂ŃJ�����̓�����⊮�ړ�����
	void SpecifiedLerp(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,float factor);

	//getter
	const DirectX::XMFLOAT4X4& GetView()const;
	const DirectX::XMFLOAT4X4& GetProjection()const;
	const DirectX::XMFLOAT3& GetEye()const;
	const DirectX::XMFLOAT3& GetFocus()const;
	const DirectX::XMFLOAT3& GetUp()const;
	const float& GetFov()const;
	const float& GetAspect()const;
	const float& GetNear()const;
	const float& GetFar()const;



	const DirectX::XMFLOAT3& GetAt()const;

	static Camera &getInstance()
	{
		static Camera instance;
		return instance;
	}
};


#define YRCamera		(Camera::getInstance())