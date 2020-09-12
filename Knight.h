#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "PlayerBase.h"
#include "YR_VECTOR3.h"
#include "AttackBox.h"
#include "HitBox.h"
#include <vector>
#include "YRModelAnim.h"


enum class KNIGHTHIT :int
{
	BODY,
	LEG,
	END
};

enum class KNIGHTATK :int
{
	ONE,
	TWO,
	END,
	HADOU,
	THU_HADOU,
	KYO_HADOU,
	ATKEND
};

class Knight : public Player
{
private:
	const float walkspeed = 10.1f;		//歩く速度(代入)
	const float dashspeed = 0.1f;		//ダッシュ速度
	const float backstepS = 0.5f;
	const float stepspeed = 0.5f;
	const float jump_max = 0.5f;
	const float high_jump_max = 0.5f;
	const float brake_speed =10000.0f;	//停止時にかかるブレーキ(基本ピタッと止まるので数値は大きめ)

	float		production_time = 0.0f;	//カメラ演出に使う変数
	
	std::shared_ptr<Model>			main = nullptr;	//モデルメインデータ(メッシュのみ。アニメーション無し)
	std::unique_ptr<ModelAnim>		anim = nullptr;	//モデル描画&アニメーション適用変数

	//個別モーション用変数
	std::shared_ptr<Model>			wait = nullptr;
	std::shared_ptr<Model>			jaku_R_f = nullptr;
	std::shared_ptr<Model>			jaku_R_t = nullptr;
	std::shared_ptr<Model>			jaku_R_l = nullptr;
	std::shared_ptr<Model>			special_R_f = nullptr;
	std::shared_ptr<Model>			special_R_t = nullptr;
	std::shared_ptr<Model>			special_R_l = nullptr;

public:
	bool fast;
	YR_Vector3 FastPos;
	YR_Vector3 hadou;
	float hadouspeed;
	
	std::array< HitBox, static_cast<int>(KNIGHTHIT::END)> hit;
	//std::array<YR_Vector3, scastI(KNIGHTHIT::END)> Hitplus;
	//std::array<YR_Vector3, scastI(KNIGHTHIT::END)> HitSize;
	
	//カラーチェンジ用テクスチャ
	std::shared_ptr<Texture>	color_texture_main = nullptr;
	std::shared_ptr<Texture>	color_texture_face = nullptr;

	~Knight();
	void Init(YR_Vector3 InitPos);
	void Uninit();
	void LoadData(int color_number);
	void Update(float decision, float elapsed_time);
	void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);

	void DrawDEBUG(
		YRShader* geoshader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);
	//float GetPosX();

	void Move(float decision);
	bool Step();
	void MoveAnimSet();
	void Jump();
	void JumpUpdate();
	void DamageCheck();
	void KnockUpdate();
	void Guard(float decision);
	void GuardBack();
	void Squat();
	void FallUpdate();
	void DownUpdate();
	void PassiveUpdate();
	void WakeUp();
	void GaugeUp(float add);
	void CancelList();

	void GuardAnimSet();
	void WaitAnimSet();
	void WinAnimSet();
	bool WinPerformance();


	void AttackInput();	//攻撃するボタンが押されたか確認し、押されていればその行動をさせる
	void Attack(float decision, float elapsed_time);		//以下の関数を制御する


	//----------------------------------//
	//			攻撃関数					//
	//----------------------------------//
	void Jaku(float elapsed_time);
	void Thu(float elapsed_time);
	void Kyo(float fream, float elapsed_time);
	void D_Jaku(float elapsed_time);
	void D_Thu(float fream, float elapsed_time);
	void U_Kyo(float fream, float elapsed_time);
	void Hadouken(float elapsed_time);
	void Thu_Hadouken(float elapsed_time);
	void Kyo_Hadouken(float elapsed_time);
	void HadouUpdate(float elapsed_time);
	void Thu_HadouUpdate(float elapsed_time);
	void Kyo_HadouUpdate(float elapsed_time);
	void P_Kyo(float elapsed_time);
	void TrackDash(float decision, float elapsed_time);
	void Steal(float elapsed_time);
	void Slow(float elapsed_time);
	void SpecialAttack(float elapsed_time);

	void Panish_N(float elapsed_time);
	void Panish_H(float elapsed_time);

	void ExtendATK(float elapsed_time);

	void StateNone();


	void DrawFastMove(YR_Vector3 position);
	void FastSet(YR_Vector3 position);



	//継承
	float Getapply(float n);
	HitBox* GetHit();
	AttackBox* GetAttack();
	int GetMax(int n);
	//float& GetPosX2();

	void NoneChange();

	void StopUpdate();
	void StopEnd();

	bool Intro();

	bool AttackLoad();			//生成時攻撃パラメーターを読み込む
	bool AttackWrite();			//デバック時パラメーターを書き出す

	bool AttackEndCheck();		//攻撃当たり判定が全て終了しているか確認する
	void EndAttackErase();		//終了した攻撃当たり判定を全て消去する。
	void AllAttackClear();		//全ての攻撃当たり判定を消去する
	void AttackUpdate(float elapsed_time);		//攻撃判定が存在する時のみ更新

	//イントロ用列挙
	enum class INTRO_KNIGHT : int
	{
		SET = 0,
		CAMERA_ZOOM,
		FINISH,
	};
	INTRO_KNIGHT	intro_state;

	//勝利演出用列挙
	enum class WIN_PERFORMANCE_KNIGHT : int
	{
		CAMERA_ZOOM = 0,
		FINISH,
	};
	WIN_PERFORMANCE_KNIGHT win_state;



public:
	//テクスチャアニメーション用

	//現在の顔の状態
	enum class FaceAnim : int
	{
		NORMAL = 0,
		WINK,
		Damage,
		YARUKI,
		KOUHUN,
	};

	//目の識別用列挙
	enum FaceEye_Num : int
	{
		NORMAL_EYE = 0,
		WINK1,
		WINK2,
		KIRAME,
		CLOSE,
		TURI,
		KAOMOJI,
		EYE_MAX,
	};

	DirectX::XMFLOAT2	face_eye_offset[FaceEye_Num::EYE_MAX] =
	{
		{ 0.0f,0.0f },
		{ 0.2f,0.0f },
		{ 0.4f,0.0f },
		{ 0.605f,0.0f },
		{ 0.0f,0.1f },
		{ 0.2f,0.1f },
		{ 0.39f,0.1f }
	};

	//口の列挙
	enum FaceMouse_Num : int
	{
		NORMAL_MOUSE = 0,
		TOZI,
		KUMON,
		KURI,
		OOGUTI,
		OTYOBO,
		POKAN,
		MOUSE_MAX,
	};

	DirectX::XMFLOAT2	face_mouse_offset[FaceMouse_Num::MOUSE_MAX] =
	{
		{ 0.0f,0.0f },
		{ 0.2f,0.0f },
		{ 0.4f,0.0f },
		{ 0.0f,0.1f },
		{ 0.2f,0.1f },
		{ 0.0f,0.09f },
		{ 0.0f,0.11f }
	};

	FaceAnim face_anim = FaceAnim::NORMAL;
	float face_wink_time;

	//まばたき用列挙
	enum Wink_State : int
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOURTH,
		FIVE,
		SIX,
	};

	Wink_State wink_state = Wink_State::FIRST;

	DirectX::XMFLOAT2	eye_offset = { 0.0f,0.0f };
	DirectX::XMFLOAT2	mouse_offset = { 0.0f,0.0f };

	void FaceAnimation(float elapsed_time);
	void FaceWink(float elapsed_time);

	void ChangeFace(FaceAnim anim);
};

#endif // !_KNIGHT_H_

