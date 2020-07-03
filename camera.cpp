#include "camera.h"

void Camera::Active()
{
	//Ž‹–ì•ÏŠ·s—ñ
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eye),
		DirectX::XMLoadFloat3(&focus),
		DirectX::XMLoadFloat3(&up)
	);
	DirectX::XMStoreFloat4x4(&view, V);

	//“Š‰e•ÏŠ·s—ñ
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