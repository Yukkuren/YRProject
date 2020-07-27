#include "camera.h"
#include "YRMouse.h"
#include "collision.h"

void Camera::Active()
{
	//視野変換行列
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eye),
		DirectX::XMLoadFloat3(&focus),
		DirectX::XMLoadFloat3(&up)
	);
	DirectX::XMStoreFloat4x4(&view, V);

	//投影変換行列
	/*DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(
		fov,
		aspect,
		nearZ,
		farZ
	);*/
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		fov,
		aspect,
		nearZ,
		farZ
	);
	DirectX::XMStoreFloat4x4(&projection, P);
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

void Camera::CameraMove(YRShader *shader)
{
	//マウスが右クリックされたら座標を保存する
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