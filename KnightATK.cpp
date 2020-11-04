#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"

//------------------------------------------------------
//				攻撃関数


void Knight::Slow(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::AttackDefault(float elapsed_time)
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
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::TIMER));
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
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
				//上方向への力を設定する
				if (attack_list[now_at_list].ground_on == Ground_C::AIR)
				{
					//speed.y = 0.0f;
				}
			}
		}
	}

	if (atk.empty())
	{
		//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
		return;
	}

	//攻撃が全て終了したことを確認する
	if (AttackEndCheck())
	{
		//まだ攻撃が残っていれば次の攻撃に移る
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
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
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}


void Knight::Jaku(float elapsed_time)
{
	AttackDefault(elapsed_time);
}

void Knight::Thu(float elapsed_time)
{

	AttackDefault(elapsed_time);
}

void Knight::Kyo(float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	//発生フレームになるまで回す
	if (fream < target_max)
	{
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
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::TIMER));
			}
		}
		if (attack_list[now_at_list].speed_on)
		{
			//攻撃に速度を付与する場合
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos, YR_Vector3(attack_list[now_at_list].speed.x*rightOrleft, attack_list[now_at_list].speed.y));
		}
		else
		{
			//付与しない場合
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos);
		}
		fream = non_target;

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
			}
			if (a.knock_start)
			{
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
	}

	if (atk.empty())
	{
		//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
		return;
	}

	//攻撃が全て終了したことを確認する
	if (AttackEndCheck())
	{
		//まだ攻撃が残っていれば次の攻撃に移る
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
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
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
		}
	}
}


void Knight::D_Jaku(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::D_Thu(float elapsed_time)
{
	AttackDefault(elapsed_time);
}

void Knight::U_Kyo(float elapsed_time)
{
	constexpr float up_gravity = 20.0f;
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		//重力を付与する
		//pos.y -= up_gravity * elapsed_time;
		return;
	}

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		//少し浮かす
		pos.y += up_gravity * elapsed_time;
		fream -= elapsed_time;
	}
	//重力の逆数を付与する
	pos.y += gravity * elapsed_time;

	//発生フレームになったら攻撃判定を生成する
	if (fream < 0.0f)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NONE;
		//前進しないようにする
		speed_X.Set(0.0f);
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft,pos);
		fream = non_target;
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.u_kyo_R, scastI(AnimAtk::TIMER));
		}
		else
		{
			anim->NodeChange(model_motion.u_kyo_L, scastI(AnimAtk::TIMER));
		}
	}

	int now_at_list = scastI(attack_state);

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
			}
			if (a.knock_start)
			{
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
	}

	if (atk.empty())
	{
		//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
		return;
	}

	//攻撃が全て終了したことを確認する
	if (AttackEndCheck())
	{
		//まだ攻撃が残っていれば次の攻撃に移る
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[scastI(attack_state)].attack_single[attack_list[now_at_list].now_attack_num].fream;
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
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
		}
	}
}

//空中弱
void Knight::A_Jaku(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


//空中中
void Knight::A_Thu(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


//空中強
void Knight::A_Kyo(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


//空中上強
void Knight::A_UKyo(float elapsed_time)
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
	pos.y += gravity * elapsed_time;
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//発生フレームになったら攻撃判定を生成する
	if (fream < 0.0f)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NONE;
		//前進しないようにする
		speed_X.Set(0.0f);

		speed_Y.Set(attack_list[now_at_list].advance_speed);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//初回の攻撃のみアニメーションを変える
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::TIMER));
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
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
				//上方向への力を設定する
				if (attack_list[now_at_list].ground_on == Ground_C::AIR)
				{
					//speed.y = 0.0f;
				}
			}
		}
	}

	if (atk.empty())
	{
		//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
		return;
	}

	//攻撃が全て終了したことを確認する
	if (AttackEndCheck())
	{
		//まだ攻撃が残っていれば次の攻撃に移る
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
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
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
			//行動終了フラグをオンに
			finish = true;
		}
	}
}


void Knight::P_Kyo(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::Hadouken(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::Thu_Hadouken(float elapsed_time)
{
	AttackDefault(elapsed_time);
}



void Knight::Kyo_Hadouken(float elapsed_time)
{
	AttackDefault(elapsed_time);
	
}



void Knight::Panish_N(float elapsed_time)
{
	AttackDefault(elapsed_time);
}



void Knight::Panish_H(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::ExtendATK(float elapsed_time)
{
	AttackDefault(elapsed_time);
}



void Knight::SpecialAttack(float elapsed_time)
{
	if (later > -1 && later < target_max)
	{
		return;
	}

	if (fream < target_max)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;

		//YR_Vector3	origin_focus;
		//YR_Vector3	origin_eye;
		YR_Vector3	focus;
		YR_Vector3	eye;
		
		//YRCamera.SetEye(eye.GetDXFLOAT3());
		//YRCamera.SetFocus(focus.GetDXFLOAT3());


		if (fream > 1.5f)
		{
			//カメラのステートが更新ではない場合は初期化にする
			if (camera_state_knight != CAMERA_STATE_KNIGHT::SECOND)
			{
				camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
			}
			YRCamera.SetFov(50.0f * 0.01745f);
		}
		else if (fream > 1.0f)
		{
			//カメラのステートが更新ではない場合は初期化にする
			if (camera_state_knight != CAMERA_STATE_KNIGHT::FOUR)
			{
				camera_state_knight = CAMERA_STATE_KNIGHT::THIRD;
			}
			ChangeFace(FaceAnim::YARUKI);
			YRCamera.SetFov(50.0f * 0.01745f);
		}
		else if (fream > 0.2f)
		{
			//カメラのステートが更新ではない場合は初期化にする
			if (camera_state_knight != CAMERA_STATE_KNIGHT::SIX)
			{
				camera_state_knight = CAMERA_STATE_KNIGHT::FIVE;
			}
			YRCamera.SetFov(50.0f * 0.01745f);
		}
		else
		{
			camera_state_knight = CAMERA_STATE_KNIGHT::SEVEN;
		}

		//演出でカメラを動かす
		switch (camera_state_knight)
		{
		case Knight::CAMERA_STATE_KNIGHT::FIRST:
			//原点を設定(初期化)
			focus = YR_Vector3(pos.x, pos.y, pos.z - 2.0f);
			eye = YR_Vector3(pos.x, pos.y, pos.z - 8.5f);
			YRCamera.SetEye(eye.GetDXFLOAT3());
			YRCamera.SetFocus(focus.GetDXFLOAT3());
			camera_state_knight = CAMERA_STATE_KNIGHT::SECOND;
			break;
		case Knight::CAMERA_STATE_KNIGHT::SECOND:
			//指定した位置までカメラを動かしていく(更新)
			focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 2.0f);
			eye = YR_Vector3(pos.x, pos.y + 5.5f, pos.z - 8.5f);
			YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.05f);
			break;
		case Knight::CAMERA_STATE_KNIGHT::THIRD:
			//原点を設定(初期化)
			focus = YR_Vector3(pos.x, pos.y, pos.z);
			eye = YR_Vector3(pos.x - Getapply(12.0f), pos.y - 2.0f, pos.z - 7.0f);
			YRCamera.SetEye(eye.GetDXFLOAT3());
			YRCamera.SetFocus(focus.GetDXFLOAT3());
			camera_state_knight = CAMERA_STATE_KNIGHT::FOUR;
			break;
		case Knight::CAMERA_STATE_KNIGHT::FOUR:
			//指定した位置までカメラを動かしていく(更新)
			focus = YR_Vector3(pos.x, pos.y, pos.z);
			eye = YR_Vector3(pos.x - Getapply(7.0f), pos.y - 2.0f, pos.z - 7.0f);
			YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.05f);
			break;
		case Knight::CAMERA_STATE_KNIGHT::FIVE:
			//原点を設定(初期化)
			focus = YR_Vector3(pos.x + Getapply(2.0f), pos.y - 6.0f, pos.z);
			eye = YR_Vector3(focus.x + Getapply(9.0f), focus.y + 2.0f, focus.z - 6.0f);
			YRCamera.SetEye(eye.GetDXFLOAT3());
			YRCamera.SetFocus(focus.GetDXFLOAT3());
			camera_state_knight = CAMERA_STATE_KNIGHT::SIX;
			break;
		case Knight::CAMERA_STATE_KNIGHT::SIX:
			//指定した位置までカメラを動かしていく(更新)
			eye = YR_Vector3(pos.x + Getapply(8.5f), pos.y + 2.8f, pos.z - 5.5f);
			focus = YR_Vector3(pos.x - Getapply(5.5f), pos.y, pos.z + 3.0f);
			YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.05f);
			break;
		case Knight::CAMERA_STATE_KNIGHT::SEVEN:
			//カメラを徐々にメインに戻す
			YRCamera.RequestCamera(Camera::Request::WEAKEN, now_player);
			break;
		}
	}


	if (fream < 0.0f)
	{
		//攻撃発生の結果を保存する
		hit_result = HitResult::NONE;
		//前進しないようにする
		speed_X.Set(0.0f);
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft,pos);
		fream = non_target;
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.special_R, scastI(AnimAtk::TIMER));
		}
		else
		{
			anim->NodeChange(model_motion.special_L, scastI(AnimAtk::TIMER));
		}
		YRCamera.RequestCamera(Camera::Request::RELEASE, now_player);
	}

	int now_at_list = scastI(attack_state);

	bool knock = false;	//一度でもknock_startに入ったら残りの当たり判定のknockbackを全て0.0fにする
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
			}
			if (a.knock_start)
			{
				pos.x -= a.parameter.knockback;
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
		//pos.x += elapsed_time * Getapply(50.0f);
		pos.x += elapsed_time * Getapply(150.0f);
	}

	if (atk.empty())
	{
		//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
		return;
	}

	if (AttackEndCheck())
	{
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[scastI(attack_state)].attack_single[attack_list[now_at_list].now_attack_num].fream;
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
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
		}
	}
}


//---------------------------------------------------------------
// **コンボセット関数**
//・入力された攻撃にコンボステートが入っていた場合、
//　そのコンボを入力された地点から開始しtrueを返す
//---------------------------------------------------------------
bool Knight::ComboSet()
{
	AttackState real = attack_list[scastI(attack_state)].real_attack;
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
				if (combolist_X.combolist[i] == real)
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
	default:
		return false;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

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
	//カメラ処理用変数を初期化
	production_time = 0.0f;
	//描画をセット
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.model_R[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L[real_num], scastI(AnimAtk::FREAM));
	}
	//攻撃ごとに個別の設定を行う
	AttackDetailsSet(attack_list[real_num].combo);

	return true;
}

//---------------------------------------------------------------
// **コンボ更新関数**
//・コンボを進めていく
//---------------------------------------------------------------
void Knight::ComboUpdate()
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
	default:
		return;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

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
	//カメラ処理用変数を初期化
	production_time = 0.0f;
	//描画をセット
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.model_R[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L[real_num], scastI(AnimAtk::FREAM));
	}
	//攻撃ごとに個別の設定を行う
	AttackDetailsSet(attack_list[real_num].combo);
}


void Knight::ComboX(float decision, float elapsed_time)
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

//
//--------------------------------------------------------------------------------------------