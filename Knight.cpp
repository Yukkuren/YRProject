#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"
#include <algorithm>
//#define _CRTDBG_MAP_ALLOC						// mallocによるメモリリーク検出でCPPファイル名と行数出力指定
//#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__)	// new によるメモリリーク検出でCPPファイル名と行数出力指定
//#include <stdio.h>
//#include <crtdbg.h>


Knight::~Knight() = default;

void Knight::Init(YR_Vector3 InitPos)
{
	//ImGui::Begin("RYU HitBox");
	pos = InitPos;
	scale = YR_Vector3( 0.1f,0.1f,0.1f );
	//scale = YR_Vector3( 0.05f,0.05f,0.05f );
	angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	speed.x = 0;
	speed.y = 0.0f;
	jumpcount = 2;
	max_jump_flag = false;
	hp = 1000;
	gravity = 40.0f;
	down_force = 200.0f;
	fall_force = 100.0f;
	knocktimer = 0.0f;
	ground = true;
	jumpflag = false;
	drawset = false;
	specialfream = 0.0f;
	fast = false;
	finish = false;
	stop_pos = { 0.0f,0.0f };
	step = false;
	hightrigger = false;
	trackgauge = 2;
	gauge = 0;
	power = 0;
	steal_escape = 0;
	speed_Y.Set(0.0f);
	combo_count = 0;
	intro_state = INTRO_KNIGHT::SET;
	win_state = WIN_PERFORMANCE_KNIGHT::CAMERA_ZOOM;

	fream = 0.0f;

	production_time = 0.0f;
	anim_ccodinate = 2.0f;

	stop_state = 0;
#pragma region HITBOXINIT
	for (int act = 0; act < hit.size(); act++)
	{
		hit[act].Init();
	}
#pragma endregion

	hadouspeed = 0.0f;
	moveflag = false;
	attack = FALSE;
	attack_state = AttackState::NONE;
	act_state = ActState::NONE;
	later = non_target;
	hadou = { pos.x + Getapply(100),pos.y };

	eye_offset = { 0.0f,0.0f };
	mouth_offset = { 0.0f,0.0f };

	face_anim = FaceAnim::NORMAL;
	face_wink_time = 0.0f;
	face_wink_interval_timer = 0.0f;
	wink_state = Wink_State::FIRST;
	jump_can_timer = 0.0f;
	lip_sync_time = 0.0f;

	camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
	intro_timer = 0.0f;

	eye_plus = YR_Vector3(0.0f, 0.0f, -10.0f);
	focus_plus = YR_Vector3(0.0f, 0.0f, 0.0f);
	face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
	ChangeFace(FaceAnim::NORMAL_LIP_SYNC);

	text_on = false;

	if (now_player == 1)
	{
		anim->NodeChange(model_motion.intro_R);
	}
	else
	{
		anim->NodeChange(model_motion.intro_L);
	}
}


void Knight::LoadData(int color_number)
{
	attack_list.resize(1);
#if USE_IMGUI
	DEBUGAttackLoad();
	AttackClean();
	DEBUGAttackLoad();
#endif // 0
	//AttackLoad();

	

	switch (color_number)
	{
	case 1:
		color_texture_main = nullptr;
		color_texture_face = nullptr;
		break;
	case 2:
		color_texture_main = std::make_shared<Texture>(L"./Data/FBX/Knight/knight_tex_nofaces2.png");
		color_texture_face = std::make_shared<Texture>(L"./Data/FBX/Knight/knight_tex_face2.png");
		break;
	default:
		color_texture_main = nullptr;
		color_texture_face = nullptr;
		break;
	}


	if (main == nullptr)
	{
		
		if (color_texture_main != nullptr)
		{
			main = std::make_shared<Model>("./Data/FBX/Knight/knight_main.fbx", color_texture_main, color_texture_face);
		}
		else
		{
			//main = std::make_shared<Model>("./Data/FBX/Knight/knight_main.fbx");
			main   = std::make_shared<Model>("./Data/FBX/Knight/knight_main.fbx");
		}
	}
	//------------------------------------------//
	//		アニメーションボーンデータ読み込み		//
	//------------------------------------------//
	//待機モーション
	if (model_motion.wait_R == nullptr)
	{
		model_motion.wait_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_wait.fbx");
	}

	//しゃがみ
	if (model_motion.squat_R == nullptr)
	{
		model_motion.squat_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_squat_R.fbx");
	}

	//歩き
	if (model_motion.walk_R == nullptr)
	{
		model_motion.walk_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_walk_R.fbx");
	}

	//後退
	if (model_motion.back_R == nullptr)
	{
		model_motion.back_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_back_R.fbx");
	}

	//ダッシュ
	if (model_motion.dash_R == nullptr)
	{
		model_motion.dash_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_dash_R.fbx");
	}

	//バックステップ
	if (model_motion.backstep_R == nullptr)
	{
		model_motion.backstep_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_backstep_R.fbx");
	}

	//ジャンプ
	if (model_motion.jump_R == nullptr)
	{
		model_motion.jump_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_jump_R.fbx");
	}

	//空中ジャンプ
	if (model_motion.air_jump_R == nullptr)
	{
		model_motion.air_jump_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_air_jump_R_f.fbx");
	}

	//イントロモーション
	if (model_motion.intro_R == nullptr)
	{
		model_motion.intro_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_Intro_R.fbx");
	}
	if (model_motion.intro_L == nullptr)
	{
		model_motion.intro_L = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_Intro_L.fbx");
	}

	//ダメージモーション
	if (model_motion.damage_R_g_u == nullptr)
	{
		model_motion.damage_R_g_u = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_damage_R_g_u.fbx");
	}

	//弱攻撃
	if (model_motion.jaku_R == nullptr)
	{
		model_motion.jaku_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_jaku_R.fbx");
	}

	//中攻撃
	if (model_motion.thu_R == nullptr)
	{
		model_motion.thu_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_thu_R.fbx");
	}

	//強攻撃(対空&下強)
	if (model_motion.kyo_R == nullptr)
	{
		model_motion.kyo_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_kyo_R.fbx");
	}

	//下弱攻撃
	if (model_motion.d_jaku_R == nullptr)
	{
		model_motion.d_jaku_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_d_jaku_R.fbx");
	}

	//下中攻撃
	if (model_motion.d_thu_R == nullptr)
	{
		model_motion.d_thu_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_d_thu_R.fbx");
	}

	//上強攻撃
	if (model_motion.u_kyo_R == nullptr)
	{
		model_motion.u_kyo_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_u_kyo_R.fbx");
	}

	//上強攻撃
	if (model_motion.u_kyo_R == nullptr)
	{
		model_motion.u_kyo_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_u_kyo_R.fbx");
	}

	//空弱攻撃
	if (model_motion.a_jaku_R == nullptr)
	{
		model_motion.a_jaku_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_air_jaku_R.fbx");
	}

	//空中攻撃
	if (model_motion.a_thu_R == nullptr)
	{
		model_motion.a_thu_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_air_thu_R.fbx");
	}

	//空強攻撃
	if (model_motion.a_kyo_R == nullptr)
	{
		model_motion.a_kyo_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_air_kyo_R.fbx");
	}

	//空上強攻撃
	if (model_motion.a_ukyo_R == nullptr)
	{
		model_motion.a_ukyo_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_air_ukyo_R.fbx");
	}

	//掴み攻撃
	if (model_motion.steal_R == nullptr)
	{
		model_motion.steal_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_steal_R.fbx");
	}

	//投げ攻撃
	if (model_motion.slow_R == nullptr)
	{
		model_motion.slow_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_slow_R.fbx");
	}

	//ホーミングダッシュ攻撃
	if (model_motion.track_R == nullptr)
	{
		model_motion.track_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_track_R.fbx");
	}

	//超必殺技
	if (model_motion.special_R == nullptr)
	{
		model_motion.special_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_special_R.fbx");
	}

	{
		model_motion.model_R.push_back(model_motion.wait_R);	//攻撃無し(代わりとして待機を入れている)
		model_motion.model_R.push_back(model_motion.jaku_R);	//弱
		model_motion.model_R.push_back(model_motion.d_thu_R);	//中(下中)
		model_motion.model_R.push_back(model_motion.kyo_R);		//下強(中の次に出る強攻撃)
		model_motion.model_R.push_back(model_motion.d_jaku_R);	//下弱
		model_motion.model_R.push_back(model_motion.d_thu_R);	//下中
		model_motion.model_R.push_back(model_motion.u_kyo_R);	//上強
		model_motion.model_R.push_back(model_motion.a_jaku_R);	//空弱攻撃
		model_motion.model_R.push_back(model_motion.a_thu_R);	//空中攻撃
		model_motion.model_R.push_back(model_motion.a_kyo_R);	//空強攻撃
		model_motion.model_R.push_back(model_motion.a_ukyo_R);	//空上強攻撃(打ち上げ攻撃)
		model_motion.model_R.push_back(model_motion.steal_R);	//掴み
		model_motion.model_R.push_back(model_motion.slow_R);	//投げ
		model_motion.model_R.push_back(model_motion.track_R);	//ホーミングダッシュ
		model_motion.model_R.push_back(model_motion.jaku_rh_R);	//前弱必殺
		model_motion.model_R.push_back(model_motion.thu_rh_R);	//前中必殺
		model_motion.model_R.push_back(model_motion.kyo_rh_R);	//前強必殺
		model_motion.model_R.push_back(model_motion.jaku_lh_R);	//後弱必殺
		model_motion.model_R.push_back(model_motion.thu_lh_R);	//後中必殺
		model_motion.model_R.push_back(model_motion.kyo_lh_R);	//後強必殺
		model_motion.model_R.push_back(model_motion.special_R);	//前超必殺
		model_motion.model_R.push_back(model_motion.disire_s_R);//後超必殺
		model_motion.model_R.push_back(model_motion.disire_m_R);//即死技
		model_motion.model_R.push_back(model_motion.extend_R);	//無敵攻撃
		model_motion.model_R.push_back(model_motion.thu_R);		//弱の次に出る中攻撃
		model_motion.model_R.push_back(model_motion.kyo_R);		//中の次に出る強攻撃

	}


	//------------------------------------------//
	//			アニメーション設定				//
	//------------------------------------------//
	if (anim == nullptr)
	{
		anim = std::make_unique<ModelAnim>(main);
		//anim->PlayAnimation(0, true);
		/*if (now_player == 1)
		{
			anim->NodeChange(model_motion.intro_R);
		}
		else
		{
			anim->NodeChange(model_motion.intro_L);
		}*/
	}
}

void Knight::Uninit()
{
	//モデル系を全て解放する
	main.reset();
	main = nullptr;
	anim.reset();
	anim = nullptr;
	model_motion.wait_R.reset();
	model_motion.wait_R = nullptr;
	model_motion.jaku_R.reset();
	model_motion.jaku_R = nullptr;
	model_motion.special_R.reset();
	model_motion.special_R = nullptr;
	model_motion.damage_R_g_u.reset();
	model_motion.damage_R_g_u = nullptr;

	model_motion.thu_R.reset();
	model_motion.kyo_R.reset();
	model_motion.d_jaku_R.reset();
	model_motion.d_thu_R.reset();
	model_motion.u_kyo_R.reset();
	model_motion.a_jaku_R.reset();
	model_motion.a_thu_R.reset();
	model_motion.a_kyo_R.reset();
	model_motion.a_ukyo_R.reset();
	model_motion.steal_R.reset();
	model_motion.slow_R.reset();
	model_motion.track_R.reset();
	model_motion.jaku_rh_R.reset();
	model_motion.thu_rh_R.reset();
	model_motion.kyo_rh_R.reset();
	model_motion.jaku_lh_R.reset();
	model_motion.thu_lh_R.reset();
	model_motion.kyo_lh_R.reset();
	model_motion.disire_s_R.reset();
	model_motion.disire_m_R.reset();
	model_motion.extend_R.reset();

	model_motion.thu_R = nullptr;
	model_motion.kyo_R = nullptr;
	model_motion.d_jaku_R = nullptr;
	model_motion.d_thu_R = nullptr;
	model_motion.u_kyo_R = nullptr;
	model_motion.a_jaku_R = nullptr;
	model_motion.a_thu_R = nullptr;
	model_motion.a_kyo_R = nullptr;
	model_motion.a_ukyo_R = nullptr;
	model_motion.steal_R = nullptr;
	model_motion.slow_R = nullptr;
	model_motion.track_R = nullptr;
	model_motion.jaku_rh_R = nullptr;
	model_motion.thu_rh_R = nullptr;
	model_motion.kyo_rh_R = nullptr;
	model_motion.jaku_lh_R = nullptr;
	model_motion.thu_lh_R = nullptr;
	model_motion.kyo_lh_R = nullptr;
	model_motion.disire_s_R = nullptr;
	model_motion.disire_m_R = nullptr;
	model_motion.extend_R = nullptr;

	model_motion.model_R.clear();
}

void Knight::Update(float decision, float elapsed_time)
{
	finish = false;
	DamageCheck();
	WaitAnimSet();
	if (pos.y <= POS_Y)
	{
		ground = true;
		speed_Y.Set(0.0f);
	}
	else
	{
		ground = false;
	}

	//-----------------------------------------------------
	// *概要*
	//・攻撃を受けるなど操作不能の状態のみステートで区分けし、
	//　それ以外は操作を受け付けるのでCANACTで処理している
	//-----------------------------------------------------
	switch (act_state)
	{
	case ActState::STATENONE:
		//ステートを奪われた状態。相手に掴まれてる
		StateNone();
		break;
	case ActState::WAKE:
		//ダウンから起き上がる
		WakeUp();
		break;
	case ActState::PASSIVE:
		//受け身をとる
		PassiveUpdate();
		break;
	case ActState::DOWN:
		//ダウン状態
		DownUpdate();
		break;
	case ActState::FALL:
		//空中でダウンしている状態
		FallUpdate(elapsed_time);
		break;
	case ActState::KNOCK:
		//攻撃を受けてのけぞる
		KnockUpdate(elapsed_time);
		break;
	case ActState::ATTACK:
		//攻撃中
		Attack(decision, elapsed_time);
		break;
	default:
		//その他個別処理
		if (ground)
		{
			//地面設置時
			pos.y = POS_Y;	//地面に設置しているときは高さを固定する
			trackgauge = Track_max;	//ホーミングダッシュ回数を戻す
			if (act_state == ActState::JUMP)
			{
				//ステートがジャンプの時はNONEに戻す
				act_state = ActState::NONE;
			}
		}
		else
		{
			//空中時
			if (pos.y > POS_Y)
			{
				//重力を付与する
				pos.y -= gravity*elapsed_time;
			}
		}

		{
		//攻撃を行っていないときの処理
			if (ground && !step)
			{
				jumpcount = 2;
				rightOrleft = decision;
				if (speed.x > 0.0f)
				{
					speed.x -= brake_speed*elapsed_time;
					if (speed.x < 0.0f)
					{
						speed.x = 0;
					}
				}
				if (speed.x < 0.0f)
				{
					speed.x += brake_speed * elapsed_time;
					if (speed.x > 0.0f)
					{
						speed.x = 0.0f;
					}
				}
			}
			//しゃがみやガードなどを先に判定
			Squat();
			//ガードでない時
			if (act_state != ActState::GUARD)
			{
				Step();
				if (step)
				{
					//ステップ中は処理しない
				}
				else
				{

					//地面についている場合のみ
					if (ground)
					{
						if (!moveflag)
						{
							MoveAnimSet();
						}
						if (moveflag)
						{
							Move(rightOrleft);
						}
					}

					AttackInput();
				}

				if (!attack)
				{
					Jump();
				}

			}
			NoneChange();

		}
		break;
	}

	if (finish)
	{
		pad->pre_input = true;
	}
	else
	{
		pad->pre_input = false;
	}

	JumpUpdate(elapsed_time);

	pos.x += speed.x * elapsed_time;
	pos.y += speed_Y.Update(elapsed_time);

	for (int list = 0; list < hit.size(); list++)
	{
		if (act_state == ActState::ATTACK)
		{
			hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(attack_state)], elapsed_time);
		}
		else
		{
			hit[list].Update(pos, hitparam_list[list].act_parameter[scastI(act_state)], elapsed_time);
		}
	}

	Guard(decision);
	GuardBack(elapsed_time);
	GuardAnimSet();

	if (!attack)
	{
		if (act_state == ActState::DASH)
		{
			//特定の行動時は指定した値を入れる為なにも処理をしない
		}
		else
		{
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}
	/*if (attack_state != AttackState::D_THU && act_state != ActState::DOWN && act_state != ActState::SQUAT && attack_state != AttackState::D_JAKU)
	{
		hit[scastI(KNIGHTHIT::BODY)].Init(pos, YR_Vector3(0.0f, 0.0f), YR_Vector3(2.0f, 2.9f));
		hit[scastI(KNIGHTHIT::LEG)].Init(pos, YR_Vector3(0.0f, 0.0f), YR_Vector3(1.4f, 0.8f));
	}

	hit[scastI(KNIGHTHIT::BODY)].Update(pos,elapsed_time);
	hit[scastI(KNIGHTHIT::LEG)].Update(pos, elapsed_time);*/

	EndAttackErase();			//攻撃判定の消去

	FaceAnimation(elapsed_time);
}

void Knight::AttackInput()
{
	
			//-------------------------------------------------------------------
			// *概要*
			//・コマンドは二種類のみ
			//--------------------------------------------------------------------
				//攻撃のリストの入力判定を全て確認する(0は攻撃無しの為、1から確認していく)
	for (int list = 1; list < attack_list.size(); list++)
	{
		int button = scastI(attack_list[list].linkage_button);
		Command now_com = attack_list[list].linkage_command;
		if (pad->x_input[button] == 1)
		{
			pad->que.back().timer = 0;
			//コマンド判定
			Command command = pad->com_list.CommandCheack(button, pad->input_history.data(), rightOrleft);
			if (now_com == command)
			{
				//指定したコマンドが同じで空中、地上の条件も同じだった場合
				if (ground == attack_list[list].ground_on)
				{
					if (attack_list[list].squat_on)
					{
						//攻撃リストにしゃがみ判定が設定されていた場合、現在のステートがしゃがみか確認する
						if (act_state != ActState::SQUAT && pad->x_input[scastI(PAD::STICK_D)] == 0)
						{
							if (pad->x_input[scastI(PAD::STICK_LDown)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0)
							{
								continue;
							}
						}
					}
					else
					{
						//設定されていない場合はしゃがんでないか確認する
						if (act_state == ActState::SQUAT || pad->x_input[scastI(PAD::STICK_D)] > 0||
							pad->x_input[scastI(PAD::STICK_LDown)] > 0|| pad->x_input[scastI(PAD::STICK_RDown)] > 0)
						{
							continue;
						}
					}
					if (attack_list[list].linkage_stick != PAD::BUTTOM_END)
					{
						//スティックの入力が指定されている場合確認する
						if (pad->x_input[scastI(attack_list[list].linkage_stick)] == 0)
						{
							continue;
						}
					}
					//実際の攻撃内容
					int real = scastI(attack_list[list].real_attack);

					if (attack_list[real].need_gauge <= gauge)
					{
						//ゲージの必要量を確認する
						//通常の攻撃の場合、実際の攻撃内容は入れずに攻撃名をそのまま入れる
						attack_state = static_cast<AttackState>(list);
					}
					else
					{
						//ゲージが足りない場合指定した技を出す
						attack_state = attack_list[real].aid_attack_name;
					}


					//攻撃を決定する
					//現在攻撃判定が出ているなら全て消去する
					AllAttackClear();
					//攻撃の結果を初期化
					hit_result = HitResult::NONE;
					//攻撃中フラグをオンに
					attack = true;
					//移動フラグをオフに
					moveflag = false;
					//行動ステートを攻撃に
					act_state = ActState::ATTACK;
					//発生フレームを決定
					fream = attack_list[real].attack_single[0].fream;
					//アニメーション速度を指定
					anim_ccodinate = ac_attack[real].fream;
					//攻撃番号を初期化
					attack_list[real].now_attack_num = 0;
					//後隙を初期化
					later = non_target;
					//カメラ処理用変数を初期化
					production_time = 0.0f;
					//描画をセット
					anim->NodeChange(model_motion.model_R[real], scastI(AnimAtk::FREAM));
					//攻撃ごとに個別の設定を行う
					AttackDetailsSet(attack_list[list].real_attack);
					return;
				}
			}
		}
	}

			//	//弱攻撃
			//	if (pad->x_input[scastI(PAD::X)] == 1)
			//	{
			//		pad->que.back().timer = 0;
			//		attack = TRUE;
			//		moveflag = false;
			//		act_state = ActState::ATTACK;
			//		//コマンド判定
			//		Command command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
			//		if (command == Command::RHURF)
			//		{
			//			//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
			//			//描画をセット
			//			attack_state = AttackState::JAKU_RHURF;
			//			//pos.x += Getapply(50.0f);
			//			specialfream = 0;
			//		}
			//		else if (command == Command::LHURF)
			//		{
			//			//描画をセット

			//			attack_state = AttackState::JAKU_LHURF;
			//			specialfream = 0;
			//		}
			//		else
			//		{
			//			//描画をセット

			//			//attack_state = AttackState::JAKU;
			//			//fream = attack_list[scastI(attack_state)].attack_single[0].fream;
			//			//motion.MeshSet(jaku_r_f);
			//			//motion.AnimReset();
			//			//anim_ccodinate = ac_attack[scastI(attack_state)].fream;
			//			//anim->NodeChange(model_motion.jaku_R_f);
			//			//attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft);
			//		}
			//	}

			//	//下段中攻撃
			//	if (pad->x_input[scastI(PAD::Y)] == 1)
			//	{
			//		pad->que.back().timer = 0;
			//		//attack = TRUE;
			//		//moveflag = false;
			//		//act_state = ActState::ATTACK;
			//		//コマンド判定
			//		Command command = pad->com_list.CommandCheack(scastI(PAD::Y), pad->input_history.data(), rightOrleft);
			//		if (command == Command::RHURF)
			//		{
			//			//描画をセット

			//			attack_state = AttackState::THU_RHURF;
			//			specialfream = 0;
			//		}
			//		else if (command == Command::LHURF)
			//		{
			//			//描画をセット

			//			attack_state = AttackState::THU_LHURF;
			//			specialfream = 0;
			//		}
			//		else
			//		{
			//			//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
			//			//描画をセット

			//			//attack_state = AttackState::D_THU;
			//			//pos.x += Getapply(50.0f);
			//			ChangeFace(FaceAnim::Damage);
			//		}
			//	}

			//	//上段強攻撃
			//	if (pad->x_input[scastI(PAD::B)] == 1)
			//	{
			//		pad->que.back().timer = 0;
			//		attack = TRUE;
			//		moveflag = false;
			//		act_state = ActState::ATTACK;
			//		//コマンド判定
			//		Command command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
			//		if (command == Command::LHURF)
			//		{
			//			if (power > 0)
			//			{
			//				//描画をセット

			//				FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
			//				attack_state = AttackState::KYO_LHURF;
			//				//エフェクトセット

			//				pos.x = tracking.rival_Pos.x - Getapply(100.0f);
			//				pos.y = tracking.rival_Pos.y - 100.0f;
			//				power--;
			//			}
			//			else
			//			{
			//				//描画をセット

			//				attack_state = AttackState::THU_LHURF;
			//				specialfream = 0;
			//			}
			//		}
			//		else if (command == Command::RHURF)
			//		{
			//			if (power > 0)
			//			{
			//				//エフェクトセット
			//				//描画をセット

			//				attack_state = AttackState::KYO_RHURF;
			//				specialfream = 0;
			//				power--;
			//			}
			//			else
			//			{
			//				//描画をセット

			//				attack_state = AttackState::THU_RHURF;
			//				specialfream = 0;
			//			}
			//		}
			//		else
			//		{
			//			//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
			//			//描画をセット

			//			//attack_state = AttackState::U_KYO;
			//			//pos.x += Getapply(50.0f);
			//			//pad->que.back().timer = 0;
			//			//attack = TRUE;
			//			//later = non_target;
			//			//moveflag = false;
			//			//act_state = ActState::ATTACK;
			//			if (ground)
			//			{
			//				//描画をセット
			//			}
			//			else
			//			{
			//				//描画をセット
			//			}
			//			//attack_state = AttackState::SPECIAL_ATTACK;
			//			//fream = attack_list[scastI(attack_state)].attack_single[0].fream;
			//			//YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
			//			//production_time = 0.0f;
			//			//anim->PlayAnimation(0, true);
			//			//anim->NodeChange(model_motion.special_R_f);
			//			//anim_ccodinate = ac_attack[scastI(attack_state)].fream;
			//			//ChangeFace(FaceAnim::KOUHUN);
			//		}
			//	}

			//	//投げ
			//	if (pad->x_input[scastI(PAD::A)] == 1)
			//	{
			//		pad->que.back().timer = 0;
			//		attack = TRUE;
			//		moveflag = false;
			//		act_state = ActState::ATTACK;
			//		//描画をセット

			//		attack_state = AttackState::STEAL;
			//	}




			//}
			//else
			//{
			//	//空中弱
			//	if (pad->x_input[scastI(PAD::X)] == 1)
			//	{
			//		pad->que.back().timer = 0;
			//		attack = TRUE;
			//		moveflag = false;
			//		act_state = ActState::ATTACK;
			//		//描画をセット

			//		attack_state = AttackState::JAKU;

			//		//pad->com_list.Reset();
			//	}

			//	//空中中
			//	if (pad->x_input[static_cast<int>(PAD::Y)] == 1)
			//	{
			//		pad->que.back().timer = 0;
			//		FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
			//		attack = TRUE;
			//		moveflag = false;
			//		act_state = ActState::ATTACK;
			//		//描画をセット

			//		attack_state = AttackState::THU;
			//		pos.x += Getapply(50.0f);
			//		specialfream = 0;
			//	}

			//	//空中強
			//	if (pad->x_input[static_cast<int>(PAD::B)] == 1)
			//	{
			//		pad->que.back().timer = 0;
			//		FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
			//		attack = TRUE;
			//		moveflag = false;
			//		act_state = ActState::ATTACK;
			//		//描画をセット

			//		attack_state = AttackState::U_KYO;
			//		pos.x += Getapply(50.0f);
			//		specialfream = 0;
			//		return;
			//	}
			//}

			////無敵攻撃
			//if (pad->x_input[scastI(PAD::L_TRIGGER)] == 1)
			//{
			//	pad->que.back().timer = 0;
			//	attack = TRUE;
			//	moveflag = false;
			//	speed.x = Getapply(5.0f);
			//	speed_Y.Set(70.0f);
			//	act_state = ActState::ATTACK;
			//	//描画をセット

			//	attack_state = AttackState::EXTENDATK;
			//}




			////ホーミングダッシュ
			//if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
			//{
			//	if (trackgauge > 0)
			//	{
			//		pad->que.back().timer = 0;
			//		attack = true;
			//		tracking.Init();
			//		act_state = ActState::ATTACK;
			//		//描画をセット

			//		attack_state = AttackState::TRACK_DASH;
			//		moveflag = false;
			//		pos.y += 5.0f;
			//		speed.x = 0.0f;
			//		speed.y = 0.0f;
			//		hightrigger = false;
			//		jumpflag = false;
			//		max_jump_flag = false;
			//		speed_Y.Set(0.0f);
			//		trackgauge--;
			//	}
	//しゃがみ攻撃
	//if (act_state == ActState::SQUAT)
	//{
	//	if (pad->x_input[scastI(PAD::X)] == 1)
	//	{
	//		pad->que.back().timer = 0;
	//		attack = TRUE;
	//		moveflag = false;
	//		act_state = ActState::ATTACK;
	//		//描画をセット

	//		attack_state = AttackState::D_JAKU;
	//		//pad->com_list.Reset();
	//	}
	//	if (pad->x_input[scastI(PAD::Y)] == 1)
	//	{
	//		pad->que.back().timer = 0;
	//		attack = TRUE;
	//		moveflag = false;
	//		act_state = ActState::ATTACK;
	//		//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
	//		//描画をセット

	//		attack_state = AttackState::D_THU;
	//		pos.x += Getapply(50.0f);
	//	}
	//	if (pad->x_input[static_cast<int>(PAD::B)] == 1)
	//	{
	//		pad->que.back().timer = 0;
	//		//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
	//		attack = TRUE;
	//		moveflag = false;
	//		act_state = ActState::ATTACK;
	//		//描画をセット

	//		attack_state = AttackState::KYO;
	//		pos.x += Getapply(50.0f);
	//		specialfream = 0;
	//	}
	//}
}


void Knight::Attack(float decision, float elapsed_time)
{
	AttackUpdate(elapsed_time);	//攻撃判定の更新
	if (ground)
	{
		if (speed.x > 0.0f)
		{
			speed.x -= brake_speed * elapsed_time;
			if (speed.x < 0.0f)
			{
				speed.x = 0;
			}
		}
		if (speed.x < 0.0f)
		{
			speed.x += brake_speed * elapsed_time;
			if (speed.x > 0.0f)
			{
				speed.x = 0.0f;
			}
		}
		if (pos.y <= POS_Y)
		{
			pos.y = POS_Y;
		}
	}
	HitBoxTransition(HitBoxState::NOGUARD);
	switch (attack_state)
	{
	case AttackState::NONE:
		break;
	//case AttackState::WAIT:
	//	//待機
	//	finish = true;
	//	later = -1;
	//	attack = FALSE;
	//	if (state != WAIT)
	//	{
	//		state = NONE;
	//	}
	//	for (int i = 0; i < scastI(KNIGHTATK::END); i++)
	//	{
	//		atk[i].Init();
	//	}
	//	break;
	//case MOVER://移動系は別で処理しているのでbreakしていない
	//case MOVEL://,,
	case AttackState::SLOW:
		//投げ
		Slow(elapsed_time);
		break;
	case AttackState::STEAL:
		//相手のステートを奪う。掴み
		Steal(elapsed_time);
		break;
	case AttackState::JAKU:
		//弱攻撃
		Jaku(elapsed_time);
		//中攻撃(連打した場合は弱→中→強と繋がるように)
		//if (later > 0 && later < target_max)
		//{
		//	//中攻撃
		//	if (pad->x_input[static_cast<int>(PAD::X)] == 1)
		//	{
		//		//pad->que.back().timer = 0;
		//		//attack = TRUE;
		//		//later = non_target;
		//		//moveflag = false;
		//		//act_state = ActState::ATTACK;
		//		//if (ground)
		//		//{
		//		//	//描画をセット
		//		//}
		//		//else
		//		//{
		//		//	//描画をセット
		//		//}
		//		//attack_state = AttackState::SPECIAL_ATTACK;
		//		//fream = attack_list[scastI(attack_state)].attack_single[0].fream;
		//		//YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
		//		//production_time = 0.0f;

		//		specialfream = 2;
		//	}
		//	//下段中攻撃(コンボルート。弱から下段中攻撃が繋がる)
		//	if (ground)
		//	{
		//		
		//	}
		//}
		break;
	case AttackState::THU:
		//中攻撃(下中)
		D_Thu(elapsed_time);
		//if (later > -1)
		//{
		//	//強攻撃
		//	if (pad->x_input[static_cast<int>(PAD::X)] == 1)
		//	{
		//		if (ground)
		//		{
		//			//pad->que.back().timer = 0;
		//			//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
		//			//attack = TRUE;
		//			//atk[scastI(KNIGHTATK::ONE)].start = FALSE;
		//			//later = -1;
		//			//for (int i = 0; i < scastI(KNIGHTATK::END); i++)
		//			//{
		//			//	atk[i].Init();
		//			//}
		//			//moveflag = false;
		//			//atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
		//			////描画をセット
		//			//act_state = ActState::ATTACK;
		//			//attack_state = AttackState::KYO;
		//			//pos.x += Getapply(50.0f);
		//			//specialfream = 4;
		//		}
		//		else
		//		{
		//			//pad->que.back().timer = 0;
		//			//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
		//			//attack = TRUE;
		//			//atk[scastI(KNIGHTATK::ONE)].start = FALSE;
		//			//later = -1;
		//			//for (int i = 0; i < scastI(KNIGHTATK::END); i++)
		//			//{
		//			//	atk[i].Init();
		//			//}
		//			//moveflag = false;
		//			//atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
		//			////描画をセット

		//			//attack_state = AttackState::U_KYO;
		//			//act_state = ActState::ATTACK;
		//			//pos.x += Getapply(50.0f);
		//			//specialfream = 4;
		//			break;
		//		}
		//	}
		//}
		break;
	case AttackState::D_KYO:
		//中の次に出る強攻撃
		Kyo(elapsed_time);
		//if (later > -1)
		//{
		//	//ホーミングダッシュ
		//	if (pad->x_input[static_cast<int>(PAD::X)] == 1)
		//	{
		//		for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
		//		{
		//			if (atk[atknum].knock_start)
		//			{
		//				/*pad->que.back().timer = 0;
		//				attack = TRUE;
		//				atk[scastI(KNIGHTATK::ONE)].start = FALSE;
		//				later = -1;
		//				for (int i = 0; i < scastI(KNIGHTATK::END); i++)
		//				{
		//					atk[i].Init();
		//				}
		//				moveflag = false;
		//				atk[scastI(KNIGHTATK::ONE)].fin = FALSE;*/
		//				if (trackgauge > 0)
		//				{
		//					tracking.Init();
		//					//描画をセット

		//					attack_state = AttackState::TRACK_DASH;
		//					act_state = ActState::ATTACK;
		//					pos.y -= 5.0f;
		//					speed.y = 0.0f;
		//					speed.x = 0.0f;
		//					hightrigger = false;
		//					jumpflag = false;
		//					max_jump_flag = false;
		//					speed_Y.Set(0.0f);
		//					trackgauge--;
		//				}
		//				break;
		//			}
		//		}
		//	}
		//}
		break;
	case AttackState::D_JAKU:
		//下段弱攻撃
		D_Jaku(elapsed_time);
		break;
	case AttackState::D_THU:
		//下段中攻撃
		D_Thu(elapsed_time);
		break;
	case AttackState::U_KYO:
		//上段強攻撃
		U_Kyo(elapsed_time);
		break;
	case AttackState::A_JAKU:
		//空中弱攻撃
		A_Jaku(elapsed_time);
		break;
	case AttackState::A_THU:
		//空中中攻撃
		A_Thu(elapsed_time);
		break;
	case AttackState::A_KYO:
		//空中強攻撃
		A_Kyo(elapsed_time);
		break;
	case AttackState::A_UKYO:
		//空中上強攻撃
		A_UKyo(elapsed_time);
		break;
	case AttackState::JAKU_RHURF:
		Hadouken(elapsed_time);
		break;
	case AttackState::THU_RHURF:
		Thu_Hadouken(elapsed_time);
		break;
	case AttackState::KYO_RHURF:
		Kyo_Hadouken(elapsed_time);
		break;
	case AttackState::TRACK_DASH:
		//ホーミングダッシュ
		TrackDash(decision, elapsed_time);
		break;
	case AttackState::KYO_LHURF:
		P_Kyo(elapsed_time);
		break;
	case AttackState::EXTENDATK:
		ExtendATK(elapsed_time);
		break;
	case AttackState::JAKU_LHURF:
		Panish_N(elapsed_time);
		break;
	case AttackState::THU_LHURF:
		Panish_H(elapsed_time);
		break;
	case AttackState::SPECIAL_ATTACK:
		SpecialAttack(elapsed_time);
		break;
	case AttackState::JAKU_THU:
		//弱の次に出る中攻撃
		Thu(elapsed_time);
		break;
	case AttackState::JAKU_KYO:
		//中の次に出る強攻撃
		Kyo(elapsed_time);
		break;
	default:
		break;
	}

	//攻撃中、攻撃が当たった場合キャンセルして攻撃する
	if (hit_result != HitResult::NONE)
	{
		AttackInput();
		Jump();
	}

	//後隙中にキャンセルして技を行えるかチェックする
	if (later > 0 && later < target_max)
	{
		later -= elapsed_time;
		//CancelList();
	}

	//後隙消費後元のステートに戻す
	if (later < 0.0f)
	{
		finish = true;
		later = non_target;
		attack = false;
		//結果を初期化する
		hit_result = HitResult::NONE;

		if (ground)
		{
			if (act_state != ActState::WAIT)
			{
				
				if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
				{
					act_state = ActState::SQUAT;
					anim_ccodinate = ac_act[scastI(ActState::SQUAT)].fream;
					anim->NodeChange(model_motion.squat_R);
				}
				else
				{
					act_state = ActState::NONE;
					anim_ccodinate = ac_act[scastI(ActState::WAIT)].fream;
					anim->NodeChange(model_motion.wait_R);
				}
				attack_state = AttackState::NONE;
				ChangeFace(FaceAnim::NORMAL);
			}
		}
		else
		{
			act_state = ActState::JUMP;
			attack_state = AttackState::NONE;
			anim_ccodinate = 1.0f;
			//描画をセット

		}
	}
}


//-------------------------------
//*攻撃のキャンセルリスト*
//----------------------------------------------------------------------
//・このリスト内にで定義した行動はほかの行動からキャンセルして出すことが出来る
//・基本的にゲージを使う技は全技キャンセル出し可能
//----------------------------------------------------------------------
void Knight::CancelList()
{
	//ジャンプ
	Jump();

	//ホーミングダッシュ
	if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
	{
		if (trackgauge > 0)
		{
			//later = -1;
			pad->que.back().timer = 0;
			attack = true;
			tracking.Init();
			act_state = ActState::ATTACK;
			//描画をセット

			attack_state = AttackState::TRACK_DASH;
			moveflag = false;
			pos.y -= 50.0f;
			trackgauge--;
		}
	}

	//214・Xコマンド
	if (pad->x_input[scastI(PAD::X)] == 1)
	{
		Command command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
		if (command == Command::LHURF)
		{
			later = non_target;
			attack = TRUE;
			moveflag = false;
			act_state = ActState::ATTACK;
			//描画をセット

			attack_state = AttackState::JAKU_RHURF;
			specialfream = 0;
		}
	}

	
	//ゲージ技
	if (pad->x_input[scastI(PAD::B)] == 1)
	{
		Command command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
		if (command == Command::LHURF)
		{
			//214・Bコマンド
			if (power > 0)
			{
				later = -1;
				//描画をセット

				FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
				attack = TRUE;
				moveflag = false;
				act_state = ActState::ATTACK;
				attack_state = AttackState::KYO_LHURF;
				//エフェクトセット

				pos.x = tracking.rival_Pos.x - Getapply(100.0f);
				pos.y = tracking.rival_Pos.y - 100.0f;
				power--;
			}
		}
		else if (command == Command::RHURF)
		{
			//236・Bコマンド
			later = -1;
			//描画をセット
			//エフェクトセット

			act_state = ActState::ATTACK;
			attack_state = AttackState::KYO_RHURF;
			specialfream = 0;
			power--;
		}
	}
}



void Knight::Draw(
	YRShader				*parallel_shader,
	YRShader				*shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	DrawFastMove(FastPos);
	drawset = false;

	//Trackの時は回す
	//angle.x += elapsed_time * 10.0f;
	//エフェクト描画
	
	bool invincible = false;

	if (attack_state == AttackState::EXTENDATK)
	{
		for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			if (hit[i].parameter.state == HitBoxState::INVINCIBLE)
			{
				invincible = true;
			}
		}
	}

	//--------------------------------------------------------------------
	// *メモ*
	//・過去2Dで制作したときは向いている向きによって描画を変えていたが
	//　今回は3Dでの制作になる為、angleを変えることに使う
	//・前回では強攻撃時の画像のサイズが通常と違っていたため
	//　こういう処理を取らざるを得なかったと考えられる
	//--------------------------------------------------------------------

	bool inversion = true;//左右反転フラグ

	DirectX::XMFLOAT4 material_color = { 1.0f,1.0f,1.0f,1.0f };

	if (fream < target_max && fream>0.0f)
	{
		//material_color = { 0.0f,0.0f,1.0f,1.0f };
	}
	if (later < target_max && later>0.0f)
	{
		//material_color = { 1.0f,0.0f,0.0f,1.0f };
	}

	//左向き
	if (rightOrleft < 0)
	{
		//angle.y = DirectX::XMConvertToRadians(180.0f);
		inversion = true;
		if (invincible)
		{
			//無敵状態
		}

	}
	//右向き
	else
	{
		angle.y = DirectX::XMConvertToRadians(0.0f);

		/*motion.DrawContinue(
			parallel_shader,
			pos.GetDXFLOAT3(),
			scale.GetDXFLOAT3(),
			angle.GetDXFLOAT3(),
			view, projection, light_direction, light_color, ambient_color, elapsed_time * anim_ccodinate,
			inversion, material_color
		);

		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x,pos.y,pos.z), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, material_color);*/

		//angle.y = DirectX::XMConvertToRadians(80.0f);
		inversion = false;
		if (invincible)
		{
			//無敵！
		}
	}

	//モデル描画
	/*base_motion->Render(
		shader,
		pos.GetDXFLOAT3(),
		scale.GetDXFLOAT3(),
		angle.GetDXFLOAT3(),
		view, projection, light_direction, light_color, ambient_color, elapsed_time, 0.0f);*/

	if (scastI(YRCamera.camera_state) == now_player)
	{
		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], material_color);
	}
	if (YRCamera.camera_state == Camera::CAMERA_STATE::MAIN)
	{
		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], material_color);
	}
	if (YRCamera.GetRequest() == Camera::Request::WEAKEN)
	{
		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], material_color);
	}

	/*motion.DrawContinue(
			parallel_shader,
			pos.GetDXFLOAT3(),
			scale.GetDXFLOAT3(),
			angle.GetDXFLOAT3(),
			view, projection, light_direction, light_color, ambient_color, elapsed_time*anim_ccodinate,
			inversion, material_color
		);*/

	//if (atk[scastI(KNIGHTATK::HADOU)].hit_ok)
	//{
	//	//波動拳の描画
	//}

	//if (atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok)
	//{
	//	//中波動拳の描画。これまとめてもいいだろ
	//}

	//if (atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok)
	//{
	//	//でかい波動拳の描画。これは大きさ違うから仕方ない
	//}

	TextDraw();

//デバッグ状態なら
#if USE_IMGUI
		//プレイヤーの中心
		if (ground)
		{
		}
		else
		{
		}

		for (int i = 0; i < 10; i++)
		{
			int j = i * 20 + 20;
		}

		//入力関係を表示してた
		//GetJoypadXInputState(DX_INPUT_PAD1, &input);

		//if (GetJoypadXInputState(DX_INPUT_PAD1, &input) != -1)
		//{
		//	// 画面に XINPUT_STATE の中身を描画
		//	Color = GetColor(255, 255, 255);
		//	GetJoypadAnalogInput(&Xbuf, &Ybuf, DX_INPUT_PAD1);
		//	DrawFormatString(0, 0, Color, "LeftTrigger:%d RightTrigger:%d",
		//		input.LeftTrigger, input.RightTrigger);
		//	DrawFormatString(0, 16, Color, "ThumbLX:%d ThumbLY:%d",
		//		Xbuf, Ybuf);
		//	DrawFormatString(0, 32, Color, "ThumbRX:%d ThumbRY:%d",
		//		input.ThumbRX, input.ThumbRY);
		//	DrawString(0, 64, "Button", Color);
		//	for (i = 0; i < 16; i++)
		//	{
		//		DrawFormatString(64 + i % 8 * 64, 64 + i / 8 * 16, Color,
		//			"%2d:%d", i, input.Buttons[i]);
		//	}
		//}

		{
			/*bool show_test_window = true;
			bool show_another_window = true;*/
			//static float f = 0.0f;
			//ImGui::Begin("RyuHitBox");
			/*ImGui::InputFloat("BodyPosX", &Hitplus[scastI(KNIGHTHIT::BODY)].x, 0.1f, 0.1f);
			ImGui::InputFloat("BodyPosY", &Hitplus[scastI(KNIGHTHIT::BODY)].y, 0.1f, 0.1f);
			ImGui::InputFloat("BodySizeX", &hit[scastI(KNIGHTHIT::BODY)].size.x, 0.1f, 0.1f);
			ImGui::InputFloat("BodySizeY", &hit[scastI(KNIGHTHIT::BODY)].size.y, 0.1f, 0.1f);

			ImGui::InputFloat("LegPosX", &Hitplus[scastI(KNIGHTHIT::LEG)].x, 0.1f, 0.1f);
			ImGui::InputFloat("LegPosY", &Hitplus[scastI(KNIGHTHIT::LEG)].y, 0.1f, 0.1f);
			ImGui::InputFloat("LegSizeX", &hit[scastI(KNIGHTHIT::LEG)].size.x, 0.1f, 0.1f);
			ImGui::InputFloat("LegSizeY", &hit[scastI(KNIGHTHIT::LEG)].size.y, 0.1f, 0.1f);*/
			//ImGui::SliderFloat("camera.y", &camera.y, -world_max_y, world_max_y);
			//ImGui::SliderFloat("camera.x", &camera.x, -world_max_x, world_max_x);
			//ImGui::Text("player.y:%f", pos.y);
			//ImGui::Text("player.x:%f", pos.x);

			//ImGui::ColorEdit3("clear color", (float*)&ImColor(114, 144, 154));
			//if (ImGui::Button("Test Window")) show_test_window ^= 1;
			//if (ImGui::Button("Another Window")) show_another_window ^= 1;
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//ImGui::End();
		}
#endif

}



//残像の描画
void Knight::DrawFastMove(YR_Vector3 position)
{
	if (!fast)
	{
		return;
	}
	//右
	if (rightOrleft < 0)
	{
		
	}
	//左
	else
	{
		
	}
	fast = false;
}

//残像をセットする
void Knight::FastSet(YR_Vector3 position)
{
	fast = true;
	FastPos = position;
}


//移動関係(ステップなど)
//-----------------------------------------------------------------
// *概要*
//・空中地上それぞれに前後のステップが存在するため全て個別に処理している
//・正直もうちょいいいやり方あったと思う
//-----------------------------------------------------------------
bool Knight::Step()
{
	//空中
	if (!ground)
	{
		//空中左ステップ
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1)
		{
			if (jumpcount > 0)
			{
				//描画をセット

				act_state = ActState::AIR_F;
				step = true;
				moveflag = false;
				speed.x = -stepspeed;
				jumpflag = false;
				jumpcount = 0;
				if (rightOrleft > 0)
				{
					step = true;
					act_state = ActState::AIR_B;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					//描画をセット

					speed.x = -stepspeed;
				}
			}
			else
			{
				pad->dash_trigger = false;
			}
		}

		//空中右ステップ
		if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1)
		{
			if (jumpcount > 0)
			{
				//描画をセット

				step = true;
				act_state = ActState::AIR_F;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				speed.x = stepspeed;
				if (rightOrleft < 0)
				{
					step = true;
					act_state = ActState::AIR_B;
					jumpflag = false;
					moveflag = false;
					jumpcount = 0;
					//描画をセット

					speed.x = stepspeed;
				}
			}
			else
			{
				pad->dash_trigger = false;
			}
		}
	}

	if (!step)
	{
		return false;
	}

	//バックステップ
	if (act_state == ActState::BACK)
	{
		//右向き
		if (rightOrleft > 0)
		{
			//右向きのバックステップはx方向の速度は0になる
			//なので全て0にしている
			if (speed.x > 0)
			{
				step = false;
				speed.x = 0;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				moveflag = false;
				act_state = ActState::NONE;
				return true;
			}
			if (speed.x < -stepspeed / 2.0f)
			{
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}

			//描画をセットはしないけど移動範囲のセット
			/*if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}*/
			speed.x++;
			return false;
		}
		if (rightOrleft < 0)
		{
			//逆も然り
			if (speed.x < 0)
			{
				step = false;
				speed.x = 0;
				pad->dash_trigger = false;
				moveflag = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				act_state = ActState::NONE;
				return true;
			}
			if (speed.x > stepspeed / 2.0f)
			{
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}
			//描画をセットはしないけど移動範囲のセット
			/*if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}*/
			speed.x--;
			return false;
		}
	}

	if (act_state == ActState::AIR_B)
	{
		speed_Y.Set(0.0f);
		pos.y -= gravity;
		if (rightOrleft > 0)
		{
			if (speed.x > -dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				act_state = ActState::JUMP;
				//描画をセット

				return true;
			}
			//描画をセットはしないけど移動範囲のセット
			/*if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}*/
			speed.x++;
			return false;
		}
		if (rightOrleft < 0)
		{
			if (speed.x < dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				act_state = ActState::JUMP;
				//描画をセット

				return true;
			}
			//描画をセットはしないけど移動範囲のセット
			/*if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}*/
			speed.x--;
			return false;
		}
	}

	if (act_state == ActState::AIR_F)
	{
		speed_Y.Set(0.0f);
		pos.y -= gravity;
		if (rightOrleft > 0)
		{
			if (speed.x < dashspeed)
			{
				speed_Y.Set(0.0f);
				step = false;
				pad->dash_trigger = false;
				act_state = ActState::JUMP;
				//描画をセット

				return true;
			}
			//描画をセットはしないけど移動範囲のセット
			/*if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}*/
			speed.x -= 2;
			return false;
		}
		if (rightOrleft < 0)
		{
			if (speed.x > -dashspeed)
			{
				speed_Y.Set(40.0f);
				step = false;
				pad->dash_trigger = false;
				act_state = ActState::JUMP;
				//描画をセット

				return true;
			}
			//描画をセットはしないけど移動範囲のセット
			/*if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}*/
			speed.x += 2;
			return false;
		}
	}

	return false;
}

//移動関係(こちらはステップ以外)
void Knight::Move(float decision)
{
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_R)]==0)
	{
		if (ground)
		{

			//描画をセット

			if (rightOrleft > 0)
			{
				//バックステップ
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				//描画をセット

				speed.x = -backstepS;
			}
			else
			{
				//ダッシュ左向き
				act_state = ActState::DASH;
			}
		}
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_L)]==0)
	{
		if (ground)
		{
			//描画をセット

			if (rightOrleft < 0)
			{
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				//描画をセット

				speed.x = backstepS;
			}
			else
			{
				//ダッシュ右向き
				act_state = ActState::DASH;
				anim->NodeChange(model_motion.dash_R,scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}


	//左移動
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0)
	{
		if (rightOrleft > 0)
		{
			//何もしない
		}
		else
		{
			if (!step)
			{
				speed.x = -dashspeed;
				act_state = ActState::DASH;
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションがダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim()==-1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
			}
		}
	}
	else
	{
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -walkspeed;
			act_state = ActState::MOVEL;
		}
	}




	//右移動
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{
		if (rightOrleft < 0)
		{
			//何もしない
		}
		else
		{
			if (!step)
			{
				speed.x = dashspeed;
				act_state = ActState::DASH;
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションがダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim()==-1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
			}
		}
	}
	else
	{
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = walkspeed;
			act_state = ActState::MOVER;
		}
	}

	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] == 0)
	{

	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{

	}

	//どちらも押されているor押されていないなら元に戻す
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	{
		if (act_state == ActState::DASH)
		{
			//現在のステートがダッシュだった場合
			act_state = ActState::ATTACK;
			attack_state = AttackState::NONE;
			attack = true;
			later = dash_later;
			anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::LATER));
			anim->PlayAnimation(scastI(AnimAtk::LATER), false);
			moveflag = false;
		}
		else
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
		//speed.x = 0.0f;
	}
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		if (act_state == ActState::DASH)
		{
			//現在のステートがダッシュだった場合
			act_state = ActState::ATTACK;
			attack_state = AttackState::NONE;
			attack = true;
			later = dash_later;
			anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::LATER));
			anim->PlayAnimation(scastI(AnimAtk::LATER), false);
			moveflag = false;
		}
		else
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
		//speed.x = 0.0f;
	}
}

void Knight::MoveAnimSet()
{
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		moveflag = true;

		if (rightOrleft > 0)
		{
			//描画をセット
			//右向き
			anim->NodeChange(model_motion.back_R);
			anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;
		}
		else
		{
			//左向き
		}
	}
	if (pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		moveflag = true;

		if (rightOrleft < 0)
		{
			//描画をセット
			//左向き
		}
		else
		{
			//右向き
			anim->NodeChange(model_motion.walk_R);
			anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;
		}
	}
}

void Knight::GuardAnimSet()
{
	if (act_state == ActState::GUARD)
	{
		if (pad->x_input[static_cast<int>(PAD::STICK_RDown)] > 0 || pad->x_input[static_cast<int>(PAD::STICK_LDown)] > 0)
		{
			//描画をセット

		}
		else
		{
			//描画をセット

		}
	}

}

void Knight::WaitAnimSet()
{
	if (act_state == ActState::NONE)
	{
#ifdef USE_IMGU
		hit[scastI(KNIGHTHIT::BODY)].size = HitSize[scastI(KNIGHTHIT::BODY)];
		hit[scastI(KNIGHTHIT::LEG)].size = HitSize[scastI(KNIGHTHIT::LEG)];
#endif // USE_IMGU
		//描画をセット

		act_state = ActState::WAIT;
		if (rightOrleft > 0)
		{
			//右向きの時
			anim->NodeChange(model_motion.wait_R);
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
		else
		{
			//左向きの時
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
		speed.x = 0.0f;
		moveflag = false;
	}
}


void Knight::Jump()
{
	if (step)
	{
		return;
	}
	if (ground)
	{
		if (pad->x_input[scastI(PAD::HIGH_UP)] == 1)
		{
			//ハイジャンプする
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount = 0;
			speed.y = 60.0f;
			hightrigger = true;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット
			act_state = ActState::JUMP;
			anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			jumpflag = true;

			//現在攻撃判定が出ているなら全て消去する
			AllAttackClear();
			//攻撃の結果を初期化
			hit_result = HitResult::NONE;
			//後隙を初期化
			later = non_target;
			//攻撃フラグをオフに
			attack = false;
			//攻撃ステートを初期化
			attack_state = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < dashspeed)
				{
					speed.x = dashspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -dashspeed)
				{
					speed.x = -dashspeed;

				}
				return;
			}
			speed.x = 0.0f;

		}


		if (pad->x_input[scastI(PAD::STICK_U)] == 1)
		{
			//じゃんぷする
			later = -1;
			attack = FALSE;
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = false;
			speed.y = 40.0f;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット
			act_state = ActState::JUMP;
			anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			anim_ccodinate = ac_act[scastI(act_state)].fream;
			jumpflag = true;
			jump_can_timer = jump_max_time;
			//現在攻撃判定が出ているなら全て消去する
			AllAttackClear();
			//攻撃の結果を初期化
			hit_result = HitResult::NONE;
			//後隙を初期化
			later = non_target;
			//攻撃フラグをオフに
			attack = false;
			//攻撃ステートを初期化
			attack_state = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < walkspeed)
				{
					speed.x = walkspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -walkspeed)
				{
					speed.x = -walkspeed;
					
				}
				return;
			}
			speed.x = 0.0f;
		}
	}
	else
	{
		if (jumpcount > 0&&jump_can_timer<=0.0f)
		{
			if (pad->x_input[scastI(PAD::STICK_U)] == 1)
			{
				later = -1;
				attack = FALSE;
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				jumpcount--;
				hightrigger = false;
				speed.y = 40.0f;
				max_jump_flag = false;
				moveflag = false;
				//描画をセット
				act_state = ActState::JUMP;
				anim->NodeChange(model_motion.air_jump_R, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				jumpflag = true;
				jump_can_timer = jump_max_time;
				//現在攻撃判定が出ているなら全て消去する
				AllAttackClear();
				//攻撃の結果を初期化
				hit_result = HitResult::NONE;
				//後隙を初期化
				later = non_target;
				//攻撃フラグをオフに
				attack = false;
				//攻撃ステートを初期化
				attack_state = AttackState::NONE;

				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					if (speed.x < walkspeed)
					{
						speed.x = walkspeed;
					}
					return;
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					if (speed.x > -walkspeed)
					{
						speed.x = -walkspeed;
					}
					return;
				}
				speed.x = 0.0f;
			}
		}
	}

}

void Knight::JumpUpdate(float elapsed_time)
{
	if (jumpcount < 2 && jumpflag)
	{
		if (!anim->GetLoopAnim())
		{
			//現在のアニメーションがジャンプの開始アニメーションだった場合
			if (anim->GetEndAnim() == -1)
			{
				//アニメーションが終了したら持続アニメーションに切り替える
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		if (jump_can_timer > 0.0f)
		{
			jump_can_timer -= elapsed_time;
		}
		if (speed_Y.speedY == 0.0f)
		{
			pos.y += (speed.y * elapsed_time);
		}
		if (!max_jump_flag)
		{
			if (hightrigger)
			{
				speed.y += (2000.0f * elapsed_time);
				if ((speed.y * elapsed_time) > (high_jump_max * elapsed_time))
				{
					max_jump_flag = true;
				}
			}
			else
			{
				speed.y += (2000.0f * elapsed_time);
				if (speed_Y.speedY == 0.0f)
				{
					pos.y += (speed.y * elapsed_time);
				}
				//float hei = speed.y * elapsed_time;
				if ((speed.y*elapsed_time) > (jump_max*elapsed_time))
				{
					max_jump_flag = true;
				}
			}
		}
	}

	if (max_jump_flag)
	{
		speed.y -= (down_force * elapsed_time);
		if (hightrigger)
		{
			if (speed.y - (gravity*elapsed_time) < 0.0f)
			{
				if (!attack)
				{
					//描画をセット

				}
				hightrigger = false;
			}
		}
	}
	if (pos.y < POS_Y)
	{
		if (max_jump_flag)
		{
			jumpcount = 2;
			max_jump_flag = false;
			hightrigger = false;
			speed.y = 0.0f;
			//act_state = ActState::NONE;
			pos.y = POS_Y;
			jumpflag = false;
			//ジャンプの着地隙を発生する
			act_state = ActState::ATTACK;
			attack_state = AttackState::NONE;
			attack = true;
			later = jump_later;
			anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::LATER));
			anim->PlayAnimation(scastI(AnimAtk::LATER), false);
		}
	}
}


void Knight::DamageCheck()
{
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].hit)
		{
			//攻撃を受けていた
			//ダメージ、吹っ飛びベクトルなどを保存
			float dg = hit[i].damege - (combo_count * 1.2f);
			if (dg <= 0)
			{
				dg = 1;
			}
			hp -= dg;
			combo_count++;
			GaugeUp(hit[i].damege / 5);
			hit[i].damege = 0;
			hit[i].hit = false;

			//プレイヤーをやられ状態にする
			attack = false;
			later = -1;
			moveflag = false;
			if (jumpcount > 1)
			{
				jumpcount = 1;
			}
			jumpflag = false;
			max_jump_flag = false;
			knocktimer = hit[i].timer;
			speed.x = 0.0f;
			speed.y = 0.0f;
			step = false;
			pad->dash_trigger = false;
			pad->high_trigger = false;
			hightrigger = false;
			speed_Y.Set(0.0f);
			AllAttackClear();
			hit_result = HitResult::NONE;
			act_state = ActState::KNOCK;
			attack_state = AttackState::NONE;
			anim->NodeChange(model_motion.damage_R_g_u);
			ChangeFace(FaceAnim::Damage);
			anim_ccodinate = 5.0f;
		}
		if (hit[i].steal)
		{
			//つかみ攻撃を受けた
			//攻撃内容を保存し、つかまれ状態にする
			HitBoxTransition(HitBoxState::NOGUARD);
			attack = false;
			later = -1;
			moveflag = false;
			speed_Y.Set(0.0f);
			if (jumpcount > 1)
			{
				jumpcount = 1;
			}
			jumpflag = false;
			max_jump_flag = false;
			knocktimer = hit[i].timer;
			speed.x = 0.0f;
			speed.y = 0.0f;
			step = false;
			hit_result = HitResult::NONE;
			pad->dash_trigger = false;
			pad->high_trigger = false;
			hightrigger = false;
			AllAttackClear();
			hit[i].steal = false;
			steal_escape = hit[i].steal_timer;
			hit[i].steal_timer = 0.0f;
			act_state = ActState::STATENONE;
			attack_state = AttackState::NONE;
		}
	}
}

void Knight::KnockUpdate(float elapsed_time)
{
	bool pflag = false;
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].hitback.x != 0)
		{
			pos.x += hit[i].hitback.x*elapsed_time;
			pflag = true;
		}
		if (hit[i].hitback.y != 0)
		{
			pos.y += hit[i].hitback.y*elapsed_time;
			pflag = true;
			if (pos.y < POS_Y)
			{
				pos.y = POS_Y;
			}
		}
		if (pflag)
		{
			break;
		}
	}
	knocktimer -= elapsed_time;
	if (knocktimer < 0)
	{
		combo_count = 0;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
			anim->NodeChange(model_motion.wait_R);
			ChangeFace(FaceAnim::NORMAL);
			anim_ccodinate = 0.0f;
		}
		if (!ground)
		{
			act_state = ActState::FALL;
			speed.y = 0.0f;
			//描画をセット

			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		if (hp == 0)
		{
			act_state = ActState::FALL;
		}
		knocktimer = 0;
		for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			hit[i].timer = 0;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
}

void Knight::Guard(float decision)
{
	
	if (step || attack)
	{
		//攻撃中、またはステップ中なら入らない
		return;
	}

	HitBoxTransition(HitBoxState::NOGUARD);

	if (rightOrleft > 0)
	{
		if (pad->x_input[scastI(PAD::STICK_LDown)] > 0)
		{
			if (!ground)
			{
				HitBoxTransition(HitBoxState::ALL);
			}
			else
			{
				HitBoxTransition(HitBoxState::DOWN);
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (!ground)
				{
					HitBoxTransition(HitBoxState::ALL);
				}
				else
				{
					HitBoxTransition(HitBoxState::MIDDLE);
				}
			}
		}
	}
	if (rightOrleft < 0)
	{
		if (pad->x_input[scastI(PAD::STICK_RDown)] > 0)
		{
			if (!ground)
			{
				HitBoxTransition(HitBoxState::ALL);
			}
			else
			{
				HitBoxTransition(HitBoxState::DOWN);
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (!ground)
				{
					HitBoxTransition(HitBoxState::ALL);
				}
				else
				{
					HitBoxTransition(HitBoxState::MIDDLE);
				}
			}
		}
	}

	if (!ground)
	{
		if (decision != rightOrleft)
		{
			if (pad->x_input[scastI(PAD::STICK_R)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0
				|| pad->x_input[scastI(PAD::STICK_L)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
			{
				HitBoxTransition(HitBoxState::ALL);
			}
		}
	}

	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].guard_ok)
		{
			GaugeUp(hit[i].damege / 4);
			speed_Y.Set(0.0f);
			hp -= hit[i].damege;
			if (hp < 0)
			{
				hp = 1;
			}
			if (decision != rightOrleft)
			{
				rightOrleft = decision;
			}
			hit[i].damege = 0;
			hit[i].guard_ok = false;
			moveflag = false;
			knocktimer = hit[i].timer / 2;
			pad->high_trigger = false;
			hightrigger = false;
			act_state = ActState::GUARD;
		}
	}

}

void Knight::GuardBack(float elapsed_time)
{
	if (act_state != ActState::GUARD)
	{
		return;
	}
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].hitback.y != 0)
		{
			if (!ground)
			{
				pos.y -= hit[i].hitback.y;
			}
			if (hit[i].hitback.x != 0)
			{
				pos.x += hit[i].hitback.x;
			}
			break;
		}
	}
	if (knocktimer > 0)
	{
		knocktimer -= elapsed_time;
		if (knocktimer == 0)
		{
			knocktimer -= elapsed_time;
		}
	}
	if (knocktimer < 0)
	{
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		if (ground)
		{
			if (pad->x_input[scastI(PAD::STICK_D)] > 0)
			{
				act_state = ActState::SQUAT;
			}
			if (pad->x_input[scastI(PAD::STICK_LDown)] > 0)
			{
				act_state = ActState::SQUAT;
			}
			if (pad->x_input[scastI(PAD::STICK_RDown)] > 0)
			{
				act_state = ActState::SQUAT;
			}
		}
		else
		{
			max_jump_flag = false;
			jumpflag = false;
			act_state = ActState::JUMP;
			//描画をセット

		}

		knocktimer = 0;
		for (int hitnum = 0; hitnum < scastI(KNIGHTHIT::END); hitnum++)
		{
			hit[hitnum].timer = 0;
			hit[hitnum].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
	else
	{
		act_state = ActState::GUARD;
	}
}

void Knight::Squat()
{
	if (act_state == ActState::GUARD || act_state == ActState::DASH|| act_state == ActState::BACK || act_state == ActState::AIR_B || act_state == ActState::AIR_F)
	{
		return;
	}
	if (!ground)
	{
		return;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] ==1 || pad->x_input[scastI(PAD::STICK_RDown)] == 1 || pad->x_input[scastI(PAD::STICK_LDown)] == 1)
	{
		//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
		//hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
		moveflag = false;
		act_state = ActState::SQUAT;
		//描画をセット
		anim->NodeChange(model_motion.squat_R);
		anim_ccodinate = ac_act[scastI(act_state)].fream;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
	{
		//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
		//hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
		moveflag = false;
		act_state = ActState::SQUAT;
		//描画をセット

	}
	if (pad->x_input[scastI(PAD::STICK_D)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0 && pad->x_input[scastI(PAD::STICK_LDown)] == 0)
	{
		if (act_state == ActState::SQUAT)
		{
			//描画をセット

		}

		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
	}
}


void Knight::FallUpdate(float elapsed_time)
{
	if (ground)
	{
		pos.y = POS_Y;
		if (hp > 0)
		{
			speed.y = -5.0f;
		}
		act_state = ActState::DOWN;
		//描画をセット

	}
	else
	{
		//空中時
		if (pos.y > POS_Y)
		{
			//重力を付与する
			pos.y -= fall_force * elapsed_time;
			//speed.y += 10.0f * elapsed_time;
		}
		if (pad->x_input[scastI(PAD::X)] == 1 || pad->x_input[scastI(PAD::Y)] == 1 ||
			pad->x_input[scastI(PAD::B)] == 1 || pad->x_input[scastI(PAD::A)] == 1)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			act_state = ActState::PASSIVE;
			//描画をセット

			HitBoxTransition(HitBoxState::INVINCIBLE);
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				speed.x = 20.0f;
				speed.y = -10.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				speed.x = -20.0f;
				speed.y = -10.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_U)] > 0)
			{
				speed.y = -20.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_D)] > 0)
			{
				speed.y = 20.0f;
			}

			if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0 &&
				pad->x_input[scastI(PAD::STICK_U)] == 0 && pad->x_input[scastI(PAD::STICK_D)] == 0)
			{
				speed.x = Getapply(-20.0f);
				speed.y = -10.0f;
			}
		}
	}
}



void Knight::DownUpdate()
{
	//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(-31.0f, 134.0f);
	//hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(110.0f, 62.0f);
	//Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(110.0f, 149.0f);
	//hit[scastI(KNIGHTHIT::LEG)].size = YR_Vector3(39.0f, 47.0f);
	pos.y += speed.y;
	if (pos.y > POS_Y)
	{
		pos.y = POS_Y;
		speed.y = 0;
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = 20.0f;
			speed.y = -10.0f;
			act_state = ActState::PASSIVE;
			//描画をセット

		}
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -20.0f;
			speed.y = -10.0f;
			act_state = ActState::PASSIVE;
			//描画をセット

		}
		if (pad->x_input[scastI(PAD::X)] > 0 || pad->x_input[scastI(PAD::Y)] > 0 ||
			pad->x_input[scastI(PAD::B)] > 0 || pad->x_input[scastI(PAD::A)] > 0)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			act_state = ActState::PASSIVE;
			//描画をセット

			HitBoxTransition(HitBoxState::INVINCIBLE);
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				speed.x = 20.0f;
				speed.y = -10.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				speed.x = -20.0f;
				speed.y = -10.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0)
			{
				speed.y = -20.0f;
				speed.x = Getapply(-10.0f);
			}
		}
		if (pad->x_input[scastI(PAD::STICK_U)] > 0)
		{
			//描画をセット

			act_state = ActState::WAKE;
		}
	}
	else
	{
		speed.y += 1.0f;
	}
}

void Knight::WakeUp()
{
	//起き上がり開始は無敵
	HitBoxTransition(HitBoxState::INVINCIBLE);

	//描画をセットはしない
	//ダウンから回復(完全に起き上がった時)無敵を解除する
	/*if (anim.elem == 0)
	{
		for (int i = 0; i < scastI(RYUHIT::END); i++)
		{
			hit[i].guard = HitBox::NOGUARD;
		}
		state = NONE;
	}*/
}


void Knight::PassiveUpdate()
{

	pos += speed;
	if (pos.y > POS_Y)
	{
		pos.y = POS_Y;
	}
	if (speed.x > 0)
	{
		speed.x--;
	}
	if (speed.x < 0)
	{
		speed.x++;
	}
	if (speed.y > 0)
	{
		speed.y--;
	}
	if (speed.y < 0)
	{
		speed.y++;
	}
	if (speed.x == 0.0f && speed.y == 0)
	{
		HitBoxTransition(HitBoxState::NOGUARD);
		if (ground)
		{
			act_state = ActState::NONE;
		}
		else
		{
			act_state = ActState::JUMP;
			//描画をセット

		}
	}
}


void Knight::StateNone()
{
	if (steal_escape > 0)
	{
		//描画をセット

		if (pad->x_input[scastI(PAD::A)] == 1)
		{
			steal_escape = 0;
			pos.x -= Getapply(150.0f);
			act_state = ActState::NONE;
			return;
		}
		steal_escape--;
	}
	else
	{

	}
}

//掴み攻撃
void Knight::Steal(float elapsed_time)
{
	AttackDefault(elapsed_time);
	//YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	//YR_Vector3 range{ 50.0f,50.0f };
	//atk[scastI(KNIGHTATK::ONE)].Update(cent, range, 10, 10, 15, 20, 13, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::STEAL, Getapply(0.0f), true,elapsed_time);
	
	//--------------------------------
	//			※要変更
	//--------------------------------
	//if (rival_state != STATENONE)
	//{
	//	if (atk[scastI(KNIGHTATK::ONE)].timer == 0)
	//	{
	//		atk[scastI(KNIGHTATK::ONE)].fin = TRUE;
	//	}
	//}
	//if (rival_state == STATENONE)
	//{
	//	if (atk[scastI(KNIGHTATK::ONE)].stealtimer > 0)
	//	{
	//		atk[scastI(KNIGHTATK::ONE)].stealtimer--;
	//	}
	//	else
	//	{
	//		later = -1;
	//		for (int i = 0; i < scastI(KNIGHTATK::END); i++)
	//		{
	//			atk[i].Init();
	//		}
	//		attack_state = AttackState::SLOW;
	//		//描画をセット

	//		if (rightOrleft > 0)
	//		{
	//			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
	//			{
	//				FastSet(pos);
	//				rightOrleft = -rightOrleft;
	//				pos.x = tracking.rival_Pos.x + 150.0f;
	//			}
	//		}
	//		else
	//		{
	//			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
	//			{
	//				FastSet(pos);
	//				rightOrleft = -rightOrleft;
	//				pos.x = tracking.rival_Pos.x - 150.0f;
	//			}
	//		}
	//	}
	//}
}


void Knight::TrackDash(float decision,float elapsed_time)
{
	if (attack_state != AttackState::TRACK_DASH)
	{
		return;
	}

	jumpflag = false;
	max_jump_flag = false;
	speed.y = 0.0f;
	rightOrleft = decision;

	YR_Vector3		plusVec = { 0,0 };

	if (!tracking.attack)
	{
		plusVec = tracking.Veccalculate(pos, decision);
		if (tracking.attack)
		{
			//描画をセット

		}
	}
	else
	{
		/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
		{
			speed_Y.Set(40.0f);
			pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
			atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
		}
		YR_Vector3 cent{ pos.x + (50.0f * decision),pos.y };
		YR_Vector3 range{ 150.0f,200.0f };
		atk[scastI(KNIGHTATK::ONE)].Update
		(
			cent, range, 3, 1, 3, 5, 40, YR_Vector3(3.0f * decision, 20.0f), AttackBox::MIDDLE, Getapply(10.0f), elapsed_time
		);*/
	}

	if (!ground)
	{
		pos.y -= gravity;
	}

	pos += plusVec;

}





//float Ryu::GetPosX()
//{
//	return pos.x;
//}

float Knight::Getapply(float n)
{
	return n * rightOrleft;
}

HitBox* Knight::GetHit()
{
	return hit.data();
}
AttackBox* Knight::GetAttack()
{
	return atk.data();
}
int Knight::GetMax(int n)
{
	if (n == 0)
	{
		return scastI(KNIGHTHIT::END);
	}
	if (n == 1)
	{
		return atk.size();
	}
	return 0;
}

//float& Ryu::GetPosX2()
//{
//	return pos.x;
//}

void Knight::NoneChange()
{
	if (act_state != ActState::GUARD)
	{
		return;
	}
	int none = 0;
	for (auto& it : pad->x_input)
	{
		if (it > 0)
		{
			none++;
		}
	}
	if (none == 0)
	{
		act_state = ActState::NONE;
	}
}

void Knight::GaugeUp(float add)
{
	gauge += add;
	if (gauge > GAUGE_MAX)
	{
		float away = gauge - GAUGE_MAX;
		power++;
		gauge = 0;
		gauge = away;
	}
	if (power > 4)
	{
		power = 5;
		gauge = GAUGE_MAX;
	}
}


void Knight::StopUpdate()
{
	float shift_pos = 0.1f;
	if (stop_pos == YR_Vector3(0.0f, 0.0f))
	{
		stop_pos = pos;
	}

	switch (stop_state)
	{
	case 0:
		pos.x += shift_pos;
		pos.y += shift_pos;
		stop_state++;
		break;
	case 1:
		pos.x -= shift_pos;
		pos.y -= shift_pos;
		stop_state++;
		break;
	case 2:
		pos.x -= shift_pos;
		pos.y -= shift_pos;
		stop_state++;
		break;
	case 3:
		pos.x += shift_pos;
		pos.y += shift_pos;
		stop_state = 0;
		break;
	}

}
void Knight::StopEnd()
{
	if (stop_pos == YR_Vector3(0.0f, 0.0f))
	{
		return;
	}
	pos = stop_pos;
	stop_pos = YR_Vector3(0.0f, 0.0f);
}


void Knight::WinAnimSet()
{
	//勝利演出用のセット
	pos = YR_Vector3(1.0f, 0.0f, 0.0f);
	YRCamera.SetFocus(pos.GetDXFLOAT3());
	YRCamera.SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -25.0f));
}

bool Knight::WinPerformance()
{
	//勝利演出の行動をする
	//カメラもこちらで動かす
	switch (win_state)
	{
	case Knight::WIN_PERFORMANCE_KNIGHT::CAMERA_ZOOM:
	{
		YR_Vector3 vec, eye, focus;
		focus = YRCamera.GetFocus();
		eye = YRCamera.GetEye();
		vec = focus - eye;
		float len = vec.Length();
		if (len < 0.5f)
		{
			win_state = WIN_PERFORMANCE_KNIGHT::FINISH;
			break;
		}
		vec.Normalize();
		eye += vec * 0.1f;
		YRCamera.SetEye(eye.GetDXFLOAT3());
	}
	break;
	case Knight::WIN_PERFORMANCE_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}


	return false;
}

bool Knight::Intro(float elapsed_time)
{
	//イントロの行動をする
	//カメラもこちらで動かす
	YR_Vector3	focus;
	YR_Vector3	eye;

	/*focus = YR_Vector3(pos.x + Getapply(focus_plus.x), pos.y + focus_plus.y, pos.z + focus_plus.z);
	eye = YR_Vector3(pos.x + Getapply(eye_plus.x), pos.y + eye_plus.y, pos.z + eye_plus.z);*/
	/*focus = YR_Vector3(pos.x + focus_plus.x, pos.y + focus_plus.y, pos.z + focus_plus.z);
	eye = YR_Vector3(pos.x + eye_plus.x, pos.y + eye_plus.y, pos.z + eye_plus.z);
	YRCamera.SetEye(eye.GetDXFLOAT3());
	YRCamera.SetFocus(focus.GetDXFLOAT3());*/
	switch (intro_state)
	{
	case Knight::INTRO_KNIGHT::SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y + 1.8f, pos.z + 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::WAIT;
		lip_text = RandTextSelect();
		break;
	case Knight::INTRO_KNIGHT::WAIT:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y +1.8f, pos.z + 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f*elapsed_time);
		if (intro_timer > 1.0f)
		{
			intro_state = INTRO_KNIGHT::ZOOM_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::ZOOM_SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(25.0f), pos.y + 5.0f, pos.z - 45.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::ZOOM;
		ChangeFace(FaceAnim::NORMAL_LIP_SYNC);
		break;
	case Knight::INTRO_KNIGHT::ZOOM:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.5f*elapsed_time);
		text_on = true;

		if (intro_timer > 3.0f)
		{
			intro_state = INTRO_KNIGHT::PULL;
			ChangeFace(FaceAnim::YARUKI);
			text_on = false;
		}
		break;
	case Knight::INTRO_KNIGHT::PULL:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.0f)
		{
			intro_state = INTRO_KNIGHT::PUSH;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 41.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.5f)
		{
			intro_state = INTRO_KNIGHT::PULL_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PULL_2:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 30.0f, pos.z - 85.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.9f)
		{
			intro_state = INTRO_KNIGHT::PUSH_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH_2:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x+Getapply(27.0f), pos.y + 5.4f, pos.z - 38.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 5.3f)
		{
			intro_state = INTRO_KNIGHT::FIN_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::FIN_SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(30.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x , pos.y, pos.z - 5.4f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::FIN;
		break;
	case Knight::INTRO_KNIGHT::FIN:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x - Getapply(48.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x - Getapply(5.0f), pos.y, pos.z - 5.4f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 8.0f * elapsed_time);
		if (intro_timer > 6.0f)
		{
			intro_state = INTRO_KNIGHT::FINISH;
		}
		break;
	case Knight::INTRO_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}
	intro_timer += elapsed_time;
	FaceAnimation(elapsed_time);
	/*if (intro_timer > 6.0f)
	{
		return true;
	}*/

	return false;
}


//イントロ後、ゲーム開始までの設定を行う
void Knight::ReadySet()
{
	text_on = false;
	anim->NodeChange(model_motion.wait_R);
	ChangeFace(FaceAnim::NORMAL);
	FaceAnimation(0.0f);
}


//カメラ調整用デバッグ関数
void Knight::IntroDEBUG()
{
#if USE_IMGUI
	ImGui::Begin(u8"イントロカメラ");
	int state_in = scastI(intro_state);
	ImGui::InputInt(u8"ステート", &state_in, 1, 10);
	if (ImGui::TreeNode("Input"))
	{
		ImGui::InputFloat("eye.X", &eye_plus.x, 0.1f, 1.0f);
		ImGui::InputFloat("eye.Y", &eye_plus.y, 0.1f, 1.0f);
		ImGui::InputFloat("eye.Z", &eye_plus.z, 0.1f, 1.0f);
		ImGui::InputFloat("focus.X", &focus_plus.x, 0.1f, 1.0f);
		ImGui::InputFloat("focus.Y", &focus_plus.y, 0.1f, 1.0f);
		ImGui::InputFloat("focus.Z", &focus_plus.z, 0.1f, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Slider"))
	{
		ImGui::SliderFloat("eye_X", &eye_plus.x, -350.1f, 350.1f);
		ImGui::SliderFloat("eye_Y", &eye_plus.y, -350.1f, 350.1);
		ImGui::SliderFloat("eye_Z", &eye_plus.z, -350.1f, 350.1);
		ImGui::SliderFloat("focus_X", &focus_plus.x, -350.1f, 350.1);
		ImGui::SliderFloat("focus_Y", &focus_plus.y, -350.1f, 350.1);
		ImGui::SliderFloat("focus_Z", &focus_plus.z, -350.1f, 350.1);
		ImGui::TreePop();
	}
	ImGui::Text("intro_timer = %.3f", intro_timer);
	ImGui::End();
#endif // USE_IMGUI

}

//攻撃当たり判定が全て終了しているか確認する
bool Knight::AttackEndCheck()
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (!a.fin)
			{
				return false;
			}
		}
	}

	return true;
}


//終了した攻撃当たり判定を全て消去する。
void Knight::EndAttackErase()
{
	if (!atk.empty())
	{
		auto result = std::remove_if(atk.begin(), atk.end(),
			[](AttackBox& a)
			{
				return a.fin;
			});
		atk.erase(result, atk.end());
		/*for (std::vector<AttackBox>::iterator& a = atk.begin(); a != atk.end();)
		{
			if (a->fin)
			{

			}
			else
			{

			}
		}*/
	}
}

//全ての攻撃当たり判定を消去する
void Knight::AllAttackClear()
{
	if (!atk.empty())
	{
		for (int a = 0; a < atk.size(); a++)
		{
			atk[a].fin = true;
		}

		EndAttackErase();
	}
}

//攻撃判定が存在する時のみ更新
void Knight::AttackUpdate(float elapsed_time)
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			a.Update(pos, elapsed_time);
			if (a.hit_result != HitResult::NONE)
			{
				//攻撃が当たっていた場合、その内容を保存する
				hit_result = a.hit_result;
				//行動終了フラグをオンに
				finish = true;
			}
		}
	}
}


//顔のテクスチャアニメーション
void Knight::FaceAnimation(float elapsed_time)
{
	switch (face_anim)
	{
	case Knight::FaceAnim::NORMAL:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
		//mouth_offset = face_mouth_offset[scastI(FaceMouth_Num::NORMAL_MOUSE)];
		face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
		face_wink_interval_timer += elapsed_time;
		if (face_wink_interval_timer > wink_interval)
		{
			face_anim = FaceAnim::WINK;
			face_wink_interval_timer = 0.0f;
		}
		break;
	case Knight::FaceAnim::NORMAL_LIP_SYNC:
		FaceLipSync(elapsed_time);
		face_wink_interval_timer += elapsed_time;
		if (face_wink_interval_timer > wink_interval)
		{
			if (FaceWink_bool(elapsed_time))
			{
				face_wink_interval_timer = 0.0f;
			}
		}
		break;
	case Knight::FaceAnim::WINK:
		FaceWink(elapsed_time);
		break;
	case Knight::FaceAnim::Damage:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::KAOMOJI)];
		face_mouth_num = FaceMouth_Num::POKAN;
		break;
	case Knight::FaceAnim::YARUKI:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::TURI)];
		face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
		break;
	case Knight::FaceAnim::KOUHUN:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::KIRAME)];
		face_mouth_num = FaceMouth_Num::OOGUTI;
		break;
	default:
		break;
	}
}


//瞬き
void Knight::FaceWink(float elapsed_time)
{
	face_wink_time += elapsed_time;
	if (face_wink_time > 0.05f)
	{
		switch (wink_state)
		{
		case Wink_State::FIRST:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SECOND;
			break;
		case Wink_State::SECOND:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::THIRD;
			break;
		case Wink_State::THIRD:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
			wink_state = Wink_State::FOURTH;
			break;
		case Wink_State::FOURTH:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::FIVE;
			break;
		case Wink_State::FIVE:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SIX;
			break;
		case Wink_State::SIX:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
			wink_state = Wink_State::FIRST;
			face_anim = FaceAnim::NORMAL;
			break;
		default:
			break;
		}
		face_wink_time = 0.0f;
	}
}


bool Knight::FaceWink_bool(float elapsed_time)
{
	//ウインクが終わったらtrueを返す
	face_wink_time += elapsed_time;
	if (face_wink_time > 0.05f)
	{
		switch (wink_state)
		{
		case Wink_State::FIRST:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SECOND;
			break;
		case Wink_State::SECOND:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::THIRD;
			break;
		case Wink_State::THIRD:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
			wink_state = Wink_State::FOURTH;
			break;
		case Wink_State::FOURTH:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::FIVE;
			break;
		case Wink_State::FIVE:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SIX;
			break;
		case Wink_State::SIX:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
			wink_state = Wink_State::FIRST;
			return true;
			break;
		default:
			break;
		}
		face_wink_time = 0.0f;
	}

	return false;
}


//口パクの処理
void Knight::FaceLipSync(float elapsed_time)
{
	lip_sync_time += elapsed_time;
	if (lip_sync_time > 0.1f)
	{
		for (int i = 0; i < 300; i++)
		{
			int mouth = rand() % lip_sync_can.size();
			if (mouth != scastI(face_mouth_num))
			{
				face_mouth_num = lip_sync_can[mouth];
				break;
			}
		}
		
		lip_sync_time = 0.0f;
	}
}



//表情変更
void Knight::ChangeFace(FaceAnim anim)
{
	face_anim = anim;
	face_wink_time = 0.0f;
	wink_state = Wink_State::FIRST;
	face_wink_interval_timer = 0.0f;
	face_wink_time = 0.0f;
	//表情がまばたき込みの場合のみ
	if (face_anim == FaceAnim::NORMAL||
		face_anim ==FaceAnim::NORMAL_LIP_SYNC)
	{
		//ランダムでまばたきの間隔を決定
		wink_interval =static_cast<float>((rand() % 5) + 1);
	}
}

void Knight::AttackDetailsSet(const AttackState& attack_state)
{
	//攻撃ごとに初回だけ通す処理を記述する
	switch (attack_state)
	{
	case AttackState::NONE:
		break;
	case AttackState::JAKU:
		ChangeFace(FaceAnim::YARUKI);
		break;
	case AttackState::THU:
		break;
	case AttackState::D_KYO:
		break;
	case AttackState::D_JAKU:
		break;
	case AttackState::D_THU:
		break;
	case AttackState::U_KYO:
		break;
	case AttackState::A_JAKU:
		break;
	case AttackState::A_THU:
		break;
	case AttackState::A_KYO:
		break;
	case AttackState::A_UKYO:
		break;
	case AttackState::STEAL:
		break;
	case AttackState::SLOW:
		break;
	case AttackState::TRACK_DASH:
		break;
	case AttackState::JAKU_RHURF:
		break;
	case AttackState::THU_RHURF:
		break;
	case AttackState::KYO_RHURF:
		break;
	case AttackState::JAKU_LHURF:
		break;
	case AttackState::THU_LHURF:
		break;
	case AttackState::KYO_LHURF:
		break;
	case AttackState::SPECIAL_ATTACK:
		YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
		camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
		ChangeFace(FaceAnim::KOUHUN);
		break;
	case AttackState::DESIRE_SPECIAL:
		break;
	case AttackState::DESIRE_METEOR:
		break;
	case AttackState::EXTENDATK:
		break;
	case AttackState::JAKU_THU:
		break;
	case AttackState::JAKU_KYO:
		break;
	case AttackState::ATTACK_END:
		break;
	default:
		break;
	}
}

std::wstring Knight::RandTextSelect()
{
	TextList rnd = static_cast<TextList>(rand() % scastI(TextList::TEXT_END));
	
	switch (rnd)
	{
	case Knight::TextList::NORMAL://通常
		return std::wstring(L"さぁ、始めようか！");
		break;
	case Knight::TextList::WARLIKE://好戦的
		return std::wstring(L"一気に行くよ！");
		break;
	case Knight::TextList::CRIOSITY://好奇心
		return std::wstring(L"私と戦いたいの？");
		break;
	case Knight::TextList::TEXT_END:
		break;
	default:
		break;
	}
	
}

void Knight::TextDraw()
{
	if (!text_on)
	{
		return;
	}

	FRAMEWORK.font->Begin(FRAMEWORK.context.Get());
	FRAMEWORK.font->Draw(
		static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f) - 150.0f,
		static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) - 100.0f, lip_text.c_str());
	FRAMEWORK.font->End(FRAMEWORK.context.Get());
}

void Knight::HitBoxTransition(HitBoxState state)
{
	for (int h = 0; h < hit.size(); h++)
	{
		hit[h].parameter.state = state;
	}
}