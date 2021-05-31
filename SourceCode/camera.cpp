#include "camera.h"
#include "YRMouse.h"
#include "Game/collision.h"
#include "YR_VectorMaster.h"
#include "framework.h"

void Camera::Active()
{
	//����ϊ��s��
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eye),
		DirectX::XMLoadFloat3(&focus),
		DirectX::XMLoadFloat3(&up)
	);
	DirectX::XMStoreFloat4x4(&view, V);

	//���e�ϊ��s��
	DirectX::XMMATRIX P1 = DirectX::XMMatrixOrthographicLH(
		FRAMEWORK.SCREEN_WIDTH,
		FRAMEWORK.SCREEN_HEIGHT,
		nearZ,
		farZ
	);
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		fov,
		aspect,
		nearZ,
		farZ
	);
	DirectX::XMStoreFloat4x4(&projection, P);

	YR_Vector3 at = YR_Vector3(focus) - YR_Vector3(eye);

	at.Normalize();
	this->at = at.GetDXFLOAT3();

	/*DirectX::XMFLOAT4X4 projection_Orth;
	DirectX::XMStoreFloat4x4(&projection_Orth, P1);

	projection.m[0][0] = projection_Orth.m[0][0];*/
}

//setter

void Camera::SetPerspective(float fov, float aspect,
	float nearZ, float farZ)
{
	this->fov = fov;
	this->aspect = aspect;
	this->nearZ = nearZ;
	this->farZ = farZ;
}

void Camera::SetEye(const DirectX::XMFLOAT3& eye)
{
	this->eye = eye;
}

void Camera::SetFocus(const DirectX::XMFLOAT3& focus)
{
	this->focus = focus;
}

void Camera::SetUp(const DirectX::XMFLOAT3& up)
{
	this->up = up;
}

void Camera::SetFov(const float fov)
{
	this->fov = fov;
}


//getter

const DirectX::XMFLOAT4X4& Camera::GetView()const
{
	return view;
}

const DirectX::XMFLOAT4X4& Camera::GetProjection()const
{
	return projection;
}

const DirectX::XMFLOAT3& Camera::GetEye()const
{
	return eye;
}

const DirectX::XMFLOAT3& Camera::GetFocus()const
{
	return focus;
}

const DirectX::XMFLOAT3& Camera::GetUp()const
{
	return up;
}

const float& Camera::GetFov()const
{
	return fov;
}

const float& Camera::GetAspect()const
{
	return aspect;
}

const float& Camera::GetNear()const
{
	return nearZ;
}

const float& Camera::GetFar()const
{
	return farZ;
}

const DirectX::XMFLOAT3& Camera::GetAt()const
{
	return at;
}

void Camera::RequestCamera(Request request, int now_player)
{
	this->camera_Request = request;
	this->request_player = now_player;
	switch (camera_Request)
	{
	case Camera::Request::NONE:
		break;
	case Camera::Request::HOLD:
		if (request_player > 1)
		{
			//���N�G�X�g��2P
			camera_state = CAMERA_STATE::PLAYER2P;
		}
		else
		{
			//���N�G�X�g��1P
			camera_state = CAMERA_STATE::PLAYER1P;
		}
		break;
	case Camera::Request::RELEASE:
		break;
	case Camera::Request::WEAKEN:
		break;
	case Camera::Request::ZOOM:
		break;
	default:
		break;
	}
}

void Camera::RequestCamera(int damage_player)
{
	damage_pl_num = damage_player;
	camera_Request = Request::ZOOM;
	request_player = 0;
}

const Camera::Request& Camera::GetRequest()const
{
	return camera_Request;
}

const int& Camera::GetRequestPlayer()const
{
	return request_player;
}

void Camera::CameraMove(YRShader *shader)
{
#ifdef EXIST_IMGUI

	if (Get_Use_ImGui())
	{
		//�}�E�X���E�N���b�N���ꂽ����W��ۑ�����
		if (pMouse.right_state == 1)
		{
			start_Pos = pMouse.pos;
		}

		if (pMouse.right_state > 1)
		{
			pColSprite.circle->DrawCircleGraph(shader, start_Pos.x, start_Pos.y, 0.0f, 5.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
			if (start_Pos.x > pMouse.pos.x)
			{
				eye.x += 0.1f;
			}
			if (pMouse.pos.x > start_Pos.x)
			{
				eye.x -= 0.1f;
			}

			if (start_Pos.y > pMouse.pos.y)
			{
				eye.y += 0.1f;
			}
			if (pMouse.pos.y > start_Pos.y)
			{
				eye.y -= 0.1f;
			}
		}
	}
#endif // EXIST_IMGUI
}

void Camera::SpecifiedLerp(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,float factor)
{
	//�w�肵���J�����̈ʒu�܂Ō��݂̃J��������������Ԉړ�����

	DirectX::XMVECTOR now_eye_vector = DirectX::XMLoadFloat3(&this->eye);
	DirectX::XMVECTOR eye_vector = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR now_focus_vector = DirectX::XMLoadFloat3(&this->focus);
	DirectX::XMVECTOR focus_vector = DirectX::XMLoadFloat3(&focus);

	DirectX::XMVECTOR eye_larp = DirectX::XMVectorLerp(now_eye_vector,eye_vector,  factor);
	DirectX::XMVECTOR focus_larp = DirectX::XMVectorLerp(now_focus_vector,focus_vector,  factor);


	DirectX::XMStoreFloat3(&this->eye, eye_larp);
	DirectX::XMStoreFloat3(&this->focus, focus_larp);

	YRCamera.Active();
}