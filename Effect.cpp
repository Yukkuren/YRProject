#include "Effect.h"
#include "CoList.h"
#include "framework.h"

YR_Effect::YR_Effect()
{
	//エフェクトレンダラーの作成
	renderer = EffekseerRendererDX11::Renderer::Create(FRAMEWORK.device.Get(), FRAMEWORK.context.Get(), square_max_count);

	//エフェクトマネージャーの生成
	manager = Effekseer::Manager::Create(square_max_count);

	//描画モジュールの作成
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	//テクスチャ、モデル、マテリアルローダーの設定をする
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());

	//ハンドルとエフェクトを生成
	handles.resize(scastI(EffectKind::END));
	effects.resize(scastI(EffectKind::END));

	//エフェクトの読み込み
	EFK_CHAR efk_char = EFK_CHAR("./Data/Effect/guard.efk");
	effects[scastI(EffectKind::GUARD)] = Effekseer::Effect::Create(manager, &efk_char);

	//ハンドルの初期化
	for (int i = 0; i < handles.size(); i++)
	{
		handles[i] = 0;
	}
}

YR_Effect::~YR_Effect()
{
	//エフェクトの解放
	for (int i = 0; i < effects.size(); i++)
	{
		ES_SAFE_RELEASE(effects[i]);
	}
	effects.clear();

	//ハンドルの解放
	for (int i = 0; i < handles.size(); i++)
	{
		handles[i] = 0;
	}
	handles.clear();

	// マネージャーの破棄
	manager->Destroy();

	// レンダラーの破棄
	renderer->Destroy();
}

//カメラのセット
void YR_Effect::CameraSet()
{
	//カメラの位置を取得
	DirectX::XMFLOAT3 eye = YRCamera.GetEye();
	
	//カメラの注視点を取得
	DirectX::XMFLOAT3 focus = YRCamera.GetFocus();

	//カメラの上向きを取得
	DirectX::XMFLOAT3 up = YRCamera.GetUp();

	//カメラのアスペクト比を取得
	float aspect = YRCamera.GetAspect();
	float fov = YRCamera.GetFov();
	float Near = YRCamera.GetNear();
	float Far = YRCamera.GetFar();

	// 視点位置を確定
	Effekseer::Vector3D g_position = ::Effekseer::Vector3D(eye.x, eye.y, eye.z);
	// 視点方向を確定
	Effekseer::Vector3D g_at = ::Effekseer::Vector3D(focus.x, focus.y, focus.z);
	// 視点上向きを確定
	Effekseer::Vector3D g_up = ::Effekseer::Vector3D(up.x, up.y, up.z);

	// 投影行列を設定
	renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		fov, aspect, Near, Far));

	// カメラ行列を設定
	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(g_position, g_at, g_up));
}

//アニメーションの描画
void YR_Effect::Draw()
{
	//マネージャーの更新
	manager->Update();
	
	// エフェクトの描画開始処理を行う。
	renderer->BeginRendering();

	// エフェクトの描画を行う。
	manager->Draw();

	// エフェクトの描画終了処理を行う。
	renderer->EndRendering();
}

//アニメーションの再生
void YR_Effect::PlayEffect(
	EffectKind kind,
	const DirectX::XMFLOAT3& pos,
	const DirectX::XMFLOAT3& scale,
	const DirectX::XMFLOAT3& axis,
	const float& angle)
{
	// エフェクトの移動
	//manager->AddLocation(handles[scastI(kind)], ::Effekseer::Vector3D(pos.x, pos.y, pos.z));

	// エフェクトの移動
	manager->SetLocation(handles[scastI(kind)], ::Effekseer::Vector3D(pos.x, pos.y, pos.z));

	// エフェクトの回転
	manager->SetRotation(handles[scastI(kind)], ::Effekseer::Vector3D(axis.x, axis.y, axis.z),angle);

	// エフェクトの拡大縮小
	manager->SetScale(handles[scastI(kind)], scale.x,scale.y,scale.z);

	// エフェクトの再生
	//handles[scastI(kind)] = manager->Play(effects[scastI(kind)], 0, 0, 0);
	handles[scastI(kind)] = manager->Play(effects[scastI(kind)], pos.x,pos.y,pos.z);
}

//アニメーション再生の停止
void YR_Effect::StopEffect(EffectKind kind)
{
	// エフェクトの停止
	manager->StopEffect(handles[scastI(kind)]);
}