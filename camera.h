#pragma once

#include <DirectXMath.h>
#include "YRShader.h"

class Camera
{
private:
	DirectX::XMFLOAT4X4		view;		//����ϊ��s��
	DirectX::XMFLOAT4X4		projection;	//���e�ϊ��s��
	DirectX::XMFLOAT3		eye;		//���_
	DirectX::XMFLOAT3		focus;		//�����_
	DirectX::XMFLOAT3		up;			//��x�N�g��
	float					fov;		//��p
	float					aspect;		//�A�X�y�N�g��
	float					nearZ;		//�j�A�N���b�v�ʂ܂ł̋���
	float					farZ;		//�t�@�[�N���b�v�ʂ܂ł̋���



	DirectX::XMFLOAT2		start_Pos;	//�J�����ړ��J�n���ɃJ�[�\���̈ʒu��ۑ�����ϐ�

	Camera() {};
public:
	//�s��X�V
	void Active();

	//setter
	void SetPerspective(float fov, float aspect,
		float nearZ, float farZ);
	void SetEye(const DirectX::XMFLOAT3& eye);
	void SetFocus(const DirectX::XMFLOAT3& focus);
	void SetUp(const DirectX::XMFLOAT3& up);

	void CameraMove(YRShader* shader);

	//getter
	const DirectX::XMFLOAT4X4& GetView()const;
	const DirectX::XMFLOAT4X4& GetProjection()const;
	const DirectX::XMFLOAT3& GetEye()const;
	const DirectX::XMFLOAT3& GetFocus()const;
	const float& GetFov()const;
	const float& GetAspect()const;
	const float& GetNear()const;
	const float& GetFar()const;

	static Camera &getInstance()
	{
		static Camera instance;
		return instance;
	}
};


class MainCamera : public Camera
{
public:
	enum MODE
	{
		MODE_FIX,
		MODE_CHASE,

		MODE_NUM,
	};
private:
	MODE				mode = MODE_FIX;	//�J�������[�h
	float				timer = 0.0f;		//�U������
	float				range = 0.0f;		//�U����
	DirectX::XMFLOAT3	moveTarget;			//�^�[�Q�b�g


	void vibrate(float elapsedTime);		//�U���J����
	void chase(float elapsedTime);			//�ǐՃJ����
	void fix(float elapsedTime);			//�Œ�J����

public:
	static const constexpr float FAR_DIST = 150.0f;
	static const constexpr float NEAR_DIST = 50.0f;

	void Update(float elapsedTime);				//�J�����X�V
	void SetMode(MODE mode);					//���[�h�ݒ�
	void SetVibration(float range, float timer);//�U���J�n

	void SetTarget(const DirectX::XMFLOAT3& moveTarget);
};

#define YRCamera		(Camera::getInstance())