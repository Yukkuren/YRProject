#include "Neru.h"
#include "../PlayerBase/PlayerBase.h"
#include "../../Key.h"
#include "../../Game/YRGamePad.h"
#include "../../camera.h"
#include "../../framework.h"
#include <algorithm>
#include "../../Game//World.h"
#include "../PlayerBase/Player_name_list.h"
//#include <memory>

//----------------------------------------------------------------
//				Neru���f���f�[�^�ǂݍ���cpp
//----------------------------------------------------------------
//�ENeru�̃��f���f�[�^��摜�f�[�^��ǂݍ��ޏ������L�q���Ă���
//----------------------------------------------------------------

void Neru::LoadData(PLCOLOR color_number)
{
	//�ʃX���b�h�Ńf�[�^��ǂݍ��ޏ���

	attack_list.resize(1);
	hit.resize(scastI(NERUHIT::END));
	hitparam_list.resize(scastI(NERUHIT::END));
//#ifdef EXIST_IMGUI
//	//DEBUGAttackLoad();
//	AttackClean();
//	AttackLoad();
//	//DEBUGAttackLoad();
//#endif // 0
	//AttackLoad();
	//DEBUGAttackLoad();
	AttackClean();
	AttackLoad();


	//�J�b�g�C���t���[���ǂݍ���
	if (cutFrame == nullptr)
	{
		cutFrame = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/CutIn_Frame.png", 1920.0f, 1440.0f, 3, 4, 640.0f, 360.0f, 10);
	}
	if (cutMask == nullptr)
	{
		cutMask = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/CutIn_Mask.png", 1920.0f, 1440.0f, 3, 4, 640.0f, 360.0f, 10);
	}


	switch (color_number)
	{
	case PLCOLOR::ORIGINAL:
		color_texture_main = nullptr;
		color_texture_face = nullptr;

		//�J�b�g�C���摜�ǂݍ���
		if (cutIn == nullptr)
		{
			cutIn = std::make_unique<Sprite>(L"./Data/Image/Character/Neru/Neru_cut0.png", 640.0f, 192.0f);
			//cutIn = std::make_unique<Sprite>(L"./Data/Image/Character/Neru/Neru_cut1.png", 640.0f, 192.0f);
		}
		break;
	case PLCOLOR::COLOR_2:
		color_texture_main = std::make_shared<Texture>(L"./Data/FBX/Neru/NeruTex2.png");
		color_texture_face = nullptr;

		//�J�b�g�C���摜�ǂݍ���
		if (cutIn == nullptr)
		{
			cutIn = std::make_unique<Sprite>(L"./Data/Image/Character/Neru/Neru_cut1.png", 640.0f, 192.0f);
		}
		break;
	case PLCOLOR::COLOR_END:
		break;
	default:
		color_texture_main = nullptr;
		color_texture_face = nullptr;
		break;
	}


	//���C���̃��b�V�����f���̓ǂݍ���
	if (main == nullptr)
	{

		if (color_texture_main != nullptr)
		{
			//�e�N�X�`����ς���ꍇ
			main = std::make_shared<Model>("./Data/FBX/Neru/Neru_main.fbx", color_texture_main, color_texture_face);
		}
		else
		{
			main = std::make_shared<Model>("./Data/FBX/Neru/Neru_main.fbx");
			//main = std::make_shared<Model>("./Data/FBX/Neru/Neru_main.fbx");
		}
	}
	//------------------------------------------//
	//		�A�j���[�V�����{�[���f�[�^�ǂݍ���		//
	//------------------------------------------//
	//		�E�����ƍ�����

	//ActState�ǂݍ���
	for (int i = 0; i < model_motion.model_R_Act.size() || i < model_motion.model_L_Act.size(); i++)
	{
		std::string file_level_R = std::string("./Data/FBX/Neru/AnimationR/Neru_");
		std::string file_level_L = std::string("./Data/FBX/Neru/AnimationL/Neru_");
		std::string file_end_R = std::string("_R.fbx");
		std::string file_end_L = std::string("_L.fbx");

		std::string file_name_R = file_level_R + GetName().act_file_list[i] + file_end_R;
		std::string file_name_L = file_level_L + GetName().act_file_list[i] + file_end_L;

		if (model_motion.model_R_Act[i] == nullptr)
		{
			model_motion.model_R_Act[i] = std::make_shared<Model>(file_name_R.c_str());
		}

		if (model_motion.model_L_Act[i] == nullptr)
		{
			model_motion.model_L_Act[i] = std::make_shared<Model>(file_name_L.c_str());
		}
	}

	//AttackState�ǂݍ���
	for (int i = 0; i < model_motion.model_R_Attack.size() || i < model_motion.model_L_Attack.size(); i++)
	{
		std::string file_level_R = std::string("./Data/FBX/Neru/AnimationR/Neru_");
		std::string file_level_L = std::string("./Data/FBX/Neru/AnimationL/Neru_");
		std::string file_end_R = std::string("_R.fbx");
		std::string file_end_L = std::string("_L.fbx");

		std::string file_name_R = file_level_R + GetName().attack_file_list[i] + file_end_R;
		std::string file_name_L = file_level_L + GetName().attack_file_list[i] + file_end_L;

		//R�t�@�C�������邩�m�F����
		{
			std::ifstream ifs_R(file_name_R.c_str());

			if (ifs_R.is_open())
			{
				//�t�@�C����������
				ifs_R.close();
				if (model_motion.model_R_Attack[i] == nullptr)
				{
					model_motion.model_R_Attack[i] = std::make_shared<Model>(file_name_R.c_str());
				}
			}
			else
			{
				//�t�@�C�����Ȃ�����
				//����Ɏ�̃A�j���[�V������ǂݍ���
				ifs_R.close();
				std::string file_name_R_j = file_level_R + GetName().attack_file_list[scastI(AttackState::JAKU)] + file_end_R;
				if (model_motion.model_R_Attack[i] == nullptr)
				{
					model_motion.model_R_Attack[i] = std::make_shared<Model>(file_name_R_j.c_str());
				}
			}
		}

		//L�t�@�C�������邩�m�F����
		{
			std::ifstream ifs_L(file_name_L.c_str());

			if (ifs_L.is_open())
			{
				//�t�@�C����������
				ifs_L.close();
				if (model_motion.model_L_Attack[i] == nullptr)
				{
					model_motion.model_L_Attack[i] = std::make_shared<Model>(file_name_L.c_str());
				}
			}
			else
			{
				//�t�@�C�����Ȃ�����
				//����Ɏ�̃A�j���[�V������ǂݍ���
				ifs_L.close();
				std::string file_name_L_j = file_level_L + GetName().attack_file_list[scastI(AttackState::JAKU)] + file_end_L;
				if (model_motion.model_L_Attack[i] == nullptr)
				{
					model_motion.model_L_Attack[i] = std::make_shared<Model>(file_name_L_j.c_str());
				}
			}
		}
	}

	//�C���g�����[�V����
	if (model_motion.intro_R == nullptr)
	{
		/*if (color_number == 1)
		{
			model_motion.intro_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_wait_R.fbx");
		}
		else*/
		{
			model_motion.intro_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_Intro_R.fbx");
		}
	}
	if (model_motion.intro_L == nullptr)
	{
		model_motion.intro_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/Neru_Intro_L.fbx");
	}

	//�������[�V����
	if (model_motion.win_R == nullptr)
	{
		model_motion.win_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_win_R.fbx");
	}
	if (model_motion.win_L == nullptr)
	{
		model_motion.win_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/Neru_win_L.fbx");
	}

	//�󒆃W�����v
	if (model_motion.air_jump_R == nullptr)
	{
		model_motion.air_jump_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_air_jump_R.fbx",true);
	}
	if (model_motion.air_jump_L == nullptr)
	{
		model_motion.air_jump_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/Neru_air_jump_L.fbx",true);
	}

	//�W�����v�����ڍs
	if (model_motion.jumpToFall_R == nullptr)
	{
		model_motion.jumpToFall_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_jumpToFall_R.fbx",true);
	}
	if (model_motion.jumpToFall_L == nullptr)
	{
		model_motion.jumpToFall_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/Neru_jumpToFall_L.fbx",true);
	}

	//�W�����v������
	if (model_motion.fall_R == nullptr)
	{
		model_motion.fall_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_fall_R.fbx");
	}
	if (model_motion.fall_L == nullptr)
	{
		model_motion.fall_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/Neru_fall_L.fbx");
	}

	//���n
	if (model_motion.landing_R == nullptr)
	{
		model_motion.landing_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_landing_R.fbx");
	}
	if (model_motion.landing_L == nullptr)
	{
		model_motion.landing_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/Neru_landing_L.fbx");
	}


	//�ҋ@���[�V����
	//if (model_motion.model_R_Act[scastI(ActState::WAIT)] == nullptr)
	//{
	//	/*if (color_number == 1)
	//	{
	//		model_motion.model_R_Act[scastI(ActState::WAIT)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_wait_R.fbx");
	//	}
	//	else*/
	//	{
	//		model_motion.model_R_Act[scastI(ActState::WAIT)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_wait_R.fbx");
	//	}
	//}
	//if (model_motion.model_L_Act[scastI(ActState::WAIT)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::WAIT)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_wait_L.fbx");
	//}

	////�K�[�h���[�V����
	//if (model_motion.model_R_Act[scastI(ActState::GUARD)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::GUARD)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_guard_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::GUARD)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::GUARD)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_guard_L.fbx");
	//}

	////�X���C�h���[�V����
	//if (model_motion.model_R_Act[scastI(ActState::SLAM)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::SLAM)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_slid_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::SLAM)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::SLAM)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_slid_L.fbx");
	//}

	////���Ⴊ��
	//if (model_motion.model_R_Act[scastI(ActState::SQUAT)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::SQUAT)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_squat_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::SQUAT)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::SQUAT)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_squat_L.fbx");
	//}

	////����
	///*if (model_motion.walk_R == nullptr)
	//{
	//	model_motion.walk_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_walk_R.fbx");
	//}
	//if (model_motion.walk_L == nullptr)
	//{
	//	model_motion.walk_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_walk_L.fbx");
	//}*/

	////���
	//if (model_motion.model_R_Act[scastI(ActState::RETREAT)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::RETREAT)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_back_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::RETREAT)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::RETREAT)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_back_L.fbx");
	//}

	////�_�b�V��
	//if (model_motion.model_R_Act[scastI(ActState::DASH)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::DASH)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_dash_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::DASH)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::DASH)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_dash_L.fbx");
	//}

	////�o�b�N�X�e�b�v
	//if (model_motion.model_R_Act[scastI(ActState::BACK)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::BACK)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_backstep_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::BACK)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::BACK)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_backstep_L.fbx");
	//}

	////�󒆃_�b�V��
	//if (model_motion.model_R_Act[scastI(ActState::AIR_F)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::AIR_F)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_air_dash_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::AIR_F)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::AIR_F)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_air_dash_L.fbx");
	//}

	////�󒆃o�b�N�X�e�b�v
	//if (model_motion.model_R_Act[scastI(ActState::AIR_B)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::AIR_B)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_air_back_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::AIR_B)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::AIR_B)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_air_back_L.fbx");
	//}

	////�󂯐g
	//if (model_motion.model_R_Act[scastI(ActState::PASSIVE)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::PASSIVE)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_passive_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::PASSIVE)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::PASSIVE)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_passive_L.fbx");
	//}

	////�W�����v
	//if (model_motion.model_R_Act[scastI(ActState::JUMP)] == nullptr)
	//{
	//	model_motion.model_R_Act[scastI(ActState::JUMP)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_jump_R.fbx");
	//}
	//if (model_motion.model_L_Act[scastI(ActState::JUMP)] == nullptr)
	//{
	//	model_motion.model_L_Act[scastI(ActState::JUMP)] = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_jump_L.fbx");
	//}

	////�󒆃W�����v
	//if (model_motion.air_jump_R == nullptr)
	//{
	//	model_motion.air_jump_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_air_jump_R_f.fbx");
	//}
	//if (model_motion.air_jump_L == nullptr)
	//{
	//	model_motion.air_jump_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_air_jump_L_f.fbx");
	//}

	////�_���[�W���[�V����
	//if (model_motion.damage_R_g_u == nullptr)
	//{
	//	model_motion.damage_R_g_u = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_damage_R_g_u.fbx");
	//}
	//if (model_motion.damage_L_g_u == nullptr)
	//{
	//	model_motion.damage_L_g_u = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_damage_L_g_u.fbx");
	//}

	////��U��
	//if (model_motion.jaku_R == nullptr)
	//{
	//	/*if (color_number == 1)
	//	{
	//		model_motion.jaku_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/Neru_jaku_R.fbx");
	//	}
	//	else*/
	//	{
	//		model_motion.jaku_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_jaku_R.fbx");
	//	}
	//}
	//if (model_motion.jaku_L == nullptr)
	//{
	//	model_motion.jaku_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_jaku_L.fbx");
	//}

	////���U��
	//if (model_motion.thu_R == nullptr)
	//{
	//	model_motion.thu_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_thu_R.fbx");
	//}
	//if (model_motion.thu_L == nullptr)
	//{
	//	model_motion.thu_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_thu_L.fbx");
	//}

	////���U��(�΋�&����)
	//if (model_motion.kyo_R == nullptr)
	//{
	//	model_motion.kyo_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_kyo_R.fbx");
	//}
	//if (model_motion.kyo_L == nullptr)
	//{
	//	model_motion.kyo_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_kyo_L.fbx");
	//}

	////����U��
	//if (model_motion.d_jaku_R == nullptr)
	//{
	//	model_motion.d_jaku_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_d_jaku_R.fbx");
	//}
	//if (model_motion.d_jaku_L == nullptr)
	//{
	//	model_motion.d_jaku_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_d_jaku_L.fbx");
	//}

	////�����U��
	//if (model_motion.d_thu_R == nullptr)
	//{
	//	model_motion.d_thu_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_d_thu_R.fbx");
	//}
	//if (model_motion.d_thu_L == nullptr)
	//{
	//	model_motion.d_thu_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_d_thu_L.fbx");
	//}

	////�㋭�U��
	//if (model_motion.u_kyo_R == nullptr)
	//{
	//	model_motion.u_kyo_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_u_kyo_R.fbx");
	//}
	//if (model_motion.u_kyo_L == nullptr)
	//{
	//	model_motion.u_kyo_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_u_kyo_L.fbx");
	//}

	////�㋭�U��
	//if (model_motion.u_kyo_R == nullptr)
	//{
	//	model_motion.u_kyo_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_u_kyo_R.fbx");
	//}
	//if (model_motion.u_kyo_L == nullptr)
	//{
	//	model_motion.u_kyo_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_u_kyo_L.fbx");
	//}

	////���U��
	//if (model_motion.a_jaku_R == nullptr)
	//{
	//	model_motion.a_jaku_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_air_jaku_R.fbx");
	//}
	//if (model_motion.a_jaku_L == nullptr)
	//{
	//	model_motion.a_jaku_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_air_jaku_L.fbx");
	//}

	////�󒆍U��
	//if (model_motion.a_thu_R == nullptr)
	//{
	//	model_motion.a_thu_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_air_thu_R.fbx");
	//}
	//if (model_motion.a_thu_L == nullptr)
	//{
	//	model_motion.a_thu_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_air_thu_L.fbx");
	//}

	////�󋭍U��
	//if (model_motion.a_kyo_R == nullptr)
	//{
	//	model_motion.a_kyo_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_air_kyo_R.fbx");
	//}
	//if (model_motion.a_kyo_L == nullptr)
	//{
	//	model_motion.a_kyo_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_air_kyo_L.fbx");
	//}

	////��㋭�U��
	//if (model_motion.a_ukyo_R == nullptr)
	//{
	//	model_motion.a_ukyo_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_air_ukyo_R.fbx");
	//}
	//if (model_motion.a_ukyo_L == nullptr)
	//{
	//	model_motion.a_ukyo_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_air_ukyo_L.fbx");
	//}

	////�͂ݍU��
	//if (model_motion.steal_R == nullptr)
	//{
	//	model_motion.steal_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_steal_R.fbx");
	//}
	//if (model_motion.steal_L == nullptr)
	//{
	//	model_motion.steal_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_steal_L.fbx");
	//}

	////�����U��
	//if (model_motion.slow_R == nullptr)
	//{
	//	model_motion.slow_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_slow_R.fbx");
	//}
	//if (model_motion.slow_L == nullptr)
	//{
	//	model_motion.slow_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_slow_L.fbx");
	//}

	////�z�[�~���O�_�b�V���U��
	//if (model_motion.track_R == nullptr)
	//{
	//	model_motion.track_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_track_R.fbx");
	//}
	//if (model_motion.track_L == nullptr)
	//{
	//	model_motion.track_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_track_L.fbx");
	//}

	////�O��K�E�Z
	//if (model_motion.jaku_rh_R == nullptr)
	//{
	//	model_motion.jaku_rh_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_Rhurf_R.fbx");
	//}
	//if (model_motion.jaku_rh_L == nullptr)
	//{
	//	model_motion.jaku_rh_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_Rhurf_L.fbx");
	//}

	////���K�E�Z
	//if (model_motion.jaku_lh_R == nullptr)
	//{
	//	model_motion.jaku_lh_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_Lhurf_R.fbx");
	//}
	//if (model_motion.jaku_lh_L == nullptr)
	//{
	//	model_motion.jaku_lh_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_Lhurf_L.fbx");
	//}

	////���K�E�Z
	//if (model_motion.special_R == nullptr)
	//{
	//	model_motion.special_R = std::make_shared<Model>("./Data/FBX/Neru/AnimationR/knight_special_R.fbx");
	//}
	//if (model_motion.special_L == nullptr)
	//{
	//	model_motion.special_L = std::make_shared<Model>("./Data/FBX/Neru/AnimationL/knight_special_L.fbx");
	//}

	////model_motion�ɓǂݍ��񂾃f�[�^��ǉ����Ă���
	//{
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�U������(����Ƃ��đҋ@�����Ă���)
	//	model_motion.model_R_Attack.push_back(model_motion.jaku_R);	//��
	//	model_motion.model_R_Attack.push_back(model_motion.d_thu_R);	//��(����)
	//	model_motion.model_R_Attack.push_back(model_motion.kyo_R);		//����(���̎��ɏo�鋭�U��)
	//	model_motion.model_R_Attack.push_back(model_motion.d_jaku_R);	//����
	//	model_motion.model_R_Attack.push_back(model_motion.d_thu_R);	//����
	//	model_motion.model_R_Attack.push_back(model_motion.u_kyo_R);	//�㋭
	//	model_motion.model_R_Attack.push_back(model_motion.a_jaku_R);	//���U��
	//	model_motion.model_R_Attack.push_back(model_motion.a_thu_R);	//�󒆍U��
	//	model_motion.model_R_Attack.push_back(model_motion.a_kyo_R);	//�󋭍U��
	//	model_motion.model_R_Attack.push_back(model_motion.a_ukyo_R);	//��㋭�U��(�ł��グ�U��)
	//	model_motion.model_R_Attack.push_back(model_motion.steal_R);	//�͂�
	//	model_motion.model_R_Attack.push_back(model_motion.slow_R);	//����
	//	model_motion.model_R_Attack.push_back(model_motion.track_R);	//�z�[�~���O�_�b�V��
	//	model_motion.model_R_Attack.push_back(model_motion.jaku_rh_R);	//�O��K�E
	//	model_motion.model_R_Attack.push_back(model_motion.jaku_rh_R);	//�O���K�E
	//	model_motion.model_R_Attack.push_back(model_motion.jaku_rh_R);	//�O���K�E
	//	model_motion.model_R_Attack.push_back(model_motion.jaku_lh_R);	//���K�E
	//	model_motion.model_R_Attack.push_back(model_motion.jaku_lh_R);	//�㒆�K�E
	//	model_motion.model_R_Attack.push_back(model_motion.kyo_lh_R);	//�㋭�K�E
	//	model_motion.model_R_Attack.push_back(model_motion.special_R);	//�O���K�E
	//	model_motion.model_R_Attack.push_back(model_motion.disire_s_R);//�㒴�K�E
	//	model_motion.model_R_Attack.push_back(model_motion.disire_m_R);//�����Z
	//	model_motion.model_R_Attack.push_back(model_motion.extend_R);	//���G�U��
	//	model_motion.model_R_Attack.push_back(model_motion.thu_R);		//��̎��ɏo�钆�U��
	//	model_motion.model_R_Attack.push_back(model_motion.kyo_R);		//���̎��ɏo�鋭�U��
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�󒆑O��K�E
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�󒆑O���K�E
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�󒆑O���K�E
	//	model_motion.model_R_Attack.push_back(model_motion.jaku_lh_R);	//�󒆌��K�E
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�󒆌㒆�K�E
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�󒆌㋭�K�E
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�󒆑O���K�E
	//	model_motion.model_R_Attack.push_back(model_motion.wait_R);	//�󒆌㒴�K�E

	//}


	//{
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�U������(����Ƃ��đҋ@�����Ă���)
	//	model_motion.model_L_Attack.push_back(model_motion.jaku_L);	//��
	//	model_motion.model_L_Attack.push_back(model_motion.d_thu_L);	//��(����)
	//	model_motion.model_L_Attack.push_back(model_motion.kyo_L);		//����(���̎��ɏo�鋭�U��)
	//	model_motion.model_L_Attack.push_back(model_motion.d_jaku_L);	//����
	//	model_motion.model_L_Attack.push_back(model_motion.d_thu_L);	//����
	//	model_motion.model_L_Attack.push_back(model_motion.u_kyo_L);	//�㋭
	//	model_motion.model_L_Attack.push_back(model_motion.a_jaku_L);	//���U��
	//	model_motion.model_L_Attack.push_back(model_motion.a_thu_L);	//�󒆍U��
	//	model_motion.model_L_Attack.push_back(model_motion.a_kyo_L);	//�󋭍U��
	//	model_motion.model_L_Attack.push_back(model_motion.a_ukyo_L);	//��㋭�U��(�ł��グ�U��)
	//	model_motion.model_L_Attack.push_back(model_motion.steal_L);	//�͂�
	//	model_motion.model_L_Attack.push_back(model_motion.slow_L);	//����
	//	model_motion.model_L_Attack.push_back(model_motion.track_L);	//�z�[�~���O�_�b�V��
	//	model_motion.model_L_Attack.push_back(model_motion.jaku_rh_L);	//�O��K�E
	//	model_motion.model_L_Attack.push_back(model_motion.jaku_rh_L);	//�O���K�E
	//	model_motion.model_L_Attack.push_back(model_motion.jaku_rh_L);	//�O���K�E
	//	model_motion.model_L_Attack.push_back(model_motion.jaku_lh_L);	//���K�E
	//	model_motion.model_L_Attack.push_back(model_motion.jaku_lh_L);	//�㒆�K�E
	//	model_motion.model_L_Attack.push_back(model_motion.kyo_lh_L);	//�㋭�K�E
	//	model_motion.model_L_Attack.push_back(model_motion.special_L);	//�O���K�E
	//	model_motion.model_L_Attack.push_back(model_motion.disire_s_L);//�㒴�K�E
	//	model_motion.model_L_Attack.push_back(model_motion.disire_m_L);//�����Z
	//	model_motion.model_L_Attack.push_back(model_motion.extend_L);	//���G�U��
	//	model_motion.model_L_Attack.push_back(model_motion.thu_L);		//��̎��ɏo�钆�U��
	//	model_motion.model_L_Attack.push_back(model_motion.kyo_L);		//���̎��ɏo�鋭�U��
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�󒆑O��K�E
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�󒆑O���K�E
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�󒆑O���K�E
	//	model_motion.model_L_Attack.push_back(model_motion.jaku_lh_L);	//�󒆌��K�E
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�󒆌㒆�K�E
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�󒆌㋭�K�E
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�󒆑O���K�E
	//	model_motion.model_L_Attack.push_back(model_motion.wait_L);	//�󒆌㒴�K�E

	//}


	//------------------------------------------//
	//			�A�j���[�V�����ݒ�				//
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