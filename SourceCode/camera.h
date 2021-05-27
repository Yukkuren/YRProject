#pragma once

#include <DirectXMath.h>
#include "YRShader.h"
//#include "YR_VectorMaster.h"

//-----------------------------------------------------------------
//			カメラクラス
//-----------------------------------------------------------------
//・Set関数で値をセットした後Activeで計算を行う
//-----------------------------------------------------------------

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

	DirectX::XMFLOAT2		start_Pos = { 0.0f,0.0f };	//カメラ移動開始時にカーソルの位置を保存する変数

	DirectX::XMFLOAT3		at = { 0.0f,0.0f,0.0f };

	Camera() {};
public:

	float viblate_timer = 0.0f;

	int damage_pl_num = 0;

	DirectX::XMFLOAT3		targetPos = { 0.0f,0.0f,0.0f };		//注視点

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
		HOLD,		//カメラをつかむ(自分のステートにする)
		RELEASE,	//カメラを離す(ステートをMAINに即座に戻す)
		WEAKEN,		//カメラを持つ手を弱める(ステートは戻さず、位置を少しずつMAINに戻す)
		ZOOM,		//カメラをズームさせる
	};
private:
	Request		camera_Request = Request::NONE;
	int			request_player = 0;
public:

	void RequestCamera(Request request, int now_player);
	void RequestCamera(int damage_player);
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