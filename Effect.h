#pragma once

#include "EXlib/Effekseer.h"
#include "EXlib/EffekseerRendererDX11.h"

#include <vector>
#include <DirectXMath.h>
//----------------------------------
//		エフェクトクラス
//----------------------------------

enum class EffectKind : int
{
	NONE = 0,
	GUARD,
	TORNADE,
	SWORD,
	DRILL,
	END,
};


class YR_Effect
{
private:
	const int square_max_count = 8000;				//エフェクト生成時に出すメッシュの最大数

	std::vector<Effekseer::Handle>	handles;		//エフェクト再生用ハンドル
	std::vector<Effekseer::Effect*> effects;		//エフェクト本体

	Effekseer::Manager *manager;					//エフェクトマネージャー
	EffekseerRendererDX11::Renderer* renderer;		//エフェクトレンダラー
public:
	//コンストラクタ
	YR_Effect() {};

	//デストラクタ
	~YR_Effect();

	//初期化
	void Init();

	//カメラをレンダラーにセットする
	void CameraSet();

	//アニメーションの再生開始
	void PlayEffect(
		EffectKind kind,
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& axis,
		const float& angle);

	void PlayEffect(
		EffectKind kind,
		Effekseer::Handle& handle,
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& axis,
		const float& angle);

	void SetLocation(
		EffectKind kind,
		const DirectX::XMFLOAT3& pos
	);
	void SetLocation(
		EffectKind kind,
		Effekseer::Handle& handle,
		const DirectX::XMFLOAT3& pos
	);

	//アニメーション再生の停止
	void StopEffect(EffectKind kind);
	void StopEffect(EffectKind kind,Effekseer::Handle& handle);

	//エフェクトの描画
	void Draw();

	static YR_Effect& getInstance()
	{
		static YR_Effect instance;
		return instance;
	}
};

inline YR_Effect& YRGetEffect()
{
	return YR_Effect::getInstance();
}
