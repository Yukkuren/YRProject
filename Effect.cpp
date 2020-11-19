#include "Effect.h"
#include "CoList.h"
#include "framework.h"

void YR_Effect::Init()
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
	effects[scastI(EffectKind::GUARD)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/guard.efk");
	effects[scastI(EffectKind::TORNADE)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/Tornade.efk");
	effects[scastI(EffectKind::SWORD)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/sword.efk");
	effects[scastI(EffectKind::DRILL)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/drill.efk");
	effects[scastI(EffectKind::OFFSET)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/Offset.efk");
	effects[scastI(EffectKind::FIRE_DRILL)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/fire_drill.efk");
	effects[scastI(EffectKind::POWER_DRILL)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/power_drill.efk");
	effects[scastI(EffectKind::SPECIAL_DRILL)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/special_drill.efk");
	effects[scastI(EffectKind::WIND)] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)L"./Data/Effect/wind.efk");

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

void YR_Effect::Update()
{
	//マネージャーの更新
	manager->Update();
}


//アニメーションの描画
void YR_Effect::Draw()
{
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

	if (kind == EffectKind::NONE)
	{
		return;
	}
	// エフェクトの再生
	//handles[scastI(kind)] = manager->Play(effects[scastI(kind)], 0, 0, 0);
	handles[scastI(kind)] = manager->Play(effects[scastI(kind)], pos.x,pos.y,pos.z);

	// エフェクトの移動
	manager->SetLocation(handles[scastI(kind)], ::Effekseer::Vector3D(pos.x, pos.y, pos.z));

	// エフェクトの回転
	manager->SetRotation(handles[scastI(kind)], ::Effekseer::Vector3D(axis.x, axis.y, axis.z), angle);

	// エフェクトの拡大縮小
	manager->SetScale(handles[scastI(kind)], scale.x, scale.y, scale.z);
}



void YR_Effect::PlayEffect(
	EffectKind kind,
	Effekseer::Handle& handle,
	const DirectX::XMFLOAT3& pos,
	const DirectX::XMFLOAT3& scale,
	const DirectX::XMFLOAT3& axis,
	const float& angle)
{
	// エフェクトの移動
	//manager->AddLocation(handles[scastI(kind)], ::Effekseer::Vector3D(pos.x, pos.y, pos.z));
	if (kind == EffectKind::NONE)
	{
		return;
	}

	// エフェクトの再生
	//handles[scastI(kind)] = manager->Play(effects[scastI(kind)], 0, 0, 0);
	handle = manager->Play(effects[scastI(kind)], pos.x,pos.y,pos.z);

	// エフェクトの移動
	manager->SetLocation(handle, ::Effekseer::Vector3D(pos.x, pos.y, pos.z));

	// エフェクトの回転
	manager->SetRotation(handle, ::Effekseer::Vector3D(axis.x, axis.y, axis.z), angle);

	// エフェクトの拡大縮小
	manager->SetScale(handle, scale.x, scale.y, scale.z);
}

void YR_Effect::SetLocation(
	EffectKind kind,
	const DirectX::XMFLOAT3& pos
)
{
	if (kind == EffectKind::NONE)
	{
		return;
	}
	// エフェクトの移動
	manager->SetLocation(handles[scastI(kind)], ::Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

void YR_Effect::SetLocation(
	EffectKind kind,
	Effekseer::Handle& handle,
	const DirectX::XMFLOAT3& pos
)
{
	if (kind == EffectKind::NONE)
	{
		return;
	}
	// エフェクトの移動
	manager->SetLocation(handle, ::Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

//アニメーション再生の停止
void YR_Effect::StopEffect(EffectKind kind)
{
	if (kind == EffectKind::NONE)
	{
		return;
	}
	// エフェクトの停止
	manager->StopEffect(handles[scastI(kind)]);
}

void YR_Effect::StopEffect(EffectKind kind, Effekseer::Handle& handle)
{
	if (kind == EffectKind::NONE)
	{
		return;
	}
	// エフェクトの停止
	manager->StopEffect(handle);
}