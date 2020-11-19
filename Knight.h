#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "PlayerBase.h"
#include "YR_VectorMaster.h"
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

//個別モーションデータ格納構造体
struct Model_MotionData
{
	std::shared_ptr<Model>			wait_R = nullptr;
	std::shared_ptr<Model>			wait_L = nullptr;
	std::shared_ptr<Model>			guard_R = nullptr;
	std::shared_ptr<Model>			guard_L = nullptr;
	std::shared_ptr<Model>			slid_R = nullptr;
	std::shared_ptr<Model>			slid_L = nullptr;
	std::shared_ptr<Model>			air_back_R = nullptr;
	std::shared_ptr<Model>			air_back_L = nullptr;
	std::shared_ptr<Model>			air_dash_R = nullptr;
	std::shared_ptr<Model>			air_dash_L = nullptr;
	std::shared_ptr<Model>			passive_R = nullptr;
	std::shared_ptr<Model>			passive_L = nullptr;
	std::shared_ptr<Model>			squat_R = nullptr;
	std::shared_ptr<Model>			squat_L = nullptr;
	std::shared_ptr<Model>			walk_R = nullptr;
	std::shared_ptr<Model>			walk_L = nullptr;
	std::shared_ptr<Model>			back_R = nullptr;
	std::shared_ptr<Model>			back_L = nullptr;
	std::shared_ptr<Model>			dash_R = nullptr;
	std::shared_ptr<Model>			dash_L = nullptr;
	std::shared_ptr<Model>			backstep_R = nullptr;
	std::shared_ptr<Model>			backstep_L = nullptr;
	std::shared_ptr<Model>			jump_R = nullptr;
	std::shared_ptr<Model>			jump_L = nullptr;
	std::shared_ptr<Model>			air_jump_R = nullptr;
	std::shared_ptr<Model>			air_jump_L = nullptr;
	std::shared_ptr<Model>			intro_R = nullptr;
	std::shared_ptr<Model>			intro_L = nullptr;
	std::shared_ptr<Model>			win_R = nullptr;
	std::shared_ptr<Model>			win_L = nullptr;
	std::shared_ptr<Model>			damage_R_g_u = nullptr;
	std::shared_ptr<Model>			damage_L_g_u = nullptr;
	std::shared_ptr<Model>			jaku_R = nullptr;
	std::shared_ptr<Model>			jaku_L = nullptr;
	std::shared_ptr<Model>			thu_R = nullptr;
	std::shared_ptr<Model>			thu_L = nullptr;
	std::shared_ptr<Model>			kyo_R = nullptr;
	std::shared_ptr<Model>			kyo_L = nullptr;
	std::shared_ptr<Model>			d_jaku_R = nullptr;
	std::shared_ptr<Model>			d_jaku_L = nullptr;
	std::shared_ptr<Model>			d_thu_R = nullptr;
	std::shared_ptr<Model>			d_thu_L = nullptr;
	std::shared_ptr<Model>			u_kyo_R = nullptr;
	std::shared_ptr<Model>			u_kyo_L = nullptr;
	std::shared_ptr<Model>			a_jaku_R = nullptr;
	std::shared_ptr<Model>			a_jaku_L = nullptr;
	std::shared_ptr<Model>			a_thu_R = nullptr;
	std::shared_ptr<Model>			a_thu_L = nullptr;
	std::shared_ptr<Model>			a_kyo_R = nullptr;
	std::shared_ptr<Model>			a_kyo_L = nullptr;
	std::shared_ptr<Model>			a_ukyo_R = nullptr;
	std::shared_ptr<Model>			a_ukyo_L = nullptr;
	std::shared_ptr<Model>			steal_R = nullptr;
	std::shared_ptr<Model>			steal_L = nullptr;
	std::shared_ptr<Model>			slow_R = nullptr;
	std::shared_ptr<Model>			slow_L = nullptr;
	std::shared_ptr<Model>			track_R = nullptr;
	std::shared_ptr<Model>			track_L = nullptr;
	std::shared_ptr<Model>			jaku_rh_R = nullptr;
	std::shared_ptr<Model>			jaku_rh_L = nullptr;
	std::shared_ptr<Model>			thu_rh_R = nullptr;
	std::shared_ptr<Model>			thu_rh_L = nullptr;
	std::shared_ptr<Model>			kyo_rh_R = nullptr;
	std::shared_ptr<Model>			kyo_rh_L = nullptr;
	std::shared_ptr<Model>			jaku_lh_R = nullptr;
	std::shared_ptr<Model>			jaku_lh_L = nullptr;
	std::shared_ptr<Model>			thu_lh_R = nullptr;
	std::shared_ptr<Model>			thu_lh_L = nullptr;
	std::shared_ptr<Model>			kyo_lh_R = nullptr;
	std::shared_ptr<Model>			kyo_lh_L = nullptr;
	std::shared_ptr<Model>			special_R = nullptr;
	std::shared_ptr<Model>			special_L = nullptr;
	std::shared_ptr<Model>			disire_s_R = nullptr;
	std::shared_ptr<Model>			disire_s_L = nullptr;
	std::shared_ptr<Model>			disire_m_R = nullptr;
	std::shared_ptr<Model>			disire_m_L = nullptr;
	std::shared_ptr<Model>			extend_R = nullptr;
	std::shared_ptr<Model>			extend_L = nullptr;

public:
	std::vector <std::shared_ptr<Model>> model_R;
	std::vector <std::shared_ptr<Model>> model_L;
};



//ナイトクラス(キャラ本体)
class Knight : public Player
{
private:
	const float walkspeed = 10.1f;		//歩く速度(代入)
	const float dashspeed = 40.1f;		//ダッシュ速度
	const float backstepS = 116.0f;		//バックステップの速度
	const float backstepD = 500.0f;		//バックステップの減少速度
	const float stepspeed = 100.0f;		//空中ステップの速度
	const float stepD = 500.0f;			//空中ステップの減少速度
	const float jump_max = 108.0f;		//ジャンプの最大速度(超えると減速し始め落ちる)
	const float high_jump_max = 47.0f;	//ハイジャンプの最大速度(超えると減速し始め落ちる)
	const float jump_speed = 108.0f;		//毎フレームジャンプの速度に加算する数値
	const float high_jump_speed = 139.0f;	//毎フレームハイジャンプの速度に加算する数値
	const float brake_speed =10000.0f;		//停止時にかかるブレーキ(基本ピタッと止まるので数値は大きめ)
	const float track_speed = 100.0f;		//ホーミングダッシュの速度
	const YR_Vector3 passive_speed = { 70.0f,30.0f };	//受け身状態にスピードに代入する速度
	const float passive_brake = 100.0f;		//受け身中に減速する速度

	float		production_time = 0.0f;	//カメラ演出に使う変数
	
	std::shared_ptr<Model>			main = nullptr;	//モデルメインデータ(メッシュのみ。アニメーション無し)
	std::unique_ptr<ModelAnim>		anim = nullptr;	//モデル描画&アニメーション適用変数

	//個別モーション用変数
	Model_MotionData				model_motion;

public:

	bool fast;
	YR_Vector3 FastPos;
	YR_Vector3 hadou;
	float hadouspeed;

	Model::Material_Attribute		lumi_material;
	
	//std::array< HitBox, static_cast<int>(KNIGHTHIT::END)> hit;
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
	bool Step(float elapsed_time);
	void AirDash(float elapsed_time);
	void MoveAnimSet();
	void Jump();
	void JumpUpdate(float decision, float elapsed_time);
	void DamageCheck(float decision);
	void KnockUpdate(float elapsed_time);
	void DownHitUpdate(float elapsed_time);
	void SlamUpdate(float elapsed_time);
	void Guard(float decision);
	void GuardBack(float elapsed_time);
	void Squat();
	void FallUpdate(float elapsed_time);
	void DownUpdate();
	void PassiveUpdate(float elapsed_time);
	void WakeUp();
	void GaugeUp(float add);
	void AttackDetailsSet(const AttackState &attack_state);

	void HitBoxTransition(HitBoxState state);

	void GuardAnimSet();
	void WaitAnimSet();
	void WinAnimSet();
	bool WinPerformance(float elapsed_time);


	void AttackInput();	//攻撃するボタンが押されたか確認し、押されていればその行動をさせる
	void Attack(float decision, float elapsed_time);		//以下の関数を制御する
	void AttackSwitch(float decision, float elapsed_time, AttackState attack_state = AttackState::NONE);	//attack_stateによってそれぞれ異なる関数を動かす
	void AttackDefault(float elapsed_time);//特殊な記述のある攻撃以外はこの関数を使用する
	void AttackProjectileDefault(float elapsed_time);//特殊な記述のある飛び道具攻撃以外はこの関数を使用する


	//----------------------------------//
	//			攻撃関数					//
	//----------------------------------//
	void Jaku(float elapsed_time);
	void Thu(float elapsed_time);
	void Kyo(float elapsed_time);
	void U_Kyo(float elapsed_time);
	void D_Jaku(float elapsed_time);
	void D_Thu(float elapsed_time);
	void A_Jaku(float elapsed_time);
	void A_Thu(float elapsed_time);
	void A_Kyo(float elapsed_time);
	void A_UKyo(float elapsed_time);
	void Jaku_Rhurf(float elapsed_time);
	void Thu_Rhurf(float elapsed_time);
	void Kyo_Rhurf(float elapsed_time);
	
	void TrackDash(float decision, float elapsed_time);
	void Steal(float elapsed_time);
	void Slow(float elapsed_time);
	void SpecialAttack(float elapsed_time);

	void Jaku_Lhurf(float elapsed_time);
	void Thu_Lhurf(float elapsed_time);
	void Kyo_Lhurf(float elapsed_time);

	void A_Jaku_Lhurf(float elapsed_time);
	void ExtendATK(float elapsed_time);

	void StateNone(float elapsed_time);


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

	bool Intro(float elapsed_time);

	bool AttackLoad();			//生成時攻撃パラメーターを読み込む
	bool AttackWrite();			//デバック時パラメーターを書き出す
	bool AttackClean();			//デバッグ時パラメーターを全て初期化する
	bool DEBUGAttackLoad();		//デバッグ時パラメーターを生成する

	bool AttackEndCheck();		//攻撃当たり判定が全て終了しているか確認する
	void EndAttackErase();		//終了した攻撃当たり判定を全て消去する。
	void AllAttackClear();		//全ての攻撃当たり判定を消去する
	void AttackUpdate(float elapsed_time);		//攻撃判定が存在する時のみ更新

	void ReadySet();			//イントロ後、ゲーム開始までの設定を行う

	bool ComboSet();
	void ComboUpdate();

	void ComboX(float decision, float elapsed_time);	//Xボタンコンボ関数
	void ComboY(float decision, float elapsed_time);	//Xボタンコンボ関数
	void ComboB(float decision, float elapsed_time);	//Xボタンコンボ関数

	bool StealRangeCheck();

	//イントロ用列挙
	enum class INTRO_KNIGHT : int
	{
		SET = 0,	//セットする
		WAIT,		//後から見る
		ZOOM_SET,	//セットする
		ZOOM,		//口パクを見る
		PULL,		//少し引く
		PUSH,		//少し寄る
		PULL_2,		//少し引く
		PUSH_2,		//少し寄る
		FIN_SET,	//セット
		FIN,		//斜め後ろから見る
		FINISH,
	};
	INTRO_KNIGHT	intro_state;

	float			intro_timer;		//イントロで使用する

	YR_Vector3 eye_plus;				//カメラの座標にプラスする値
	YR_Vector3 focus_plus;				//カメラの注視点にプラスする値

	void IntroDEBUG();

	//勝利演出用列挙
	enum class WIN_PERFORMANCE_KNIGHT : int
	{
		SET = 0,
		ZOOM,
		PULL,
		STOP,
		FINISH,
	};
	WIN_PERFORMANCE_KNIGHT	win_state;

	float					win_timer;

	void WinDEBUG();

	//特殊技カメラ処理用列挙
	enum class CAMERA_STATE_KNIGHT : int
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,
		ELEVEN,
		TWELVE,
		THIRTEEN,
		STATE_END,
	}; CAMERA_STATE_KNIGHT camera_state_knight;



public:
	//テクスチャアニメーション用

	//現在の顔の状態
	enum class FaceAnim : int
	{
		NORMAL = 0,
		NORMAL_LIP_SYNC,
		WINK,
		Damage,
		YARUKI,
		KOUHUN,
		TOZI,
		YEAH,
	};

	//目の識別用列挙
	enum class FaceEye_Num : int
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

	DirectX::XMFLOAT2	face_eye_offset[scastI(FaceEye_Num::EYE_MAX)] =
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
	enum class FaceMouth_Num : int
	{
		NORMAL_MOUSE = 0,
		TOZI,
		KUMON,
		KURI,
		OOGUTI,
		OTYOBO,
		POKAN,
		MOUTH_MAX,
	}; FaceMouth_Num face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;

	static constexpr std::array<FaceMouth_Num,4> lip_sync_can =
	{
		FaceMouth_Num::NORMAL_MOUSE,
		FaceMouth_Num::TOZI,
		FaceMouth_Num::OOGUTI,
		FaceMouth_Num::KURI,
	};

	DirectX::XMFLOAT2	face_mouth_offset[scastI(FaceMouth_Num::MOUTH_MAX)] =
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
	float face_wink_time = 0.0f;				//ウインク処理用変数
	float face_wink_interval_timer = 0.0f;		//ウインクの間隔を調整する変数
	float wink_interval = 3.0f;					//ウインクの間隔
	float lip_sync_time = 0.0f;					//口パク処理用変数

	//まばたき用列挙
	enum class Wink_State : int
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOURTH,
		FIVE,
		SIX,
	};Wink_State wink_state = Wink_State::FIRST;


	DirectX::XMFLOAT2	eye_offset = { 0.0f,0.0f };
	DirectX::XMFLOAT2	mouth_offset = { 0.0f,0.0f };

	void FaceAnimation(float elapsed_time);				//表情のアニメーション処理
	void FaceWink(float elapsed_time);					//ウインクの処理
	bool FaceWink_bool(float elapsed_time);				//ウインクの処理(口パクなどと合わせる用)
	void FaceLipSync(float elapsed_time);				//口パクの処理

	void ChangeFace(FaceAnim anim);						//表情を変える関数(enumで定義)

	std::wstring RandTextSelect();						//ランダムであらかじめ設定されたテキストを選択して返す

	std::wstring lip_text;								//表示するテキスト
	bool		text_on = false;						//trueならテキストを表示する

	void TextDraw();									//テキストを描画する
	enum class TextList : int
	{
		NORMAL,
		WARLIKE,
		CRIOSITY,
		TEXT_END,
	};
};

#endif // !_KNIGHT_H_

