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
	//		�A�j���[�V�����{�[���f�[�^�ǂݍ���		//
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
	//			�A�j���[�V�����ݒ�				//
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
	//AttackState�̏��ɐ�������
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
	//AttackState�̏��ɕۑ�����
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
	// *�T�v*
	//�E�U�����󂯂�ȂǑ���s�\�̏�Ԃ̂݃X�e�[�g�ŋ敪�����A
	//�@����ȊO�͑�����󂯕t����̂�CANACT�ŏ������Ă���
	//-----------------------------------------------------
	switch (act_state)
	{
	case ActState::STATENONE:
		//�X�e�[�g��D��ꂽ��ԁB����ɒ͂܂�Ă�
		StateNone();
		break;
	case ActState::WAKE:
		//�_�E������N���オ��
		WakeUp();
		break;
	case ActState::PASSIVE:
		//�󂯐g���Ƃ�
		PassiveUpdate();
		break;
	case ActState::DOWN:
		//�_�E�����
		DownUpdate();
		break;
	case ActState::FALL:
		//�󒆂Ń_�E�����Ă�����
		FallUpdate();
		break;
	case ActState::KNOCK:
		//�U�����󂯂Ă̂�����
		KnockUpdate();
		break;
	case ActState::ATTACK:
		//�U����
		Attack(decision, elapsed_time);
		break;
	default:
		//���̑��ʏ���
		if (ground)
		{
			//�n�ʐݒu��
			pos.y = POS_Y;	//�n�ʂɐݒu���Ă���Ƃ��͍������Œ肷��
			trackgauge = Track_max;	//�z�[�~���O�_�b�V���񐔂�߂�
			if (act_state == ActState::JUMP)
			{
				//�X�e�[�g���W�����v�̎���NONE�ɖ߂�
				act_state = ActState::NONE;
			}
		}
		else
		{
			//�󒆎�
			if (pos.y < POS_Y)
			{
				//�d�͂�t�^����
				pos.y += gravity;
			}
		}

		{
		//�U�����s���Ă��Ȃ��Ƃ��̏���
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
			//���Ⴊ�݂�K�[�h�Ȃǂ��ɔ���
			Squat();
			Guard(decision);
			GuardBack();
			GuardAnimSet();

			//���Ⴊ�݁A�K�[�h�̂ǂ���ł��Ȃ���
			if (act_state != ActState::SQUAT && act_state != ActState::GUARD)
			{
				Step();
				if (step)
				{
					//�X�e�b�v���͏������Ȃ�
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
						// *�T�v*
						//�E��ɃR�}���h�̔�������A�������Ă���ꍇ�Ή������U����state�ɓ��͂���
						//�E�R�}���h�͓��ނ̂�
						//--------------------------------------------------------------------


						//��U��
						if (pad->x_input[scastI(PAD::X)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//�R�}���h����
							int command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
							if (command == CommandList::RHURF)
							{
								//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//�`����Z�b�g
								attack_state = AttackState::JAKU_RHURF;
								//pos.x += Getapply(50.0f);
								specialfream = 0;
							}
							else if (command == CommandList::LHURF)
							{
								//�`����Z�b�g

								attack_state = AttackState::JAKU_LHURF;
								specialfream = 0;
							}
							else
							{
								//�`����Z�b�g

								attack_state = AttackState::JAKU;
								fream = attack_list[scastI(attack_state)].attack_single[0].fream;
								//motion.MeshSet(jaku_r_f);
								//motion.AnimReset();
								anim_ccodinate = attack_list[scastI(attack_state)].attack_single[0].fream * 100.0f;
								//anim->NodeChange(wait);
								//attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft);
							}
						}

						//���i���U��
						if (pad->x_input[scastI(PAD::Y)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//�R�}���h����
							int command = pad->com_list.CommandCheack(scastI(PAD::Y), pad->input_history.data(), rightOrleft);
							if (command == CommandList::RHURF)
							{
								//�`����Z�b�g

								attack_state = AttackState::THU_RHURF;
								specialfream = 0;
							}
							else if (command == CommandList::LHURF)
							{
								//�`����Z�b�g

								attack_state = AttackState::THU_LHURF;
								specialfream = 0;
							}
							else
							{
								//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//�`����Z�b�g

								attack_state = AttackState::D_THU;
								//pos.x += Getapply(50.0f);
								ChangeFace(FaceAnim::Damage);
							}
						}

						//��i���U��
						if (pad->x_input[scastI(PAD::B)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//�R�}���h����
							int command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
							if (command == CommandList::LHURF)
							{
								if (power > 0)
								{
									//�`����Z�b�g

									FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
									attack_state = AttackState::KYO_LHURF;
									//�G�t�F�N�g�Z�b�g

									pos.x = tracking.rival_Pos.x - Getapply(100.0f);
									pos.y = tracking.rival_Pos.y - 100.0f;
									power--;
								}
								else
								{
									//�`����Z�b�g

									attack_state = AttackState::THU_LHURF;
									specialfream = 0;
								}
							}
							else if (command == CommandList::RHURF)
							{
								if (power > 0)
								{
									//�G�t�F�N�g�Z�b�g
									//�`����Z�b�g

									attack_state = AttackState::KYO_RHURF;
									specialfream = 0;
									power--;
								}
								else
								{
									//�`����Z�b�g

									attack_state = AttackState::THU_RHURF;
									specialfream = 0;
								}
							}
							else
							{
								//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//�`����Z�b�g

								//attack_state = AttackState::U_KYO;
								//pos.x += Getapply(50.0f);
								pad->que.back().timer = 0;
								attack = TRUE;
								later = non_target;
								moveflag = false;
								act_state = ActState::ATTACK;
								if (ground)
								{
									//�`����Z�b�g
								}
								else
								{
									//�`����Z�b�g
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

						//����
						if (pad->x_input[scastI(PAD::A)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//�`����Z�b�g

							attack_state = AttackState::STEAL;
						}




					}
					else
					{
						//�󒆎�
						if (pad->x_input[scastI(PAD::X)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//�`����Z�b�g

							attack_state = AttackState::JAKU;

							//pad->com_list.Reset();
						}

						//�󒆒�
						if (pad->x_input[static_cast<int>(PAD::Y)] == 1)
						{
							pad->que.back().timer = 0;
							FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//�`����Z�b�g

							attack_state = AttackState::THU;
							pos.x += Getapply(50.0f);
							specialfream = 0;
						}

						//�󒆋�
						if (pad->x_input[static_cast<int>(PAD::B)] == 1)
						{
							pad->que.back().timer = 0;
							FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							attack = TRUE;
							moveflag = false;
							act_state = ActState::ATTACK;
							//�`����Z�b�g

							attack_state = AttackState::U_KYO;
							pos.x += Getapply(50.0f);
							specialfream = 0;
							break;
						}
					}

					//���G�U��
					if (pad->x_input[scastI(PAD::L_TRIGGER)] == 1)
					{
						pad->que.back().timer = 0;
						attack = TRUE;
						moveflag = false;
						speed.x = Getapply(5.0f);
						speed_Y.Set(70.0f);
						act_state = ActState::ATTACK;
						//�`����Z�b�g

						attack_state = AttackState::EXTENDATK;
					}




					//�z�[�~���O�_�b�V��
					if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
					{
						if (trackgauge > 0)
						{
							pad->que.back().timer = 0;
							attack = true;
							tracking.Init();
							act_state = ActState::ATTACK;
							//�`����Z�b�g

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
			//���Ⴊ�ݍU��
			if (act_state == ActState::SQUAT)
			{
				if (pad->x_input[scastI(PAD::X)] == 1)
				{
					pad->que.back().timer = 0;
					attack = TRUE;
					moveflag = false;
					act_state = ActState::ATTACK;
					//�`����Z�b�g

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
					//�`����Z�b�g

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
					//�`����Z�b�g

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

	EndAttackErase();			//�U������̏���

	FaceAnimation(elapsed_time);
}



void Knight::Attack(float decision, float elapsed_time)
{
	AttackUpdate(elapsed_time);	//�U������̍X�V
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
	//	//�ҋ@
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
	//case MOVER://�ړ��n�͕ʂŏ������Ă���̂�break���Ă��Ȃ�
	//case MOVEL://,,
	case AttackState::SLOW:
		//����
		Slow(elapsed_time);
		break;
	case AttackState::STEAL:
		//����̃X�e�[�g��D���B�͂�
		Steal(elapsed_time);
		break;
	case AttackState::JAKU:
		//��U��
		Jaku(elapsed_time);
		//���U��(�A�ł����ꍇ�͎と�������ƌq����悤��)
		if (later > 0 && later < target_max)
		{
			//���U��
			if (pad->x_input[static_cast<int>(PAD::X)] == 1)
			{
				//pad->que.back().timer = 0;
				//attack = TRUE;
				//later = non_target;
				//moveflag = false;
				//act_state = ActState::ATTACK;
				//if (ground)
				//{
				//	//�`����Z�b�g
				//}
				//else
				//{
				//	//�`����Z�b�g
				//}
				//attack_state = AttackState::SPECIAL_ATTACK;
				//fream = attack_list[scastI(attack_state)].attack_single[0].fream;
				//YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
				//production_time = 0.0f;

				specialfream = 2;
			}
			//���i���U��(�R���{���[�g�B�ォ�牺�i���U�����q����)
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
							////�`����Z�b�g

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
		//���U��
		Thu(elapsed_time);
		if (later > -1)
		{
			//���U��
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
					////�`����Z�b�g
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
					////�`����Z�b�g

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
		//���U��
		Kyo(specialfream, elapsed_time);
		if (later > -1)
		{
			//�z�[�~���O�_�b�V��
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
							//�`����Z�b�g

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
		//���i���U��
		D_Thu(specialfream, elapsed_time);
		if (later > -1)
		{
			//��i���U��
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
						////�`����Z�b�g

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
		//��i���U��
		U_Kyo(specialfream, elapsed_time);
		break;
	case AttackState::D_JAKU:
		//���i��U��
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
		//�z�[�~���O�_�b�V��
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


	//�U������̏I���t���O���o����㌄�����肵���炷�������s��
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

	//�㌄���ɃL�����Z�����ċZ���s���邩�`�F�b�N����
	if (later > 0 && later < target_max)
	{
		later -= elapsed_time;
		CancelList();
	}

	//�㌄����㌳�̃X�e�[�g�ɖ߂�
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
			//�`����Z�b�g

		}
	}
}


//-------------------------------
//*�U���̃L�����Z�����X�g*
//----------------------------------------------------------------------
//�E���̃��X�g���ɂŒ�`�����s���͂ق��̍s������L�����Z�����ďo�����Ƃ��o����
//�E��{�I�ɃQ�[�W���g���Z�͑S�Z�L�����Z���o���\
//----------------------------------------------------------------------
void Knight::CancelList()
{
	//�W�����v
	Jump();

	//�z�[�~���O�_�b�V��
	if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
	{
		if (trackgauge > 0)
		{
			//later = -1;
			pad->que.back().timer = 0;
			attack = true;
			tracking.Init();
			act_state = ActState::ATTACK;
			//�`����Z�b�g

			attack_state = AttackState::TRACK_DASH;
			moveflag = false;
			pos.y -= 50.0f;
			trackgauge--;
		}
	}

	//214�EX�R�}���h
	if (pad->x_input[scastI(PAD::X)] == 1)
	{
		int command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
		if (command == CommandList::LHURF)
		{
			later = non_target;
			attack = TRUE;
			moveflag = false;
			act_state = ActState::ATTACK;
			//�`����Z�b�g

			attack_state = AttackState::JAKU_RHURF;
			specialfream = 0;
		}
	}

	
	//�Q�[�W�Z
	if (pad->x_input[scastI(PAD::B)] == 1)
	{
		int command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
		if (command == CommandList::LHURF)
		{
			//214�EB�R�}���h
			if (power > 0)
			{
				later = -1;
				//�`����Z�b�g

				FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
				attack = TRUE;
				moveflag = false;
				act_state = ActState::ATTACK;
				attack_state = AttackState::KYO_LHURF;
				//�G�t�F�N�g�Z�b�g

				pos.x = tracking.rival_Pos.x - Getapply(100.0f);
				pos.y = tracking.rival_Pos.y - 100.0f;
				power--;
			}
		}
		else if (command == CommandList::RHURF)
		{
			//236�EB�R�}���h
			later = -1;
			//�`����Z�b�g
			//�G�t�F�N�g�Z�b�g

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

	//Track�̎��͉�
	//angle.x += elapsed_time * 10.0f;
	//�G�t�F�N�g�`��
	
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
	// *����*
	//�E�ߋ�2D�Ő��삵���Ƃ��͌����Ă�������ɂ���ĕ`���ς��Ă�����
	//�@�����3D�ł̐���ɂȂ�ׁAangle��ς��邱�ƂɎg��
	//�E�O��ł͋��U�����̉摜�̃T�C�Y���ʏ�ƈ���Ă�������
	//�@����������������炴��𓾂Ȃ������ƍl������
	//--------------------------------------------------------------------

	bool inversion = true;//���E���]�t���O

	DirectX::XMFLOAT4 material_color = { 1.0f,1.0f,1.0f,1.0f };

	if (fream < target_max && fream>0.0f)
	{
		material_color = { 0.0f,0.0f,1.0f,1.0f };
	}
	if (later < target_max && later>0.0f)
	{
		material_color = { 1.0f,0.0f,0.0f,1.0f };
	}

	//������
	if (rightOrleft < 0)
	{
		//angle.y = DirectX::XMConvertToRadians(180.0f);
		inversion = true;
		if (invincible)
		{
			//���G���
		}

	}
	//�E����
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
			//���G�I
		}
	}

	//���f���`��
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
	//	//�g�����̕`��
	//}

	//if (atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok)
	//{
	//	//���g�����̕`��B����܂Ƃ߂Ă���������
	//}

	//if (atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok)
	//{
	//	//�ł����g�����̕`��B����͑傫���Ⴄ����d���Ȃ�
	//}



//�f�o�b�O��ԂȂ�
#if USE_IMGUI
		//�v���C���[�̒��S
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

		//���͊֌W��\�����Ă�
		//GetJoypadXInputState(DX_INPUT_PAD1, &input);

		//if (GetJoypadXInputState(DX_INPUT_PAD1, &input) != -1)
		//{
		//	// ��ʂ� XINPUT_STATE �̒��g��`��
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




//�c���̕`��
void Knight::DrawFastMove(YR_Vector3 position)
{
	if (!fast)
	{
		return;
	}
	//�E
	if (rightOrleft < 0)
	{
		
	}
	//��
	else
	{
		
	}
	fast = false;
}

//�c�����Z�b�g����
void Knight::FastSet(YR_Vector3 position)
{
	fast = true;
	FastPos = position;
}


//�ړ��֌W(�X�e�b�v�Ȃ�)
//-----------------------------------------------------------------
// *�T�v*
//�E�󒆒n�セ�ꂼ��ɑO��̃X�e�b�v�����݂��邽�ߑS�Čʂɏ������Ă���
//�E�����������傢���������������Ǝv��
//-----------------------------------------------------------------
bool Knight::Step()
{
	//��
	if (!ground)
	{
		//�󒆍��X�e�b�v
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1)
		{
			if (jumpcount > 0)
			{
				//�`����Z�b�g

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
					//�`����Z�b�g

					speed.x = -stepspeed;
				}
			}
			else
			{
				pad->dash_trigger = false;
			}
		}

		//�󒆉E�X�e�b�v
		if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1)
		{
			if (jumpcount > 0)
			{
				//�`����Z�b�g

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
					//�`����Z�b�g

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

	//�o�b�N�X�e�b�v
	if (act_state == ActState::BACK)
	{
		//�E����
		if (rightOrleft > 0)
		{
			//�E�����̃o�b�N�X�e�b�v��x�����̑��x��0�ɂȂ�
			//�Ȃ̂őS��0�ɂ��Ă���
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

			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
			/*if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}*/
			speed.x++;
			return false;
		}
		if (rightOrleft < 0)
		{
			//�t���R��
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
			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
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
				//�`����Z�b�g

				return true;
			}
			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
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
				//�`����Z�b�g

				return true;
			}
			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
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
				//�`����Z�b�g

				return true;
			}
			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
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
				//�`����Z�b�g

				return true;
			}
			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
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

//�ړ��֌W(������̓X�e�b�v�ȊO)
void Knight::Move(float decision)
{
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_R)]==0)
	{
		if (ground)
		{

			//�`����Z�b�g

			if (rightOrleft > 0)
			{
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				//�`����Z�b�g

				speed.x = -backstepS;
			}
		}
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_L)]==0)
	{
		if (ground)
		{
			//�`����Z�b�g

			if (rightOrleft < 0)
			{
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				//�`����Z�b�g

				speed.x = backstepS;
			}
		}
	}


	//���ړ�
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0)
	{
		if (rightOrleft > 0)
		{
			//�������Ȃ�
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




	//�E�ړ�
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{
		if (rightOrleft < 0)
		{
			//�������Ȃ�
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


	//�ǂ����������Ă���or������Ă��Ȃ��Ȃ猳�ɖ߂�
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
		//�`����Z�b�g

		if (rightOrleft > 0)
		{
			//�`����Z�b�g
		}

	}
	if (pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		moveflag = true;
		//�`����Z�b�g

		if (rightOrleft < 0)
		{
			//�`����Z�b�g

		}
	}
}

void Knight::GuardAnimSet()
{
	if (act_state == ActState::GUARD)
	{
		if (pad->x_input[static_cast<int>(PAD::STICK_RDown)] > 0 || pad->x_input[static_cast<int>(PAD::STICK_LDown)] > 0)
		{
			//�`����Z�b�g

		}
		else
		{
			//�`����Z�b�g

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
		//�`����Z�b�g

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
			//�`����Z�b�g

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
			//�`����Z�b�g

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
			//�`����Z�b�g

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
			//�`����Z�b�g

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
			//�`����Z�b�g

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
			//�`����Z�b�g

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
				//�`����Z�b�g

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
				//�`����Z�b�g

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
				//�`����Z�b�g

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
					//�`����Z�b�g

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
			//�U�����󂯂Ă���
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
			//�`����Z�b�g

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
			//�`����Z�b�g

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
		//�`����Z�b�g

	}
	if (pad->x_input[scastI(PAD::STICK_D)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0 && pad->x_input[scastI(PAD::STICK_LDown)] == 0)
	{
		if (act_state == ActState::SQUAT)
		{
			//�`����Z�b�g

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
		//�`����Z�b�g

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
			//�`����Z�b�g

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
			//�`����Z�b�g

		}
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -20.0f;
			speed.y = -10.0f;
			act_state = ActState::PASSIVE;
			//�`����Z�b�g

		}
		if (pad->x_input[scastI(PAD::X)] > 0 || pad->x_input[scastI(PAD::Y)] > 0 ||
			pad->x_input[scastI(PAD::B)] > 0 || pad->x_input[scastI(PAD::A)] > 0)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			act_state = ActState::PASSIVE;
			//�`����Z�b�g

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
			//�`����Z�b�g

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
	//�N���オ��J�n�͖��G
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		hit[i].guard = HitBox::INVINCIBLE;
	}

	//�`����Z�b�g�͂��Ȃ�
	//�_�E�������(���S�ɋN���オ������)���G����������
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
			//�`����Z�b�g

		}
	}
}


void Knight::StateNone()
{
	if (steal_escape > 0)
	{
		//�`����Z�b�g

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

//�͂ݍU��
void Knight::Steal(float elapsed_time)
{
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	YR_Vector3 range{ 50.0f,50.0f };
	//atk[scastI(KNIGHTATK::ONE)].Update(cent, range, 10, 10, 15, 20, 13, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::STEAL, Getapply(0.0f), true,elapsed_time);
	
	//--------------------------------
	//			���v�ύX
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
	//		//�`����Z�b�g

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
			//�`����Z�b�g

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
	//�������o�p�̃Z�b�g
	pos = YR_Vector3(1.0f, 0.0f, 0.0f);
	YRCamera.SetFocus(pos.GetDXFLOAT3());
	YRCamera.SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -25.0f));
}

bool Knight::WinPerformance()
{
	//�������o�̍s��������
	//�J������������œ�����
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
	//�C���g���̍s��������
	//�J������������œ�����
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

//�U�������蔻�肪�S�ďI�����Ă��邩�m�F����
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


//�I�������U�������蔻���S�ď�������B
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

//�S�Ă̍U�������蔻�����������
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

//�U�����肪���݂��鎞�̂ݍX�V
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


//��̃e�N�X�`���A�j���[�V����
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


//�u��
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