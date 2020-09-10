#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"
#include <algorithm>


Knight::~Knight()
{

}

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
#pragma region HITBOXINIT
	//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 1.0f);
	hit[scastI(KNIGHTHIT::BODY)].Init(pos,YR_Vector3(0.0f,0.0f), YR_Vector3(2.0f, 2.9f));
	//HitSize[scastI(KNIGHTHIT::BODY)] = hit[scastI(KNIGHTHIT::BODY)].size;
	//Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(0.0f, 0.2f);
	hit[scastI(KNIGHTHIT::LEG)].Init(pos , YR_Vector3(0.0f, 0.0f), YR_Vector3(1.4f, 0.8f));
	//HitSize[scastI(KNIGHTHIT::LEG)] = hit[scastI(KNIGHTHIT::LEG)].size;
#pragma endregion

	hadouspeed = 0.0f;
	moveflag = false;
	attack = FALSE;
	attack_state = AttackState::NONE;
	act_state = ActState::NONE;
	later = non_target;
	hadou = { pos.x + Getapply(100),pos.y };

	eye_offset = { 0.0f,0.0f };
	mouse_offset = { 0.0f,0.0f };

	face_anim = FaceAnim::NORMAL;
	face_wink_time = 0.0f;
	wink_state = Wink_State::FIRST;
}


void Knight::LoadData(int color_number)
{
	AttackLoad();

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
			main = std::make_shared<Model>("./Data/FBX/Knight/knight_main.fbx");
		}
	}

	//------------------------------------------//
	//		アニメーションボーンデータ読み込み		//
	//------------------------------------------//
	if (wait == nullptr)
	{
		wait = std::make_shared<Model>("./Data/FBX/Knight/knight_wait.fbx");
	}
	if (special_R_f == nullptr)
	{
		special_R_f = std::make_shared<Model>("./Data/FBX/Knight/knight_special_R_f.fbx");
	}



	//------------------------------------------//
	//			アニメーション設定				//
	//------------------------------------------//
	if (anim == nullptr)
	{
		anim = std::make_unique<ModelAnim>(main);
		anim->PlayAnimation(0, true);
		anim->NodeChange(wait);
	}
}

bool Knight::AttackLoad()
{
	//AttackStateの順に生成する
	attack_list.resize(scastI(AttackState::ATTACK_END));

	attack_list[scastI(AttackState::NONE)].attack_name = AttackState::NONE;
	attack_list[scastI(AttackState::JAKU)].attack_name = AttackState::JAKU;
	attack_list[scastI(AttackState::JAKU)].attack_max = 1;
	attack_list[scastI(AttackState::JAKU)].later = 0.2f;
	attack_list[scastI(AttackState::JAKU)].attack_single.resize(attack_list[scastI(AttackState::JAKU)].attack_max);
	for (int i = 0; i < attack_list[scastI(AttackState::JAKU)].attack_max; i++)
	{
		attack_list[scastI(AttackState::JAKU)].attack_single[i].fream = 0.1f;
		attack_list[scastI(AttackState::JAKU)].attack_single[i].quantity = 1;
		attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter.resize(attack_list[scastI(AttackState::JAKU)].attack_single[i].quantity);
		for (int v = 0; v < attack_list[scastI(AttackState::JAKU)].attack_single[i].quantity; v++)
		{
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].damege = 5;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].distance.x = 1.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].distance.y = 2.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].gaugeout = true;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].HB_timer = 1.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].hitback.x = Getapply(1.0f);
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].hitback.y = 1.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].knockback = 1.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].size.x = 5.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].size.y = 5.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].stealtimer = 0.0f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].timer = 0.15f;
			attack_list[scastI(AttackState::JAKU)].attack_single[i].parameter[v].type = AttackBox::MIDDLE;
		}
	}
	//attack_list.push_back(AttackList());
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_name = AttackState::SPECIAL_ATTACK;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_max = 1;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].later = 1.0f;
	attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single.resize(attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_max);
	for (int i = 0; i < attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_max; i++)
	{
		attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].fream = 2.0f;
		attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].quantity = 1;
		attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter.resize(attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].quantity);
		for (int v = 0; v < attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].quantity; v++)
		{
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].damege = 5;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].distance.x = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].distance.y = 2.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].gaugeout = true;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].HB_timer = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].hitback.x = Getapply(1.0f);
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].hitback.y = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].knockback = 1.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].size.x = 5.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].size.y = 5.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].stealtimer = 0.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].timer = 2.0f;
			attack_list[scastI(AttackState::SPECIAL_ATTACK)].attack_single[i].parameter[v].type = AttackBox::MIDDLE;
		}
	}
	return true;
}

bool Knight::AttackWrite()
{
	//AttackStateの順に保存する
	return true;
}

void Knight::Uninit()
{
	
}
void Knight::Update(float decision, float elapsed_time)
{
	finish = false;
	DamageCheck();
	WaitAnimSet();
	HadouUpdate(elapsed_time);
	Thu_HadouUpdate(elapsed_time);
	Kyo_HadouUpdate(elapsed_time);
	if (pos.y >= POS_Y)
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
		FallUpdate();
		break;
	case ActState::KNOCK:
		//攻撃を受けてのけぞる
		KnockUpdate();
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
			if (pos.y < POS_Y)
			{
				//重力を付与する
				pos.y += gravity;
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
			Guard(decision);
			GuardBack();
			GuardAnimSet();

			//しゃがみ、ガードのどちらでもない時
			if (act_state != ActState::SQUAT && act_state != ActState::GUARD)
			{
				Step();
				if (step)
				{
					//ステップ中は処理しない
				}
				else
				{
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

						//-------------------------------------------------------------------
						// *概要*
						//・先にコマンドの判定を取り、成立している場合対応した攻撃をstateに入力する
						//・コマンドは二種類のみ
						//--------------------------------------------------------------------


						//弱攻撃
						if (pad->x_input[scastI(PAD::X)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//コマンド判定
							int command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
							if (command == CommandList::RHURF)
							{
								//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//描画をセット
								attack_state = AttackState::JAKU_RHURF;
								//pos.x += Getapply(50.0f);
								specialfream = 0;
							}
							else if (command == CommandList::LHURF)
							{
								//描画をセット

								attack_state = AttackState::JAKU_LHURF;
								specialfream = 0;
							}
							else
							{
								//描画をセット

								attack_state = AttackState::JAKU;
								fream = attack_list[scastI(attack_state)].attack_single[0].fream;
								//motion.MeshSet(jaku_r_f);
								//motion.AnimReset();
								anim_ccodinate = attack_list[scastI(attack_state)].attack_single[0].fream * 100.0f;
								//anim->NodeChange(wait);
								//attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft);
							}
						}

						//下段中攻撃
						if (pad->x_input[scastI(PAD::Y)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//コマンド判定
							int command = pad->com_list.CommandCheack(scastI(PAD::Y), pad->input_history.data(), rightOrleft);
							if (command == CommandList::RHURF)
							{
								//描画をセット

								attack_state = AttackState::THU_RHURF;
								specialfream = 0;
							}
							else if (command == CommandList::LHURF)
							{
								//描画をセット

								attack_state = AttackState::THU_LHURF;
								specialfream = 0;
							}
							else
							{
								//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//描画をセット

								attack_state = AttackState::D_THU;
								//pos.x += Getapply(50.0f);
								ChangeFace(FaceAnim::Damage);
							}
						}

						//上段強攻撃
						if (pad->x_input[scastI(PAD::B)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//コマンド判定
							int command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
							if (command == CommandList::LHURF)
							{
								if (power > 0)
								{
									//描画をセット

									FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
									attack_state = AttackState::KYO_LHURF;
									//エフェクトセット

									pos.x = tracking.rival_Pos.x - Getapply(100.0f);
									pos.y = tracking.rival_Pos.y - 100.0f;
									power--;
								}
								else
								{
									//描画をセット

									attack_state = AttackState::THU_LHURF;
									specialfream = 0;
								}
							}
							else if (command == CommandList::RHURF)
							{
								if (power > 0)
								{
									//エフェクトセット
									//描画をセット

									attack_state = AttackState::KYO_RHURF;
									specialfream = 0;
									power--;
								}
								else
								{
									//描画をセット

									attack_state = AttackState::THU_RHURF;
									specialfream = 0;
								}
							}
							else
							{
								//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//描画をセット

								//attack_state = AttackState::U_KYO;
								//pos.x += Getapply(50.0f);
								pad->que.back().timer = 0;
								attack = TRUE;
								later = non_target;
								moveflag = false;
								act_state = ActState::ATTACK;
								if (ground)
								{
									//描画をセット
								}
								else
								{
									//描画をセット
								}
								attack_state = AttackState::SPECIAL_ATTACK;
								fream = attack_list[scastI(attack_state)].attack_single[0].fream;
								YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
								production_time = 0.0f;
								anim->PlayAnimation(0, true);
								anim->NodeChange(special_R_f);
								anim_ccodinate = 1.0f;
							}
						}

						//投げ
						if (pad->x_input[scastI(PAD::A)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//描画をセット

							attack_state = AttackState::STEAL;
						}




					}
					else
					{
						//空中弱
						if (pad->x_input[scastI(PAD::X)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//描画をセット

							attack_state = AttackState::JAKU;

							//pad->com_list.Reset();
						}

						//空中中
						if (pad->x_input[static_cast<int>(PAD::Y)] == 1)
						{
							pad->que.back().timer = 0;
							FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//描画をセット

							attack_state = AttackState::THU;
							pos.x += Getapply(50.0f);
							specialfream = 0;
						}

						//空中強
						if (pad->x_input[static_cast<int>(PAD::B)] == 1)
						{
							pad->que.back().timer = 0;
							FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//描画をセット

							attack_state = AttackState::U_KYO;
							pos.x += Getapply(50.0f);
							specialfream = 0;
							break;
						}
					}

					//無敵攻撃
					if (pad->x_input[scastI(PAD::L_TRIGGER)] == 1)
					{
						pad->que.back().timer = 0;
						attack = TRUE;
						moveflag = false;
						speed.x = Getapply(5.0f);
						speed_Y.Set(70.0f);
						act_state = ActState::ATTACK;
						//描画をセット

						attack_state = AttackState::EXTENDATK;
					}




					//ホーミングダッシュ
					if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
					{
						if (trackgauge > 0)
						{
							pad->que.back().timer = 0;
							attack = true;
							tracking.Init();
							act_state = ActState::ATTACK;
							//描画をセット

							attack_state = AttackState::TRACK_DASH;
							moveflag = false;
							pos.y -= 50.0f;
							speed.x = 0.0f;
							speed.y = 0.0f;
							hightrigger = false;
							jumpflag = false;
							max_jump_flag = false;
							speed_Y.Set(0.0f);
							trackgauge--;
						}
					}

					Jump();
				}
				NoneChange();
			}
			//しゃがみ攻撃
			if (act_state == ActState::SQUAT)
			{
				if (pad->x_input[scastI(PAD::X)] == 1)
				{
					pad->que.back().timer = 0;
					attack = TRUE;
					moveflag = false;
					act_state = ActState::ATTACK;
					//描画をセット

					attack_state = AttackState::D_JAKU;
					//pad->com_list.Reset();
				}
				if (pad->x_input[scastI(PAD::Y)] == 1)
				{
					pad->que.back().timer = 0;
					attack = TRUE;
					moveflag = false;
					act_state = ActState::ATTACK;
					//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
					//描画をセット

					attack_state = AttackState::D_THU;
					pos.x += Getapply(50.0f);
				}
				if (pad->x_input[static_cast<int>(PAD::B)] == 1)
				{
					pad->que.back().timer = 0;
					//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
					attack = TRUE;
					moveflag = false;
					act_state = ActState::ATTACK;
					//描画をセット

					attack_state = AttackState::KYO;
					pos.x += Getapply(50.0f);
					specialfream = 0;
				}
			}
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

	JumpUpdate();

	pos.x += speed.x * elapsed_time;
	pos.y -= speed_Y.Update(elapsed_time);

	if (attack_state != AttackState::D_THU && act_state != ActState::DOWN && act_state != ActState::SQUAT && attack_state != AttackState::D_JAKU)
	{
		//hit[scastI(KNIGHTHIT::BODY)].size = HitSize[scastI(KNIGHTHIT::BODY)];
		//hit[scastI(KNIGHTHIT::LEG)].size = HitSize[scastI(KNIGHTHIT::LEG)];
	}
#ifdef USE_IMGUI
	{
		std::string now_play = std::to_string(now_player);
		now_play += std::string(":RyuHitBox");
		ImGui::Begin(now_play.c_str());
		ImGui::InputFloat("BodyPosX", &hit[scastI(KNIGHTHIT::BODY)].distance.x, 0.1f, 0.1f);
		ImGui::InputFloat("BodyPosY", &hit[scastI(KNIGHTHIT::BODY)].distance.y, 0.1f, 0.1f);
		ImGui::InputFloat("BodySizeX", &hit[scastI(KNIGHTHIT::BODY)].size.x, 0.1f, 0.1f);
		ImGui::InputFloat("BodySizeY", &hit[scastI(KNIGHTHIT::BODY)].size.y, 0.1f, 0.1f);

		ImGui::InputFloat("LegPosX", &hit[scastI(KNIGHTHIT::LEG)].distance.x, 0.1f, 0.1f);
		ImGui::InputFloat("LegPosY", &hit[scastI(KNIGHTHIT::LEG)].distance.y, 0.1f, 0.1f);
		ImGui::InputFloat("LegSizeX", &hit[scastI(KNIGHTHIT::LEG)].size.x, 0.1f, 0.1f);
		ImGui::InputFloat("LegSizeY", &hit[scastI(KNIGHTHIT::LEG)].size.y, 0.1f, 0.1f);
		/*ImGui::SliderFloat("eye_offset.x", &eye_offset.x, 0.0f, 2048.0f);
		ImGui::SliderFloat("eye_offset.y", &eye_offset.y, 0.0f, 2048.0f);*/
		ImGui::InputFloat("eye_offset.x", &eye_offset.x, 0.01f, 0.01f);
		ImGui::InputFloat("eye_offset.y", &eye_offset.y, 0.01f, 0.01f);
		ImGui::InputFloat("mouse_offset.x", &mouse_offset.x, 0.01f, 0.01f);
		ImGui::InputFloat("mouse_offset.y", &mouse_offset.y, 0.01f, 0.01f);
		ImGui::Text("player.y:%f", pos.y);
		ImGui::Text("player.x:%f", pos.x);

		//ImGui::ColorEdit3("clear color", (float*)&ImColor(114, 144, 154));
		//if (ImGui::Button("Test Window")) show_test_window ^= 1;
		//if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
#endif // USE_IMGUI
	hit[scastI(KNIGHTHIT::BODY)].Update(pos,elapsed_time);
	hit[scastI(KNIGHTHIT::LEG)].Update(pos, elapsed_time);

	EndAttackErase();			//攻撃判定の消去

	FaceAnimation(elapsed_time);
}



void Knight::Attack(float decision, float elapsed_time)
{
	AttackUpdate(elapsed_time);	//攻撃判定の更新
	if (ground)
	{
		if (speed.x > 0)
		{
			speed.x--;
			if (speed.x < 0)
			{
				speed.x = 0;
			}
		}
		if (speed.x < 0)
		{
			speed.x++;
			if (speed.x > 0)
			{
				speed.x = 0;
			}
		}
	}
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		hit[i].guard = HitBox::NOGUARD;
	}
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
		if (later > 0 && later < target_max)
		{
			//中攻撃
			if (pad->x_input[static_cast<int>(PAD::X)] == 1)
			{
				//pad->que.back().timer = 0;
				//attack = TRUE;
				//later = non_target;
				//moveflag = false;
				//act_state = ActState::ATTACK;
				//if (ground)
				//{
				//	//描画をセット
				//}
				//else
				//{
				//	//描画をセット
				//}
				//attack_state = AttackState::SPECIAL_ATTACK;
				//fream = attack_list[scastI(attack_state)].attack_single[0].fream;
				//YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
				//production_time = 0.0f;

				specialfream = 2;
			}
			//下段中攻撃(コンボルート。弱から下段中攻撃が繋がる)
			if (ground)
			{
				if (pad->x_input[static_cast<int>(PAD::Y)] == 1)
				{
					for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
					{
						if (atk[atknum].knock_start)
						{
							//pad->que.back().timer = 0;
							//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							//attack = TRUE;
							//atk[scastI(KNIGHTATK::ONE)].start = FALSE;
							//later = -1;
							//for (int i = 0; i < scastI(KNIGHTATK::END); i++)
							//{
							//	atk[i].Init();
							//}
							//moveflag = false;
							//atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
							//act_state = ActState::ATTACK;
							////描画をセット

							//attack_state = AttackState::D_THU;
							//pos.x += Getapply(50.0f);
							//specialfream = 3;
							break;
						}
					}
				}
			}
		}
		break;
	case AttackState::THU:
		//中攻撃
		Thu(elapsed_time);
		if (later > -1)
		{
			//強攻撃
			if (pad->x_input[static_cast<int>(PAD::X)] == 1)
			{
				if (ground)
				{
					//pad->que.back().timer = 0;
					//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
					//attack = TRUE;
					//atk[scastI(KNIGHTATK::ONE)].start = FALSE;
					//later = -1;
					//for (int i = 0; i < scastI(KNIGHTATK::END); i++)
					//{
					//	atk[i].Init();
					//}
					//moveflag = false;
					//atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
					////描画をセット
					//act_state = ActState::ATTACK;
					//attack_state = AttackState::KYO;
					//pos.x += Getapply(50.0f);
					//specialfream = 4;
				}
				else
				{
					//pad->que.back().timer = 0;
					//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
					//attack = TRUE;
					//atk[scastI(KNIGHTATK::ONE)].start = FALSE;
					//later = -1;
					//for (int i = 0; i < scastI(KNIGHTATK::END); i++)
					//{
					//	atk[i].Init();
					//}
					//moveflag = false;
					//atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
					////描画をセット

					//attack_state = AttackState::U_KYO;
					//act_state = ActState::ATTACK;
					//pos.x += Getapply(50.0f);
					//specialfream = 4;
					break;
				}
			}
		}
		break;
	case AttackState::KYO:
		//強攻撃
		Kyo(specialfream, elapsed_time);
		if (later > -1)
		{
			//ホーミングダッシュ
			if (pad->x_input[static_cast<int>(PAD::X)] == 1)
			{
				for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
				{
					if (atk[atknum].knock_start)
					{
						/*pad->que.back().timer = 0;
						attack = TRUE;
						atk[scastI(KNIGHTATK::ONE)].start = FALSE;
						later = -1;
						for (int i = 0; i < scastI(KNIGHTATK::END); i++)
						{
							atk[i].Init();
						}
						moveflag = false;
						atk[scastI(KNIGHTATK::ONE)].fin = FALSE;*/
						if (trackgauge > 0)
						{
							tracking.Init();
							//描画をセット

							attack_state = AttackState::TRACK_DASH;
							act_state = ActState::ATTACK;
							pos.y -= 50.0f;
							speed.y = 0.0f;
							speed.x = 0.0f;
							hightrigger = false;
							jumpflag = false;
							max_jump_flag = false;
							speed_Y.Set(0.0f);
							trackgauge--;
						}
						break;
					}
				}
			}
		}
		break;
	case AttackState::D_THU:
		//下段中攻撃
		D_Thu(specialfream, elapsed_time);
		if (later > -1)
		{
			//上段強攻撃
			if (pad->x_input[static_cast<int>(PAD::B)] == 1)
			{
				for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
				{
					if (atk[atknum].knock_start)
					{
						//pad->que.back().timer = 0;
						//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
						//attack = TRUE;
						//atk[scastI(KNIGHTATK::ONE)].start = FALSE;
						//later = -1;
						//for (int i = 0; i < scastI(KNIGHTATK::END); i++)
						//{
						//	atk[i].Init();
						//}
						//moveflag = false;
						//atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
						////描画をセット

						//attack_state = AttackState::U_KYO;
						//act_state = ActState::ATTACK;
						//pos.x += Getapply(50.0f);
						//specialfream = 4;
						break;
					}
				}
			}
		}
		break;
	case AttackState::U_KYO:
		//上段強攻撃
		U_Kyo(specialfream, elapsed_time);
		break;
	case AttackState::D_JAKU:
		//下段弱攻撃
		D_Jaku(elapsed_time);
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
	default:
		break;
	}


	//攻撃判定の終了フラグが出たら後隙を決定し減らす処理を行う
	/*for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
	{
		if (atk[atknum].fin)
		{
			if (later == -1)
			{
				later = atk[atknum].later;
			}
			later--;
		}
	}*/

	//後隙中にキャンセルして技を行えるかチェックする
	if (later > 0 && later < target_max)
	{
		later -= elapsed_time;
		CancelList();
	}

	//後隙消費後元のステートに戻す
	if (later < 0.0f)
	{
		finish = true;
		later = non_target;
		attack = FALSE;
		//atk[scastI(RYUATK::ONE)].start = FALSE;
		if (ground)
		{
			if (act_state != ActState::WAIT)
			{
				act_state = ActState::NONE;
				attack_state = AttackState::NONE;
				anim->PlayAnimation(0, true);
				anim->NodeChange(wait);
			}
		}
		else
		{
			act_state = ActState::JUMP;
			attack_state = AttackState::NONE;
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
		int command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
		if (command == CommandList::LHURF)
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
		int command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
		if (command == CommandList::LHURF)
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
		else if (command == CommandList::RHURF)
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
			if (hit[i].guard == HitBox::INVINCIBLE)
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
		material_color = { 0.0f,0.0f,1.0f,1.0f };
	}
	if (later < target_max && later>0.0f)
	{
		material_color = { 1.0f,0.0f,0.0f,1.0f };
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

	if (static_cast<int>(YRCamera.camera_state) == now_player)
	{
		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, mouse_offset, material_color);
	}
	if (YRCamera.camera_state == Camera::CAMERA_STATE::MAIN)
	{
		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, eye_offset, mouse_offset, material_color);
	}
	if (YRCamera.GetRequest() == Camera::Request::WEAKEN)
	{
		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, mouse_offset, material_color);
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



void Knight::DrawDEBUG(
	YRShader* geoshader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		hit[i].Draw(geoshader, view, projection, light_direction, light_color, ambient_color);
	}

	if (attack)
	{
		if (!atk.empty())
		{
			for (auto& a : atk)
			{
				if (a.attack)
				{
					if (a.hit_ok)
					{
						a.Draw(geoshader, view, projection, light_direction, light_color, ambient_color);
					}
				}
			}
		}
	}
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
			if (speed.x < -stepspeed / 2)
			{
				for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
				{
					hit[i].guard = HitBox::INVINCIBLE;
				}
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
			if (speed.x > stepspeed / 2)
			{
				for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
				{
					hit[i].guard = HitBox::INVINCIBLE;
				}
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
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				//描画をセット

				speed.x = -backstepS;
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


	//どちらも押されているor押されていないなら元に戻す
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	{
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
		speed.x = 0.0f;
	}
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
		speed.x = 0.0f;
	}


}

void Knight::MoveAnimSet()
{
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		moveflag = true;
		//描画をセット

		if (rightOrleft > 0)
		{
			//描画をセット
		}

	}
	if (pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		moveflag = true;
		//描画をセット

		if (rightOrleft < 0)
		{
			//描画をセット

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
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			speed.y = 60.0f;
			hightrigger = true;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット

			act_state = ActState::JUMP;
			jumpflag = true;
		}
		if (pad->x_input[scastI(PAD::HIGH_UP_R)] == 1)
		{
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = true;
			speed.y = 60.0f;
			if (speed.x < dashspeed)
			{
				speed.x = dashspeed;
			}
			max_jump_flag = false;
			moveflag = false;
			//描画をセット

			act_state = ActState::JUMP;
			jumpflag = true;
		}

		if (pad->x_input[scastI(PAD::HIGH_UP_L)] == 1)
		{
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = true;
			speed.y = 60.0f;
			if (speed.x > -dashspeed)
			{
				speed.x = -dashspeed;
			}
			max_jump_flag = false;
			moveflag = false;
			//描画をセット

			act_state = ActState::JUMP;
			jumpflag = true;
		}

		if (pad->x_input[scastI(PAD::STICK_U)] == 1)
		{
			later = -1;
			attack = FALSE;
			/*for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}*/
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = false;
			speed.y = 40.0f;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット

			act_state = ActState::JUMP;
			jumpflag = true;
		}
		if (pad->x_input[scastI(PAD::STICK_RUp)] == 1)
		{
			later = -1;
			attack = FALSE;
			/*for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}*/
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = false;
			speed.y = 40.0f;
			if (speed.x < walkspeed)
			{
				speed.x = walkspeed;
			}
			max_jump_flag = false;
			moveflag = false;
			//描画をセット

			act_state = ActState::JUMP;
			jumpflag = true;
		}
		if (pad->x_input[scastI(PAD::STICK_LUp)] == 1)
		{
			later = -1;
			attack = FALSE;
			/*for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}*/
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = false;
			speed.y = 40.0f;
			if (speed.x > -walkspeed)
			{
				speed.x = -walkspeed;
			}
			max_jump_flag = false;
			moveflag = false;
			//描画をセット

			act_state = ActState::JUMP;
			jumpflag = true;
		}
	}
	else
	{
		if (jumpcount > 0)
		{
			if (pad->x_input[scastI(PAD::STICK_U)] == 1)
			{
				later = -1;
				attack = FALSE;
				/*for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}*/
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				jumpcount = 0;
				speed.y = 40.0f;
				moveflag = false;
				max_jump_flag = false;
				act_state = ActState::JUMP;
				//描画をセット

				jumpflag = true;
			}
			if (pad->x_input[scastI(PAD::STICK_RUp)] == 1)
			{
				later = -1;
				attack = FALSE;
				/*for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}*/
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				jumpcount--;
				speed.y = 40.0f;
				if (speed.x < walkspeed)
				{
					speed.x = walkspeed;
				}
				max_jump_flag = false;
				moveflag = false;
				//描画をセット

				act_state = ActState::JUMP;
				jumpflag = true;
			}
			if (pad->x_input[scastI(PAD::STICK_LUp)] == 1)
			{
				later = -1;
				attack = FALSE;
				/*for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}*/
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				jumpcount--;
				speed.y = 40.0f;
				if (speed.x > -walkspeed)
				{
					speed.x = -walkspeed;
				}
				max_jump_flag = false;
				moveflag = false;
				//描画をセット

				act_state = ActState::JUMP;
				jumpflag = true;
			}
		}
	}


}

void Knight::JumpUpdate()
{
	if (jumpcount < 2 && jumpflag)
	{
		if (speed_Y.speedY == 0.0f)
		{
			pos.y -= speed.y;
		}
		if (!max_jump_flag)
		{
			if (hightrigger)
			{
				speed.y += 20.0f;
				if (speed.y > high_jump_max)
				{
					max_jump_flag = true;
				}
			}
			else
			{
				speed.y += 10.0f;
				if (speed.y > jump_max)
				{
					max_jump_flag = true;
				}
			}
		}
	}

	if (max_jump_flag)
	{
		speed.y -= 2.0f;
		if (hightrigger)
		{
			if (speed.y - gravity < 0)
			{
				if (!attack)
				{
					//描画をセット

				}
				hightrigger = false;
			}
		}
	}
	if (pos.y > POS_Y)
	{
		if (max_jump_flag)
		{
			jumpcount = 2;
			max_jump_flag = false;
			hightrigger = false;
			speed.y = 0.0f;
			act_state = ActState::NONE;
			pos.y = POS_Y;
			jumpflag = false;
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
			/*for (int j = 0; j < scastI(KNIGHTATK::END); j++)
			{
				atk[j].Init();
			}*/
			AllAttackClear();
			act_state = ActState::KNOCK;
			attack_state = AttackState::NONE;
		}
		if (hit[i].steal)
		{
			for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
			{
				hit[i].guard = HitBox::NOGUARD;
			}
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
			pad->dash_trigger = false;
			pad->high_trigger = false;
			hightrigger = false;
			/*for (int j = 0; j < scastI(KNIGHTATK::END); j++)
			{
				atk[j].Init();
			}*/
			AllAttackClear();
			hit[i].steal = false;
			steal_escape = hit[i].steal_timer;
			hit[i].steal_timer = 0.0f;
			act_state = ActState::STATENONE;
			attack_state = AttackState::NONE;
		}
	}
}

void Knight::KnockUpdate()
{
	bool pflag = false;
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].hitback.x != 0)
		{
			pos.x += hit[i].hitback.x;
			pflag = true;
		}
		if (hit[i].hitback.y != 0)
		{
			pos.y -= hit[i].hitback.y;
			pflag = true;
			if (pos.y > POS_Y)
			{
				pos.y = POS_Y;
			}
		}
		if (pflag)
		{
			break;
		}
	}
	knocktimer--;
	if (knocktimer < 0)
	{
		combo_count = 0;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
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
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		hit[i].guard = HitBox::NOGUARD;
	}
	if (step)
	{
		return;
	}

	if (rightOrleft > 0)
	{
		if (pad->x_input[scastI(PAD::STICK_LDown)] > 0)
		{
			if (!ground)
			{
				for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
				{
					hit[i].guard = HitBox::ALL;
				}
			}
			else
			{
				for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
				{
					hit[i].guard = HitBox::DOWN;
				}
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (!ground)
				{
					for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
					{
						hit[i].guard = HitBox::ALL;
					}
				}
				else
				{
					for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
					{
						hit[i].guard = HitBox::MIDDLE;
					}
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
				for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
				{
					hit[i].guard = HitBox::ALL;
				}
			}
			else
			{
				for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
				{
					hit[i].guard = HitBox::DOWN;
				}
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (!ground)
				{
					for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
					{
						hit[i].guard = HitBox::ALL;
					}
				}
				else
				{
					for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
					{
						hit[i].guard = HitBox::MIDDLE;
					}
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
				for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
				{
					hit[i].guard = HitBox::ALL;
				}
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

void Knight::GuardBack()
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
		knocktimer--;
		if (knocktimer == 0)
		{
			knocktimer--;
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
	if (act_state == ActState::GUARD || act_state == ActState::BACK || act_state == ActState::AIR_B || act_state == ActState::AIR_F)
	{
		return;
	}
	if (!ground)
	{
		return;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
	{
		//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
		hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
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


void Knight::FallUpdate()
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
		if (pos.y < POS_Y)
		{
			pos.y += speed.y;
			speed.y += 1.0f;
		}
		if (pad->x_input[scastI(PAD::X)] == 1 || pad->x_input[scastI(PAD::Y)] == 1 ||
			pad->x_input[scastI(PAD::B)] == 1 || pad->x_input[scastI(PAD::A)] == 1)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			act_state = ActState::PASSIVE;
			//描画をセット

			for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
			{
				hit[i].guard = HitBox::INVINCIBLE;
			}
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
	hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(110.0f, 62.0f);
	//Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(110.0f, 149.0f);
	hit[scastI(KNIGHTHIT::LEG)].size = YR_Vector3(39.0f, 47.0f);
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

			for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
			{
				hit[i].guard = HitBox::INVINCIBLE;
			}
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
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		hit[i].guard = HitBox::INVINCIBLE;
	}

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
		for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			hit[i].guard = HitBox::NOGUARD;
		}
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
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	YR_Vector3 range{ 50.0f,50.0f };
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
	static int state = 0;
	if (stop_pos == YR_Vector3(0.0f, 0.0f))
	{
		stop_pos = pos;
	}

	switch (state)
	{
	case 0:
		pos.x += 10;
		pos.y += 10;
		state++;
		break;
	case 1:
		pos.x -= 10;
		pos.y -= 10;
		state++;
		break;
	case 2:
		pos.x -= 10;
		pos.y -= 10;
		state++;
		break;
	case 3:
		pos.x += 10;
		pos.y += 10;
		state = 0;
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

bool Knight::Intro()
{
	//イントロの行動をする
	//カメラもこちらで動かす
	switch (intro_state)
	{
	case Knight::INTRO_KNIGHT::SET:
		YRCamera.SetFocus(pos.GetDXFLOAT3());
		YRCamera.SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -25.0f));
		intro_state = INTRO_KNIGHT::CAMERA_ZOOM;
		break;
	case Knight::INTRO_KNIGHT::CAMERA_ZOOM:
	{
		YR_Vector3 vec, eye, focus;
		focus = YRCamera.GetFocus();
		eye = YRCamera.GetEye();
		vec = focus - eye;
		float len = vec.Length();
		if (len < 0.5f)
		{
			intro_state = INTRO_KNIGHT::FINISH;
			break;
		}
		vec.Normalize();
		eye += vec * 0.1f;
		//eye.y += 0.1f;
		YRCamera.SetEye(eye.GetDXFLOAT3());
	}
		break;
	case Knight::INTRO_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}
	

	return false;
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
		for (auto& a : atk)
		{
			a.fin = true;
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
		}
	}
}


//顔のテクスチャアニメーション
void Knight::FaceAnimation(float elapsed_time)
{
	switch (face_anim)
	{
	case Knight::FaceAnim::NORMAL:
		face_wink_time += elapsed_time;
		if (face_wink_time > static_cast<float>(now_player) * 3.0f)
		{
			face_anim = FaceAnim::WINK;
			face_wink_time = 0.0f;
		}
		break;
	case Knight::FaceAnim::WINK:
		FaceWink(elapsed_time);
		break;
	case Knight::FaceAnim::Damage:
		eye_offset = face_eye_offset[FaceEye_Num::KAOMOJI];
		mouse_offset = face_mouse_offset[FaceMouse_Num::POKAN];
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
		case Knight::FIRST:
			eye_offset = face_eye_offset[FaceEye_Num::WINK1];
			wink_state = Wink_State::SECOND;
			break;
		case Knight::SECOND:
			eye_offset = face_eye_offset[FaceEye_Num::WINK2];
			wink_state = Wink_State::THIRD;
			break;
		case Knight::THIRD:
			eye_offset = face_eye_offset[FaceEye_Num::CLOSE];
			wink_state = Wink_State::FOURTH;
			break;
		case Knight::FOURTH:
			eye_offset = face_eye_offset[FaceEye_Num::WINK2];
			wink_state = Wink_State::FIVE;
			break;
		case Knight::FIVE:
			eye_offset = face_eye_offset[FaceEye_Num::WINK1];
			wink_state = Wink_State::SIX;
			break;
		case Knight::SIX:
			eye_offset = face_eye_offset[FaceEye_Num::NORMAL_EYE];
			wink_state = Wink_State::FIRST;
			face_anim = FaceAnim::NORMAL;
			break;
		default:
			break;
		}
		face_wink_time = 0.0f;
	}
}

void Knight::ChangeFace(FaceAnim anim)
{
	face_anim = anim;
	face_wink_time = 0.0f;
	wink_state = Wink_State::FIRST;
}