#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "../PlayerBase/PlayerBase.h"
#include "../../YR_VectorMaster.h"
#include "../../Game/AttackBox.h"
#include "../../Game/HitBox.h"
#include <vector>
#include "../../Trajectory.h"
#include "../../Model/Geometric_primitive.h"


enum class KNIGHTHIT :int
{
	HEAD,
	BODY,
	LEG,
	END
};



//ナイトクラス(キャラ本体)
class Knight : public Player
{
private:
	const size_t	max_traject_count = 128;	//剣の軌跡の最大値

public:

	//カラーチェンジ用テクスチャ
	std::shared_ptr<Texture>	color_texture_main = nullptr;
	std::shared_ptr<Texture>	color_texture_face = nullptr;

	//剣の軌跡
	Trajectory					traject;
	YR_Vector3					sword_head = { 0.0f,0.0f,0.0f };
	YR_Vector3					sword_tail = { 0.0f,0.0f,0.0f };

	~Knight();
	void CharaInit();
	void Uninit();
	void LoadData(PLCOLOR color_number);
	void Update(float decision, float elapsed_time);
	void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);

	void AttackDetailsSet(const AttackState &attack_state)override;

	void WinAnimSet();
	bool WinPerformance(float elapsed_time);


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

	void A_Jaku_Lhurf(float elapsed_time)override;
	void ExtendATK(float elapsed_time);



	//継承

	bool Intro(float elapsed_time);

	void DrawDEBUGHitParam();

	//bool DEBUGAttackLoad();		//デバッグ時パラメーターを生成する


	void ReadySet();			//イントロ後、ゲーム開始までの設定を行う


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

	void ChangeFace(FaceAnim anim)override;				//表情を変える関数(enumで定義)

	std::wstring RandTextSelect();						//ランダムであらかじめ設定されたテキストを選択して返す


	enum class TextList : int
	{
		NORMAL,
		WARLIKE,
		CRIOSITY,
		TEXT_END,
	};
};

#endif // !_KNIGHT_H_

