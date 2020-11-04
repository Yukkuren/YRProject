#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"
#include <algorithm>
#include "World.h"


void Knight::LoadData(int color_number)
{
	attack_list.resize(1);
#if USE_IMGUI
	DEBUGAttackLoad();
	AttackClean();
	DEBUGAttackLoad();
#endif // 0
	//AttackLoad();



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
	//		�E�����ƍ�����

	//�ҋ@���[�V����
	if (model_motion.wait_R == nullptr)
	{
		model_motion.wait_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_wait_R.fbx");
	}
	if (model_motion.wait_L == nullptr)
	{
		model_motion.wait_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_wait_L.fbx");
	}

	//�X���C�h���[�V����
	if (model_motion.slid_R == nullptr)
	{
		model_motion.slid_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_slid_R.fbx");
	}
	if (model_motion.slid_L == nullptr)
	{
		model_motion.slid_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_slid_L.fbx");
	}

	//���Ⴊ��
	if (model_motion.squat_R == nullptr)
	{
		model_motion.squat_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_squat_R.fbx");
	}
	if (model_motion.squat_L == nullptr)
	{
		model_motion.squat_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_squat_L.fbx");
	}

	//����
	if (model_motion.walk_R == nullptr)
	{
		model_motion.walk_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_walk_R.fbx");
	}
	if (model_motion.walk_L == nullptr)
	{
		model_motion.walk_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_walk_L.fbx");
	}

	//���
	if (model_motion.back_R == nullptr)
	{
		model_motion.back_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_back_R.fbx");
	}
	if (model_motion.back_L == nullptr)
	{
		model_motion.back_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_back_L.fbx");
	}

	//�_�b�V��
	if (model_motion.dash_R == nullptr)
	{
		model_motion.dash_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_dash_R.fbx");
	}
	if (model_motion.dash_L == nullptr)
	{
		model_motion.dash_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_dash_L.fbx");
	}

	//�o�b�N�X�e�b�v
	if (model_motion.backstep_R == nullptr)
	{
		model_motion.backstep_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_backstep_R.fbx");
	}
	if (model_motion.backstep_L == nullptr)
	{
		model_motion.backstep_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_backstep_L.fbx");
	}

	//�󒆃_�b�V��
	if (model_motion.air_dash_R == nullptr)
	{
		model_motion.air_dash_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_air_dash_R.fbx");
	}
	if (model_motion.air_dash_L == nullptr)
	{
		model_motion.air_dash_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_air_dash_L.fbx");
	}

	//�󒆃o�b�N�X�e�b�v
	if (model_motion.air_back_R == nullptr)
	{
		model_motion.air_back_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_air_back_R.fbx");
	}
	if (model_motion.air_back_L == nullptr)
	{
		model_motion.air_back_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_air_back_L.fbx");
	}

	//�󂯐g
	if (model_motion.passive_R == nullptr)
	{
		model_motion.passive_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_passive_R.fbx");
	}
	if (model_motion.passive_L == nullptr)
	{
		model_motion.passive_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_passive_L.fbx");
	}

	//�W�����v
	if (model_motion.jump_R == nullptr)
	{
		model_motion.jump_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_jump_R.fbx");
	}
	if (model_motion.jump_L == nullptr)
	{
		model_motion.jump_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_jump_L.fbx");
	}

	//�󒆃W�����v
	if (model_motion.air_jump_R == nullptr)
	{
		model_motion.air_jump_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_air_jump_R_f.fbx");
	}
	if (model_motion.air_jump_L == nullptr)
	{
		model_motion.air_jump_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_air_jump_L_f.fbx");
	}

	//�C���g�����[�V����
	if (model_motion.intro_R == nullptr)
	{
		model_motion.intro_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_Intro_R.fbx");
	}
	if (model_motion.intro_L == nullptr)
	{
		model_motion.intro_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_Intro_L.fbx");
	}

	//�_���[�W���[�V����
	if (model_motion.damage_R_g_u == nullptr)
	{
		model_motion.damage_R_g_u = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_damage_R_g_u.fbx");
	}
	if (model_motion.damage_L_g_u == nullptr)
	{
		model_motion.damage_L_g_u = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_damage_L_g_u.fbx");
	}

	//��U��
	if (model_motion.jaku_R == nullptr)
	{
		model_motion.jaku_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_jaku_R.fbx");
	}
	if (model_motion.jaku_L == nullptr)
	{
		model_motion.jaku_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_jaku_L.fbx");
	}

	//���U��
	if (model_motion.thu_R == nullptr)
	{
		model_motion.thu_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_thu_R.fbx");
	}
	if (model_motion.thu_L == nullptr)
	{
		model_motion.thu_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_thu_L.fbx");
	}

	//���U��(�΋�&����)
	if (model_motion.kyo_R == nullptr)
	{
		model_motion.kyo_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_kyo_R.fbx");
	}
	if (model_motion.kyo_L == nullptr)
	{
		model_motion.kyo_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_kyo_L.fbx");
	}

	//����U��
	if (model_motion.d_jaku_R == nullptr)
	{
		model_motion.d_jaku_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_d_jaku_R.fbx");
	}
	if (model_motion.d_jaku_L == nullptr)
	{
		model_motion.d_jaku_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_d_jaku_L.fbx");
	}

	//�����U��
	if (model_motion.d_thu_R == nullptr)
	{
		model_motion.d_thu_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_d_thu_R.fbx");
	}
	if (model_motion.d_thu_L == nullptr)
	{
		model_motion.d_thu_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_d_thu_L.fbx");
	}

	//�㋭�U��
	if (model_motion.u_kyo_R == nullptr)
	{
		model_motion.u_kyo_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_u_kyo_R.fbx");
	}
	if (model_motion.u_kyo_L == nullptr)
	{
		model_motion.u_kyo_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_u_kyo_L.fbx");
	}

	//�㋭�U��
	if (model_motion.u_kyo_R == nullptr)
	{
		model_motion.u_kyo_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_u_kyo_R.fbx");
	}
	if (model_motion.u_kyo_L == nullptr)
	{
		model_motion.u_kyo_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_u_kyo_L.fbx");
	}

	//���U��
	if (model_motion.a_jaku_R == nullptr)
	{
		model_motion.a_jaku_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_air_jaku_R.fbx");
	}
	if (model_motion.a_jaku_L == nullptr)
	{
		model_motion.a_jaku_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_air_jaku_L.fbx");
	}

	//�󒆍U��
	if (model_motion.a_thu_R == nullptr)
	{
		model_motion.a_thu_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_air_thu_R.fbx");
	}
	if (model_motion.a_thu_L == nullptr)
	{
		model_motion.a_thu_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_air_thu_L.fbx");
	}

	//�󋭍U��
	if (model_motion.a_kyo_R == nullptr)
	{
		model_motion.a_kyo_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_air_kyo_R.fbx");
	}
	if (model_motion.a_kyo_L == nullptr)
	{
		model_motion.a_kyo_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_air_kyo_L.fbx");
	}

	//��㋭�U��
	if (model_motion.a_ukyo_R == nullptr)
	{
		model_motion.a_ukyo_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_air_ukyo_R.fbx");
	}
	if (model_motion.a_ukyo_L == nullptr)
	{
		model_motion.a_ukyo_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_air_ukyo_L.fbx");
	}

	//�͂ݍU��
	if (model_motion.steal_R == nullptr)
	{
		model_motion.steal_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_steal_R.fbx");
	}
	if (model_motion.steal_L == nullptr)
	{
		model_motion.steal_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_steal_L.fbx");
	}

	//�����U��
	if (model_motion.slow_R == nullptr)
	{
		model_motion.slow_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_slow_R.fbx");
	}
	if (model_motion.slow_L == nullptr)
	{
		model_motion.slow_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_slow_L.fbx");
	}

	//�z�[�~���O�_�b�V���U��
	if (model_motion.track_R == nullptr)
	{
		model_motion.track_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_track_R.fbx");
	}
	if (model_motion.track_L == nullptr)
	{
		model_motion.track_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_track_L.fbx");
	}

	//���K�E�Z
	if (model_motion.special_R == nullptr)
	{
		model_motion.special_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_special_R.fbx");
	}
	if (model_motion.special_L == nullptr)
	{
		model_motion.special_L = std::make_shared<Model>("./Data/FBX/Knight/AnimationL/knight_special_L.fbx");
	}

	{
		model_motion.model_R.push_back(model_motion.wait_R);	//�U������(����Ƃ��đҋ@�����Ă���)
		model_motion.model_R.push_back(model_motion.jaku_R);	//��
		model_motion.model_R.push_back(model_motion.d_thu_R);	//��(����)
		model_motion.model_R.push_back(model_motion.kyo_R);		//����(���̎��ɏo�鋭�U��)
		model_motion.model_R.push_back(model_motion.d_jaku_R);	//����
		model_motion.model_R.push_back(model_motion.d_thu_R);	//����
		model_motion.model_R.push_back(model_motion.u_kyo_R);	//�㋭
		model_motion.model_R.push_back(model_motion.a_jaku_R);	//���U��
		model_motion.model_R.push_back(model_motion.a_thu_R);	//�󒆍U��
		model_motion.model_R.push_back(model_motion.a_kyo_R);	//�󋭍U��
		model_motion.model_R.push_back(model_motion.a_ukyo_R);	//��㋭�U��(�ł��グ�U��)
		model_motion.model_R.push_back(model_motion.steal_R);	//�͂�
		model_motion.model_R.push_back(model_motion.slow_R);	//����
		model_motion.model_R.push_back(model_motion.track_R);	//�z�[�~���O�_�b�V��
		model_motion.model_R.push_back(model_motion.jaku_rh_R);	//�O��K�E
		model_motion.model_R.push_back(model_motion.thu_rh_R);	//�O���K�E
		model_motion.model_R.push_back(model_motion.kyo_rh_R);	//�O���K�E
		model_motion.model_R.push_back(model_motion.jaku_lh_R);	//���K�E
		model_motion.model_R.push_back(model_motion.thu_lh_R);	//�㒆�K�E
		model_motion.model_R.push_back(model_motion.kyo_lh_R);	//�㋭�K�E
		model_motion.model_R.push_back(model_motion.special_R);	//�O���K�E
		model_motion.model_R.push_back(model_motion.disire_s_R);//�㒴�K�E
		model_motion.model_R.push_back(model_motion.disire_m_R);//�����Z
		model_motion.model_R.push_back(model_motion.extend_R);	//���G�U��
		model_motion.model_R.push_back(model_motion.thu_R);		//��̎��ɏo�钆�U��
		model_motion.model_R.push_back(model_motion.kyo_R);		//���̎��ɏo�鋭�U��
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆑O��K�E
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆑O���K�E
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆑O���K�E
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆌��K�E
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆌㒆�K�E
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆌㋭�K�E
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆑O���K�E
		model_motion.model_R.push_back(model_motion.wait_R);	//�󒆌㒴�K�E

	}


	{
		model_motion.model_L.push_back(model_motion.wait_L);	//�U������(����Ƃ��đҋ@�����Ă���)
		model_motion.model_L.push_back(model_motion.jaku_L);	//��
		model_motion.model_L.push_back(model_motion.d_thu_L);	//��(����)
		model_motion.model_L.push_back(model_motion.kyo_L);		//����(���̎��ɏo�鋭�U��)
		model_motion.model_L.push_back(model_motion.d_jaku_L);	//����
		model_motion.model_L.push_back(model_motion.d_thu_L);	//����
		model_motion.model_L.push_back(model_motion.u_kyo_L);	//�㋭
		model_motion.model_L.push_back(model_motion.a_jaku_L);	//���U��
		model_motion.model_L.push_back(model_motion.a_thu_L);	//�󒆍U��
		model_motion.model_L.push_back(model_motion.a_kyo_L);	//�󋭍U��
		model_motion.model_L.push_back(model_motion.a_ukyo_L);	//��㋭�U��(�ł��グ�U��)
		model_motion.model_L.push_back(model_motion.steal_L);	//�͂�
		model_motion.model_L.push_back(model_motion.slow_L);	//����
		model_motion.model_L.push_back(model_motion.track_L);	//�z�[�~���O�_�b�V��
		model_motion.model_L.push_back(model_motion.jaku_rh_L);	//�O��K�E
		model_motion.model_L.push_back(model_motion.thu_rh_L);	//�O���K�E
		model_motion.model_L.push_back(model_motion.kyo_rh_L);	//�O���K�E
		model_motion.model_L.push_back(model_motion.jaku_lh_L);	//���K�E
		model_motion.model_L.push_back(model_motion.thu_lh_L);	//�㒆�K�E
		model_motion.model_L.push_back(model_motion.kyo_lh_L);	//�㋭�K�E
		model_motion.model_L.push_back(model_motion.special_L);	//�O���K�E
		model_motion.model_L.push_back(model_motion.disire_s_L);//�㒴�K�E
		model_motion.model_L.push_back(model_motion.disire_m_L);//�����Z
		model_motion.model_L.push_back(model_motion.extend_L);	//���G�U��
		model_motion.model_L.push_back(model_motion.thu_L);		//��̎��ɏo�钆�U��
		model_motion.model_L.push_back(model_motion.kyo_L);		//���̎��ɏo�鋭�U��
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆑O��K�E
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆑O���K�E
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆑O���K�E
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆌��K�E
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆌㒆�K�E
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆌㋭�K�E
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆑O���K�E
		model_motion.model_L.push_back(model_motion.wait_L);	//�󒆌㒴�K�E

	}


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