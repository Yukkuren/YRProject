#include "../Scene/Scene.h"

//-----------------------------------------------------------------
//			AI(ステートベース)
//-----------------------------------------------------------------
//・簡易的なAIのため、コントローラーの入力を内部的にさせてキャラを動かしている
//-----------------------------------------------------------------

void SceneGame::AIControll(float elapsed_time)
{

	float dis = player2p->pos.Distance(player1p->pos);

	if (player2p->knocktimer > 0.0f&& player2p->knocktimer<target_max)
	{
		//プレイヤーがダメージを受けた
		AI2P.state = AI_Controller::AI_State::KNOCK;
		player2p->pad->Init();
	}
	if (player2p->hp == 0.0f)
	{
		AI2P.state = AI_Controller::AI_State::DEAPTH;
	}
	if (player2p->act_state == ActState::DOWN)
	{
		AI2P.state = AI_Controller::AI_State::DOWN;
		player2p->pad->Init();
	}

	/*if (player1p->act_state == ActState::SLAM)
	{
		player2p->pad->Init();
		AI2P.state = AI_Controller::AI_State::SPECIAL;
	}*/

	if (player2p->steal_escape > 0.0f)
	{
		if (!AI2P.steal_escape)
		{
			int ra = rand() % 10;
			if (ra < 5)
			{
				player2p->pad->x_input[scastI(PAD::RB)] = 1;
			}
			AI2P.steal_escape = true;
		}
	}

	switch (AI2P.state)
	{
	case AI_Controller::AI_State::INIT:
		//最初の設定
		player2p->pad->Init();
		AI2P.state = AI_Controller::AI_State::RAND_SELECT;
		AI2P.timer = 0.0f;
		AI2P.steal_escape = false;
		break;
	case AI_Controller::AI_State::RAND_SELECT:
	{
		//ランダムで行動を設定する

		if (!player2p->ground)
		{
			//空中なら何もしない
			return;
		}

		/*AI2P.state = AI_Controller::AI_State::FOREVER_GUARD;
		return;*/

		if (dis < AI2P.max_dis)
		{
			//プレイヤー同士の距離が一定以下なら攻撃か離れるかガードする
			while (AI2P.state == AI2P.before_state)
			{
				int ra = rand() % 10;

				if (ra < AI2P.attack_probability)
				{
					//Xボタン連打
					AI2P.state = AI_Controller::AI_State::COMBO;
				}
				else if (ra < AI2P.guard_probability)
				{
					//ガードする
					AI2P.state = AI_Controller::AI_State::GUARD;
				}
				else
				{
					//離れる
					AI2P.state = AI_Controller::AI_State::LEAVE;
				}
			}
		}
		else
		{
			while (AI2P.state == AI2P.before_state)
			{
				//一定以下なら近づく又はホーミングダッシュを行う
				int ra = rand() % 10;

				if (ra < AI2P.track_probability)
				{
					//ホーミングダッシュ
					AI2P.state = AI_Controller::AI_State::TRACK_DASH;
				}
				else
				{
					//近づく
					AI2P.state = AI_Controller::AI_State::APPROACH;
				}
			}
		}
		AI2P.before_state = AI2P.state;
	}
		break;
	case AI_Controller::AI_State::APPROACH:
		//近づく

		AI2P.timer += elapsed_time;

		if (player2p->rightOrleft > 0)
		{
			//右向き
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)]++;
		}
		else
		{
			//左向き
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)]++;
		}

		if (dis < AI2P.max_dis)
		{
			//プレイヤー同士の距離が一定以下になったら初期ステートに戻す
			AI2P.state = AI_Controller::AI_State::INIT;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
		}
		if (AI2P.timer > AI2P.timer_max)
		{
			//一定時間になったら行動を初期化
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::LEAVE:
		//離れる

		AI2P.timer += elapsed_time;

		if (player2p->rightOrleft > 0)
		{
			//右向き
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 1;
			player2p->pad->x_input[scastI(PAD::L_DASH)] = 1;
		}
		else
		{
			//左向き
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 1;
			player2p->pad->x_input[scastI(PAD::R_DASH)] = 1;
		}

		if (AI2P.timer > AI2P.timer_max)
		{
			//一定時間になったら行動を初期化
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::R_DASH)] = 0;
			player2p->pad->x_input[scastI(PAD::L_DASH)] = 0;
			AI2P.timer = 0.0f;
			if (player2p->power > 0)
			{
				//クリスタルがあれば超必
				AI2P.state = AI_Controller::AI_State::SPECIAL;
			}
			else
			{
				//なければ通常のステートに
				AI2P.state = AI_Controller::AI_State::INIT;
			}
		}
		break;
	case AI_Controller::AI_State::COMBO:
		//Xボタンを連打する
		AI2P.timer += elapsed_time;

		player2p->pad->x_input[scastI(PAD::X)] = 1;

		if (player2p->attack_state == AttackState::JAKU_KYO)
		{
			if (player2p->later < target_max)
			{
				//Xボタンコンボを空振りもしくはガードされたら終了
				player2p->pad->x_input[scastI(PAD::X)] = 0;
				AI2P.state = AI_Controller::AI_State::INIT;
			}
		}
		if (player2p->attack_state == AttackState::A_KYO)
		{
			if (player2p->later < target_max)
			{
				//Xボタンコンボを空振りもしくはガードされたら終了
				player2p->pad->x_input[scastI(PAD::X)] = 0;
				AI2P.state = AI_Controller::AI_State::INIT;
			}
		}

		if (AI2P.timer > AI2P.timer_max)
		{
			//一定時間になったら行動を初期化
			player2p->pad->x_input[scastI(PAD::X)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::TRACK_DASH:
		//ホーミングダッシュをする
		AI2P.timer += elapsed_time;

		player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 1;

		if (player2p->hit_result == HitResult::HIT)
		{
			//当たってたらXボタン連打させる
			player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 0;
			AI2P.state = AI_Controller::AI_State::COMBO;
			AI2P.timer = 0.0f;
		}

		if (AI2P.timer > AI2P.timer_max)
		{
			//一定時間になったら行動を初期化
			player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::GUARD:
		//ガードしようとする
		AI2P.timer += elapsed_time;

		if (player2p->rightOrleft > 0)
		{
			//右向き
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_RDown)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_LDown)]++;
		}
		else
		{
			//左向き
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_LDown)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_RDown)]++;
		}

		if (AI2P.timer > AI2P.guard_timer_max)
		{
			//一定時間になったら行動を初期化
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_RDown)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_LDown)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::KNOCK:
		if (player2p->knocktimer == non_target)
		{
			player2p->pad->x_input[scastI(PAD::X)] = 1;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::DEAPTH:
		player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
		player2p->pad->x_input[scastI(PAD::X)] = 0;
		player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_RDown)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_LDown)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_U)] = 0;
		break;
	case AI_Controller::AI_State::DOWN:
		player2p->pad->x_input[scastI(PAD::STICK_U)] = 1;
		AI2P.state = AI_Controller::AI_State::INIT;
		break;
	case AI_Controller::AI_State::SPECIAL:
		AI2P.timer += elapsed_time;
		player2p->pad->x_input[scastI(PAD::A)] = 1;
		if (AI2P.timer > 0.5f)
		{
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::FOREVER_GUARD:
		if (player2p->rightOrleft > 0)
		{
			//右向き
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 1;
			//player2p->pad->x_input[scastI(PAD::L_DASH)] = 1;
		}
		else
		{
			//左向き
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 1;
			//player2p->pad->x_input[scastI(PAD::R_DASH)] = 1;
		}
		break;
	default:
		break;
	}
}