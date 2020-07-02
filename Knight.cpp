#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"


Knight::~Knight()
{

}

void Knight::Init(YR_Vector3 InitPos)
{
	//ImGui::Begin("RYU HitBox");
	pos = InitPos;
	scale = YR_Vector3( 0.05f,0.05f,0.05f );
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
	for (int i = 0; i < scastI(KNIGHTATK::ATKEND); i++)
	{
		atk[i].Init();
	}
#pragma region HITBOXINIT
	Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, -8.0f);
	hit[scastI(KNIGHTHIT::BODY)].Init(pos + Hitplus[scastI(KNIGHTHIT::BODY)], YR_Vector3(65.0f, 130.0f));
	HitSize[scastI(KNIGHTHIT::BODY)] = hit[scastI(KNIGHTHIT::BODY)].size;
	Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(0.0f, 163.0f);
	hit[scastI(KNIGHTHIT::LEG)].Init(pos + Hitplus[scastI(KNIGHTHIT::LEG)], YR_Vector3(73.0f, 40.0f));
	HitSize[scastI(KNIGHTHIT::LEG)] = hit[scastI(KNIGHTHIT::LEG)].size;
#pragma endregion

	hadouspeed = 0.0f;
	moveflag = false;
	attack = FALSE;
	state = NONE;
	later = -1;
	hadou = { pos.x + Getapply(100),pos.y };
}


void Knight::LoadData()
{
	if (base == nullptr)
	{
		base = std::make_unique<Skinned_mesh>("./Data/FBX/danbo_fbx/danbo_taiki.fbx");
	}
	motion.MeshSet(base);
	motion.AnimReset();
}

void Knight::Uninit()
{
	base.reset();
	base = nullptr;
}
void Knight::Update(float decision, float elapsed_time)
{
	finish = false;
	DamageCheck();
	WaitAnimSet();
	HadouUpdate();
	Thu_HadouUpdate();
	Kyo_HadouUpdate();
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
	//�@����ȊO�͑�����󂯕t����̂�defalut�ŏ������Ă���
	//-----------------------------------------------------
	switch (state)
	{
	case STATENONE:
		//�X�e�[�g��D��ꂽ��ԁB����ɒ͂܂�Ă�
		StateNone();
		break;
	case WAKE:
		//�_�E������N���オ��
		WakeUp();
		break;
	case PASSIVE:
		//�󂯐g���Ƃ�
		PassiveUpdate();
		break;
	case DOWN:
		//�_�E�����
		DownUpdate();
		break;
	case FALL:
		//�󒆂Ń_�E�����Ă�����
		FallUpdate();
		break;
	case KNOCK:
		//�U�����󂯂Ă̂�����
		KnockUpdate();
		break;
	default:
		if (ground)
		{
			pos.y = POS_Y;
			trackgauge = 2;
			if (state == JUMP)
			{
				state = NONE;
			}
		}
		else
		{
			if (pos.y < POS_Y)
			{
				/*speed.y += gravity;
				if (speed.y > 30.0f)
				{
					speed.y = 30.0f;
				}*/
				pos.y += gravity;
			}
		}



		//�U�����̏���
		if (attack)
		{
			//speed.x = 0;
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
			switch (state)
			{
			case NONE:
				break;
			case WAIT:
				//�ҋ@
				finish = true;
				later = -1;
				attack = FALSE;
				if (state != WAIT)
				{
					state = NONE;
				}
				for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}
				break;
			case MOVER://�ړ��n�͕ʂŏ������Ă���̂�break���Ă��Ȃ�
			case MOVEL://,,
			case SLOW:
				//����
				Slow();
				break;
			case STEAL:
				//����̃X�e�[�g��D���B�͂�
				Steal();
				break;
			case JAKU:
				//��U��
				Jaku();
				//���U��(�A�ł����ꍇ�͎と�������ƌq����悤��)
				if (later > -1)
				{
					if (pad->x_input[static_cast<int>(PAD::X)] == 1)
					{
						pad->que.back().timer = 0;
						FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
						attack = TRUE;
						atk[scastI(KNIGHTATK::ONE)].start = FALSE;
						later = -1;
						for (int i = 0; i < scastI(KNIGHTATK::END); i++)
						{
							atk[i].Init();
						}
						moveflag = false;
						atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
						if (ground)
						{
							//�`����Z�b�g
						}
						else
						{
							//�`����Z�b�g
						}
						state = THU;
						pos.x += Getapply(50.0f);
						specialfream = 2;
					}
					//���i���U��(�R���{���[�g)
					if (ground)
					{
						if (pad->x_input[static_cast<int>(PAD::Y)] == 1)
						{
							for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
							{
								if (atk[atknum].knock_start)
								{
									pad->que.back().timer = 0;
									FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
									attack = TRUE;
									atk[scastI(KNIGHTATK::ONE)].start = FALSE;
									later = -1;
									for (int i = 0; i < scastI(KNIGHTATK::END); i++)
									{
										atk[i].Init();
									}
									moveflag = false;
									atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
									//�`����Z�b�g

									state = D_THU;
									pos.x += Getapply(50.0f);
									specialfream = 3;
									break;
								}
							}
						}
					}
				}
				break;
			case THU:
				//���U��
				Thu(specialfream);
				if (later > -1)
				{
					if (pad->x_input[static_cast<int>(PAD::X)] == 1)
					{
						if (ground)
						{
							pad->que.back().timer = 0;
							FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							attack = TRUE;
							atk[scastI(KNIGHTATK::ONE)].start = FALSE;
							later = -1;
							for (int i = 0; i < scastI(KNIGHTATK::END); i++)
							{
								atk[i].Init();
							}
							moveflag = false;
							atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
							//�`����Z�b�g

							state = KYO;
							pos.x += Getapply(50.0f);
							specialfream = 4;
						}
						else
						{
							pad->que.back().timer = 0;
							FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							attack = TRUE;
							atk[scastI(KNIGHTATK::ONE)].start = FALSE;
							later = -1;
							for (int i = 0; i < scastI(KNIGHTATK::END); i++)
							{
								atk[i].Init();
							}
							moveflag = false;
							atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
							//�`����Z�b�g

							state = U_KYO;
							pos.x += Getapply(50.0f);
							specialfream = 4;
							break;
						}
					}
				}
				break;
			case KYO:
				//���U��
				Kyo(specialfream);
				if (later > -1)
				{
					if (pad->x_input[static_cast<int>(PAD::X)] == 1)
					{
						for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
						{
							if (atk[atknum].knock_start)
							{
								pad->que.back().timer = 0;
								attack = TRUE;
								atk[scastI(KNIGHTATK::ONE)].start = FALSE;
								later = -1;
								for (int i = 0; i < scastI(KNIGHTATK::END); i++)
								{
									atk[i].Init();
								}
								moveflag = false;
								atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
								if (trackgauge > 0)
								{
									tracking.Init();
									//�`����Z�b�g

									state = TRACK_DASH;
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
			case D_THU:
				//���i���U��
				D_Thu(specialfream);
				if (later > -1)
				{
					if (pad->x_input[static_cast<int>(PAD::B)] == 1)
					{
						for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
						{
							if (atk[atknum].knock_start)
							{
								pad->que.back().timer = 0;
								FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								attack = TRUE;
								atk[scastI(KNIGHTATK::ONE)].start = FALSE;
								later = -1;
								for (int i = 0; i < scastI(KNIGHTATK::END); i++)
								{
									atk[i].Init();
								}
								moveflag = false;
								atk[scastI(KNIGHTATK::ONE)].fin = FALSE;
								//�`����Z�b�g

								state = U_KYO;
								pos.x += Getapply(50.0f);
								specialfream = 4;
								break;
							}
						}
					}
				}
				break;
			case U_KYO:
				//��i���U��
				U_Kyo(specialfream);
				break;
			case D_JAKU:
				//���i��U��
				D_Jaku();
				break;
			case HADOUKEN:
				Hadouken();
				break;
			case THU_HADOUKEN:
				Thu_Hadouken();
				break;
			case KYO_HADOUKEN:
				Kyo_Hadouken();
				break;
			case TRACK_DASH:
				//�z�[�~���O�_�b�V��
				TrackDash(decision);
				break;
			case P_KYO:
				P_Kyo();
				break;
			case EXTENDATK:
				ExtendATK();
				break;
			case PANISH_N:
				Panish_N();
				break;
			case PANISH_H:
				Panish_H();
				break;
			}


			//�U������̏I���t���O���o����㌄�����肵���炷�������s��
			for (int atknum = 0; atknum < scastI(KNIGHTATK::END); atknum++)
			{
				if (atk[atknum].fin)
				{
					if (later == -1)
					{
						later = atk[atknum].later;
					}
					later--;
				}
			}
			//�㌄���ɃL�����Z�����ċZ���s���邩�`�F�b�N����
			if (later > 0)
			{
				CancelList();
			}

			//�㌄����㌳�̃X�e�[�g�ɖ߂�
			if (later == 0)
			{
				finish = true;
				later = -1;
				attack = FALSE;
				//atk[scastI(RYUATK::ONE)].start = FALSE;
				if (ground)
				{
					if (state != WAIT)
					{
						state = NONE;
					}
				}
				else
				{
					state = JUMP;
					//�`����Z�b�g

				}
				for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}
			}
		}
		else
		{
		//�U�����s���Ă��Ȃ��Ƃ��̏���
			if (ground && !step)
			{
				jumpcount = 2;
				rightOrleft = decision;
				if (speed.x > 0.0f)
				{
					speed.x-=elapsed_time;
					if (speed.x < 0.0f)
					{
						speed.x = 0;
					}
				}
				if (speed.x < 0.0f)
				{
					speed.x+=elapsed_time;
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
			if (state != SQUAT && state != GUARD)
			{
				Step();
				if (step)
				{

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
							//�R�}���h����
							int command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
							if (command == CommandList::RHURF)
							{
								//FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//�`����Z�b�g

								state = HADOUKEN;
								//pos.x += Getapply(50.0f);
								specialfream = 0;
							}
							else if (command == CommandList::LHURF)
							{
								//�`����Z�b�g

								state = PANISH_N;
								specialfream = 0;
							}
							else
							{
								//�`����Z�b�g

								state = JAKU;
							}
						}

						//���i���U��
						if (pad->x_input[scastI(PAD::Y)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							//�R�}���h����
							int command = pad->com_list.CommandCheack(scastI(PAD::Y), pad->input_history.data(), rightOrleft);
							if (command == CommandList::RHURF)
							{
								//�`����Z�b�g

								state = THU_HADOUKEN;
								specialfream = 0;
							}
							else if (command == CommandList::LHURF)
							{
								//�`����Z�b�g

								state = PANISH_H;
								specialfream = 0;
							}
							else
							{
								FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//�`����Z�b�g

								state = D_THU;
								pos.x += Getapply(50.0f);
							}
						}

						//��i���U��
						if (pad->x_input[scastI(PAD::B)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							//�R�}���h����
							int command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
							if (command == CommandList::LHURF)
							{
								if (power > 0)
								{
									//�`����Z�b�g

									FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
									state = P_KYO;
									//�G�t�F�N�g�Z�b�g

									pos.x = tracking.rival_Pos.x - Getapply(100.0f);
									pos.y = tracking.rival_Pos.y - 100.0f;
									power--;
								}
								else
								{
									//�`����Z�b�g

									state = PANISH_H;
									specialfream = 0;
								}
							}
							else if (command == CommandList::RHURF)
							{
								if (power > 0)
								{
									//�G�t�F�N�g�Z�b�g
									//�`����Z�b�g

									state = KYO_HADOUKEN;
									specialfream = 0;
									power--;
								}
								else
								{
									//�`����Z�b�g

									state = THU_HADOUKEN;
									specialfream = 0;
								}
							}
							else
							{
								FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
								//�`����Z�b�g

								state = U_KYO;
								pos.x += Getapply(50.0f);
							}
						}

						//����
						if (pad->x_input[scastI(PAD::A)] == 1)
						{
							pad->que.back().timer = 0;
							attack = TRUE;
							moveflag = false;
							//�`����Z�b�g

							state = STEAL;
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
							//�`����Z�b�g

							state = JAKU;

							//pad->com_list.Reset();
						}

						//�󒆒�
						if (pad->x_input[static_cast<int>(PAD::Y)] == 1)
						{
							pad->que.back().timer = 0;
							FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
							attack = TRUE;
							moveflag = false;
							//�`����Z�b�g

							state = THU;
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
							//�`����Z�b�g

							state = U_KYO;
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
						//�`����Z�b�g

						state = EXTENDATK;
					}




					//�z�[�~���O�_�b�V��
					if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
					{
						if (trackgauge > 0)
						{
							pad->que.back().timer = 0;
							attack = true;
							tracking.Init();
							//�`����Z�b�g

							state = TRACK_DASH;
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
			if (state == SQUAT)
			{
				if (pad->x_input[scastI(PAD::X)] == 1)
				{
					pad->que.back().timer = 0;
					attack = TRUE;
					moveflag = false;
					//�`����Z�b�g

					state = D_JAKU;
					//pad->com_list.Reset();
				}
				if (pad->x_input[scastI(PAD::Y)] == 1)
				{
					pad->que.back().timer = 0;
					attack = TRUE;
					moveflag = false;
					FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
					//�`����Z�b�g

					state = D_THU;
					pos.x += Getapply(50.0f);
				}
				if (pad->x_input[static_cast<int>(PAD::B)] == 1)
				{
					pad->que.back().timer = 0;
					FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
					attack = TRUE;
					moveflag = false;
					//�`����Z�b�g

					state = KYO;
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

	pos.x += speed.x;
	pos.y -= speed_Y.Update(elapsed_time);

	if (state != D_THU && state != DOWN && state != SQUAT && state != D_JAKU)
	{
		Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, -8.0f);
		hit[scastI(KNIGHTHIT::BODY)].size = HitSize[scastI(KNIGHTHIT::BODY)];
		Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(0.0f, 163.0f);
		hit[scastI(KNIGHTHIT::LEG)].size = HitSize[scastI(KNIGHTHIT::LEG)];
	}
	hit[scastI(KNIGHTHIT::BODY)].Update(pos + Hitplus[scastI(KNIGHTHIT::BODY)], hit[scastI(KNIGHTHIT::BODY)].size);
	hit[scastI(KNIGHTHIT::LEG)].Update(pos + Hitplus[scastI(KNIGHTHIT::LEG)], hit[scastI(KNIGHTHIT::LEG)].size);
}




void Knight::CancelList()
{
	Jump();

	if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
	{
		if (trackgauge > 0)
		{
			later = -1;
			for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}
			pad->que.back().timer = 0;
			attack = true;
			tracking.Init();
			//�`����Z�b�g

			state = TRACK_DASH;
			moveflag = false;
			pos.y -= 50.0f;
			trackgauge--;
		}
	}

	if (pad->x_input[scastI(PAD::X)] == 1)
	{
		int command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
		if (command == CommandList::LHURF)
		{
			later = -1;
			for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}
			attack = TRUE;
			moveflag = false;
			//�`����Z�b�g

			state = HADOUKEN;
			specialfream = 0;
		}
	}

	if (pad->x_input[scastI(PAD::B)] == 1)
	{
		int command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
		if (command == CommandList::LHURF)
		{
			if (power > 0)
			{
				later = -1;
				for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}
				//�`����Z�b�g

				FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
				attack = TRUE;
				moveflag = false;
				state = P_KYO;
				//�G�t�F�N�g�Z�b�g

				pos.x = tracking.rival_Pos.x - Getapply(100.0f);
				pos.y = tracking.rival_Pos.y - 100.0f;
				power--;
			}
		}
		else if (command == CommandList::RHURF)
		{
			later = -1;
			for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}
			//�`����Z�b�g
			//�G�t�F�N�g�Z�b�g

			state = KYO_HADOUKEN;
			specialfream = 0;
			power--;
		}
	}
}



void Knight::Draw(
	YRShader				*shader,
	YRShader				*geoshader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	DrawFastMove(FastPos);
	drawset = false;

	//�G�t�F�N�g�`��
	
	bool invincible = false;

	if (state == EXTENDATK)
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

	//������
	if (rightOrleft < 0)
	{
		//�������ꂢ��Ȃ�
		if (state == KYO)
		{
			drawset = true;
		}

		if (!drawset)
		{
			//���U����`�悵������`�悵�Ȃ�
		}
		if (invincible)
		{
			//���G���
		}

	}
	//�E����
	else
	{
		//�Ƃ�������΂���Ȃ�
		if (state == KYO)
		{
			drawset = true;
		}

		if (!drawset)
		{
			//������������3D���f��������`�敪����K�v�Ȃ�
		}
		if (invincible)
		{
			//�n�C�p�[���G�I
		}
	}

	//���f���`��
	/*base->Render(
		pos.GetDXFLOAT3(),
		scale.GetDXFLOAT3(),
		angle.GetDXFLOAT3(),
		view, projection, light_direction, light_color, ambient_color, elapsed_time, 1.0f);*/
	motion.DrawContinue(
		shader,
		pos.GetDXFLOAT3(),
		scale.GetDXFLOAT3(),
		angle.GetDXFLOAT3(),
		view, projection, light_direction, light_color, ambient_color, elapsed_time
	);


	if (atk[scastI(KNIGHTATK::HADOU)].hit_ok)
	{
		//�g�����̕`��
	}

	if (atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok)
	{
		//���g�����̕`��B����܂Ƃ߂Ă���������
	}

	if (atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok)
	{
		//�ł����g�����̕`��B����͑傫���Ⴄ����d���Ȃ�
	}



//�f�o�b�O��ԂȂ�
#if USE_IMGUI
		for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			hit[i].Draw();
		}

		if (attack)
		{
			for (int atknum = 0; atknum < scastI(KNIGHTATK::ATKEND); atknum++)
			{
				if (atk[atknum].attack)
				{
					if (atk[atknum].hit_ok)
					{
						atk[atknum].Draw();
					}
				}
			}
		}
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
			ImGui::Begin("RyuHitBox");
			//ImGui::Text(u8"�悤�����ADX���C�u������");
			ImGui::SliderFloat("BodyPosX", &Hitplus[scastI(KNIGHTHIT::BODY)].x, -1000.0f, 1000.0f);
			ImGui::SliderFloat("BodyPosY", &Hitplus[scastI(KNIGHTHIT::BODY)].y, -1000.0f, 1000.0f);
			ImGui::SliderFloat("BodySizeX", &hit[scastI(KNIGHTHIT::BODY)].size.x, -1000.0f, 1000.0f);
			ImGui::SliderFloat("BodySizeY", &hit[scastI(KNIGHTHIT::BODY)].size.y, -1000.0f, 1000.0f);

			ImGui::SliderFloat("LegPosX", &Hitplus[scastI(KNIGHTHIT::LEG)].x, -1000.0f, 1000.0f);
			ImGui::SliderFloat("LegPosY", &Hitplus[scastI(KNIGHTHIT::LEG)].y, -1000.0f, 1000.0f);
			ImGui::SliderFloat("LegSizeX", &hit[scastI(KNIGHTHIT::LEG)].size.x, -1000.0f, 1000.0f);
			ImGui::SliderFloat("LegSizeY", &hit[scastI(KNIGHTHIT::LEG)].size.y, -1000.0f, 1000.0f);
			//ImGui::SliderFloat("camera.y", &camera.y, -world_max_y, world_max_y);
			//ImGui::SliderFloat("camera.x", &camera.x, -world_max_x, world_max_x);
			ImGui::Text("player.y:%f", pos.y);
			ImGui::Text("player.x:%f", pos.x);

			//ImGui::ColorEdit3("clear color", (float*)&ImColor(114, 144, 154));
			//if (ImGui::Button("Test Window")) show_test_window ^= 1;
			//if (ImGui::Button("Another Window")) show_another_window ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
#endif

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

				state = AIR_F;
				step = true;
				moveflag = false;
				speed.x = -stepspeed;
				jumpflag = false;
				jumpcount = 0;
				if (rightOrleft > 0)
				{
					step = true;
					state = AIR_B;
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
				state = AIR_F;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				speed.x = stepspeed;
				if (rightOrleft < 0)
				{
					step = true;
					state = AIR_B;
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
	if (state == BACK)
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
				state = NONE;
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
				state = NONE;
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

	if (state == AIR_B)
	{
		speed_Y.Set(0.0f);
		pos.y -= gravity;
		if (rightOrleft > 0)
		{
			if (speed.x > -dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				state = JUMP;
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
				state = JUMP;
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

	if (state == AIR_F)
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
				state = JUMP;
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
				state = JUMP;
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
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1)
	{
		if (ground)
		{

			//�`����Z�b�g

			if (rightOrleft > 0)
			{
				step = true;
				state = BACK;
				moveflag = false;
				//�`����Z�b�g

				speed.x = -backstepS;
			}
		}
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1)
	{
		if (ground)
		{
			//�`����Z�b�g

			if (rightOrleft < 0)
			{
				step = true;
				state = BACK;
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
				state = DASH;
			}
		}
	}
	else
	{
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -walkspeed;
			state = MOVEL;
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
				state = DASH;
			}
		}
	}
	else
	{
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = walkspeed;
			state = MOVER;
		}
	}


	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	{
		if (state != WAIT)
		{
			state = NONE;
		}
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		if (state != WAIT)
		{
			state = NONE;
		}
		moveflag = false;
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
	if (state == GUARD)
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
	if (state == NONE)
	{
		hit[scastI(KNIGHTHIT::BODY)].size = HitSize[scastI(KNIGHTHIT::BODY)];
		hit[scastI(KNIGHTHIT::LEG)].size = HitSize[scastI(KNIGHTHIT::LEG)];
		//�`����Z�b�g

		state = WAIT;
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

			state = JUMP;
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

			state = JUMP;
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

			state = JUMP;
			jumpflag = true;
		}

		if (pad->x_input[scastI(PAD::STICK_U)] == 1)
		{
			later = -1;
			attack = FALSE;
			for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = false;
			speed.y = 40.0f;
			max_jump_flag = false;
			moveflag = false;
			//�`����Z�b�g

			state = JUMP;
			jumpflag = true;
		}
		if (pad->x_input[scastI(PAD::STICK_RUp)] == 1)
		{
			later = -1;
			attack = FALSE;
			for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}
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

			state = JUMP;
			jumpflag = true;
		}
		if (pad->x_input[scastI(PAD::STICK_LUp)] == 1)
		{
			later = -1;
			attack = FALSE;
			for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}
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

			state = JUMP;
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
				for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				jumpcount = 0;
				speed.y = 40.0f;
				moveflag = false;
				max_jump_flag = false;
				state = JUMP;
				//�`����Z�b�g

				jumpflag = true;
			}
			if (pad->x_input[scastI(PAD::STICK_RUp)] == 1)
			{
				later = -1;
				attack = FALSE;
				for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}
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

				state = JUMP;
				jumpflag = true;
			}
			if (pad->x_input[scastI(PAD::STICK_LUp)] == 1)
			{
				later = -1;
				attack = FALSE;
				for (int i = 0; i < scastI(KNIGHTATK::END); i++)
				{
					atk[i].Init();
				}
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

				state = JUMP;
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
			state = NONE;
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
			for (int j = 0; j < scastI(KNIGHTATK::END); j++)
			{
				atk[j].Init();
			}
			state = KNOCK;
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
			for (int j = 0; j < scastI(KNIGHTATK::END); j++)
			{
				atk[j].Init();
			}
			hit[i].steal = false;
			steal_escape = hit[i].steal_timer;
			hit[i].steal_timer = 0;
			state = STATENONE;
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
		if (state != WAIT)
		{
			state = NONE;
		}
		if (!ground)
		{
			state = FALL;
			speed.y = 0.0f;
			//�`����Z�b�g

			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		if (hp == 0)
		{
			state = FALL;
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
			state = GUARD;
		}
	}

}

void Knight::GuardBack()
{
	if (state != GUARD)
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
		if (state != WAIT)
		{
			state = NONE;
		}
		if (ground)
		{
			if (pad->x_input[scastI(PAD::STICK_D)] > 0)
			{
				state = SQUAT;
			}
			if (pad->x_input[scastI(PAD::STICK_LDown)] > 0)
			{
				state = SQUAT;
			}
			if (pad->x_input[scastI(PAD::STICK_RDown)] > 0)
			{
				state = SQUAT;
			}
		}
		else
		{
			max_jump_flag = false;
			jumpflag = false;
			state = JUMP;
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
		state = GUARD;
	}
}

void Knight::Squat()
{
	if (state == GUARD || state == BACK || state == AIR_B || state == AIR_F)
	{
		return;
	}
	if (!ground)
	{
		return;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
	{
		Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
		hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
		moveflag = false;
		state = SQUAT;
		//�`����Z�b�g

	}
	if (pad->x_input[scastI(PAD::STICK_D)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0 && pad->x_input[scastI(PAD::STICK_LDown)] == 0)
	{
		if (state == SQUAT)
		{
			//�`����Z�b�g

		}
		if (state != WAIT)
		{
			state = NONE;
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
		state = DOWN;
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
			state = PASSIVE;
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
	Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(-31.0f, 134.0f);
	hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(110.0f, 62.0f);
	Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(110.0f, 149.0f);
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
			state = PASSIVE;
			//�`����Z�b�g

		}
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -20.0f;
			speed.y = -10.0f;
			state = PASSIVE;
			//�`����Z�b�g

		}
		if (pad->x_input[scastI(PAD::X)] > 0 || pad->x_input[scastI(PAD::Y)] > 0 ||
			pad->x_input[scastI(PAD::B)] > 0 || pad->x_input[scastI(PAD::A)] > 0)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			state = PASSIVE;
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

			state = WAKE;
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
			state = NONE;
		}
		else
		{
			state = JUMP;
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
			state = NONE;
			return;
		}
		steal_escape--;
	}
	else
	{

	}
}


void Knight::Steal()
{
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	YR_Vector3 range{ 50.0f,50.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(cent, range, 10, 10, 15, 20, 13, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::STEAL, Getapply(0.0f), true);
	if (rival_state != STATENONE)
	{
		if (atk[scastI(KNIGHTATK::ONE)].timer == 0)
		{
			atk[scastI(KNIGHTATK::ONE)].fin = TRUE;
		}
	}
	if (rival_state == STATENONE)
	{
		if (atk[scastI(KNIGHTATK::ONE)].stealtimer > 0)
		{
			atk[scastI(KNIGHTATK::ONE)].stealtimer--;
		}
		else
		{
			later = -1;
			for (int i = 0; i < scastI(KNIGHTATK::END); i++)
			{
				atk[i].Init();
			}
			state = SLOW;
			//�`����Z�b�g

			if (rightOrleft > 0)
			{
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					FastSet(pos);
					rightOrleft = -rightOrleft;
					pos.x = tracking.rival_Pos.x + 150.0f;
				}
			}
			else
			{
				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					FastSet(pos);
					rightOrleft = -rightOrleft;
					pos.x = tracking.rival_Pos.x - 150.0f;
				}
			}
		}
	}
}


void Knight::TrackDash(float decision)
{
	if (state != TRACK_DASH)
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
		if (atk[scastI(KNIGHTATK::ONE)].knock_start)
		{
			speed_Y.Set(40.0f);
			pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
			atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
		}
		YR_Vector3 cent{ pos.x + (50.0f * decision),pos.y };
		YR_Vector3 range{ 150.0f,200.0f };
		atk[scastI(KNIGHTATK::ONE)].Update
		(
			cent, range, 3, 1, 3, 5, 40, YR_Vector3(3.0f * decision, 20.0f), AttackBox::MIDDLE, Getapply(10.0f)
		);
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
		return scastI(KNIGHTATK::ATKEND);
	}
	return 0;
}

//float& Ryu::GetPosX2()
//{
//	return pos.x;
//}

void Knight::NoneChange()
{
	if (state != GUARD)
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
		state = NONE;
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