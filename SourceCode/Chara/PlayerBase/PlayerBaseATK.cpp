#include "PlayerBase.h"
#include "../../Effect.h"
#include "../../YRSound.h"
#include "../../framework.h"

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
	pos.x -= ((vec + (combo_count_player * 0.1f)) * rightOrleft);
}





//------------------------------------------------------
//				攻撃関数

void Player::AttackDefault(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}






//---------------------------------------------------------
//			飛び道具デフォルト関数
void Player::AttackProjectileDefault(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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

		now_at_num = attack_list[now_at_list].now_attack_num;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}

	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));

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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
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
		anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
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
		anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
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






//------------------------------------------------------
//				何もしない

void Player::AttackNone(float decision, float elapsed_time)
{
	//意図的に何もしない
}






//------------------------------------------------------
//				AttackSwicth関数に入る

void Player::AttackSwicthIn(float decision, float elapsed_time)
{
	AttackSwitch(decision, elapsed_time, attack_list[scastI(attack_state)].real_attack);
}






//------------------------------------------------------
//				ジャンプして攻撃

void Player::AttackJump(float decision, float elapsed_time)
{
	constexpr float up_gravity = 20.0f;
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
		//少し浮かす
		pos.y += up_gravity * elapsed_time;
		fream -= elapsed_time;
	}
	//重力の逆数を付与する
	pos.y += gravity * elapsed_time;

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{
		//重力を付与する
		pos.y -= up_gravity * elapsed_time;
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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}









//------------------------------------------------------
//				空中で跳ねて攻撃

void Player::AttackAirUp(float decision, float elapsed_time)
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

	//重力の逆数を付与する
	if (speed.y < 30.0f)
	{
		pos.y += gravity * elapsed_time;
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
	//発生フレームになったら攻撃判定を生成する
	if (fream < 0.0f)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NONE;
		//前進しないようにする
		speed_X.Set(0.0f);


		if (speed.y < 30.0f)
		{
			speed_Y.Set(attack_list[now_at_list].advance_speed);
		}

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//初回の攻撃のみアニメーションを変える
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}









//------------------------------------------------------
//				掴み攻撃

void Player::AttackSteal(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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


	if (knock)
	{
		//掴み攻撃がヒットしたら投げ攻撃に移行する

		int slow = scastI(AttackState::SLOW);
		int steal = scastI(AttackState::STEAL);

		attack_state = AttackState::SLOW;
		AllAttackClear();
		HitBoxTransition(HitBoxState::NOGUARD);
		//この攻撃をキャンセルするための条件を保存する
		atk_result = attack_list[slow].conditions_hit;
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
		fream = attack_list[slow].attack_single[0].fream;
		//攻撃番号を初期化
		attack_list[slow].now_attack_num = 0;
		//後隙を初期化
		later = non_target;
		//持続を初期化
		timer = non_target;
		//投げ時間をセットする
		steal_escape = attack_list[steal].attack_single[0].parameter[0].param.steal_timer;
		//攻撃ごとに個別の設定を行う
		AttackDetailsSet(attack_list[slow].combo);
		return;
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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}










//------------------------------------------------------
//				投げ攻撃

void Player::AttackSlow(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	if (steal_escape > 0.0f && steal_escape < target_max)
	{
		steal_escape -= elapsed_time;

		if (rival_state != ActState::STATENONE)
		{
			//攻撃番号を初期化
			attack_list[scastI(attack_list[scastI(attack_state)].real_attack)].now_attack_num = 0;
			//後隙を設定
			later = attack_list[scastI(attack_list[scastI(attack_state)].real_attack)].later;
			//アニメーション速度を指定
			anim_ccodinate = ac_attack[scastI(attack_list[scastI(attack_state)].real_attack)].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//掴み抜けされたらはじかれるようにする
			speed.x = (-300.0f * rightOrleft);
			steal_escape = non_target;
			//行動終了フラグをオンに
			finish = true;
		}

		if (steal_escape < 0.0f)
		{
			steal_escape = non_target;
			int steal = scastI(AttackState::STEAL);

			if (rightOrleft > 0)
			{
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					FRAMEWORK.scenegame.PlayerMoveReq(now_player, pos);
					pos = tracking.rival_Pos;
					rightOrleft = -rightOrleft;
				}
			}
			else
			{
				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					FRAMEWORK.scenegame.PlayerMoveReq(now_player, pos);
					pos = tracking.rival_Pos;
					rightOrleft = -rightOrleft;
				}
			}
			int slow = scastI(AttackState::SLOW);
			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].fream;

			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::FREAM));
			}
		}
		else
		{
			return;
		}
	}

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}









//------------------------------------------------------
//				横移動しながら攻撃

void Player::AttackSlid(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{
		//攻撃判定時前進させて回転させる
		speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
		//angle.y += elapsed_time * (50.0f * rightOrleft);

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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//後隙を設定
			later = attack_list[now_at_list].later;

			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;

			//角度を戻す
			angle.y = 0.0f;
			//速度を戻す
			speed.x = 0.0f;

			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}












//------------------------------------------------------
//				横移動しながら回転攻撃

void Player::AttackSlidRoll(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{
		//攻撃判定時前進させて回転させる
		speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
		angle.y += elapsed_time * (50.0f * rightOrleft);

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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//後隙を設定
			later = attack_list[now_at_list].later;

			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;

			//角度を戻す
			angle.y = 0.0f;
			//速度を戻す
			speed.x = 0.0f;

			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}










//------------------------------------------------------
//				横移動しながら空中回転攻撃

void Player::AttackAirSlidRoll(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{
		//攻撃判定時前進させて回転させる
		speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
		angle.y += elapsed_time * (50.0f * rightOrleft);

		//空中なので少しふわっとさせる
		pos.y += (gravity * elapsed_time);

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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//後隙を設定
			later = attack_list[now_at_list].later;

			//角度を戻す
			angle.y = 0.0f;
			//速度を戻す
			speed.x = 0.0f;


			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}










//------------------------------------------------------
//				横移動しながら回転攻撃して戻る

void Player::AttackSlidRollTurn(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
				//持続時間再設定
				timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//持続フレームを減らしていく
		timer -= elapsed_time;
		if (atk.empty())
		{
			//当たるかガードされたら後退させながら回転させる
			speed.x = -(attack_list[now_at_list].advance_speed * decision);
			angle.y -= elapsed_time * (50.0f * rightOrleft);
		}
		else
		{
			//攻撃判定時前進させて回転させる
			speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
			angle.y += elapsed_time * (50.0f * rightOrleft);
		}
	}

	//if (atk.empty())
	//{
	//	//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
	//	return;
	//}
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//timer = non_target;
		}
		else
		{
			//ない場合は後隙に移行する
			//攻撃番号を初期化
			attack_list[now_at_list].now_attack_num = 0;

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//後隙を設定
			later = attack_list[now_at_list].later;

			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;

			//角度を戻す
			angle.y = 0.0f;
			//速度を戻す
			speed.x = -attack_list[now_at_list].advance_speed * decision;

			HitBoxTransition(HitBoxState::NOGUARD);

			//持続フレームを初期化
			timer = non_target;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}








//------------------------------------------------------
//				ホーミングアタック攻撃

void Player::AttackTrack(float decision, float elapsed_time)
{
	if (attack_state != AttackState::TRACK_DASH)
	{
		return;
	}

	rightOrleft = decision;

	//正規化された相手に向かうベクトル
	YR_Vector3	plusVec = { 0.0f,0.0f,0.0f };
	plusVec = tracking.Veccalculate(hit[1].center, decision);//当たり判定の1の要素は必ず体の判定に設定している

	float adjust_angle = 1.0f;

	//相手に向かうための角度を求める(内積)
	if (pos.y < tracking.rival_Pos.y)
	{
		adjust_angle = -1.0f;
	}

	YR_Vector3	a_vec = YR_Vector3(pos.x, pos.y) - YR_Vector3(tracking.rival_Pos.x, tracking.rival_Pos.y);
	YR_Vector3	b_vec = YR_Vector3(pos.x, tracking.rival_Pos.y) - YR_Vector3(tracking.rival_Pos.x, tracking.rival_Pos.y);

	float upper = (a_vec.x * b_vec.x) + (a_vec.y * b_vec.y);

	float dowwnn = sqrtf((a_vec.x * a_vec.x) + (a_vec.y * a_vec.y)) * sqrtf((b_vec.x * b_vec.x) + (b_vec.y * b_vec.y));

	float dot_angle = acosf(upper / dowwnn);

	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	int now_at_num = attack_list[now_at_list].now_attack_num;

	jumpflag = false;

	//重力の逆数を付与する
	pos.y += gravity * elapsed_time;

	//スピードをすべて0にする
	speed.x = 0.0f;
	speed.y = 0.0f;

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;

		//ちょっとずつ浮かせていく
		pos.y += 5.0f * elapsed_time;

		//発生まで少しずつ角度を変えていく
		DirectX::XMVECTOR now_angle_v = DirectX::XMLoadFloat(&angle.z);
		DirectX::XMVECTOR should_angle_v = DirectX::XMLoadFloat(&dot_angle);

		DirectX::XMVECTOR lerp_v = DirectX::XMVectorLerp(now_angle_v, should_angle_v, 0.1f);

		float lerp = 0.0f;

		DirectX::XMStoreFloat(&lerp, lerp_v);

		angle.z = lerp * Getapply(-1.0f) * adjust_angle;
	}
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;

		//アーマーを付与
		HitBoxTransition(HitBoxState::PROJECTILE_ARMOR);
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{

		//常に相手の方向に向ける
		angle.z = dot_angle * Getapply(-1.0f) * adjust_angle;

		pos.x += ((plusVec.x * chara_state.track_speed) * elapsed_time);
		pos.y += ((plusVec.y * chara_state.track_speed) * elapsed_time);

		for (auto& a : atk)
		{
			if (rightOrleft < 0)
			{
				YRGetEffect().SetAngle(a.handle, DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 3.0f + angle.z);
			}
			else
			{
				YRGetEffect().SetAngle(a.handle, DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), angle.z);
			}

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


	if (knock)
	{
		//ホーミングダッシュは当たった時点で攻撃が終了するので後隙を入力する
		//攻撃側のY座標を相手の座標に固定する
		pos.y = tracking.rival_Pos.y;
		//X座標も追撃可能な位置に固定する
		pos.x = tracking.rival_Pos.x + (track_adjust_x * (-decision));
		//上方向への速度を入力する
		speed_X.Set(0.0f);
		//speed_Y.Set(attack_list[now_at_list].advance_speed);
		speed.y = attack_list[now_at_list].advance_speed;


		//YRGetEffect().StopEffect(EffectKind::TRACK);

		//エフェクトを止める為、一度更新処理を掛ける
		AttackUpdate(0.0f);

		//攻撃をすべて消去する
		AllAttackClear();
		//攻撃番号を初期化
		attack_list[now_at_list].now_attack_num = 0;
		//持続を初期化
		timer = non_target;
		//後隙を設定
		later = attack_list[now_at_list].later;
		//アニメーション速度を指定
		anim_ccodinate = ac_attack[now_at_list].later;
		HitBoxTransition(HitBoxState::NOGUARD);
		//行動終了フラグをオンに
		finish = true;
		//角度を戻す
		angle.z = 0.0f;
		angle.x = 0.0f;
		//ジャンプ回数を減らす
		if (jumpcount != 0)
		{
			jumpcount = 1;
		}
		//ホーミングダッシュ回数を減らす
		trackgauge--;
		//ジャンプ状態にする
		jumpflag = true;
		max_jump_flag = true;
	}
	else
	{
		if (timer > 0.0f && timer < target_max)
		{
			//最低保障として1フレームは持たせる
			if (timer < attack_list[now_at_list].attack_single[0].parameter[0].timer - elapsed_time)
			{
				YR_Vector3 dis = tracking.rival_Pos - pos;
				if (dis.Length() < attack_list[now_at_list].attack_single[0].parameter[0].size.x)
				{
					//当たらなかった場合、一定の距離以上に近づいていれば強制的に終了させる

					//エフェクトを止める為、一度更新処理を掛ける
					AttackUpdate(0.0f);
					//攻撃をすべて消去する
					AllAttackClear();
					//タイマーをマイナスにする
					timer = -0.1f;
					//YRGetEffect().StopEffect(EffectKind::TRACK);
				}
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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//上方向への速度を入力する(ちょっとホップさせる)
			speed_X.Set(0.0f);
			speed.y = 0.0f;
			if (plusVec.y > 0.5f)
			{
				//上に向かって飛んでいる場合は速度を入れる
				speed.y = attack_list[now_at_list].advance_speed;
			}

			//攻撃番号を初期化
			attack_list[now_at_list].now_attack_num = 0;

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;

			//角度を戻す
			angle.z = 0.0f;
			angle.x = 0.0f;
			HitBoxTransition(HitBoxState::NOGUARD);
			//ジャンプ回数を減らす
			if (jumpcount != 0)
			{
				jumpcount = 1;
			}
			//ホーミングダッシュ回数を減らす
			trackgauge--;
			if (pos.y > POS_Y)
			{
				//ジャンプ状態にする
				jumpflag = true;
				max_jump_flag = true;
			}
		}
	}
}












//------------------------------------------------------
//				回転ホーミングダッシュ攻撃

void Player::AttackTrackRoll(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	rightOrleft = decision;

	//正規化された相手に向かうベクトル
	YR_Vector3	plusVec = { 0.0f,0.0f,0.0f };
	plusVec = tracking.Veccalculate(hit[1].center, decision);//当たり判定の1の要素は必ず体の判定に設定している

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	int now_at_num = attack_list[now_at_list].now_attack_num;

	jumpflag = false;

	//重力の逆数を付与する
	pos.y += gravity * elapsed_time;

	//スピードをすべて0にする
	speed.x = 0.0f;
	speed.y = 0.0f;

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
		//ちょっとずつ浮かせていく
		pos.y += 5.0f * elapsed_time;
	}

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;

		//飛び道具アーマーを付与
		HitBoxTransition(HitBoxState::PROJECTILE_ARMOR);
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{
		for (auto& a : atk)
		{

			//常に回転させる
			angle.z -= (50.0f * rightOrleft) * elapsed_time;

			pos.x += ((plusVec.x * chara_state.track_speed) * elapsed_time);
			pos.y += ((plusVec.y * chara_state.track_speed) * elapsed_time);

			GetSound().SEPlay(SEKind::TRACK);

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


	if (knock)
	{
		//ホーミングダッシュは当たった時点で攻撃が終了するので後隙を入力する
		//攻撃側のY座標を相手の座標に固定する
		pos.y = tracking.rival_Pos.y;
		//X座標も追撃可能な位置に固定する
		pos.x = tracking.rival_Pos.x + (track_adjust_x * (-decision));
		//上方向への速度を入力する
		speed_X.Set(0.0f);
		//speed_Y.Set(attack_list[now_at_list].advance_speed);
		speed.y = attack_list[now_at_list].advance_speed;


		//YRGetEffect().StopEffect(EffectKind::TRACK);

		//エフェクトを止める為、一度更新処理を掛ける
		AttackUpdate(0.0f);

		//攻撃をすべて消去する
		AllAttackClear();
		//攻撃番号を初期化
		attack_list[now_at_list].now_attack_num = 0;
		//持続を初期化
		timer = non_target;
		//後隙を設定
		later = attack_list[now_at_list].later;
		//アニメーション速度を指定
		anim_ccodinate = ac_attack[now_at_list].later;
		HitBoxTransition(HitBoxState::NOGUARD);
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
		//角度を戻す
		angle.z = 0.0f;
		angle.x = 0.0f;
		//ジャンプ回数を減らす
		if (jumpcount != 0)
		{
			jumpcount = 1;
		}
		//ホーミングダッシュ回数を減らす
		trackgauge--;
		//ジャンプ状態にする
		jumpflag = true;
		max_jump_flag = true;
	}
	else
	{
		if (timer > 0.0f && timer < target_max)
		{
			//最低保障として1フレームは持たせる
			if (timer < attack_list[now_at_list].attack_single[0].parameter[0].timer - elapsed_time)
			{
				YR_Vector3 dis = tracking.rival_Pos - pos;
				if (dis.Length() < attack_list[now_at_list].attack_single[0].parameter[0].size.x)
				{
					//当たらなかった場合、一定の距離以上に近づいていれば強制的に終了させる

					//エフェクトを止める為、一度更新処理を掛ける
					AttackUpdate(0.0f);
					//攻撃をすべて消去する
					AllAttackClear();
					//タイマーをマイナスにする
					timer = -0.1f;
					//YRGetEffect().StopEffect(EffectKind::TRACK);
				}
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
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


	//持続時間が全て終了したことを確認する
	if (timer < 0.0f && timer < target_max)
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

			speed.y = 0.0f;
			if (plusVec.y > 0.5f)
			{
				//上に向かって飛んでいる場合は速度を入れる
				speed.y = attack_list[now_at_list].advance_speed;
			}

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;

			//角度を戻す
			angle.z = 0.0f;
			angle.x = 0.0f;
			HitBoxTransition(HitBoxState::NOGUARD);
			//ジャンプ回数を減らす
			if (jumpcount != 0)
			{
				jumpcount = 1;
			}
			//ホーミングダッシュ回数を減らす
			trackgauge--;
			if (pos.y > POS_Y)
			{
				//ジャンプ状態にする
				jumpflag = true;
				max_jump_flag = true;
			}
		}
	}
}










//------------------------------------------------------
//				ジャンプして無敵攻撃

void Player::AttackJumpEx(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;

		//攻撃発生まで無敵
		HitBoxTransition(HitBoxState::INVINCIBLE);

		//重力の逆数を付与する
		if (!ground)
		{
			pos.y += gravity * elapsed_time;
			speed_Y.Set(0.0f);
			speed.y = 0.0f;
			speed.x = 0.0f;
		}
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
	//発生フレームになったら攻撃判定を生成する
	if (fream < 0.0f)
	{
		//攻撃が発生したら無敵解除
		HitBoxTransition(HitBoxState::NOGUARD);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		speed.y = attack_list[now_at_list].advance_speed;

		//ジャンプ状態にする
		jumpflag = true;
		if (!ground)max_jump_flag = false;

		now_at_num = attack_list[now_at_list].now_attack_num;
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

		//重力の逆数を付与する
		//pos.y += gravity * elapsed_time;

		//pos.y += attack_list[now_at_list].advance_speed * elapsed_time;
	}

	//if (atk.empty())
	//{
	//	//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
	//	return;
	//}
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;

			//ジャンプ状態にする
			jumpflag = true;
			max_jump_flag = true;
		}
	}
}








//------------------------------------------------------
//				前超必殺攻撃

void Player::AttackSpecial(float decision, float elapsed_time)
{
	//前進しないようにする
	speed_X.Set(0.0f);

	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	//イベントシーンを再生する
	if (!special_event.CameraEventUpdate(elapsed_time, this))
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
			}

			//衝撃波エフェクト生成
			YRGetEffect().PlayEffect(EffectKind::LASER_SHOCK,
				DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
				attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
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

		//攻撃発生中は無敵
		//HitBoxTransition(HitBoxState::INVINCIBLE);

		//持続時間を設定
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//持続フレームを減らしていく
		timer -= elapsed_time;
		//if (hit_result != HitResult::GUARD)
		//{
		//	//攻撃中は前に移動させる
		//	pos.x += elapsed_time * Getapply(attack_list[now_at_list].advance_speed);
		//}
		//else
		//{
		//	//無敵を消す
		//	HitBoxTransition(HitBoxState::NOGUARD);
		//}
	}

	//if (atk.empty())
	//{
	//	//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
	//	return;
	//}
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			if (attack_list[now_at_list].now_attack_num == attack_list[now_at_list].attack_max - 1)
			{
				//最終段の攻撃なら
				//衝撃波エフェクト生成
				YRGetEffect().PlayEffect(EffectKind::LASER_SHOCK,
					DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
					attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
			}
		}
		else
		{
			//ブルームを切る
			lumi_material = Model::Material_Attribute::NONE;
			//ない場合は後隙に移行する
			//攻撃番号を初期化
			attack_list[now_at_list].now_attack_num = 0;

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//後隙を設定
			later = attack_list[now_at_list].later;
			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//無敵を消す
			//HitBoxTransition(HitBoxState::NOGUARD);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}













//------------------------------------------------------
//				前超必殺攻撃(前進して無敵)

void Player::AttackSpecial_INV(float decision, float elapsed_time)
{
	//前進しないようにする
	speed_X.Set(0.0f);

	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	//イベントシーンを再生する
	if (!special_event.CameraEventUpdate(elapsed_time, this))
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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

		//攻撃発生中は無敵
		HitBoxTransition(HitBoxState::INVINCIBLE);

		//持続時間を設定
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//持続フレームを減らしていく
		timer -= elapsed_time;
		if (hit_result != HitResult::GUARD)
		{
			//攻撃中は前に移動させる
			pos.x += elapsed_time * Getapply(attack_list[now_at_list].advance_speed);
		}
		else
		{
			//無敵を消す
			HitBoxTransition(HitBoxState::NOGUARD);
		}
	}

	//if (atk.empty())
	//{
	//	//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
	//	return;
	//}
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//ブルームを切る
			lumi_material = Model::Material_Attribute::NONE;
			//ない場合は後隙に移行する
			//攻撃番号を初期化
			attack_list[now_at_list].now_attack_num = 0;

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//後隙を設定
			later = attack_list[now_at_list].later;
			//アニメーション速度を指定
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//持続フレームを初期化
			timer = non_target;
			//無敵を消す
			HitBoxTransition(HitBoxState::NOGUARD);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}


//void Player::SpecialAttack(float elpsed_time)
//{
//	//オーバーライド
//}









//------------------------------------------------------
//				バースト攻撃

void Player::AttackBurst(float decision, float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}


	//重力の逆数を渡す
	if(!ground)pos.y += gravity * elapsed_time;
	//速度を全て0にする
	speed.x = 0.0f;
	speed_X.Set(0.0f);

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	int now_at_num = attack_list[now_at_list].now_attack_num;

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;

		//少し上に跳ねるように
		speed.y = attack_list[now_at_list].advance_speed;

		//当たり判定をバースト無敵にする
		HitBoxTransition(HitBoxState::BURST_INVINCIBLE);
		KnockReset();
	}

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//エフェクト生成
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//プレイヤーの角度を依存させる場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//依存させない場合
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;

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

		//速度を全て0にする
		speed.x = 0.0f;
		speed_X.Set(0.0f);
		speed.y = 0.0f;
		speed_Y.Set(0.0f);
	}

	//if (atk.empty())
	//{
	//	//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
	//	return;
	//}
	//エフェクト更新
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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

			//エフェクト消去
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[now_at_list].anim_kind)], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;

			//ジャンプ状態にする
			jumpflag = true;
			if (jumpcount > 1)jumpcount--;
			max_jump_flag = true;

			//速度を全て0にする
			speed.x = 0.0f;
			speed.y = 0.0f;
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);

			ChangeFace(FaceAnim::NORMAL);

			//当たり判定を戻す
			HitBoxTransition(HitBoxState::NOGUARD);
		}
	}
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





//void Player::A_Jaku_Rhurf(float elapsed_time)
//{
//	//使用する場合はオーバーライド
//}
//
//void Player::A_Thu_Rhurf(float elapsed_time)
//{
//	//使用する場合はオーバーライド
//}
//
//void Player::A_Kyo_Rhurf(float elapsed_time)
//{
//	//使用する場合はオーバーライド
//}
//
//
//void Player::A_Jaku_Lhurf(float elapsed_time)
//{
//	//使用する場合はオーバーライド
//}
//
//void Player::A_Thu_Lhurf(float elapsed_time)
//{
//	//使用する場合はオーバーライド
//}
//
//void Player::A_Kyo_Lhurf(float elapsed_time)
//{
//	//使用する場合はオーバーライド
//}