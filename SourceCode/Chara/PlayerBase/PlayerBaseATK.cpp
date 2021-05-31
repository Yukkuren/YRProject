#include "PlayerBase.h"
#include "../../Effect.h"
#include "../../YRSound.h"

//------------------------------------------------
//					攻撃の処理
//------------------------------------------------
//・攻撃系の関数はこちらに移している
//
//・特別な処理を挟まない場合はAttackDefault関数を中で回す
//------------------------------------------------




void Player::PosKnockPlus(float vec)
{
	//プレイヤーの位置に引数で受け取った値を与える(ノックバック)
	pos.x -= vec * (rightOrleft + (combo_count_player * 0.1f));
}





//------------------------------------------------------
//				攻撃関数

void Player::AttackDefault(float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//発生フレームになったら攻撃判定を生成する
	if (fream < 0.0f)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NONE;
		//前進しないようにする
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//初回の攻撃のみアニメーションを変える
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
		}
		if (attack_list[now_at_list].speed_on)
		{
			//攻撃に速度を付与する場合
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos, attack_list[now_at_list].speed);
		}
		else
		{
			//付与しない場合
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos);
		}
		fream = non_target;


		//持続時間を設定
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
				//上方向への力を設定する
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//持続フレームを減らしていく
		timer -= elapsed_time;
	}

	//if (atk.empty())
	//{
	//	//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
	//	return;
	//}


	//持続時間が全て終了したことを確認する
	if (timer < 0.0f)
	{
		////攻撃が全て終了したことを確認する
		//if (AttackEndCheck())
		//{
			//まだ攻撃が残っていれば次の攻撃に移る
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
			//持続フレームを初期化
			timer = non_target;
		}
		else
		{
			//ない場合は後隙に移行する
			//攻撃番号を初期化
			attack_list[now_at_list].now_attack_num = 0;
			//後隙を設定
			later = attack_list[now_at_list].later;
			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}






//---------------------------------------------------------
//			飛び道具デフォルト関数
void Player::AttackProjectileDefault(float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
		timer = non_target;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//発生フレームになったら攻撃判定を生成する
	if (fream < 0.0f)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NONE;
		//前進しないようにする
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//初回の攻撃のみアニメーションを変える
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
		}

		attack_list[now_at_list].SetAttack(&projectile_atk, rightOrleft, pos, attack_list[now_at_list].speed);

		//YRGetEffect().PlayEffect(EffectKind::DRILL, projectile_atk.back().handle, projectile_atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -90.0f * rightOrleft);

		//projectile_atk.back().effect_kind = EffectKind::DRILL;

		//SE再生
		GetSound().SESinglePlay(SEKind::PROJECTILE);

		//発生フレームを初期化
		fream = non_target;

		//持続時間を設定
		timer = attack_list[now_at_list].timer;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}


	//if (projectile_atk.empty())
	//{
	//	//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
	//	return;
	//}

	if (timer > 0.0f && timer < target_max)
	{
		//持続フレームを減らしていく
		timer -= elapsed_time;
	}

	//持続時間が全て終了したことを確認する
	if (timer < 0.0f)
	{
		//まだ攻撃が残っていれば次の攻撃に移る
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
			//持続フレームを初期化
			timer = non_target;
		}
		else
		{
			//ない場合は後隙に移行する
			//攻撃番号を初期化
			attack_list[now_at_list].now_attack_num = 0;
			//後隙を設定
			later = attack_list[now_at_list].later;
			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;

			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}






// -------------------------------------------------------------- -
// **コンボセット関数**
//・入力された攻撃にコンボステートが入っていた場合、
//　そのコンボを入力された地点から開始しtrueを返す
//---------------------------------------------------------------
bool Player::ComboSet()
{
	AttackState real = attack_list[scastI(attack_state)].real_attack;
	AttackState now_atk = attack_list[scastI(attack_state)].attack_name;
	AttackState truth = attack_list[scastI(attack_state)].real_attack;
	AttackState combo = attack_list[scastI(attack_state)].combo;

	switch (combo)
	{
	case AttackState::COMBO_X:
	{
		//Xボタンコンボ
		combolist_X.now_pos = -1;
		if (!combolist_X.combolist.empty())
		{
			for (int i = 0; i < combolist_X.combolist.size(); i++)
			{
				//コンボリストのどの位置に今の攻撃があるかを確認する
				if (combolist_X.combolist[i] == now_atk)
				{
					combolist_X.now_pos = i;
					break;
				}
			}
		}

		if (combolist_X.now_pos < 0)
		{
			//その攻撃がコンボに含まれていなかった場合false
			return false;
		}
		//コンボを更新
		combolist_X.now_pos++;

		//次の攻撃を設定する
		truth = combolist_X.combolist[combolist_X.now_pos];
	}
	break;
	case AttackState::COMBO_Y:
	{
		//Xボタンコンボ
		combolist_Y.now_pos = -1;
		if (!combolist_Y.combolist.empty())
		{
			for (int i = 0; i < combolist_Y.combolist.size(); i++)
			{
				//コンボリストのどの位置に今の攻撃があるかを確認する
				if (combolist_Y.combolist[i] == now_atk)
				{
					combolist_Y.now_pos = i;
					break;
				}
			}
		}

		if (combolist_Y.now_pos < 0)
		{
			//その攻撃がコンボに含まれていなかった場合false
			return false;
		}
		//コンボを更新
		combolist_Y.now_pos++;

		//次の攻撃を設定する
		truth = combolist_Y.combolist[combolist_Y.now_pos];
	}
	break;
	case AttackState::COMBO_B:
	{
		//Xボタンコンボ
		combolist_B.now_pos = -1;
		if (!combolist_B.combolist.empty())
		{
			for (int i = 0; i < combolist_B.combolist.size(); i++)
			{
				//コンボリストのどの位置に今の攻撃があるかを確認する
				if (combolist_B.combolist[i] == now_atk)
				{
					combolist_B.now_pos = i;
					break;
				}
			}
		}

		if (combolist_B.now_pos < 0)
		{
			//その攻撃がコンボに含まれていなかった場合false
			return false;
		}
		//コンボを更新
		combolist_B.now_pos++;

		//次の攻撃を設定する
		truth = combolist_B.combolist[combolist_B.now_pos];
	}
	break;
	default:
		return false;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

	//地上判定を確認する
	switch (attack_list[real_num].ground_on)
	{
	case Ground_C::AIR:
		if (ground)
		{
			return false;
		}
		break;
	case Ground_C::GROUND:
		if (!ground)
		{
			return false;
		}
		break;
	case Ground_C::EITHER:
		break;
	default:
		return false;
		break;
	}

	if (attack_list[real_num].attack_name == AttackState::TRACK_DASH)
	{
		if (trackgauge < 1)
		{
			return false;
		}
	}

	//攻撃を決定する
	//現在攻撃判定が出ているなら全て消去する
	AllAttackClear();
	//この攻撃をキャンセルするための条件を保存する
	atk_result = attack_list[real_num].conditions_hit;
	//攻撃を保存する
	last_attack = combo;
	//攻撃内容をコンボに
	attack_state = combo;
	//攻撃の結果を初期化
	hit_result = HitResult::NOT_OCCURRENCE;
	//攻撃中フラグをオンに
	attack = true;
	//移動フラグをオフに
	moveflag = false;
	//行動ステートを攻撃に
	act_state = ActState::ATTACK;
	//発生フレームを決定
	fream = attack_list[real_num].attack_single[0].fream;
	//アニメーション速度を指定
	anim_ccodinate = ac_attack[real_num].fream;
	//攻撃番号を初期化
	attack_list[real_num].now_attack_num = 0;
	//当たり判定を初期化
	if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET_UP)
	{
		HitBoxTransition(HitBoxState::UP_INVINCIBLE);
	}
	else if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET)
	{
		HitBoxTransition(HitBoxState::INVINCIBLE);
	}
	else
	{
		HitBoxTransition(HitBoxState::NOGUARD);
	}
	//攻撃発生前の前進距離を設定する(地上攻撃のみ)
	if (attack_list[real_num].ground_on == Ground_C::GROUND)
	{
		speed_X.Set(attack_list[real_num].advance_speed);
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
	//角度を元に戻す
	angle.y = 0.0f;
	angle.z = 0.0f;
	//剣の軌跡を表示するかどうかを取得する
	traject_on = attack_list[real_num].traject_on;
	//描画をセット
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.model_R_Attack[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[real_num], scastI(AnimAtk::FREAM));
	}
	//攻撃ごとに個別の設定を行う
	AttackDetailsSet(attack_list[real_num].combo);

	return true;
}







//---------------------------------------------------------------
// **コンボ更新関数**
//・コンボを進めていく
//---------------------------------------------------------------
void Player::ComboUpdate()
{
	AttackState truth = last_attack;
	AttackState combo = last_attack;

	switch (combo)
	{
	case AttackState::COMBO_X:
	{
		//Xボタンコンボ
		//コンボを更新
		combolist_X.now_pos++;
		if (combolist_X.combolist.empty())
		{
			return;
		}
		if (combolist_X.now_pos >= combolist_X.combolist.size())
		{
			//コンボの最終地点だった場合

			return;
		}
		//次の攻撃を設定する
		truth = combolist_X.combolist[combolist_X.now_pos];
	}
	break;
	case AttackState::COMBO_Y:
	{
		//Xボタンコンボ
		//コンボを更新
		combolist_Y.now_pos++;
		if (combolist_Y.combolist.empty())
		{
			return;
		}
		if (combolist_Y.now_pos >= combolist_Y.combolist.size())
		{
			//コンボの最終地点だった場合

			return;
		}
		//次の攻撃を設定する
		truth = combolist_Y.combolist[combolist_Y.now_pos];
	}
	break;
	case AttackState::COMBO_B:
	{
		//Xボタンコンボ
		//コンボを更新
		combolist_B.now_pos++;
		if (combolist_B.combolist.empty())
		{
			return;
		}
		if (combolist_B.now_pos >= combolist_B.combolist.size())
		{
			//コンボの最終地点だった場合

			return;
		}
		//次の攻撃を設定する
		truth = combolist_B.combolist[combolist_B.now_pos];
	}
	break;
	default:
		return;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

	//地上判定を確認する
	switch (attack_list[real_num].ground_on)
	{
	case Ground_C::AIR:
		if (ground)
		{
			return;
		}
		break;
	case Ground_C::GROUND:
		if (!ground)
		{
			return;
		}
		break;
	case Ground_C::EITHER:
		break;
	default:
		return;
		break;
	}

	//攻撃を決定する
	if (attack_list[real_num].attack_name == AttackState::TRACK_DASH)
	{
		if (trackgauge < 1)
		{
			return;
		}
	}
	//現在攻撃判定が出ているなら全て消去する
	AllAttackClear();
	//この攻撃をキャンセルするための条件を保存する
	atk_result = attack_list[real_num].conditions_hit;
	//攻撃を保存する
	last_attack = combo;
	//攻撃内容をコンボに
	attack_state = combo;
	//攻撃の結果を初期化
	hit_result = HitResult::NOT_OCCURRENCE;
	//攻撃中フラグをオンに
	attack = true;
	//移動フラグをオフに
	moveflag = false;
	//行動ステートを攻撃に
	act_state = ActState::ATTACK;
	//発生フレームを決定
	fream = attack_list[real_num].attack_single[0].fream;
	//アニメーション速度を指定
	anim_ccodinate = ac_attack[real_num].fream;
	//攻撃番号を初期化
	attack_list[real_num].now_attack_num = 0;
	//当たり判定を初期化
	if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET_UP)
	{
		HitBoxTransition(HitBoxState::UP_INVINCIBLE);
	}
	else if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET)
	{
		HitBoxTransition(HitBoxState::INVINCIBLE);
	}
	else
	{
		HitBoxTransition(HitBoxState::NOGUARD);
	}
	//攻撃発生前の前進距離を設定する(地上攻撃のみ)
	if (attack_list[real_num].ground_on == Ground_C::GROUND)
	{
		speed_X.Set(attack_list[real_num].advance_speed);
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
	//角度を元に戻す
	angle.y = 0.0f;
	angle.z = 0.0f;
	//剣の軌跡を表示するかどうかを取得する
	traject_on = attack_list[real_num].traject_on;
	//描画をセット
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.model_R_Attack[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[real_num], scastI(AnimAtk::FREAM));
	}
	//攻撃ごとに個別の設定を行う
	AttackDetailsSet(attack_list[real_num].combo);
}







void Player::ComboX(float decision, float elapsed_time)
{
	//Xボタンコンボ関数
	if (combolist_X.now_pos >= combolist_X.combolist.size())
	{
		return;
	}
	AttackState truth = combolist_X.combolist[combolist_X.now_pos];
	int truth_num = scastI(truth);
	attack_state = attack_list[truth_num].real_attack;

	AttackSwitch(decision, elapsed_time);

	attack_state = last_attack;
}




void Player::ComboY(float decision, float elapsed_time)
{
	//Yボタンコンボ関数
	if (combolist_Y.now_pos >= combolist_Y.combolist.size())
	{
		return;
	}
	AttackState truth = combolist_Y.combolist[combolist_Y.now_pos];
	int truth_num = scastI(truth);
	attack_state = attack_list[truth_num].real_attack;

	AttackSwitch(decision, elapsed_time);

	attack_state = last_attack;
}




void Player::ComboB(float decision, float elapsed_time)
{
	//Bボタンコンボ関数
	if (combolist_B.now_pos >= combolist_B.combolist.size())
	{
		return;
	}
	AttackState truth = combolist_B.combolist[combolist_B.now_pos];
	int truth_num = scastI(truth);
	attack_state = attack_list[truth_num].real_attack;

	AttackSwitch(decision, elapsed_time);

	attack_state = last_attack;
}






//------------------------------
//		掴み攻撃時に
bool Player::StealRangeCheck()
{
	//最終的には掴み攻撃時に相手に向かって自動的にダッシュするようにしたい
	return true;
	if (pos.x > tracking.rival_Pos.x)
	{
		//プレイヤーが右にいる場合

		//掴みを出した場合の攻撃出現位置
		float atk_dis = pos.x - attack_list[scastI(AttackState::STEAL)].attack_single[0].parameter[0].distance.x;

		//攻撃を出したときに相手に当たる位置なら
		if (tracking.rival_Pos.x >= atk_dis)
		{
			return true;
		}
		else
		{
			//当たらないなら走らせる
		}
	}
	else
	{
		//プレイヤーが左にいる場合
		float dis = tracking.rival_Pos.x - pos.x;

		//掴みを出した場合の攻撃出現位置
		float atk_dis = pos.x + attack_list[scastI(AttackState::STEAL)].attack_single[0].parameter[0].distance.x;

		//攻撃を出したときに相手に当たる位置なら
		if (tracking.rival_Pos.x <= atk_dis)
		{
			return true;
		}
		else
		{
			//当たらないなら走らせる
		}
	}
}


//
//--------------------------------------------------------------------------------------------





void Player::A_Jaku_Rhurf(float elapsed_time)
{
	//使用する場合はオーバーライド
}

void Player::A_Thu_Rhurf(float elapsed_time)
{
	//使用する場合はオーバーライド
}

void Player::A_Kyo_Rhurf(float elapsed_time)
{
	//使用する場合はオーバーライド
}


void Player::A_Jaku_Lhurf(float elapsed_time)
{
	//使用する場合はオーバーライド
}

void Player::A_Thu_Lhurf(float elapsed_time)
{
	//使用する場合はオーバーライド
}

void Player::A_Kyo_Lhurf(float elapsed_time)
{
	//使用する場合はオーバーライド
}