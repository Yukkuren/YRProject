#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"

//------------------------------------------------------
//				攻撃関数




void Knight::Slow(float elapsed_time)
{
	/*YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	YR_Vector3 range{ 200.0f,50.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 5, 15, 20, 40, YR_Vector3(Getapply(10.0f), 30.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);*/
}



void Knight::Jaku(float elapsed_time)
{
	//後隙が設定された後はこの関数には入らない
	if (later > -1 && later < target_max)
	{
		return;
	}

	//発生フレームになるまで回す
	if (fream < target_max)
	{
		fream -= elapsed_time;
	}
	

	//発生フレームになったら攻撃判定を生成する
	if (fream < 0.0f)
	{
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft);
		fream = non_target;
		anim->NodeChange(model_motion.jaku_R_t);
		//motion.MeshSet(jaku_r_t);
		//motion.AnimReset();
		
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
				pos.x -= a.parameter.knockback*rightOrleft;
				a.parameter.knockback = 0.0f;
				if (!ground)
				{
					speed_Y.Set(60.0f);
				}
				knock = true;
			}
		}
	}

	if (atk.empty())
	{
		//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
		return;
	}

	//atk.back().SpeedPlus(YR_Vector3(5.0f, 5.0f), elapsed_time);

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
			attack_list[now_at_list].now_attack_num = 0;
			later = attack_list[now_at_list].later;
			anim_ccodinate = ac_attack[scastI(attack_state)].later;
			anim->NodeChange(model_motion.jaku_R_l);
			//motion.MeshSet(base);
			//motion.AnimReset();
		}
	}

	specialfream = 0;
	/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
		if (!ground)
		{
			speed_Y.Set(60.0f);
		}
	}
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	YR_Vector3 range{ 50.0f,50.0f };
	if (ground)
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 5, 15, 10, 15, YR_Vector3(Getapply(5.0f), 0.0f), AttackBox::MIDDLE, Getapply(10.0f),elapsed_time);
	}
	else
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 5, 15, 10, 20, YR_Vector3(Getapply(5.0f), 20.0f), AttackBox::MIDDLE, Getapply(10.0f),elapsed_time);
	}
	specialfream = 0;*/
}

void Knight::Thu(float elapsed_time)
{

	if (later > -1 && later < target_max)
	{
		return;
	}

	if (fream < target_max)
	{
		fream -= elapsed_time;

		YR_Vector3	eye = YRCamera.GetEye();
		YR_Vector3	focus = YRCamera.GetFocus();
		YRCamera.SetFov(50.0f * 0.01745f);
		if (fream > 1.5f)
		{
			focus = pos;
			eye.x = focus.x;
			eye.y = focus.y + 2.0f;
			eye.z = focus.z - 8.0f;
		}
		else if(fream > 1.0f)
		{

			focus = pos;
			eye.x = pos.x - Getapply(7.0f);
			eye.y = focus.y - 2.0f;
			eye.z = pos.z - 7.0f;
		}
		else if(fream > 0.5f)
		{
			focus = pos;
			focus.x + 2.0f;
			focus.y - 2.0f;
			eye.x = focus.x + Getapply(9.0f);
			eye.y = focus.y + 6.0f;
			eye.z = focus.z - 6.0f;
		}
		else
		{
			//カメラを徐々にメインに戻す
			YRCamera.RequestCamera(Camera::Request::WEAKEN, now_player);
		}
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
	}


	if (fream < 0.0f)
	{
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft);
		fream = non_target;
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
				if (!ground)
				{
					speed_Y.Set(60.0f);
				}
				knock = true;
			}
		}
	}

	if (atk.empty())
	{
		//もし攻撃がまだ出ていないならここでreturnして次の攻撃に移らないようにする
		return;
	}

	//atk.back().SpeedPlus(YR_Vector3(5.0f, 5.0f), elapsed_time);

	if (AttackEndCheck())
	{
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[scastI(attack_state)].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			attack_list[now_at_list].now_attack_num = 0;
			later = attack_list[now_at_list].later;
		}
	}

	specialfream = 0;
	/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
		if (!ground)
		{
			speed_Y.Set(60.0f);
		}
	}
	YR_Vector3 cent{ pos.x + Getapply(120.0f),pos.y };
	YR_Vector3 range{ 100.0f,200.0f };
	if (fream != 0)
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, fream, 20, 30, 40, YR_Vector3(Getapply(20.0f), 0.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
		else
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, fream, 20, 30, 40, YR_Vector3(Getapply(20.0f), 30.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
	}
	else
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, 20, 20, 30, 40, YR_Vector3(Getapply(20.0f), 0.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
		else
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, 20, 20, 30, 40, YR_Vector3(Getapply(20.0f), 30.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
	}
	specialfream = 0;*/
}

void Knight::Kyo(float fream, float elapsed_time)
{
	/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(300.0f),pos.y };
	YR_Vector3 range{ 150.0f,150.0f };
	if (specialfream != 0)
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, specialfream, 30, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
		cent = { pos.x + Getapply(100.0f),pos.y };
		range = { 100.0f,50.0f };
		atk[scastI(KNIGHTATK::TWO)].Update(
			cent, range, 5, specialfream, 30, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
	}
	else
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 25, 20, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
		cent = { pos.x + Getapply(100.0f),pos.y };
		range = { 100.0f,50.0f };
		atk[scastI(KNIGHTATK::TWO)].Update(
			cent, range, 5, 25, 20, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
	}
	specialfream = 0;*/
}


void Knight::D_Jaku(float elapsed_time)
{
	/*Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
	hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
	Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(8.0f, 163.0f);
	hit[scastI(KNIGHTHIT::LEG)].size = YR_Vector3(86.0f, 40.0f);
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(60.0f),pos.y + 150.0f };
	YR_Vector3 range{ 50.0f,50.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 2, 2, 2, 5, 15, YR_Vector3(Getapply(3.0f), 0.0f), AttackBox::DOWN, Getapply(5.0f),elapsed_time);
	specialfream = 0;*/
}


void Knight::D_Thu(float fream, float elapsed_time)
{
	//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(-31.0f, 134.0f);
	//hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(110.0f, 62.0f);
	//Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(110.0f, 149.0f);
	//hit[scastI(KNIGHTHIT::LEG)].size = YR_Vector3(39.0f, 47.0f);
	//if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	//{
	//	pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
	//	atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	//}
	//YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y + 150.0f };
	//YR_Vector3 range{ 100.0f,50.0f };
	//if (fream != 0)
	//{//3
	//	atk[scastI(KNIGHTATK::ONE)].Update(
	//		cent, range, 3, fream, 35, 30, 30, YR_Vector3(Getapply(10.0f), 10.0f), AttackBox::DOWN, Getapply(30.0f),elapsed_time);
	//}
	//else
	//{
	//	atk[scastI(KNIGHTATK::ONE)].Update(
	//		cent, range, 3, 6, 35, 30, 30, YR_Vector3(Getapply(10.0f), 10.0f), AttackBox::DOWN, Getapply(30.0f),elapsed_time);
	//}
	//if (later < 0)
	//{
	//	pos.x += Getapply(20.0f);
	//}
	//specialfream = 0;
}

void Knight::U_Kyo(float fream, float elapsed_time)
{
	/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y - 100.0f };
	YR_Vector3 range{ 150.0f,150.0f };
	if (specialfream != 0)
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, specialfream, 30, 50, 30, YR_Vector3(Getapply(50.0f), 10.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
		else
		{
			cent = { pos.x + Getapply(100.0f),pos.y + 50.0f };
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, specialfream, 30, 50, 30, YR_Vector3(Getapply(50.0f), -25.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
	}
	else
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, 20, 30, 50, 30, YR_Vector3(Getapply(50.0f), 10.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
		else
		{
			cent = { pos.x + Getapply(100.0f),pos.y + 50.0f };
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, 20, 30, 50, 30, YR_Vector3(Getapply(50.0f), -25.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
	}
	specialfream = 0;*/
}

void Knight::P_Kyo(float elapsed_time)
{
	/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	pos.y -= gravity;
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y - 100.0f };
	YR_Vector3 range{ 150.0f,150.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 10, 20, 80, 50, YR_Vector3(Getapply(50.0f), 40.0f), AttackBox::UP, Getapply(50.0f), 0,elapsed_time);*/
}


void Knight::Hadouken(float elapsed_time)
{
	/*YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 0.1f,0.1f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 1, 15, 15, 1, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	if (atk[scastI(KNIGHTATK::ONE)].hit_ok)
	{
		atk[scastI(KNIGHTATK::ONE)].damege = 0;
		if (!atk[scastI(KNIGHTATK::HADOU)].hit_ok)
		{
			hadou.x = pos.x + Getapply(100);
			hadou.y = pos.y;
			atk[scastI(KNIGHTATK::HADOU)].Init();
			atk[scastI(KNIGHTATK::HADOU)].hit_ok = true;
			hadouspeed = Getapply(30);
		}
	}
	specialfream = 0;*/
}


void Knight::Thu_Hadouken(float elapsed_time)
{
	/*YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 0.1f,0.1f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 1, 15, 15, 1, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	if (atk[scastI(KNIGHTATK::ONE)].hit_ok)
	{
		atk[scastI(KNIGHTATK::ONE)].damege = 0;
		if (!atk[scastI(KNIGHTATK::HADOU)].hit_ok)
		{
			hadou.x = pos.x + Getapply(100);
			hadou.y = pos.y;
			atk[scastI(KNIGHTATK::THU_HADOU)].Init();
			atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok = true;
			hadouspeed = Getapply(50);
		}
	}
	specialfream = 0;*/
}



void Knight::Kyo_Hadouken(float elapsed_time)
{
	/*YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 0.1f,0.1f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 1, 15, 15, 1, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	if (atk[scastI(KNIGHTATK::ONE)].hit_ok)
	{
		atk[scastI(KNIGHTATK::ONE)].damege = 0;
		hadou.x = pos.x + Getapply(100);
		hadou.y = pos.y;
		atk[scastI(KNIGHTATK::KYO_HADOU)].Init();
		atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok = true;
		hadouspeed = Getapply(60);
	}
	specialfream = 0;*/
}




void Knight::HadouUpdate(float elapsed_time)
{
	/*if (!atk[scastI(KNIGHTATK::HADOU)].hit_ok)
	{
		return;
	}

	YR_Vector3 cent{ hadou.x,hadou.y };
	YR_Vector3 range{ 50.0f,50.0f };

	atk[scastI(KNIGHTATK::HADOU)].UpdateMissile(
		cent, range, 60, 15, 20, YR_Vector3(Getapply(5.0f), 20.0f), AttackBox::MIDDLE,elapsed_time);
	hadou.x += hadouspeed;*/
}

void Knight::Thu_HadouUpdate(float elapsed_time)
{
	/*if (!atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok)
	{
		return;
	}

	YR_Vector3 cent{ hadou.x,hadou.y };
	YR_Vector3 range{ 50.0f,50.0f };

	atk[scastI(KNIGHTATK::THU_HADOU)].UpdateMissile(
		cent, range, 60, 20, 5, YR_Vector3(Getapply(5.0f), 0.0f), AttackBox::MIDDLE,elapsed_time);
	hadou.x += hadouspeed;*/
}


void Knight::Kyo_HadouUpdate(float elapsed_time)
{
	/*if (!atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok)
	{
		return;
	}

	YR_Vector3 cent{ hadou.x,hadou.y };
	YR_Vector3 range{ 150.0f,150.0f };

	atk[scastI(KNIGHTATK::KYO_HADOU)].UpdateMissile(
		cent, range, 60, 30, 30, YR_Vector3(Getapply(20.0f), 30.0f), AttackBox::MIDDLE, 0,elapsed_time);
	hadou.x += hadouspeed;*/
}





void Knight::Panish_N(float elapsed_time)
{
	/*if (atk[scastI(RYUATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(RYUATK::ONE)].knockback;
		atk[scastI(RYUATK::ONE)].knockback = 0.0f;;
		if (!ground)
		{
			speed_Y.Set(60.0f);
		}
	}*/
	/*YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 1.0f,1.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 25, 5, 0, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);

	if (atk[scastI(KNIGHTATK::ONE)].fream == 0)
	{
		FastSet(pos);
		pos.x += Getapply(400.0f);
		pos.y -= 500.0f;
	}
	pos.y -= gravity * 0.75f;
	specialfream = 0;*/
}



void Knight::Panish_H(float elapsed_time)
{
	/*YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 1.0f,1.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 35, 5, 0, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);

	if (atk[scastI(KNIGHTATK::ONE)].fream == 0)
	{
		FastSet(pos);
		pos.x = tracking.rival_Pos.x - Getapply(150.0f);
		pos.y = tracking.rival_Pos.y - 400.0f;
	}

	pos.y -= gravity * 0.75f;

	specialfream = 0;*/
}


void Knight::ExtendATK(float elapsed_time)
{
	/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y - 50 };
	YR_Vector3 range{ 50.0f,200.0f };
	if (ground)
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 3, 80, 10, 50, YR_Vector3(Getapply(30.0f), 30.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	}
	else
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 3, 80, 10, 50, YR_Vector3(Getapply(30.0f), 30.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	}
	if (atk[scastI(KNIGHTATK::ONE)].timer > 0)
	{
		for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			hit[i].guard = HitBox::INVINCIBLE;
		}
	}

	specialfream = 0;*/
}



void Knight::SpecialAttack(float elapsed_time)
{
	if (later > -1 && later < target_max)
	{
		return;
	}

	if (fream < target_max)
	{
		fream -= elapsed_time;

		YR_Vector3	eye = YRCamera.GetEye();
		YR_Vector3	focus = YRCamera.GetFocus();
		YRCamera.SetFov(50.0f * 0.01745f);
		if (fream > 1.5f)
		{
			focus = pos;
			eye.x = focus.x;
			eye.y = focus.y - 2.5f;
			eye.z = focus.z - 10.0f;
			focus.y += 2.5f;
		}
		else if (fream > 1.0f)
		{
			focus = pos;
			eye.x = pos.x - Getapply(7.0f);
			eye.y = focus.y - 2.0f;
			eye.z = pos.z - 7.0f;
			ChangeFace(FaceAnim::YARUKI);
		}
		else if (fream > 0.2f)
		{
			focus = pos;
			focus.x + Getapply(2.0f);
			focus.y - 2.0f;
			eye.x = focus.x + Getapply(9.0f);
			eye.y = focus.y + 6.0f;
			eye.z = focus.z - 6.0f;
		}
		else
		{
			//カメラを徐々にメインに戻す
			YRCamera.RequestCamera(Camera::Request::WEAKEN, now_player);
		}
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
	}


	if (fream < 0.0f)
	{
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft);
		fream = non_target;
		anim->NodeChange(model_motion.special_R_t);
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
				if (!ground)
				{
					speed_Y.Set(60.0f);
				}
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
			attack_list[now_at_list].now_attack_num = 0;
			later = attack_list[now_at_list].later;
			anim_ccodinate = ac_attack[scastI(attack_state)].later;
			anim->NodeChange(model_motion.special_R_l);
		}
	}

	specialfream = 0;
}


//
//--------------------------------------------------------------------------------------------