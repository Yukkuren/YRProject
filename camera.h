#pragma once

#include <DirectXMath.h>
#include "YRShader.h"

class Camera
{
private:
	DirectX::XMFLOAT4X4		view = 
	{ 
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f };		//視野変換行列
	DirectX::XMFLOAT4X4		projection = 
	{ 
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f 
	};	//投影変換行列
	DirectX::XMFLOAT3		eye = { 0.0f,0.0f,0.0f };	//視点
	DirectX::XMFLOAT3		focus = { 0.0f,0.0f,0.0f };		//注視点
	DirectX::XMFLOAT3		up = { 0.0f,0.0f,0.0f };			//上ベクトル
	float					fov = 0.0f;		//画角
	float					aspect = 0.0f;		//アスペクト比
	float					nearZ = 0.0f;		//ニアクリップ面までの距離
	float					farZ = 0.0f;		//ファークリップ面までの距離

	DirectX::XMFLOAT2		start_Pos;	//カメラ移動開始時にカーソルの位置を保存する変数

	DirectX::XMFLOAT3		at = { 0.0f,0.0f,0.0f };

	Camera() {};
public:

	enum CAMERA_STATE
	{
		MAIN = 0,
		PLAYER1P,
		PLAYER2P,
	};
	CAMERA_STATE camera_state = CAMERA_STATE::MAIN;

	enum Request
	{
		NONE,
		HOLD,		//カメラをつかむ(自分のステートにする)
		RELEASE,	//カメラを離す(ステートをMAINに即座に戻す)
		WEAKEN,		//カメラを持つ手を弱める(ステートは戻さず、位置を少しずつMAINに戻す)
	};
private:
	Request		camera_Request = Request::NONE;
	int			request_player = 0;
public:

	void RequestCamera(Request request, int now_player);
	const Request& GetRequest()const;
	const int& GetRequestPlayer()const;

	//行列更新
	void Active();

	//setter
	void SetPerspective(float fov, float aspect,
		float nearZ, float farZ);
	void SetEye(const DirectX::XMFLOAT3& eye);
	void SetFocus(const DirectX::XMFLOAT3& focus);
	void SetUp(const DirectX::XMFLOAT3& up);
	void SetFov(const float fov);

	void CameraMove(YRShader* shader);

	//指定した位置までカメラの動きを補完移動する
	void SpecifiedLerp(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,float factor);

	//getter
	const DirectX::XMFLOAT4X4& GetView()const;
	const DirectX::XMFLOAT4X4& GetProjection()const;
	const DirectX::XMFLOAT3& GetEye()const;
	const DirectX::XMFLOAT3& GetFocus()const;
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
	MODE				mode = MODE_FIX;	//カメラモード
	float				timer = 0.0f;		//振動時間
	float				range = 0.0f;		//振幅幅
	DirectX::XMFLOAT3	moveTarget;			//ターゲット


	void vibrate(float elapsedTime);		//振動カメラ
	void chase(float elapsedTime);			//追跡カメラ
	void fix(float elapsedTime);			//固定カメラ

public:
	static const constexpr float FAR_DIST = 150.0f;
	static const constexpr float NEAR_DIST = 50.0f;

	void Update(float elapsedTime);				//カメラ更新
	void SetMode(MODE mode);					//モード設定
	void SetVibration(float range, float timer);//振動開始

	void SetTarget(const DirectX::XMFLOAT3& moveTarget);
};

#define YRCamera		(Camera::getInstance())