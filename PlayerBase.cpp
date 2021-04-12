#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"
#include <algorithm>
#include "World.h"
#include "Effect.h"
#include "YRSound.h"

//-----------------------------------------------------------------
//			Playerクラス
//-----------------------------------------------------------------
//	・このクラスがキャラの基底クラスとなる為、ほとんどの動作を
//	  このクラス内で行う。キャラごとに必要な処理は関数をオーバーライドする
//-----------------------------------------------------------------

void Player::Init(YR_Vector3 InitPos)
{
	//初期化
	pos = InitPos;

	//fream = non_target;
	fream = 0.0f;
	timer = non_target;
	later = non_target;

	speed.x = 0;
	speed.y = 0.0f;
	jumpcount = 2;
	air_dash_count = 1;
	max_jump_flag = false;
	hp = chara_state.max_hp;
	gravity = 40.0f;
	down_force = 200.0f;
	fall_force = 100.0f;
	knocktimer = 0.0f;
	ground = true;
	jumpflag = false;
	finish = false;
	stop_pos = { 0.0f,0.0f };
	step = false;
	hightrigger = false;
	trackgauge = 2;
	gauge = 0;
	power = 0;
	steal_escape = 0;
	speed_X.Set(0.0f);
	speed_Y.Set(0.0f);
	combo_count = 0;


	production_time = 0.0f;

	stop_state = 0;
	air_dash_state = AirDashState::NONE;
	power = 1;

	draw_count = 0;

	light_direction = DirectX::XMFLOAT4(-1.0f, -0.1, 1.0f, 0.0f);
#pragma region HITBOXINIT
	for (int act = 0; act < hit.size(); act++)
	{
		hit[act].Init();
	}
#pragma endregion

	moveflag = false;
	attack = FALSE;
	attack_state = AttackState::NONE;
	last_attack = AttackState::NONE;
	act_state = ActState::NONE;
	later = non_target;

	jump_can_timer = 0.0f;

	passive_timer = 0.0f;

	text_on = false;

	lumi_material = Model::Material_Attribute::NONE;


	if (now_player == 1)
	{
		anim->NodeChange(model_motion.intro_R);
	}
	else
	{
		anim->NodeChange(model_motion.intro_L);
	}

	cut_in_timer = 0.0f;

	traject_on = false;

	CharaInit();
}






void Player::Update(float decision, float elapsed_time)
{
	//更新処理

	//elapsed_timeが0、つまりヒットストップなどでゲームが止まっている場合
	if (elapsed_time == 0.0f)
	{
		//早期returnで処理に入らないようにする
		return;
	}
	finish = false;
	DamageCheck(decision);
	WaitAnimSet();

	HitResultUpdate();

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
	//　それ以外は操作を受け付けるのでdefaultで処理している
	//-----------------------------------------------------
	switch (act_state)
	{
	case ActState::STATENONE:
		//ステートを奪われた状態。相手に掴まれてる
		StateNone(elapsed_time);
		break;
	case ActState::WAKE:
		//ダウンから起き上がる
		WakeUp();
		break;
	case ActState::PASSIVE:
		//受け身をとる
		PassiveUpdate(elapsed_time);
		break;
	case ActState::DOWN:
		//ダウン状態
		DownUpdate();
		break;
	case ActState::DOWN_HIT:
		//ダウンヒット状態
		DownHitUpdate(elapsed_time);
		break;
	case ActState::FALL:
		//空中でダウンしている状態
		FallUpdate(elapsed_time);
		break;
	case ActState::KNOCK:
		//攻撃を受けてのけぞる
		KnockUpdate(elapsed_time);
		break;
	case ActState::SLAM:
		//叩きつけられ状態
		SlamUpdate(elapsed_time);
		break;
	case ActState::ATTACK:
		//攻撃中
		Attack(decision, elapsed_time);
		break;
	case ActState::GUARD:
		Guard(decision);
		GuardBack(elapsed_time);
		Squat();
		break;
	default:
		//その他個別処理

		//角度を戻す
		angle.z = 0.0f;
		if (ground)
		{
			//地面設置時
			//pos.y = POS_Y;	//地面に設置しているときは高さを固定する
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
				pos.y -= gravity * elapsed_time;
			}
		}

		{
			//攻撃を行っていないときの処理
			if (ground && !step)
			{
				jumpcount = 2;
				air_dash_count = 1;
				if (rightOrleft != decision)
				{
					//前回フレームと今回の向きが異なった場合
					if (act_state == ActState::WAIT)
					{
						if (decision > 0)
						{
							//右向きの時
							anim->NodeChange(model_motion.wait_R);
							anim_ccodinate = ac_act[scastI(act_state)].fream;
						}
						else
						{
							//左向きの時
							anim->NodeChange(model_motion.wait_L);
							anim_ccodinate = ac_act[scastI(act_state)].fream;
						}
					}
				}
				if (!moveflag)
				{
					rightOrleft = decision;
				}
				if (speed.x > 0.0f)
				{
					speed.x -= chara_state.brake_speed * elapsed_time;
					if (speed.x < 0.0f)
					{
						speed.x = 0;
					}
				}
				if (speed.x < 0.0f)
				{
					speed.x += chara_state.brake_speed * elapsed_time;
					if (speed.x > 0.0f)
					{
						speed.x = 0.0f;
					}
				}
			}
			//しゃがみやガードなどを先に判定
			Squat();
			Guard(decision);
			//ガードでない時
			if (act_state != ActState::GUARD)
			{
				Step(elapsed_time);
				if (step)
				{
					//ステップ中は処理しない
				}
				else
				{

					//地面についている場合のみ
					if (ground)
					{
						MoveAnimSet();
						Move(rightOrleft);
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

	AttackUpdate(elapsed_time);	//攻撃判定の更新

	JumpUpdate(decision, elapsed_time);
	AirDash(elapsed_time);

	//速度による位置の更新
	pos.x += ((speed_X.Update(elapsed_time) * elapsed_time) * rightOrleft);
	pos.x += (speed.x * elapsed_time);

	speed.y += (speed_Y.Update(elapsed_time) * elapsed_time);
	pos.y += (speed.y * elapsed_time);

	//当たり判定の更新
	//コンボ中や個別のステートによっては別途処理が必要になるため、
	//switch分で個別に分けている
	for (int list = 0; list < hit.size(); list++)
	{
		if (act_state == ActState::ATTACK)
		{
			switch (attack_state)
			{
			case AttackState::COMBO_X:
			{
				AttackState truth = combolist_X.combolist[combolist_X.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, elapsed_time);
			}
			break;
			case AttackState::COMBO_Y:
			{
				AttackState truth = combolist_Y.combolist[combolist_Y.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, elapsed_time);
			}
			break;
			case AttackState::COMBO_B:
			{
				AttackState truth = combolist_B.combolist[combolist_B.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, elapsed_time);
			}
			break;
			case AttackState::VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, elapsed_time);
			}
			break;
			case AttackState::A_VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::A_VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, elapsed_time);
			}
			break;
			default:
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(attack_state)], rightOrleft, elapsed_time);
				break;
			}
		}
		else
		{
			hit[list].Update(pos, hitparam_list[list].act_parameter[scastI(act_state)], rightOrleft, elapsed_time);
		}
	}

	//アニメーション処理更新
	GuardAnimSet();

	if (!attack)
	{
		if (act_state == ActState::DASH || act_state == ActState::BACK)
		{
			//特定の行動時は指定した値を入れる為なにも処理をしない
		}
		else
		{
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}
	EndAttackErase();			//攻撃判定の消去
}









void Player::AttackInput()
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
			if (!pad->que.empty())
			{
				pad->que.back().timer = 0;
			}
			//コマンド判定
			Command command = pad->com_list.CommandCheack(button, pad->input_history.data(), rightOrleft);
			if (now_com == command)
			{
				//指定したコマンドが同じで空中、地上の条件も同じだった場合
				if (ground)
				{
					//現在地上にいる場合は攻撃が空中専用じゃない場合は行う
					if (attack_list[list].ground_on < Ground_C::GROUND)
					{
						continue;
					}
				}
				else
				{
					//現在空中にいる場合は攻撃が地上専用じゃない場合は行う
					if (attack_list[list].ground_on == Ground_C::GROUND)
					{
						continue;
					}
				}

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
					if (act_state == ActState::SQUAT || pad->x_input[scastI(PAD::STICK_D)] > 0 ||
						pad->x_input[scastI(PAD::STICK_LDown)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0)
					{
						if (static_cast<AttackState>(list) != AttackState::TRACK_DASH &&
							static_cast<AttackState>(list) != AttackState::VERSATILE_ATTACK &&
							static_cast<AttackState>(list) != AttackState::A_VERSATILE_ATTACK)
						{
							//ホーミングダッシュとプレイヤー選択技の場合はしゃがんでても出せるように判定する
							continue;
						}
					}
				}
				if (attack_list[list].linkage_stick != PAD::BUTTOM_END)
				{
					if (static_cast<AttackState>(list) != AttackState::TRACK_DASH &&
						static_cast<AttackState>(list) != AttackState::VERSATILE_ATTACK &&
						static_cast<AttackState>(list) != AttackState::A_VERSATILE_ATTACK)
					{
						//スティックの入力が指定されている場合確認する
						if (attack_list[list].linkage_stick == PAD::STICK_D)
						{
							//下スティック入力の場合は別の判定もとる
							if (pad->x_input[scastI(PAD::STICK_D)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0 && pad->x_input[scastI(PAD::STICK_LDown)] == 0)
							{
								continue;
							}
						}
						else
						{
							if (pad->x_input[scastI(attack_list[list].linkage_stick)] == 0)
							{
								continue;
							}
						}
					}
				}
				//実際の攻撃内容
				int real = scastI(attack_list[list].real_attack);
				if (attack_list[real].combo == last_attack)
				{
					if (last_attack == AttackState::COMBO_X || last_attack == AttackState::COMBO_Y || last_attack == AttackState::COMBO_B)
					{
						//コンボ状態に入っている場合はコンボ関数で攻撃のステートを決定する
						ComboUpdate();
						return;
					}
				}

				if (attack_list[real].attack_name == AttackState::TRACK_DASH && trackgauge < 1)
				{
					continue;
				}

				//int next = scastI(attack_list[list].real_attack);
				if (attack_list[real].need_power <= power)
				{
					//ゲージの必要量を確認する
					//通常の攻撃の場合、実際の攻撃内容は入れずに攻撃名をそのまま入れる
					attack_state = static_cast<AttackState>(list);
					if (attack_list[real].need_power > 0)
					{
						//ゲージを消費する攻撃だった場合はゲージを減らす
						power -= attack_list[real].need_power;
					}
				}
				else
				{
					//ゲージが足りない場合指定した技を出す
					attack_state = attack_list[real].aid_attack_name;
					real = scastI(attack_list[scastI(attack_list[real].aid_attack_name)].real_attack);
				}

				//最後に行った攻撃が同じだった場合、コンボを確認して存在するならコンボを開始する
				if (last_attack == attack_state)
				{
					if (ComboSet())
					{
						return;
					}
					//attack_state = attack_list[real].next_attack;
					//next = scastI(attack_list[list].next_attack);
				}


				//攻撃を決定する
				//pad->com_list.Reset();
				//現在攻撃判定が出ているなら全て消去する
				AllAttackClear();
				//この攻撃をキャンセルするための条件を保存する
				atk_result = attack_list[real].conditions_hit;
				//攻撃を保存する
				last_attack = attack_state;
				//攻撃の結果を初期化
				hit_result = HitResult::NOT_OCCURRENCE;
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
				//角度を元に戻す
				angle.y = 0.0f;
				angle.z = 0.0f;
				//当たり判定を初期化
				if (attack_list[real].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET_UP)
				{
					HitBoxTransition(HitBoxState::UP_INVINCIBLE);
				}
				else if (attack_list[real].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET)
				{
					HitBoxTransition(HitBoxState::INVINCIBLE);
				}
				else
				{
					HitBoxTransition(HitBoxState::NOGUARD);
				}
				//攻撃発生前の前進距離を設定する(地上攻撃のみ)
				if (attack_list[real].ground_on == Ground_C::GROUND)
				{
					speed_X.Set(attack_list[real].advance_speed);
				}
				if (!ground)
				{
					//ジャンプフラグも設定しておく
					jumpflag = true;
					max_jump_flag = true;
				}

				//後隙を初期化
				later = non_target;
				//持続を初期化
				timer = non_target;
				//カメラ処理用変数を初期化
				production_time = 0.0f;
				//軌跡を表示するかどうかを取得する
				traject_on = attack_list[real].traject_on;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.model_R[real], scastI(AnimAtk::FREAM));
				}
				else
				{
					anim->NodeChange(model_motion.model_L[real], scastI(AnimAtk::FREAM));
				}

				//攻撃ごとに個別の設定を行う
				AttackDetailsSet(attack_list[real].combo);
				return;
			}
		}
	}
}







void Player::Attack(float decision, float elapsed_time)
{
	//攻撃中に入る関数
	if (ground)
	{
		if (speed.x > 0.0f)
		{
			speed.x -= chara_state.brake_speed * elapsed_time;
			if (speed.x < 0.0f)
			{
				speed.x = 0;
			}
		}
		if (speed.x < 0.0f)
		{
			speed.x += chara_state.brake_speed * elapsed_time;
			if (speed.x > 0.0f)
			{
				speed.x = 0.0f;
			}
		}
	}
	//空中時
	if (pos.y > POS_Y && speed_Y.speed == 0.0f)
	{
		//上方向への力がない場合、重力を付与する
		pos.y -= gravity * elapsed_time;
	}

	AttackSwitch(decision, elapsed_time);

	//攻撃中、攻撃が当たった場合キャンセルして攻撃する
	//前回の攻撃時にどの条件でキャンセルできるかを保存してるため、それ以上ならキャンセルして攻撃する
	if (hit_result >= atk_result)
	{
		AttackInput();
		Jump();
	}

	if (later > 0 && later < target_max)
	{
		later -= elapsed_time;
		//CancelList();
	}

	//後隙消費後元のステートに戻す
	if (later < 0.0f)
	{
		lumi_material = Model::Material_Attribute::NONE;
		finish = true;
		later = non_target;
		attack = false;
		//結果を初期化する
		hit_result = HitResult::NOT_OCCURRENCE;
		//キャンセル条件を初期化する
		atk_result = HitResult::NONE;
		//最終入力内容を初期化する
		last_attack = AttackState::NONE;
		angle.y = 0.0f;

		if (ground)
		{
			//地面についている場合
			if (act_state != ActState::WAIT)
			{

				//もし、下入力がされていた場合、ステートをしゃがみにする
				if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
				{
					act_state = ActState::SQUAT;
					anim_ccodinate = ac_act[scastI(ActState::SQUAT)].fream;
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.squat_R);
					}
					else
					{
						anim->NodeChange(model_motion.squat_L);
					}

				}
				else
				{
					act_state = ActState::NONE;
					anim_ccodinate = ac_act[scastI(ActState::WAIT)].fream;
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.wait_R);
					}
					else
					{
						anim->NodeChange(model_motion.wait_L);
					}
				}
				attack_state = AttackState::NONE;
				ChangeFace(FaceAnim::NORMAL);
			}
		}
		else
		{
			//空中にいる場合

			//ステートをジャンプにする
			act_state = ActState::JUMP;

			//攻撃は何もしていない状態にする
			attack_state = AttackState::NONE;
			//描画をセット
			anim_ccodinate = ac_act[scastI(ActState::JUMP)].timer;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
			}
		}
	}
}







void Player::AttackSwitch(float decision, float elapsed_time, AttackState attack_state)
{
	//攻撃ごとに関数を指定する関数

	AttackState now_attack_state;	//現在の攻撃

	//引数で攻撃を指定していればその攻撃を現在の攻撃として使用する
	if (attack_state == AttackState::NONE)
	{
		now_attack_state = this->attack_state;
	}
	else
	{
		now_attack_state = attack_state;
	}

	switch (now_attack_state)
	{
	case AttackState::NONE:
		break;
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
		break;
	case AttackState::THU:
		//中攻撃(下中)
		D_Thu(elapsed_time);
		break;
	case AttackState::D_KYO:
		//中の次に出る強攻撃
		Kyo(elapsed_time);
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
		//飛び道具(弱)
		Jaku_Rhurf(elapsed_time);
		break;
	case AttackState::THU_RHURF:
		//飛び道具(中)
		Thu_Rhurf(elapsed_time);
		break;
	case AttackState::KYO_RHURF:
		Kyo_Rhurf(elapsed_time);
		break;
	case AttackState::TRACK_DASH:
		//ホーミングダッシュ
		TrackDash(decision, elapsed_time);
		break;
	case AttackState::KYO_LHURF:
		Kyo_Lhurf(elapsed_time);
		break;
	case AttackState::EXTENDATK:
		ExtendATK(elapsed_time);
		break;
	case AttackState::JAKU_LHURF:
		Jaku_Lhurf(elapsed_time);
		break;
	case AttackState::THU_LHURF:
		Thu_Lhurf(elapsed_time);
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
	case AttackState::A_JAKU_LHURF:
		A_Jaku_Lhurf(elapsed_time);
		break;
	case AttackState::COMBO_X:
		ComboX(decision, elapsed_time);
		break;
	case AttackState::COMBO_Y:
		ComboY(decision, elapsed_time);
		break;
	case AttackState::COMBO_B:
		ComboB(decision, elapsed_time);
		break;
	case AttackState::VERSATILE_ATTACK:
		AttackSwitch(decision, elapsed_time, attack_list[scastI(now_attack_state)].real_attack);
		break;
	case AttackState::A_VERSATILE_ATTACK:
		AttackSwitch(decision, elapsed_time, attack_list[scastI(now_attack_state)].real_attack);
		break;
	default:
		break;
	}
}








void Player::MoveAnimSet()
{
	//スティックを両方とも倒している場合
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		return;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{
		return;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		return;
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		return;
	}

	//スティックの左が入力された
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 1)
	{
		moveflag = true;

		if (rightOrleft > 0)
		{
			//描画をセット
			//右向き
			//後退
			anim->NodeChange(model_motion.back_R);
			anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
		}
		//歩きは削除(ダッシュのみ)
		else
		{
			//描画をセット
			//左向き
			/*anim->NodeChange(model_motion.walk_L);
			anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;*/
			//ダッシュ左向き
			act_state = ActState::DASH;
			anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}
	//スティックの右が入力された
	if (pad->x_input[static_cast<int>(PAD::STICK_R)] == 1)
	{
		moveflag = true;

		if (rightOrleft < 0)
		{
			//描画をセット
			//左向き
			//後退
			anim->NodeChange(model_motion.back_L);
			anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
		}
		//歩きは削除(ダッシュのみ)
		else
		{
			//描画をセット
			//右向き
			/*anim->NodeChange(model_motion.walk_R);
			anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;*/
			//ダッシュ右向き
			act_state = ActState::DASH;
			anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}

	if (!moveflag)
	{
		//攻撃後にスティックが入力されたままだとアニメーションの遷移が行われないため
		//moveflagを確認して移動しているかどうかを判別している
		//左にスティックを倒していた
		if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
		{
			moveflag = true;

			if (rightOrleft > 0)
			{
				//描画をセット
				//右向き
				//後退
				anim->NodeChange(model_motion.back_R);
				anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
			}
			//歩きは削除(ダッシュのみ)
			else
			{
				//描画をセット
				//左向き
				/*anim->NodeChange(model_motion.walk_L);
				anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;*/
				//ダッシュ左向き
				act_state = ActState::DASH;
				anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
		//右スティックを倒していた
		if (pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
		{
			moveflag = true;

			if (rightOrleft < 0)
			{
				//描画をセット
				//左向き
				//後退
				anim->NodeChange(model_motion.back_L);
				anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
			}
			//歩きは削除(ダッシュのみ)
			else
			{
				//描画をセット
				//右向き
				/*anim->NodeChange(model_motion.walk_R);
				anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;*/
				//ダッシュ右向き
				act_state = ActState::DASH;
				anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}

}





void Player::Move(float decision)
{
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1 && pad->x_input[scastI(PAD::STICK_R)] == 0)
	{
		if (ground)
		{

			if (rightOrleft > 0)
			{
				//右向きバックステップ
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				GetSound().SESinglePlay(SEKind::BACKSTEP);
				//描画をセット
				anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = -chara_state.backstepS;
			}
			else
			{
				//ダッシュ左向き
				//ダッシュの遷移は別で行うため消去
				//act_state = ActState::DASH;
				//anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
				//anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				//anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1 && pad->x_input[scastI(PAD::STICK_L)] == 0)
	{
		if (ground)
		{
			//描画をセット

			if (rightOrleft < 0)
			{
				//左向きバックステップ
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				GetSound().SESinglePlay(SEKind::BACKSTEP);
				//描画をセット
				anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = chara_state.backstepS;
			}
			else
			{
				//ダッシュ右向き
				//ダッシュの遷移は別で行うため消去
				//act_state = ActState::DASH;
				//anim->NodeChange(model_motion.dash_R,scastI(AnimAtk::FREAM));
				//anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				//anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}


	//左移動
	if (pad->x_input[scastI(PAD::STICK_L)] > 0)
	{
		if (!step)
		{
			if (rightOrleft < 0)
			{
				//左向き
				speed.x = -chara_state.dashspeed;
				act_state = ActState::DASH;
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションがダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
			}
			else
			{
				speed.x = -chara_state.walkspeed;
				act_state = ActState::RETREAT;
			}
		}
	}




	//右移動
	if (pad->x_input[scastI(PAD::STICK_R)] > 0)
	{
		if (!step)
		{
			if (rightOrleft > 0)
			{
				//右向き
				speed.x = chara_state.dashspeed;
				act_state = ActState::DASH;
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションがダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
			}
			else
			{
				//左向き
				speed.x = chara_state.walkspeed;
				act_state = ActState::RETREAT;
			}
		}
	}

	MoveStop();
}



void Player::MoveStop()
{
	//移動停止時処理
	//以前は後隙を設定していたが操作の都合上削除

	//スティックを何も倒していない場合
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	{
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] == 0)
		{
			speed.x = 0.0f;
			if (act_state != ActState::WAIT)
			{
				act_state = ActState::NONE;
			}
			moveflag = false;
		}
	}

	//スティックを両方とも倒している場合
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
}


//移動関係(ステップなど)
//-----------------------------------------------------------------
// *概要*
//・地上の場合、バックステップの処理を行う(前方へはダッシュだけの為)
//・空中の場合は空中ダッシュの設定を行い、処理は別関数で行う
//-----------------------------------------------------------------
bool Player::Step(float elapsed_time)
{
	//空中
	if (!ground)
	{
		//空中左ステップ
		if (pad->x_input[scastI(PAD::L_TRIGGER)] == 1)
		{
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				//スティックを左に倒している時
				if (air_dash_count == 1)
				{
					//ダッシュカウントが残っている場合は空中ダッシュを行う
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 0;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					//スピードはどっちも同じ
					speed.x = -chara_state.stepspeed;
					if (rightOrleft > 0)
					{
						//空中バックダッシュ
						act_state = ActState::AIR_B;
						air_dash_state = AirDashState::AIR_B;
						//描画をセット
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
					}
					else
					{
						//空中前ダッシュ
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
				}
				else
				{
					//ジャンプがない場合はトリガーを外す
					pad->dash_trigger = false;
				}
			}
			else if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				//スティックを右に倒している時
				if (air_dash_count == 1)
				{
					//ダッシュカウントが残っている場合は空中ダッシュを行う
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 1;
					//スピードはどっちも同じ
					speed.x = chara_state.stepspeed;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					if (rightOrleft > 0)
					{
						//空中前ダッシュ
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
					else
					{
						//空中バックダッシュ
						act_state = ActState::AIR_B;
						air_dash_state = AirDashState::AIR_B;
						//描画をセット
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
					}
				}
				else
				{
					//ジャンプがない場合はトリガーを外す
					pad->dash_trigger = false;
				}
			}
			else
			{
				if (air_dash_count == 1)
				{
					//ダッシュカウントが残っている場合は空中ダッシュを行う
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 1;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					if (rightOrleft > 0)
					{
						//空中前ダッシュ
						speed.x = chara_state.stepspeed;
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
					else
					{
						//空中前ダッシュ
						//スピードはどっちも同じ
						speed.x = -chara_state.stepspeed;
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
				}
				else
				{
					//ジャンプがない場合はトリガーを外す
					pad->dash_trigger = false;
				}
			}
		}

		//空中左ステップ
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1)
		{
			if (air_dash_count == 1)
			{
				//ダッシュカウントが残っている場合は空中ダッシュを行う
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 0;
				GetSound().SESinglePlay(SEKind::HIGH_JUMP);
				//スピードはどっちも同じ
				speed.x = -chara_state.stepspeed;
				if (rightOrleft > 0)
				{
					//空中バックダッシュ
					act_state = ActState::AIR_B;
					air_dash_state = AirDashState::AIR_B;
					//描画をセット
					anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
				}
				else
				{
					//空中前ダッシュ
					act_state = ActState::AIR_F;
					air_dash_state = AirDashState::AIR_F;
					//描画をセット
					anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
				}
			}
			else
			{
				//ジャンプがない場合はトリガーを外す
				pad->dash_trigger = false;
			}
		}

		//空中右ステップ
		if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1)
		{
			if (air_dash_count == 1)
			{
				//ダッシュカウントが残っている場合は空中ダッシュを行う
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 1;
				//スピードはどっちも同じ
				speed.x = chara_state.stepspeed;
				GetSound().SESinglePlay(SEKind::HIGH_JUMP);
				if (rightOrleft > 0)
				{
					//空中前ダッシュ
					act_state = ActState::AIR_F;
					air_dash_state = AirDashState::AIR_F;
					//描画をセット
					anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
				}
				else
				{
					//空中バックダッシュ
					act_state = ActState::AIR_B;
					air_dash_state = AirDashState::AIR_B;
					//描画をセット
					anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
				}
			}
			else
			{
				//ジャンプがない場合はトリガーを外す
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
			if (speed.x > 0.0f)
			{
				step = false;
				speed.x = 0.0f;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				moveflag = false;
				act_state = ActState::NONE;
				lumi_material = Model::Material_Attribute::NONE;
				return true;
			}
			if (speed.x < (-chara_state.backstepS / 5.0f))
			{
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}

			if (!anim->GetLoopAnim())
			{
				//現在のアニメーションがバックステップの開始アニメーションだった場合
				if (anim->GetEndAnim() == -1)
				{
					//アニメーションが終了したら持続アニメーションに切り替える
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::TIMER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::TIMER));
					}
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
			}
			else
			{
				//現在のアニメーションがバックステップの持続アニメーションだった場合
				if (speed.x > -chara_state.backstepS)
				{
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::LATER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::LATER));
					}
					anim->PlayAnimation(scastI(AnimAtk::LATER), false);
					anim_ccodinate = ac_act[scastI(act_state)].later;
				}
			}
			//移動範囲のセット
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}
			speed.x += (chara_state.backstepD * elapsed_time);
			return false;
		}
		if (rightOrleft < 0)
		{
			//逆も同じ
			if (speed.x < 0.0f)
			{
				step = false;
				speed.x = 0.0f;
				pad->dash_trigger = false;
				moveflag = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				act_state = ActState::NONE;
				lumi_material = Model::Material_Attribute::NONE;
				return true;
			}
			if (speed.x > (chara_state.backstepS / 5.0f))
			{
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}
			//移動範囲のセット
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}
			if (!anim->GetLoopAnim())
			{
				//現在のアニメーションがバックステップの開始アニメーションだった場合
				if (anim->GetEndAnim() == -1)
				{
					//アニメーションが終了したら持続アニメーションに切り替える
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::TIMER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::TIMER));
					}
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
			}
			else
			{
				//現在のアニメーションがバックステップの持続アニメーションだった場合
				if (speed.x > chara_state.backstepS)
				{
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::LATER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::LATER));
					}
					anim->PlayAnimation(scastI(AnimAtk::LATER), false);
					anim_ccodinate = ac_act[scastI(act_state)].later;
				}
			}
			speed.x -= (chara_state.backstepD * elapsed_time);
			return false;
		}
	}
	else
	{
		step = false;
	}

	return false;
}


//空中ダッシュ処理
//-----------------------------------------------------------------
// *概要*
//・空中ダッシュの処理を行う
//・AirDashStateによって処理を変える
//-----------------------------------------------------------------
void Player::AirDash(float elapsed_time)
{
	switch (air_dash_state)
	{
	case AirDashState::NONE:
		//何もしない
		break;
	case AirDashState::AIR_F:
	{
		//空中ダッシュ
		speed.y = 0.0f;
		speed_X.Set(0.0f);
		speed_Y.Set(0.0f);
		//重力の逆数を付与する
		pos.y += gravity * elapsed_time;
		if (rightOrleft > 0)
		{
			if (speed.x < chara_state.dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				moveflag = false;
				air_dash_count = 0;
				air_dash_state = AirDashState::NONE;
				if (!attack)
				{
					//描画をセット
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}

			if (!attack)
			{
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中ダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中ダッシュの持続アニメーションだった場合
					if (speed.x < chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x -= (chara_state.stepD * elapsed_time);
			return;
		}
		if (rightOrleft < 0)
		{
			if (speed.x > -chara_state.dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
				if (!attack)
				{
					//描画をセット
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}

			if (!attack)
			{
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中ダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中ダッシュの持続アニメーションだった場合
					if (speed.x > -chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x += (chara_state.stepD * elapsed_time);
			return;
		}
	}
	break;
	case AirDashState::AIR_B:
	{
		//空中バックステップ
		speed.y = 0.0f;
		speed_X.Set(0.0f);
		speed_Y.Set(0.0f);
		//重力の逆数を付与する
		pos.y += gravity * elapsed_time;
		if (rightOrleft > 0)
		{
			//右向き
			//スピードがダッシュ以下になったら終了
			if (speed.x > -chara_state.dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
				//描画をセット
				if (!attack)
				{
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}

			if (!attack)
			{
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中バックステップの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中バックステップの持続アニメーションだった場合
					if (speed.x > -chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x += (chara_state.stepD * elapsed_time);

			return;
		}
		if (rightOrleft < 0)
		{
			//左向き
			//スピードがダッシュ以下になったら終了
			if (speed.x < chara_state.dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
				//描画をセット
				if (!attack)
				{
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}

			if (!attack)
			{
				//アニメーション遷移
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中バックステップの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中バックステップの持続アニメーションだった場合
					if (speed.x < chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x -= (chara_state.stepD * elapsed_time);
			return;
		}
	}
	break;
	case AirDashState::ACT_END:
		break;
	default:
		break;
	}
}



void Player::Jump()
{
	//ジャンプ開始処理
	if (step)
	{
		//ステップ中はジャンプできないためreturn
		return;
	}
	if (ground)
	{
		if (pad->x_input[scastI(PAD::HIGH_UP)] == 1)
		{
			//ハイジャンプする
			speed_Y.Set(0.0f);
			speed_X.Set(0.0f);
			pad->que.back().timer = 0;
			GetSound().SESinglePlay(SEKind::HIGH_JUMP);
			jumpcount = 0;
			speed.y = chara_state.high_jump_speed;
			hightrigger = true;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット
			act_state = ActState::JUMP;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::FREAM));
			}
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			jumpflag = true;

			//現在攻撃判定が出ているなら全て消去する
			AllAttackClear();
			//攻撃の結果を初期化
			hit_result = HitResult::NOT_OCCURRENCE;
			//キャンセルの条件を初期化
			atk_result = HitResult::NONE;
			//後隙を初期化
			later = non_target;
			//攻撃フラグをオフに
			attack = false;
			//攻撃ステートを初期化
			attack_state = AttackState::NONE;
			//角度を戻す
			angle.z = 0.0f;
			angle.y = 0.0f;
			//最終入力内容を初期化する
			last_attack = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < chara_state.dashspeed)
				{
					speed.x = chara_state.dashspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -chara_state.dashspeed)
				{
					speed.x = -chara_state.dashspeed;

				}
				return;
			}
			speed.x = 0.0f;
			return;

		}


		if (pad->x_input[scastI(PAD::STICK_U)] == 1)
		{
			//じゃんぷする
			//later = -1;
			attack = FALSE;
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			GetSound().SESinglePlay(SEKind::JUMP);
			jumpcount--;
			hightrigger = false;
			speed.y = chara_state.jump_speed;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット
			act_state = ActState::JUMP;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::FREAM));
			}
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			anim_ccodinate = ac_act[scastI(act_state)].fream;
			jumpflag = true;
			jump_can_timer = jump_max_time;
			//現在攻撃判定が出ているなら全て消去する
			AllAttackClear();
			//攻撃の結果を初期化
			hit_result = HitResult::NOT_OCCURRENCE;
			//後隙を初期化
			later = non_target;
			//攻撃フラグをオフに
			attack = false;
			//攻撃ステートを初期化
			attack_state = AttackState::NONE;
			//角度を戻す
			angle.z = 0.0f;
			angle.y = 0.0f;
			//最終入力内容を初期化する
			last_attack = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < chara_state.walkspeed)
				{
					speed.x = chara_state.walkspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -chara_state.walkspeed)
				{
					speed.x = -chara_state.walkspeed;

				}
				return;
			}
			speed.x = 0.0f;
		}
	}
	else
	{
		//空中ジャンプの場合は処理が異なる
		//スティックを上に倒していると地上ジャンプをした後直ぐに空中ジャンプが出てしまうため
		//ジャンプタイマーが0以下になるまでは空中ジャンプが出ないようにしている
		if (jumpcount > 0 && jump_can_timer <= 0.0f)
		{
			if (pad->x_input[scastI(PAD::STICK_U)] == 1)
			{
				//空中ジャンプする
				//later = -1;
				attack = FALSE;
				speed_X.Set(0.0f);
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				GetSound().SESinglePlay(SEKind::JUMP);
				jumpcount = 0;
				air_dash_count = 0;
				hightrigger = false;
				speed.y = chara_state.jump_speed;
				max_jump_flag = false;
				moveflag = false;
				//描画をセット
				act_state = ActState::JUMP;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.air_jump_R, scastI(AnimAtk::FREAM));
				}
				else
				{
					anim->NodeChange(model_motion.air_jump_L, scastI(AnimAtk::FREAM));
				}
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				jumpflag = true;
				jump_can_timer = jump_max_time;
				//現在攻撃判定が出ているなら全て消去する
				AllAttackClear();
				//攻撃の結果を初期化
				hit_result = HitResult::NOT_OCCURRENCE;
				//キャンセルの条件を初期化
				atk_result = HitResult::NONE;
				//後隙を初期化
				later = non_target;
				//攻撃フラグをオフに
				attack = false;
				//攻撃ステートを初期化
				attack_state = AttackState::NONE;
				//角度を戻す
				angle.z = 0.0f;
				angle.y = 0.0f;
				//最終入力内容を初期化する
				last_attack = AttackState::NONE;

				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					if (speed.x < chara_state.walkspeed)
					{
						speed.x = chara_state.walkspeed;
					}
					return;
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					if (speed.x > -chara_state.walkspeed)
					{
						speed.x = -chara_state.walkspeed;
					}
					return;
				}
				speed.x = 0.0f;
			}
		}
	}
}



void Player::JumpUpdate(float decision, float elapsed_time)
{
	//ジャンプの更新処理
	if (jumpcount < 2 && jumpflag)
	{
		//現在ジャンプしているようなら入る
		if (!anim->GetLoopAnim())
		{
			//現在のアニメーションがジャンプの開始アニメーションだった場合
			if (anim->GetEndAnim() == -1)
			{
				//アニメーションが終了したら持続アニメーションに切り替える
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
				}
				else
				{
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		//ジャンプタイマーを減らしていく
		if (jump_can_timer > 0.0f)
		{
			jump_can_timer -= elapsed_time;
		}

		//ジャンプの最高速度に到達していない場合
		if (!max_jump_flag)
		{
			if (hightrigger)
			{
				speed.y += (chara_state.high_jump_speed * elapsed_time);
				if (speed.y > chara_state.high_jump_max)
				{
					//ジャンプの速度が一定に達した
					max_jump_flag = true;
				}
			}
			else
			{
				speed.y += (chara_state.jump_speed * elapsed_time);
				if (speed.y > chara_state.jump_max)
				{
					//ジャンプの速度が一定に達した
					max_jump_flag = true;
				}
			}
		}
	}

	//ジャンプの速度が一定に達した状態
	if (max_jump_flag)
	{
		if (!ground)
		{
			if (attack)
			{
				//攻撃中の場合攻撃が当たっていれば速度は下げない
				if (hit_result <= HitResult::HIT && combo_count == 0)
				{
					speed.y -= (down_force * elapsed_time);
				}
			}
			else
			{
				//速度を下げて落ちるようにする
				speed.y -= (down_force * elapsed_time);
			}

		}
		if (hightrigger)
		{
			if (speed.y - (gravity * elapsed_time) < 0.0f)
			{
				hightrigger = false;
			}
		}
	}
	if (pos.y < POS_Y)
	{
		//地面についた場合

		//ジャンプのカウントを戻す
		jumpcount = 2;
		air_dash_count = 1;
		max_jump_flag = false;
		hightrigger = false;
		speed.y = 0.0f;
		speed_Y.Set(0.0f);
		//act_state = ActState::NONE;
		pos.y = POS_Y;
		jumpflag = false;
		//ジャンプの着地隙を発生する
		act_state = ActState::ATTACK;
		attack_state = AttackState::NONE;
		attack = true;
		later = jump_later;
		//攻撃でキャンセルできるように
		hit_result = HitResult::NOT_OCCURRENCE;
		atk_result = HitResult::NOT_OCCURRENCE;
		rightOrleft = decision;
		angle.y = 0.0f;
		//角度を元に戻す
		angle.y = 0.0f;
		angle.z = 0.0f;
		timer = non_target;
		//着地音を鳴らす
		GetSound().SESinglePlay(SEKind::LANDING);
		//描画をセット
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::LATER));
		}
		else
		{
			anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::LATER));
		}
		anim->PlayAnimation(scastI(AnimAtk::LATER), false);
		//攻撃判定をすべて消去する
		AllAttackClear();
	}
}


void Player::DamageCheck(float decision)
{
	//ダメージ処理
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hit)
		{
			//攻撃を受けていた
			GetSound().SEStop(SEKind::SLIDE);
			YRGetEffect().StopEffect(EffectKind::SMOKE);
			rightOrleft = decision;
			//条件ごとに設定
			switch (hit[i].hit_state)
			{
			case HitStateKind::NORMAL:
				//通常攻撃
				act_state = ActState::KNOCK;
				steal_escape = 0.0f;
				hit[i].hit_state = HitStateKind::NORMAL;
				timer = non_target;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				if (hit[i].hitback.y < 0.0f)
				{
					//吹っ飛びベクトルがマイナスの場合
					if (pos.y <= POS_Y)
					{
						//下方向の3分の1の速度をXに渡す
						if (hit[i].hitback.x >= 0.0f)
						{
							hit[i].hitback.x += (-hit[i].hitback.y * 0.3f);
						}
						else
						{
							hit[i].hitback.x += (hit[i].hitback.y * 0.3f);
						}
						//地面についている場合は下方向の速度を0にする
						hit[i].hitback.y = 0.0f;
						//のけぞり時間を3分の1にする
						hit[i].timer *= 0.25f;
					}
				}
				break;
			case HitStateKind::STEAL:
				//掴み攻撃
				act_state = ActState::STATENONE;
				steal_escape = hit[i].steal_timer;
				hit[i].hit_state = HitStateKind::STEAL;
				hit[i].steal_timer = 0.0f;
				timer = non_target;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				break;
			case HitStateKind::SLAM:
				//叩きつけ攻撃
				act_state = ActState::SLAM;
				hit[i].hit_state = HitStateKind::NORMAL;
				steal_escape = 0.0f;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				timer = non_target;
				break;
			case HitStateKind::DOWN:
				//ダウン攻撃
				act_state = ActState::DOWN_HIT;
				hit[i].hit_state = HitStateKind::NORMAL;
				steal_escape = 0.0f;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				break;
			case HitStateKind::ARMOR:
				//アーマーが発動した
			{
				steal_escape = 0.0f;
				hit[i].hit_state = HitStateKind::NORMAL;
				float dg = hit[i].damege - (combo_count * 1.2f);
				if (dg <= 0)
				{
					dg = 1;
				}
				hp -= dg;
				if (hp <= 0.0f)
				{
					hp = 0.0f;
					break;
				}
				GaugeUp(hit[i].damege / 5.0f);
				hit[i].damege = 0.0f;
				hit[i].hit = false;
				return;
			}
			break;
			default:
				break;
			}

			//ダメージ、吹っ飛びベクトルなどを保存
			HitBoxTransition(HitBoxState::NOGUARD);
			float dg = hit[i].damege - (static_cast<float>(combo_count) * 1.2f);
			if (dg <= 0.0f)
			{
				dg = 1.0f;
			}
			hp -= dg;
			if (hp <= 0.0f)
			{
				hp = 0.0f;
				act_state = ActState::DOWN_HIT;
			}
			combo_count++;
			GaugeUp(hit[i].damege / 5.0f);
			hit[i].damege = 0.0f;
			hit[i].hit = false;

			//プレイヤーをやられ状態にする
			attack = false;
			later = non_target;
			moveflag = false;
			if (jumpcount > 1)
			{
				jumpcount = 1;
			}
			air_dash_count = 1;
			jumpflag = false;
			max_jump_flag = false;
			knocktimer = hit[i].timer;
			speed.x = 0.0f;
			speed.y = 0.0f;
			step = false;
			pad->dash_trigger = false;
			pad->high_trigger = false;
			hightrigger = false;
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);
			AllAttackClear();
			hit_result = HitResult::NOT_OCCURRENCE;
			lumi_material = Model::Material_Attribute::NONE;
			//キャンセルの条件を初期化
			atk_result = HitResult::NONE;
			attack_state = AttackState::NONE;
			//角度を元に戻す
			angle.y = 0.0f;
			angle.z = 0.0f;
			//最終入力内容を初期化する
			last_attack = AttackState::NONE;
		}
	}
}






void Player::KnockUpdate(float elapsed_time)
{
	//のけぞり処理
	bool pflag = false;		//ノックバックが発動したらtrueにする
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += hit[i].hitback.x * elapsed_time;
			if (pos.x > Limit::Right_max)
			{
				//一定の速度以上で壁に当たった場合跳ね返るようにする
				if (hit[i].hitback.x > Reflection_range_min)
				{
					hit[i].hitback.x = (-hit[i].hitback.x * Reflection_attenuation_factor);
					YRGetEffect().PlayEffect(EffectKind::WALL_SHOCK, DirectX::XMFLOAT3(pos.x - 5.0f, pos.y, pos.z), DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMConvertToRadians(90.0f));
				}
			}
			if (pos.x < Limit::Left_max)
			{
				//一定の速度以上で壁に当たった場合跳ね返るようにする
				if (hit[i].hitback.x < -Reflection_range_min)
				{
					hit[i].hitback.x = (-hit[i].hitback.x * Reflection_attenuation_factor);
					YRGetEffect().PlayEffect(EffectKind::WALL_SHOCK, DirectX::XMFLOAT3(pos.x + 5.0f, pos.y, pos.z), DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMConvertToRadians(-90.0f));
				}
			}
			pflag = true;
		}
		if (hit[i].hitback.y != 0.0f)
		{
			pos.y += hit[i].hitback.y * elapsed_time;
			pflag = true;
		}
		if (pflag)
		{
			break;
		}
	}
	//角度を戻す
	angle.z = 0.0f;
	knocktimer -= elapsed_time;

	if (pos.y < POS_Y)
	{
		pos.y = POS_Y;
		//のけぞり中に地面についた場合
		knocktimer = 0.0f;
		combo_count = 0;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
		if (pad->x_input[scastI(PAD::STICK_D)] > 0 ||
			pad->x_input[scastI(PAD::STICK_LDown)] > 0 ||
			pad->x_input[scastI(PAD::STICK_RDown)] > 0)
		{
			//スティックの下入力がある場合は落下状態にする
			act_state = ActState::DOWN;
			speed.y = 0.0f;
			GetSound().SESinglePlay(SEKind::SLAM);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.slid_R);
			}
			else
			{
				anim->NodeChange(model_motion.slid_L);
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
		else
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			//何も入力していない場合は自動的に受け身を取る
			speed.x = Getapply(-chara_state.passive_speed.x);
			speed.y = chara_state.passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
			if (hp <= 0.0f)
			{
				//体力がなくなったら落下状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		return;
	}

	if (knocktimer < 0.0f)
	{
		//のけぞり時間が経過した

		//コンボカウントを0にする
		combo_count = 0;
		if (ground)
		{
			if (act_state != ActState::WAIT)
			{
				act_state = ActState::NONE;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.wait_R);
				}
				else
				{
					anim->NodeChange(model_motion.wait_L);
				}
				ChangeFace(FaceAnim::NORMAL);
				anim_ccodinate = 0.0f;
			}
		}
		else
		{
			//空中でのけぞり状態が解除された
			if (hp <= 0.0f)
			{
				//体力が無い場合は自動的に受け身を取らないようにする
			}
			else
			{
				//入力が何もない場合は受け身を自動で取る
				speed.x = 0.0f;
				speed.y = 0.0f;
				act_state = ActState::PASSIVE;
				GetSound().SESinglePlay(SEKind::PASSIVE);
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.passive_R);
				}
				else
				{
					anim->NodeChange(model_motion.passive_L);
				}
				HitBoxTransition(HitBoxState::INVINCIBLE);
				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					speed.x = chara_state.passive_speed.x;
					speed.y = chara_state.passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					speed.x = -chara_state.passive_speed.x;
					speed.y = chara_state.passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_U)] > 0)
				{
					speed.y = chara_state.passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_D)] > 0)
				{
					speed.y = -chara_state.passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}

				if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0 &&
					pad->x_input[scastI(PAD::STICK_U)] == 0 && pad->x_input[scastI(PAD::STICK_D)] == 0)
				{
					speed.x = Getapply(-chara_state.passive_speed.x);
					speed.y = chara_state.passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
			}

			//ホーミングダッシュ回数が0以下にならないように
			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		if (hp == 0.0f)
		{
			//体力がなくなったら落下状態にする
			act_state = ActState::FALL;
			speed.y = 0.0f;
			jumpflag = true;
			max_jump_flag = true;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.slid_R);
			}
			else
			{
				anim->NodeChange(model_motion.slid_L);
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
		knocktimer = 0.0f;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
}







void Player::FallUpdate(float elapsed_time)
{
	//落下中処理

	if (ground)
	{
		//地面についたらステートをダウンにする
		pos.y = POS_Y;
		if (hp > 0)
		{
			speed.y = 5.0f;
		}
		act_state = ActState::DOWN;
	}
	else
	{
		//空中時
		if (pos.y < POS_Y)
		{
			//重力を付与する
			pos.y -= gravity * elapsed_time;
		}
	}
}





void Player::DownHitUpdate(float elapsed_time)
{
	//ダウン中の処理

	bool pflag = false;		//ノックバックが発動したらtrueにする

	//速度を加算していく
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += hit[i].hitback.x * elapsed_time;
			pflag = true;
		}
		if (hit[i].hitback.y != 0.0f)
		{
			pos.y += hit[i].hitback.y * elapsed_time;
			pflag = true;
		}
		if (pflag)
		{
			break;
		}
	}
	//角度を戻す
	angle.z = 0.0f;

	if (knocktimer > 0.0f)
	{
		knocktimer -= elapsed_time;
	}
	else
	{
		//当たり判定に入力されている速度を全て初期化する
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].hitback.x = 0.0f;
			hit[i].hitback.y = 0.0f;
		}
		if (!ground)
		{
			max_jump_flag = true;
		}
		if (hp == 0.0f)
		{
			if (ground)
			{
				combo_count = 0;
				knocktimer = 0.0f;
				//ダウン状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				for (int i = 0; i < hit.size(); i++)
				{
					hit[i].timer = 0.0f;
					hit[i].hitback = YR_Vector3(0.0f, 0.0f);
				}
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
	}

	//地面についた時
	if (pos.y < POS_Y)
	{
		combo_count = 0;
		knocktimer = 0.0f;
		//ダウン状態にする
		act_state = ActState::DOWN;
		GetSound().SESinglePlay(SEKind::SLAM);
		speed.y = 0.0f;
		speed.x = 0.0f;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
		//描画をセット
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.slid_R);
		}
		else
		{
			anim->NodeChange(model_motion.slid_L);
		}
		anim_ccodinate = ac_act[scastI(act_state)].timer;
		//地面のめり込みを治す
		pos.y = POS_Y;
	}
}





void Player::DownUpdate()
{
	//ダウン中処理

	if (ground)
	{
		//ダウン中特定の入力があればステートを切り替える
		pos.y = POS_Y;
		speed.y = 0.0f;
		speed.x = 0.0f;
		ChangeFace(FaceAnim::NORMAL);
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = chara_state.passive_speed.x;
			speed.y = chara_state.passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
		}
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -chara_state.passive_speed.x;
			speed.y = chara_state.passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
		}
		if (pad->x_input[scastI(PAD::X)] > 0 || pad->x_input[scastI(PAD::Y)] > 0 ||
			pad->x_input[scastI(PAD::B)] > 0 || pad->x_input[scastI(PAD::A)] > 0)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				speed.x = chara_state.passive_speed.x;
				speed.y = chara_state.passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				speed.x = -chara_state.passive_speed.x;
				speed.y = chara_state.passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
			}
			if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0)
			{
				speed.x = Getapply(-chara_state.passive_speed.x);
				speed.y = chara_state.passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
			}
		}
		if (pad->x_input[scastI(PAD::STICK_U)] > 0)
		{
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R, 1);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L, 1);
			}
			anim->PlayAnimation(1, false);
			act_state = ActState::WAKE;
		}
	}
}






void Player::SlamUpdate(float elapsed_time)
{
	//叩きつけられ処理

	bool pflag = false;		//ノックバックが発動したらtrueにする

	DirectX::XMFLOAT2 hit_back_s = { 0.0f,0.0f };		//このフレーム中に加算された速度

	//速度を加算する
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += hit[i].hitback.x * elapsed_time;
			hit_back_s.x = hit[i].hitback.x;
			pflag = true;
		}
		if (hit[i].hitback.y != 0.0f)
		{
			pos.y += hit[i].hitback.y * elapsed_time;
			hit_back_s.y = hit[i].hitback.y;
			pflag = true;
		}
		if (pflag)
		{
			break;
		}
	}
	//角度を戻す
	angle.z = 0.0f;
	knocktimer -= elapsed_time;


	//地面についた時
	if (pos.y < POS_Y)
	{
		//X方向のスピードを吹っ飛びベクトルから生成する
		DirectX::XMFLOAT2 x_back_s = { (rightOrleft),1.0f };
		//内積を計算して射影ベクトルを作成
		//内積
		float dot = ((hit_back_s.x * x_back_s.x) + (hit_back_s.y * x_back_s.y));
		//X方向の速度に射影ベクトルを設定
		speed.x = (x_back_s.x * (dot / attenuation_slam));

		//当たり判定に入力されている速度を全て初期化する
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].hitback.x = 0.0f;
			hit[i].hitback.y = 0.0f;
		}

		//アニメーションを変更する
		ChangeFace(FaceAnim::NORMAL);
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.slid_R, 1);
		}
		else
		{
			anim->NodeChange(model_motion.slid_L, 1);
		}
		anim->PlayAnimation(1, true);
		//地面のめり込みを治す
		pos.y = POS_Y;

		YRGetEffect().PlayEffect(EffectKind::SHOCKWAVE, DirectX::XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(10.0f, 20.0f, 10.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

		float smoke_angle = 0.0f;
		if (rightOrleft < 1)
		{
			smoke_angle = 90.0f;
		}
		YRGetEffect().PlayEffect(EffectKind::SMOKE, DirectX::XMFLOAT3(pos.x + Getapply(5.0f), pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), smoke_angle);
	}

	//地面を滑っている時
	if (ground)
	{
		GetSound().SEPlay(SEKind::SLIDE);
		HitBoxTransition(HitBoxState::SLIDE);
		YRGetEffect().SetLocation(EffectKind::SMOKE, DirectX::XMFLOAT3(pos.x + Getapply(5.0f), pos.y - 5.0f, pos.z));
		if (speed.x > 0.0f)
		{
			speed.x -= (attenuation_slam * elapsed_time);
			if (speed.x < 0.0f)
			{
				speed.x = 0;
			}
		}
		if (speed.x < 0.0f)
		{
			speed.x += (attenuation_slam * elapsed_time);
			if (speed.x > 0.0f)
			{
				speed.x = 0.0f;
			}
		}
	}

	//のけぞり時間経過
	if (knocktimer < 0.0f)
	{
		GetSound().SEStop(SEKind::SLIDE);
		YRGetEffect().StopEffect(EffectKind::SMOKE);
		combo_count = 0;
		if (ground)
		{
			pos.y = POS_Y;
			knocktimer = 0.0f;
			combo_count = 0;
			for (int i = 0; i < hit.size(); i++)
			{
				hit[i].timer = 0.0f;
				hit[i].hitback = YR_Vector3(0.0f, 0.0f);
			}
			if (pad->x_input[scastI(PAD::STICK_D)] > 0 ||
				pad->x_input[scastI(PAD::STICK_LDown)] > 0 ||
				pad->x_input[scastI(PAD::STICK_RDown)] > 0)
			{
				//スティックの下入力がある場合は落下状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
			else if (hp == 0.0f)
			{
				//体力がない場合は落下状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
			else
			{
				speed.x = 0.0f;
				speed.y = 0.0f;
				//何も入力していない場合は自動的に受け身を取る
				speed.x = Getapply(-chara_state.passive_speed.x);
				speed.y = chara_state.passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
				act_state = ActState::PASSIVE;
				GetSound().SESinglePlay(SEKind::PASSIVE);
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.passive_R);
				}
				else
				{
					anim->NodeChange(model_motion.passive_L);
				}
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}

			if (hp == 0.0f)
			{
				//体力がなくなったら落下状態にする
				act_state = ActState::DOWN;
			}
			return;
		}
		else
		{
			act_state = ActState::FALL;
			speed.y = 0.0f;
			//描画をセット

			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		knocktimer = 0.0f;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
}







void Player::PassiveUpdate(float elapsed_time)
{
	//受け身処理

	if (rightOrleft > 0)
	{
		//右向き
		angle.z += 30.0f * elapsed_time;
	}
	if (rightOrleft < 0)
	{
		//左向き
		angle.z -= 30.0f * elapsed_time;
	}

	//地面にめり込まないように
	if (pos.y < POS_Y)
	{
		pos.y = POS_Y;
	}

	//受け身時間を減らしていく
	passive_timer -= elapsed_time;

	//それぞれの速度を減算、加算して0に近づけていく
	if (speed.x > 0)
	{
		speed.x -= (chara_state.passive_brake * elapsed_time);
		if (speed.x < 0.0f)
		{
			speed.x = 0.0f;
		}
	}
	if (speed.x < 0)
	{
		speed.x += (chara_state.passive_brake * elapsed_time);
		if (speed.x > 0.0f)
		{
			speed.x = 0.0f;
		}
	}
	if (speed.y > 0)
	{
		speed.y -= (chara_state.passive_brake * elapsed_time);
		if (speed.y < 0.0f)
		{
			speed.y = 0.0f;
		}
	}
	if (speed.y < 0)
	{
		speed.y += (chara_state.passive_brake * elapsed_time);
		if (speed.y > 0.0f)
		{
			speed.y = 0.0f;
		}
	}

	//受け身時間が0になったら終了
	if (passive_timer < 0.0f)
	{
		HitBoxTransition(HitBoxState::NOGUARD);
		ChangeFace(FaceAnim::NORMAL);
		angle.z = 0.0f;
		passive_timer = 0.0f;
		if (ground)
		{
			ChangeFace(FaceAnim::NORMAL);
			act_state = ActState::NONE;
		}
		else
		{
			ChangeFace(FaceAnim::NORMAL);
			act_state = ActState::JUMP;
			max_jump_flag = true;
			jumpflag = true;
			lumi_material = Model::Material_Attribute::NONE;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
	}
}





void Player::WakeUp()
{
	//起き上がり処理

	//起き上がり開始は無敵
	HitBoxTransition(HitBoxState::INVINCIBLE);

	//ダウンから回復(完全に起き上がった時)無敵を解除する
	//現在のアニメーションが起き上がりだった場合
	if (anim->GetEndAnim() == -1)
	{
		HitBoxTransition(HitBoxState::NOGUARD);
		act_state = ActState::NONE;
	}
}






void Player::GuardAnimSet()
{
	//ガード時のアニメーション遷移処理
	if (act_state == ActState::GUARD)
	{
		if (ground)
		{
			//スティックの下が入力されている場合は下段ガードに
			if (pad->x_input[static_cast<int>(PAD::STICK_RDown)] > 0 || pad->x_input[static_cast<int>(PAD::STICK_LDown)] > 0)
			{
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.guard_R, 2);
				}
				else
				{
					anim->NodeChange(model_motion.guard_L, 2);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
			else
			{
				//それ以外はガードアニメションに
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.guard_R);
				}
				else
				{
					anim->NodeChange(model_motion.guard_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		else
		{
			//空中ガードアニメーション
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.guard_R, 1);
			}
			else
			{
				anim->NodeChange(model_motion.guard_L, 1);
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
	}

}







void Player::Guard(float decision)
{
	//ガード処理

	if (step || attack || act_state == ActState::PASSIVE || act_state == ActState::WAKE)
	{
		//攻撃中、またはステップ中、受け身中なら入らない
		return;
	}

	//当たり判定のガード状態を解除する(この後の判定処理でガードしていた場合は付与しなおす)
	HitBoxTransition(HitBoxState::NOGUARD);

	//入力判定処理
	//・下入力がされていた場合は下段ガードになる
	//・空中にいる場合は空中ガードになる
	if (rightOrleft > 0)
	{
		//右向きの場合
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
		//左向きの場合
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
		//空中の場合
		if (decision != rightOrleft)
		{
			//向いている方向と現在の処理中の方向が違う場合は両方の向きで判定を取る
			//(ジャンプで相手を飛び越えた時にガード出来ない現象の救済処置)
			if (pad->x_input[scastI(PAD::STICK_R)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0
				|| pad->x_input[scastI(PAD::STICK_L)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
			{
				HitBoxTransition(HitBoxState::ALL);
			}
		}
	}

	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].guard_ok)
		{
			//ガードが出来ているかどうかを当たり判定を見て判定する
			GaugeUp(hit[i].damege / 4.0f);
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);
			speed.x = 0.0f;
			hp -= hit[i].damege;
			if (hp < 0)
			{
				hp = 1;
			}
			if (decision != rightOrleft)
			{
				rightOrleft = decision;
			}
			//ベクトルが内側だった場合外側にする
			if (rightOrleft > 0)
			{
				//右向き時
				if (hit[i].hitback.x > 0.0f)
				{
					hit[i].hitback.x = -hit[i].hitback.x;
				}
			}
			else
			{
				//左向き
				if (hit[i].hitback.x < 0.0f)
				{
					hit[i].hitback.x = -hit[i].hitback.x;
				}
			}
			hit[i].damege = 0;
			hit[i].guard_ok = false;
			moveflag = false;
			knocktimer = hit[i].timer;
			pad->high_trigger = false;
			hightrigger = false;
			act_state = ActState::GUARD;

			GetSound().SESinglePlay(SEKind::GUARD);
			if (rightOrleft > 0)
			{
				YRGetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x + draw_guarf_effect_add_pos_x, pos.y, -2.0f), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 5.5f);
			}
			else
			{
				YRGetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x - draw_guarf_effect_add_pos_x, pos.y, -2.0f), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -5.5f);
			}
		}
	}
}







void Player::GuardBack(float elapsed_time)
{
	//ガードバック処理

	if (act_state != ActState::GUARD)
	{
		return;
	}

	bool hit_on = false;	//ガードバックが発動したらtrueにする


	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.y != 0.0f)
		{
			if (!ground)
			{
				pos.y += (hit[i].hitback.y * elapsed_time);
				hit_on = true;
			}
		}
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += (hit[i].hitback.x * elapsed_time);
			hit_on = true;
		}

		if (hit_on)
		{
			break;
		}
	}
	if (knocktimer > 0.0f)
	{
		knocktimer -= elapsed_time;
		if (knocktimer == 0)
		{
			knocktimer -= elapsed_time;
		}
	}
	if (knocktimer < 0.0f)
	{
		if (ground)
		{
			if (act_state != ActState::WAIT)
			{
				act_state = ActState::NONE;
			}
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
			if (act_state == ActState::SQUAT)
			{
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.squat_R);
				}
				else
				{
					anim->NodeChange(model_motion.squat_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
		else
		{
			act_state = ActState::JUMP;
			//描画をセット
			max_jump_flag = true;
			jumpflag = true;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}

		knocktimer = 0;
		for (int hitnum = 0; hitnum < hit.size(); hitnum++)
		{
			hit[hitnum].timer = 0.0f;
			hit[hitnum].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
	else
	{
		act_state = ActState::GUARD;
	}
}







void Player::WaitAnimSet()
{
	//待機アニメーション遷移処理
	if (act_state == ActState::NONE)
	{
		act_state = ActState::WAIT;
		lumi_material = Model::Material_Attribute::NONE;
		//描画をセット
		if (rightOrleft > 0)
		{
			//右向きの時
			anim->NodeChange(model_motion.wait_R);
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
		else
		{
			//左向きの時
			anim->NodeChange(model_motion.wait_L);
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
		speed.x = 0.0f;
		speed.y = 0.0f;
		moveflag = false;
	}
}






void Player::NoneChange()
{
	//ガードしていない時に何も入力されていない場合はステートをNONEにする処理

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







void Player::Squat()
{
	//しゃがみ処理

	//ガード中、ダッシュ中、バックステップ、空中ダッシュ時は処理を行わない
	if (act_state == ActState::GUARD || act_state == ActState::DASH || act_state == ActState::BACK || act_state == ActState::AIR_B || act_state == ActState::AIR_F)
	{
		return;
	}
	//地面についていないときは処理を行わない
	if (!ground)
	{
		return;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] == 1 || pad->x_input[scastI(PAD::STICK_RDown)] == 1 || pad->x_input[scastI(PAD::STICK_LDown)] == 1)
	{
		moveflag = false;
		act_state = ActState::SQUAT;
		//描画をセット
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.squat_R);
		}
		else
		{
			anim->NodeChange(model_motion.squat_L);
		}
		anim_ccodinate = ac_act[scastI(act_state)].fream;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
	{
		moveflag = false;
		act_state = ActState::SQUAT;
		//描画をセット
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.squat_R);
		}
		else
		{
			anim->NodeChange(model_motion.squat_L);
		}
		anim_ccodinate = ac_act[scastI(act_state)].fream;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0 && pad->x_input[scastI(PAD::STICK_LDown)] == 0)
	{
		//下が入力されていない場合は待機に戻す
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
	}
}





void Player::StateNone(float elapsed_time)
{
	//ステート奪われた状態の処理

	if (steal_escape > 0.0f)
	{
		if (pad->x_input[scastI(PAD::RB)] == 1)
		{
			steal_escape = 0.0f;
			speed.x = 0.0f;
			speed.y = 0.0f;
			knocktimer = 0.0f;
			for (int i = 0; i < hit.size(); i++)
			{
				hit[i].timer = 0.0f;
				hit[i].hitback = YR_Vector3(0.0f, 0.0f);
			}
			combo_count = 0;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);

			speed.x = Getapply(-chara_state.passive_speed.x);
			speed.y = chara_state.passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			return;
		}
		steal_escape -= elapsed_time;
	}
}





void Player::GaugeUp(float add)
{
	//ゲージ、パワー増加処理

	gauge += add;
	if (power > 4)
	{
		power = 5;
		if (gauge > GAUGE_MAX)
		{
			gauge = GAUGE_MAX;
		}
	}
	else
	{
		if (gauge > GAUGE_MAX)
		{
			float away = gauge - GAUGE_MAX;
			power++;
			gauge = 0;
			gauge = away;
		}
	}
}





//攻撃当たり判定が全て終了しているか確認する
bool Player::AttackEndCheck()
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
void Player::EndAttackErase()
{
	if (!atk.empty())
	{
		auto result = std::remove_if(atk.begin(), atk.end(),
			[](AttackBox& a)
			{
				return a.fin;
			});
		atk.erase(result, atk.end());
	}

	if (!projectile_atk.empty())
	{
		auto result = std::remove_if(projectile_atk.begin(), projectile_atk.end(),
			[](AttackBox& a)
			{
				return a.fin;
			});
		projectile_atk.erase(result, projectile_atk.end());
	}
}





//全ての攻撃当たり判定を消去する
void Player::AllAttackClear()
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
void Player::AttackUpdate(float elapsed_time)
{
	if (YRCamera.camera_state == Camera::CAMERA_STATE::MAIN)
	{
		if (!atk.empty())
		{
			for (auto& a : atk)
			{
				a.Update(pos, elapsed_time);
			}
		}

		if (!projectile_atk.empty())
		{
			for (int i = 0; i < projectile_atk.size(); i++)
			{
				projectile_atk[i].Update(pos, elapsed_time);
			}
		}
	}
}






//攻撃判定が存在する時のみ更新(当たり判定が取得した結果をプレイヤーに送る)
void Player::HitResultUpdate()
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
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





void Player::HitBoxTransition(HitBoxState state)
{
	//全ての当たり判定のステートを変更する

	for (int h = 0; h < hit.size(); h++)
	{
		hit[h].state = state;
	}
}





float Player::Getapply(float n)
{
	//現在向いている向きに合わせた数字を返す
	return n * rightOrleft;
}





void Player::AttackDetailsSet(const AttackState& attack_state)
{
	//攻撃ごとに初回だけ通す処理を記述する
	//基本は何もしないが使用するキャラはオーバーライドすること
}




void Player::StopUpdate()
{
	//ヒットストップ中行う処理

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





void Player::StopEnd()
{
	//ヒットストップ終了時に行う処理

	if (stop_pos == YR_Vector3(0.0f, 0.0f))
	{
		return;
	}
	pos = stop_pos;
	stop_pos = YR_Vector3(0.0f, 0.0f);
}





void Player::StopHitParamUpdate()
{
	//デバッグ用コード
	//Pause中にパラメーターをいじれるようにするための関数
	for (int list = 0; list < hit.size(); list++)
	{
		if (act_state == ActState::ATTACK)
		{
			switch (attack_state)
			{
			case AttackState::COMBO_X:
			{
				AttackState truth = combolist_X.combolist[combolist_X.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, 0.0f);
			}
			break;
			case AttackState::COMBO_Y:
			{
				AttackState truth = combolist_Y.combolist[combolist_Y.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, 0.0f);
			}
			break;
			case AttackState::COMBO_B:
			{
				AttackState truth = combolist_B.combolist[combolist_B.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, 0.0f);
			}
			break;
			case AttackState::VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, 0.0f);
			}
			break;
			case AttackState::A_VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::A_VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, 0.0f);
			}
			break;
			default:
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(attack_state)], rightOrleft, 0.0f);
				break;
			}
		}
		else
		{
			hit[list].Update(pos, hitparam_list[list].act_parameter[scastI(act_state)], rightOrleft, 0.0f);
		}
	}
}






//表情変更
void Player::ChangeFace(FaceAnim anim)
{
	//通常は何も行わない。
	//テクスチャアニメーションで表情を変更するモデルを使用する場合はこの関数をオーバーライドして
	//表情の変更処理を行う。
	return;
}




void Player::TextDraw()
{
	//設定されたテキストを表示する

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




void Player::DrawCutIn(
	YRShader* shader,
	float elapsed_time
)
{
	//カットインの表示

	float center_x = static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f;
	float center_y = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f;

	float fream = 0.03f;
	float size = 3.0f;

	bool anim_fin = cutFrame->AnimFinCheck();

	if (anim_fin)
	{
		cut_in_timer += elapsed_time;
		{
			//通常の描画(最後のアニメーションを描画し続ける)
			cutFrame->DrawRotaDivGraph(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				cutFrame->max - 1,
				SpriteMask::FRAME
			);

			cutMask->DrawRotaDivGraph(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				cutMask->max - 1,
				SpriteMask::WRITE
			);
		}
	}
	else
	{
		if (cut_in_timer < cut_in_max_time)
		{
			//カットイン再生
			bool finish_cut = cutFrame->DrawRotaDivGraphOnec(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::FRAME
			);

			cutMask->DrawRotaDivGraphOnec(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::WRITE
			);
		}
	}

	//一定の時間以上になったら逆再生を開始する
	if (cut_in_timer > cut_in_max_time)
	{
		bool finish_cut = cutFrame->DrawRotaDivGraphReverse(
			shader,
			center_x,
			center_y,
			0.0f,
			size,
			fream,
			elapsed_time,
			SpriteMask::FRAME
		);

		cutMask->DrawRotaDivGraphReverse(
			shader,
			center_x,
			center_y,
			0.0f,
			size,
			fream,
			elapsed_time,
			SpriteMask::WRITE
		);
	}


	cutIn->DrawRotaGraph(
		shader,
		center_x,
		center_y,
		0.0f,
		3.0f,
		false,
		SpriteMask::INDRAW
	);
}