//#pragma once

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

//#include "../../Model/Skinned_mesh.h"

#include "../../YR_VectorMaster.h"
#include "../../Game/HitBox.h"
#include "../../Game/AttackBox.h"
#include "../../Game/Tracking.h"
#include <memory>
#include "../../Game/YRGamePad.h"
#include "../../Model/YRModelAnim.h"
#include "../../Texture.h"
#include <vector>
#include <string>
#include "../../sprite.h"
//#include "Effect.h"


constexpr float POS_Y = 0.0f;							//最低の高さ
constexpr float GAUGE_MAX = 100.0f;						//ゲージ最大量
constexpr int	Track_max = 2;							//ホーミングダッシュ最大数
constexpr float	non_target = 110.0f;					//この値を入れられたフレームは条件から外れるようにする
constexpr float target_max = 100.0f;					//条件式でこの値以上は外れるようにする
constexpr float attenuation_slam = 8.0f;				//滑り中速度の減衰率
constexpr float draw_guarf_effect_add_pos_x = 3.0f;		//ガードエフェクト描画時の位置補間値
constexpr float Reflection_range_min = 100.0f;			//この数値以上の吹っ飛びなら壁で跳ね返る
constexpr float Reflection_attenuation_factor = 0.8f;	//壁反射時の減衰率
constexpr float cut_in_max_time = 0.5f;					//カットイン表示時間
constexpr float passive_max_time = 0.2f;				//受け身中無敵時間

//--------------------------------------
//	**キャラ名設定
//	・新しいキャラを追加するならここに記入
//--------------------------------------
enum class PLSELECT :int
{
	KNIGHT = 0,
	NERU,
	PLSELECT_END,
};


//--------------------------------------
//	**キャラカラー
//	・新しいカラーを追加する場合はここに追加
//	※全てのキャラのカラー数を同じにする必要あり
//--------------------------------------
enum class PLCOLOR :int
{
	ORIGINAL = 0,
	COLOR_2,
	COLOR_END,
};


//--------------------------------------
//	**アニメーション選択
//	・NodeChange時に送り、アニメーションを指定する
//--------------------------------------
enum class AnimAtk :int
{
	FREAM = 0,	//発生
	LATER,		//後隙
	TIMER,		//持続
};

//--------------------------------------
//	**アニメーション選択(ダッシュ)
//	・NodeChange時に送り、アニメーションを指定する
//--------------------------------------
enum class AnimAtk_Dash :int
{
	FREAM = 0,	//発生
	TIMER,		//持続
};


//--------------------------------------
//	**アニメーション選択(ガード)
//	・NodeChange時に送り、アニメーションを指定する
//--------------------------------------
enum class AnimAtk_Guard :int
{
	NORMAL = 0,	//通常
	AIR,		//空中
	SQUAT,		//しゃがみ
};



//--------------------------------------
//	**攻撃時のプレイヤーの地面判定列挙
//--------------------------------------
enum class Ground_C :int
{
	AIR = 0,	//空中
	GROUND,		//地上
	EITHER,		//どちらでも
	END,
};



//---------------------------------------------
// **スピードクラス**
//・プレイヤーに加算するX,Y軸方向の力を処理する
//・攻撃関数実行時にSet関数を動かし、速度を入れる
//・空中攻撃時のポップなどに利用する
//----------------------------------------------
class Speed
{
public:
	float speed = 0.0f;
	const float decrease_value = 100.0f;		//スピードの減少値

	void Set(float speed)
	{
		this->speed = speed;
	};
	float Update(float elapsed_time)
	{
		if (speed > 0.0f)
		{
			speed -= (decrease_value * elapsed_time);
		}
		else
		{
			speed = 0.0f;
		}
		return speed;
	};
};


//---------------------------------------------
// **ステートクラス**
//・ActStateは攻撃以外の判断をする。判断しないものはcaseに入れない(その他の条件式回避のために使用する)
//・AttackStateはActStateがATTACKだった場合どの攻撃をしているかを判断する
//----------------------------------------------
enum class ActState : int
{
	//右向きの時
	NONE = 0,		//何もない(default)
	WAIT,			//待機(default)m
	GUARD,			//ガード(default)
	SQUAT,			//しゃがみ(default)m
	RETREAT,		//後退(default)m
	//DUMMY,			//ダミー(数値補間用)
	DASH,			//ダッシュ(default)m
	BACK,			//バックステップ(default)m
	JUMP,			//ジャンプ(default)m
	AIR_F,			//空中前ダッシュ(default)
	AIR_B,			//空中後ダッシュ(default)
	STATENONE,		//ステートを奪われた状態。相手に掴まれてるor超必？
	WAKE,			//ダウンから起き上がる
	PASSIVE,		//受け身をとる
	DOWN,			//ダウン状態
	FALL,			//空中でダウンしている状態
	KNOCK,			//攻撃を受けてのけぞる
	SLAM,			//叩きつけられ状態
	DOWN_HIT,		//ダウン攻撃を受けた
	ATTACK,			//攻撃中
	ACT_END,		//(終点)
};

enum class AttackState : int
{
	NONE = 0,		//攻撃無し
	JAKU,			//弱m
	THU,			//中(下中)m
	D_KYO,			//下強(中の次に出る強攻撃)
	D_JAKU,			//下弱m
	D_THU,			//下中m
	U_KYO,			//上強(強攻撃)m
	A_JAKU,			//空弱攻撃m
	A_THU,			//空中攻撃m
	A_KYO,			//空強攻撃m
	A_UKYO,			//空上強攻撃(打ち上げ攻撃)m
	STEAL,			//掴みm
	SLOW,			//投げm
	TRACK_DASH,		//ホーミングダッシュm
	JAKU_RHURF,		//前弱必殺
	THU_RHURF,		//前中必殺
	KYO_RHURF,		//前強必殺
	JAKU_LHURF,		//後弱必殺
	THU_LHURF,		//後中必殺
	KYO_LHURF,		//後強必殺
	SPECIAL_ATTACK,	//前超必殺m
	DESIRE_SPECIAL,	//後超必殺
	DESIRE_METEOR,	//即死技
	EXTENDATK,		//無敵攻撃
	JAKU_THU,		//弱の次に出る中攻撃m
	JAKU_KYO,		//中の次に出る強攻撃m
	A_JAKU_RHURF,	//空中前弱必殺
	A_THU_RHURF,	//空中前中必殺
	A_KYO_RHURF,	//空中前強必殺
	A_JAKU_LHURF,	//空中後弱必殺
	A_THU_LHURF,	//空中後中必殺
	A_KYO_LHURF,	//空中後強必殺
	A_SPECIAL_ATTACK,//空中前超必殺
	A_DESIRE_SPECIAL,//空中後超必殺

	NORMAL_ATTACK_END,//攻撃の最大サイズ(コンボ、プレイヤー選択技を省いた数<モーション用>)

	COMBO_X,		//Xボタンコンボ
	COMBO_Y,		//Yボタンコンボ
	COMBO_B,		//Bボタンコンボ

	VERSATILE_ATTACK,//Aボタンを押すと出る攻撃(プレイヤー選択)
	A_VERSATILE_ATTACK,//Aボタンを押すと出る空中攻撃(プレイヤー選択)

	ATTACK_END,		//最大サイズ
};

//Desire(欲望、望み)とCrystal(結晶)




//---------------------------------------------------
//		個別モーションデータ格納構造体
//---------------------------------------------------
// ・今までは個別の変数を一つの配列でまとめて使用していたが
//使用するメリットがないため、配列を直接持つようにする
//---------------------------------------------------
struct Model_MotionData
{
	/*std::shared_ptr<Model>			wait_R = nullptr;
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
	std::shared_ptr<Model>			extend_L = nullptr;*/

public:

	//特定の行動リストに入っていないアニメーションは個別で持つ
	std::shared_ptr<Model>			intro_R = nullptr;
	std::shared_ptr<Model>			intro_L = nullptr;
	std::shared_ptr<Model>			win_R = nullptr;
	std::shared_ptr<Model>			win_L = nullptr;
	std::shared_ptr<Model>			air_jump_R = nullptr;
	std::shared_ptr<Model>			air_jump_L = nullptr;
	std::shared_ptr<Model>			jumpToFall_R = nullptr;
	std::shared_ptr<Model>			jumpToFall_L = nullptr;
	std::shared_ptr<Model>			fall_R = nullptr;
	std::shared_ptr<Model>			fall_L = nullptr;
	std::shared_ptr<Model>			landing_R = nullptr;
	std::shared_ptr<Model>			landing_L = nullptr;
	std::array <std::shared_ptr<Model>, scastI(AttackState::NORMAL_ATTACK_END)> model_R_Attack = { nullptr };
	std::array <std::shared_ptr<Model>, scastI(AttackState::NORMAL_ATTACK_END)> model_L_Attack = { nullptr };
	std::array <std::shared_ptr<Model>, scastI(ActState::ACT_END)> model_R_Act = { nullptr };
	std::array <std::shared_ptr<Model>, scastI(ActState::ACT_END)> model_L_Act = { nullptr };
};




//---------------------------------------------
// **空中ダッシュステート列挙**
//・NONE以外の場合はそれぞれ適用した挙動を行う
//----------------------------------------------
enum class AirDashState : int
{
	NONE = 0,		//何もない
	AIR_F,			//空中ダッシュ
	AIR_B,			//空中バックダッシュ
	ACT_END,		//(終点)
};


//---------------------------------------------------------------
// **攻撃内容保存構造体**
//・攻撃の発生フレーム、内部パラメータ変数、その生成数を保存する構造体
//---------------------------------------------------------------
struct AttackSingle
{
public:
	std::vector<AttackParameter>	parameter;		//内部パラメーター
	int								quantity = 1;	//一度の攻撃で生成する当たり判定の数量
	float							fream = 0.0f;	//発生フレーム
};


//------------------------------------------------------------------------
// **攻撃リストクラス**
//・攻撃のパラメーターをキャラ生成時に読み込み、
//　攻撃を行う際に当たり判定に対応したパラメーターを送る
//			以下、整理用
//１：攻撃発生ボタンを押す
//２：プレイヤーに発生フレームを送る
//３：発生フレームになったらSetAttack関数で当たり判定を生成
//４：当たり判定の持続は当たり判定本体で調整する為何も触らない
//５：攻撃が複数回発生する場合はattack_maxにその回数を入れてその分ループさせる
//
//		攻撃当たり判定
//１：生成時にInit関数でパラメーターを送る。
//		この時、distanceに入れるのはプレイヤーからどれだけ離れているかという値
//２：Update関数の引数にプレイヤーのposを渡し、常に位置が更新されるようにする
//３：AttackBoxクラスに「攻撃判定に付与するスピードを増加させる関数」を作り、Updateで値を変える
//etc...Update内：pos=player_pos+distance+speed;
//						speedは関数で変えられるため、posの位置が変わっていく
//-------------------------------------------------------------------------
class AttackList
{
public:
	AttackState						attack_name;	//攻撃名(プレイヤー生成時に保存)
	int								attack_max;		//攻撃回数
	std::vector<AttackSingle>		attack_single;	//攻撃内容保存
	float							later;			//後隙フレーム
	int								now_attack_num;	//現在実行している攻撃番号は何番かを示す(attack_maxまで回ったらlaterをプレイヤーに)
	PAD								linkage_button;	//どのボタンで攻撃を発生させるか
	Command							linkage_command;//どのコマンドで攻撃を発生させるか
	Ground_C						ground_on;		//攻撃は空中、地上、どちらでも発生させるか
	bool							squat_on;		//しゃがみ攻撃がどうか(trueでしゃがみ攻撃)
	int								need_power;		//攻撃を行うのに必要なゲージ量(1単位で記述)
	PAD								linkage_stick;	//どの方向への入力で攻撃を発生させるか
	AttackState						aid_attack_name;//ゲージが足りなかった場合出す技
	AttackState						real_attack;	//実際の攻撃(基本はattack_nameと同じものを入れ、特定の攻撃と同じ攻撃を出す場合はその攻撃名を入れる)
	bool							speed_on;		//スピードを入れる場合はtrue
	YR_Vector3						speed;			//攻撃判定に加算する速度
	float							advance_speed;	//攻撃時前進する距離
	AttackState						combo;			//連打した時にするコンボ
	HitResult						conditions_hit;	//キャンセルするための攻撃ヒット条件
	float							timer;			//飛び道具の場合自身に付与する持続時間
	bool							traject_on;		//剣の軌跡を表示するならtrue
	EffectParameter					effect_param;	//エフェクトのパラメーター(KindがNoneなら判定ごとのパラメータを使用する)
	Effekseer::Handle				handle;			//エフェクトのハンドル

public:
	AttackList() : now_attack_num(0), attack_name(AttackState::NONE), later(0.0f),
		attack_max(0), linkage_button(PAD::BUTTOM_END), linkage_command(Command::NOCOMMAND), ground_on(Ground_C::GROUND), squat_on(false),
		need_power(0), linkage_stick(PAD::BUTTOM_END), aid_attack_name(AttackState::NONE), real_attack(attack_name),
		speed_on(false), speed(0.0f, 0.0f, 0.0f), advance_speed(0.0f), combo(AttackState::NONE), conditions_hit(HitResult::HIT),timer(0.0f),traject_on(true),effect_param(),handle(0) {};
	//攻撃当たり判定を生成する
	void SetAttack(std::vector<AttackBox> *atk, float rightOrleft, YR_Vector3 pl_pos)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->emplace_back();
			atk->back().Init(scastI(attack_name), attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos, attack_single[now_attack_num].parameter[quantity].effect_param);
		}
		now_attack_num++;
	}

	//飛び道具攻撃当たり判定をセットする
	void SetAttack(std::vector<AttackBox>* atk, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->emplace_back();
			atk->back().Init(scastI(attack_name), attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos, plus_speed, attack_single[now_attack_num].parameter[quantity].effect_param);
		}
		now_attack_num++;
	}
};




//---------------------------------------------------------------------------
// **当たり判定リストクラス**
//・当たり判定のパラメーターを各行動ごとに保存するクラス
//・プレイヤークラスのUpdateで毎回パラメーターを入れていく
//・ただし当たり判定の数はキャラによって変わる為、変数はvector型で持たせ、
//　キャラ生成時に必要な個数分生成する
//----------------------------------------------------------------------------
class HitParameterList
{
public:
	std::array<HitParameter,scastI(ActState::ACT_END)>			act_parameter;		//攻撃以外の行動時の当たり判定
	std::array<HitParameter, scastI(AttackState::ATTACK_END)>	attack_parameter;	//攻撃時の当たり判定
};




//---------------------------------------------------------------
// **アニメーション調整値保存構造体**
//・発生、持続、後隙それぞれのアニメーション速度調整値を保存する構造体
//---------------------------------------------------------------
struct Animation_Coordinate
{
public:
	float							fream = 1.0f;			//発生
	float							timer = 1.0f;			//持続
	float							later = 1.0f;			//後隙
};


//---------------------------------------------------------------
// **コンボ保存構造体**
//・コンボの種類、現在の番号を保存する構造体
//---------------------------------------------------------------
struct ComboList
{
public:
	std::vector<AttackState>		combolist;		//コンボリスト
	int								now_pos = 0;	//現在のコンボの位置
};




//---------------------------------------------------------------
// **キャラ毎のステータス保持クラス**
//・キャラ毎のダッシュ速度やジャンプ速度の数値をテキストファイルから
//  読み込み、割り当てるクラス。
//・変更した値をテキストに書き出す役割ももつ
//(※デフォルトの値としてナイトの初期ステータスを入れている)
//---------------------------------------------------------------
class CharaStateParameter
{
public:
	float walkspeed = 20.0f;					//歩く速度(代入)
	float dashspeed = 40.0f;					//ダッシュ速度
	float backstepS = 116.0f;					//バックステップの速度
	float backstepD = 500.0f;					//バックステップの減少速度
	float stepspeed = 100.0f;					//空中ステップの速度
	float stepD = 500.0f;						//空中ステップの減少速度
	float jump_max = 108.0f;					//ジャンプの最大速度(超えると減速し始め落ちる)
	float high_jump_max = 47.0f;				//ハイジャンプの最大速度(超えると減速し始め落ちる)
	float jump_speed = 108.0f;					//毎フレームジャンプの速度に加算する数値
	float high_jump_speed = 139.0f;				//毎フレームハイジャンプの速度に加算する数値
	float brake_speed = 10000.0f;				//停止時にかかるブレーキ(基本ピタッと止まるので数値は大きめ)
	float track_speed = 100.0f;					//ホーミングダッシュの速度
	YR_Vector3 passive_speed = { 70.0f,70.0f };	//受け身状態にスピードに代入する速度
	float passive_brake = 200.0f;				//受け身中に減速する速度
	float max_hp = 1000.0f;						//HPの初期値

public:
	void Load(PLSELECT chara_name);		//テキストファイルから読み込み
	void Save(PLSELECT chara_name);		//テキストファイル書き出し

	void Draw();		//デバッグ用のツールを表示する
};





class Player
{
protected:
	const float jump_max_time = 0.05f;	//ジャンプしたらjump_can_timerに入れる値
	const float guard_range = 15.0f;		//ガード状態に入る距離
public:
	//モデル用変数
	std::shared_ptr<Model>			main = nullptr;	//モデルメインデータ(メッシュのみ。アニメーション無し)
	std::unique_ptr<ModelAnim>		anim = nullptr;	//モデル描画&アニメーション適用変数

	//個別モーション用変数
	Model_MotionData				model_motion;
public:
	//ゲーム内処理用変数
	CharaStateParameter	chara_state;							//キャラパラメーター(ダッシュ速度など)
	PLSELECT			chara_name = PLSELECT::KNIGHT;			//キャラ名
	float				hp = 0;									//体力
	int					jumpcount = 2;							//ジャンプの回数保持
	int					air_dash_count = 1;						//空中ダッシュの回数保持
	bool				jumpflag = false;						//ジャンプしている
	bool				max_jump_flag = false;					//ジャンプの最大値を超えた
	float				jump_can_timer = 0.0f;					//ジャンプ可能になるまでの時間を測定する(0.0以下になればジャンプ可能)
	bool				ground = true;							//TRUEなら地面についている
	bool				attack = false;							//TRUEなら攻撃中
	ActState			act_state = ActState::NONE;				//今の行動。また行動不可ならどういう状態か
	ActState			rival_state = ActState::NONE;			//相手の今の行動。SceneGameで代入する
	float				rival_fream = non_target;				//相手の今のフレーム(発生)情報。SceneGameで代入し、ガード時等に使用する
	AttackState			attack_state = AttackState::NONE;		//今何の攻撃をしているか
	AttackState			last_attack = AttackState::NONE;		//最後になんの攻撃をしたか
	AirDashState		air_dash_state = AirDashState::NONE;	//空中ダッシュステート
	float				rightOrleft = 1.0f;						//右向きなら*1左向きなら*-1
	bool				moveflag = false;						//TRUEなら動いている
	float				fream = non_target;						//発生フレーム
	float				later = non_target;						//後隙フレーム(初期値にnon_targetを入れておく)
	float				timer = non_target;						//持続フレーム(初期値にnon_targetを入れておく)
	float				knocktimer = 0.0f;						//喰らい時間
	float				gravity = 0.0f;							//重力値
	float				down_force = 0.0f;						//ジャンプ時に重力とは別に働く下方向の力
	float				fall_force = 0.0f;						//空中ダウン中に重力とは別に働く下方向の力
	bool				finish = false;							//行動が終了したときのみtrue
	bool				step = false;							//ステップ中はtrue(他の処理を行わない)
	bool				hightrigger = false;					//ハイジャンプ時true、通常時false
	int					trackgauge = 2;							//追尾ダッシュの残り回数
	float				gauge = 0.0f;							//ゲージ
	int					power = 1;								//ゲージが最大になると1増える。特殊技はこっちを消費する
	float				steal_escape = 0.0f;					//投げ抜け可能時間
	int					combo_count = 0;						//コンボカウント。こっちは攻撃を受けた際に増え、相手に送るカウント
	int					now_player = 0;							//どのプレイヤーがこのキャラを操作しているか(1:1P、2:2P)
	float				anim_ccodinate = 1.0f;					//アニメーション速度を調整する変数
	int					stop_state = 0;							//ヒットストップ中の処理で使用
	float				cut_in_timer = 0.0f;					//カットインの表示時間
	float				passive_timer = 0.0f;					//受け身中の無敵時間測定変数
	int					combo_count_player = 0;					//コンボカウント。こちらは相手から受け取る
	int					draw_count = 0.0f;						//描画時に使用するカウント
	DirectX::XMFLOAT4	light_direction =
		DirectX::XMFLOAT4(-1.0f, -0.1, 1.0f, 0.0f);				//ライトの進む方向
	HitResult			hit_result = HitResult::NOT_OCCURRENCE;	//攻撃が当たった場合の結果を保存する
	HitResult			atk_result = HitResult::HIT;			//攻撃決定時にキャンセル用の条件を保存するための変数
	std::vector<AttackBox> atk;									//攻撃当たり判定
	std::vector<AttackBox> projectile_atk;						//飛び道具当たり判定
	std::vector<HitBox>	hit;									//当たり判定
	bool				traject_on = true;						//軌跡を表示する場合true(攻撃発生時にパラメーターから取得する)


	std::array<Animation_Coordinate, scastI(AttackState::ATTACK_END)>	ac_attack;	//攻撃ごとのアニメーション調整値
	std::array<Animation_Coordinate, scastI(ActState::ACT_END)>			ac_act;		//行動ごとのアニメーション調整値

	float												jump_later = 1.0f;			//ジャンプの着地隙
	float												dash_later = 1.0f;			//ダッシュの後隙

	std::vector<AttackList>								attack_list;	//攻撃のリスト。生成時に読み込み、保存する(攻撃発生時にパラメーターを送る)
	std::vector<HitParameterList>						hitparam_list;	//当たり判定のリスト。生成時に読み込み、保存する(当たり判定の数だけ生成する)
	ComboList											combolist_X;	//Xボタンコンボリスト
	ComboList											combolist_Y;	//Yボタンコンボリスト
	ComboList											combolist_B;	//Bボタンコンボリスト

public:
	//モデル用変数
	//MeshMotion						motion;
	//std::unique_ptr<Skinned_mesh>	base = nullptr;
public:
	//座標、モデル描画用変数
	YR_Vector3		pos = { 0.0f,0.0f,0.0f };
	YR_Vector3		scale = { 1.0f,1.0f,1.0f };
	YR_Vector3		angle = { 0.0f,0.0f,0.0f };
	YR_Vector3		speed = { 0.0f,0.0f,0.0f };
	YR_Vector3		stop_pos = { 0.0f,0.0f,0.0f };
	Tracking		tracking;
	Speed			speed_X;
	Speed			speed_Y;
	std::unique_ptr<GamepadBase> pad;

	int				chara_color = 0;		//キャラのカラー番号

	Model::Material_Attribute		lumi_material;	//ブルームを適用するマテリアル


public:
	//画像
	std::unique_ptr<Sprite> cutFrame = nullptr;		//カットインのフレーム
	std::unique_ptr<Sprite> cutMask = nullptr;		//カットインのマスク画像
	std::unique_ptr<Sprite> cutIn = nullptr;		//カットイン画像

	float		production_time = 0.0f;	//カメラ演出に使う変数

public:
	std::wstring lip_text;								//表示するテキスト
	bool		text_on = false;						//trueならテキストを表示する

public:
	//基本処理関数
	virtual void Init(YR_Vector3 InitPos);
	virtual void CharaInit() = 0;

	virtual void LoadData(PLCOLOR color_number) = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision,float elapsed_time);
	virtual void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time ) = 0;

	virtual void DrawDEBUG(
		YRShader* geoshader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);

	virtual void DrawDEBUGHitParam();

	virtual void DrawCutIn(
		YRShader* shader,
		float elapsed_time
	);

	virtual void TextDraw();			//テキストを描画する

	virtual ~Player() = default;

	virtual float Getapply(float n);

public:
	//行動系関数

	virtual void MoveAnimSet();
	virtual void Move(float decision);
	virtual void MoveStop();
	virtual bool Step(float elapsed_time);
	virtual void AirDash(float elapsed_time);
	virtual void Jump();
	virtual void JumpUpdate(float decision, float elapsed_time);
	virtual void DamageCheck(float decision);
	virtual void GuardAnimSet();
	virtual void GuardCheack(float decision);
	virtual void Guard(float decision);
	virtual void GuardBack(float elapsed_time);
	virtual void HitGuardCheck(float decision);
	virtual void Squat();
	virtual void FallUpdate(float elapsed_time);
	virtual void DownUpdate();
	virtual void PassiveUpdate(float elapsed_time);
	virtual void WakeUp();
	virtual void GaugeUp(float add);
	virtual void StateNone(float elapsed_time);
	virtual void KnockUpdate(float elapsed_time);
	virtual void DownHitUpdate(float elapsed_time);
	virtual void SlamUpdate(float elapsed_time);
	virtual void WaitAnimSet();
	virtual void NoneChange();
	virtual void Brake(float elapsed_time);
	virtual bool BackStepCheck();

	virtual void AnimChangeSelect(ActState act);

	virtual void HitBoxTransition(HitBoxState state);
	virtual void AttackDetailsSet(const AttackState & attack_state);

	virtual void AttackInput();			//攻撃するボタンが押されたか確認し、押されていればその行動をさせる
	virtual void StopUpdate();
	virtual void StopEnd();
	virtual void StopHitParamUpdate();

	virtual bool AttackLoad();		//生成時攻撃パラメーターを読み込む
	virtual bool AttackWrite();		//デバック時パラメーターを書き出す
	virtual bool AttackClean();		//デバッグ時パラメーターを全て初期化する

	virtual void Attack(float decision, float elapsed_time);	//以下の関数を制御する
	virtual void AttackSwitch(float decision, float elapsed_time, AttackState attack_state = AttackState::NONE);//attack_stateによってそれぞれ異なる関数を動かす




	virtual void WinAnimSet() = 0;
	virtual bool Intro(float elapsed_time) = 0;
	virtual void IntroDEBUG() = 0;
	virtual bool WinPerformance(float elapsed_time) = 0;
	virtual void WinDEBUG() = 0;
	virtual void ReadySet() = 0;
	virtual void HitBoxReset();


	//----------------------------------//
	//			攻撃関数					//
	//----------------------------------//
	virtual void Jaku(float elapsed_time) = 0;
	virtual void Thu(float elapsed_time) = 0;
	virtual void Kyo(float elapsed_time) = 0;
	virtual void U_Kyo(float elapsed_time) = 0;
	virtual void D_Jaku(float elapsed_time) = 0;
	virtual void D_Thu(float elapsed_time) = 0;
	virtual void A_Jaku(float elapsed_time) = 0;
	virtual void A_Thu(float elapsed_time) = 0;
	virtual void A_Kyo(float elapsed_time) = 0;
	virtual void A_UKyo(float elapsed_time) = 0;

	virtual void Jaku_Rhurf(float elapsed_time) = 0;
	virtual void Thu_Rhurf(float elapsed_time) = 0;
	virtual void Kyo_Rhurf(float elapsed_time) = 0;
	virtual void Jaku_Lhurf(float elapsed_time) = 0;
	virtual void Thu_Lhurf(float elapsed_time) = 0;
	virtual void Kyo_Lhurf(float elapsed_time) = 0;

	virtual void TrackDash(float decision, float elapsed_time) = 0;
	virtual void Steal(float elapsed_time) = 0;
	virtual void Slow(float elapsed_time) = 0;
	virtual void SpecialAttack(float elapsed_time) = 0;
	virtual void ExtendATK(float elapsed_time) = 0;

	virtual bool StealRangeCheck();
	virtual void PosKnockPlus(float vec);

	virtual bool ComboSet();					//コンボ開始時、どの攻撃をしているかを確認してその後のコンボをセットする
	virtual void ComboUpdate();					//コンボ更新。ステートを設定する

	virtual void ComboX(float decision, float elapsed_time);	//Xボタンコンボ関数
	virtual void ComboY(float decision, float elapsed_time);	//Xボタンコンボ関数
	virtual void ComboB(float decision, float elapsed_time);	//Xボタンコンボ関数


	virtual void AttackDefault(float elapsed_time);//特殊な記述のある攻撃以外はこの関数を使用する
	virtual void AttackProjectileDefault(float elapsed_time);//特殊な記述のある飛び道具攻撃以外はこの関数を使用する



	//空中版コマンドはキャラによっては実装しないため、
	//意図的に純粋仮想関数にしていない
	virtual void A_Jaku_Rhurf(float elapsed_time);
	virtual void A_Thu_Rhurf(float elapsed_time);
	virtual void A_Kyo_Rhurf(float elapsed_time);
	virtual void A_Jaku_Lhurf(float elapsed_time);
	virtual void A_Thu_Lhurf(float elapsed_time);
	virtual void A_Kyo_Lhurf(float elapsed_time);


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

	virtual void ChangeFace(FaceAnim anim);						//表情を変える関数(enumで定義)



public:
	virtual bool AttackEndCheck();								//攻撃当たり判定が全て終了しているか確認する
	virtual void EndAttackErase();								//終了した攻撃当たり判定を全て消去する。
	virtual void AllAttackClear();								//全ての攻撃当たり判定を消去する
	virtual void AttackUpdate(float elapsed_time);				//攻撃判定が存在する時のみ更新
	virtual void HitResultUpdate();								//攻撃判定が存在する時のみ更新(当たり判定が取得した結果をプレイヤーに送る)

};

#endif // !PLAYERBASE_H_