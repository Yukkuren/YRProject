#include "PlayerBase.h"
#include "../../framework.h"
#include <fstream>
#include "Player_name_list.h"

//-------------------------------------------------------------------------------------
//							�v���C���[�X�e�[�^�X�����pcpp
//-------------------------------------------------------------------------------------
//�E�L�������Ƃ̃X�e�[�^�X(�_�b�V�����x�Ȃ�)�̒������s���A�f�[�^�̃��[�h�A�����o�����s���Ă���
//-------------------------------------------------------------------------------------




//�e�L�X�g�t�@�C������ǂݍ���
void CharaStateParameter::Load(PLSELECT chara_name)
{
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara = GetName().chara_name_list[scastI(chara_name)];

	std::string Chara_level = f_level + chara + std::string("/CharaState.txt");
	std::ifstream ifs(Chara_level);

	ifs >> walkspeed;
	ifs >> dashspeed;
	ifs >> backstepS;
	ifs >> backstepD;
	ifs >> stepspeed;
	ifs >> stepD;
	ifs >> jump_max;
	ifs >> high_jump_max;
	ifs >> jump_speed;
	ifs >> high_jump_speed;
	ifs >> brake_speed;
	ifs >> track_speed;
	ifs >> passive_speed.x;
	ifs >> passive_speed.y;
	ifs >> passive_brake;
	ifs >> max_hp;

	//������������G���[���o��
	if (ifs.fail())
	{
		assert("ifs failled");
	}

	ifs.close();
}


//�e�L�X�g�t�@�C���ɏ����o��
void CharaStateParameter::Save(PLSELECT chara_name)
{
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara = GetName().chara_name_list[scastI(chara_name)];

	std::string Chara_level = f_level + chara + std::string("/CharaState.txt");
	std::ofstream outputfile(Chara_level);

	outputfile << walkspeed << std::endl;
	outputfile << dashspeed << std::endl;
	outputfile << backstepS << std::endl;
	outputfile << backstepD << std::endl;
	outputfile << stepspeed << std::endl;
	outputfile << stepD << std::endl;
	outputfile << jump_max << std::endl;
	outputfile << high_jump_max << std::endl;
	outputfile << jump_speed << std::endl;
	outputfile << high_jump_speed << std::endl;
	outputfile << brake_speed << std::endl;
	outputfile << track_speed << std::endl;
	outputfile << passive_speed.x << std::endl;
	outputfile << passive_speed.y << std::endl;
	outputfile << passive_brake << std::endl;
	outputfile << max_hp << std::endl;

	outputfile.close();
}


//�f�o�b�O�p�c�[���`��
void CharaStateParameter::Draw()
{
#ifdef EXIST_IMGUI

	if (Get_Use_ImGui())
	{
		if (ImGui::TreeNode(u8"�X�e�[�^�X����"))
		{
			ImGui::SliderFloat(u8"��ޑ��x", &walkspeed, 0.0f, 100.0f);
			ImGui::SliderFloat(u8"�_�b�V���X�s�[�h", &dashspeed, 0.0f, 100.0f);
			ImGui::SliderFloat(u8"�o�b�N�X�e�b�v���x", &backstepS, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"�o�b�N�X�e�b�v�u���[�L", &backstepD, 0.0f, 800.0f);
			ImGui::SliderFloat(u8"�X�e�b�v���x", &stepspeed, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"�X�e�b�v�u���[�L", &stepD, 0.0f, 800.0f);
			ImGui::SliderFloat(u8"�W�����v�����x", &jump_speed, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"�W�����v�ő呬�x", &jump_max, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"�n�C�W�����v�ő呬�x", &high_jump_speed, 0.0f, 200.0f);
			ImGui::SliderFloat(u8"�n�C�W�����v�ő呬�x", &high_jump_max, 0.0f, 200.0f);
			ImGui::SliderFloat(u8"�_�b�V���u���[�L", &brake_speed, 0.0f, 20000.0f);
			ImGui::SliderFloat(u8"�z�[�~���O�_�b�V�����x", &track_speed, 0.0f, 200.0f);
			ImGui::SliderFloat(u8"�󂯐g�����xX", &passive_speed.x, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"�󂯐g�����xY", &passive_speed.y, 0.0f, 300.0f);
			ImGui::SliderFloat(u8"�󂯐g�u���[�L", &passive_brake, 0.0f, 500.0f);
			ImGui::SliderFloat(u8"�ő�HP", &max_hp, 0.0f, 3000.0f);
			ImGui::TreePop();
		}
	}

#endif // EXIST_IMGUI

}








//-------------------------------------------------------------------------------------
//							�L�����X�e�[�^�X����
//-------------------------------------------------------------------------------------
//�E�L�������Ƃ̓����蔻��Ȃǂ̒������s���A�f�[�^�̃��[�h�A�����o�����s���Ă���
//-------------------------------------------------------------------------------------


//�����o���ꂽ�e�L�X�g�f�[�^����U�����X�g�𐶐�����
bool Player::AttackLoad()
{
	attack_list.resize(scastI(AttackState::ATTACK_END));

	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name = GetName().chara_name_list[scastI(this->chara_name)];

	std::string AP_level = f_level + chara_name + std::string("/AttackParam.txt");
	std::ifstream ifs(AP_level);

	ifs >> jump_later;
	ifs >> dash_later;

	for (int list = 0; list < attack_list.size(); list++)
	{
		/*if (list == scastI(AttackState::BURST)|| list == scastI(AttackState::COMBOBURST))
		{
			continue;
		}*/

		attack_list[list].attack_name = static_cast<AttackState>(list);
		ifs >> attack_list[list].later;
		ifs >> attack_list[list].attack_max;
		int pad, com, stick, aid, real, next, result, ground, effect_k, anim_k, func_n;
		EffectParameter e_param;
		ifs >> pad;
		ifs >> com;
		attack_list[list].linkage_button = static_cast<PAD>(pad);
		attack_list[list].linkage_command = static_cast<Command>(com);
		ifs >> ground;
		attack_list[list].ground_on = static_cast<Ground_C>(ground);
		ifs >> attack_list[list].squat_on;
		ifs >> attack_list[list].need_power;
		ifs >> stick;
		ifs >> aid;
		ifs >> real;
		attack_list[list].linkage_stick = static_cast<PAD>(stick);
		attack_list[list].aid_attack_name = static_cast<AttackState>(aid);
		attack_list[list].real_attack = static_cast<AttackState>(real);
		attack_list[list].attack_single.resize(attack_list[list].attack_max);
		ifs >> attack_list[list].speed_on;
		ifs >> attack_list[list].speed.x;
		ifs >> attack_list[list].speed.y;
		ifs >> attack_list[list].speed.z;
		ifs >> attack_list[list].advance_speed;
		ifs >> next;
		ifs >> result;
		attack_list[list].combo = static_cast<AttackState>(next);
		attack_list[list].conditions_hit = static_cast<HitResult>(result);
		ifs >> attack_list[list].timer;
		ifs >> attack_list[list].traject_on;
		ifs >> anim_k;
		ifs >> func_n;
		attack_list[list].anim_kind = static_cast<AttackState>(anim_k);
		attack_list[list].function_num = static_cast<AT_Function_List>(func_n);


		ifs >> effect_k;
		attack_list[list].effect_param.effect_kind = static_cast<EffectKind>(effect_k);
		ifs >> attack_list[list].effect_param.distance.x;
		ifs >> attack_list[list].effect_param.distance.y;
		ifs >> attack_list[list].effect_param.distance.z;
		ifs >> attack_list[list].effect_param.scale.x;
		ifs >> attack_list[list].effect_param.scale.y;
		ifs >> attack_list[list].effect_param.scale.z;
		ifs >> attack_list[list].effect_param.axis.x;
		ifs >> attack_list[list].effect_param.axis.y;
		ifs >> attack_list[list].effect_param.axis.z;
		ifs >> attack_list[list].effect_param.angle;
		ifs >> attack_list[list].effect_param.rightORleft;

		/*ifs >> effect_k;
		e_param.effect_kind = static_cast<EffectKind>(effect_k);
		ifs >> e_param.distance.x;
		ifs >> e_param.distance.y;
		ifs >> e_param.distance.z;
		ifs >> e_param.scale.x;
		ifs >> e_param.scale.y;
		ifs >> e_param.scale.z;
		ifs >> e_param.axis.x;
		ifs >> e_param.axis.y;
		ifs >> e_param.axis.z;
		ifs >> e_param.angle;
		ifs >> e_param.rightORleft;*/


		//�U���񐔂��Ƃ̃p�����[�^�ǂݍ���
		if (!attack_list[list].attack_single.empty())
		{
			for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
			{
				ifs >> attack_list[list].attack_single[sin].fream;
				ifs >> attack_list[list].attack_single[sin].quantity;
				attack_list[list].attack_single[sin].parameter.resize(attack_list[list].attack_single[sin].quantity);
				if (!attack_list[list].attack_single[sin].parameter.empty())
				{
					for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
					{
						//�U����������p�����[�^�[�ǂݍ���
						ifs >> attack_list[list].attack_single[sin].parameter[para].distance.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].distance.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].size.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].size.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.damage;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.HB_timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.hitback.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.hitback.y;
						int type = 0;
						ifs >> type;
						attack_list[list].attack_single[sin].parameter[para].type = static_cast<AttackKind>(type);
						ifs >> attack_list[list].attack_single[sin].parameter[para].knockback;
						ifs >> attack_list[list].attack_single[sin].parameter[para].gaugeout;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.steal_timer;
						int hs;
						ifs >> hs;
						attack_list[list].attack_single[sin].parameter[para].HS_timer = static_cast<HitStopTime>(hs);
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_back.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_back.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_shaving;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.guard_timer;
						ifs >> attack_list[list].attack_single[sin].parameter[para].param.combo_correction;
						ifs >> attack_list[list].attack_single[sin].parameter[para].gauge_get;

						ifs >> effect_k;
						attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind = static_cast<EffectKind>(effect_k);
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.distance.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.distance.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.distance.z;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.axis.x;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.axis.y;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.axis.z;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.angle;
						ifs >> attack_list[list].attack_single[sin].parameter[para].effect_param.rightORleft;

						//attack_list[list].attack_single[sin].parameter[para].effect_param = e_param;

					}
				}
			}
		}
	}
	//������������G���[���o��
	if (ifs.fail())
	{
		assert("ifs failled");
	}
	//�t�@�C�������
	ifs.close();

	//�A�j���[�V���������l��ǂݍ���
	std::string Anim_level = f_level + chara_name + std::string("/AnimParam.txt");
	std::ifstream anim_ifs(Anim_level);
	for (int list = 0; list < ac_attack.size(); list++)
	{
		/*if (list == scastI(AttackState::BURST) || list == scastI(AttackState::COMBOBURST))
		{
			continue;
		}*/
		anim_ifs >> ac_attack[list].fream;
		anim_ifs >> ac_attack[list].timer;
		anim_ifs >> ac_attack[list].later;
	}
	//������������G���[���o��
	if (anim_ifs.fail())
	{
		assert("anim_ifs failled");
	}
	//�t�@�C�������
	anim_ifs.close();



	//�A�j���[�V���������l��ǂݍ���
	std::string Anim_Act_level = f_level + chara_name + std::string("/AnimActParam.txt");
	std::ifstream anim_act_ifs(Anim_Act_level);
	for (int list = 0; list < ac_act.size(); list++)
	{
		anim_act_ifs >> ac_act[list].fream;
		anim_act_ifs >> ac_act[list].timer;
		anim_act_ifs >> ac_act[list].later;
	}
	//������������G���[���o��
	if (anim_act_ifs.fail())
	{
		assert("anim_act_ifs failled");
	}
	//�t�@�C�������
	anim_act_ifs.close();


	//X�{�^���R���{���X�g��ǂݍ���
	std::string ComboX_level = f_level + chara_name + std::string("/Combo_X.txt");
	std::ifstream combo_X_ifs(ComboX_level);
	int size = -1;
	int  com = 0;
	combo_X_ifs >> size;
	if (size > 0)
	{
		combolist_X.combolist.resize(size);

		if (!combolist_X.combolist.empty())
		{
			for (int list = 0; list < combolist_X.combolist.size(); list++)
			{
				combo_X_ifs >> com;
				combolist_X.combolist[list] = static_cast<AttackState>(com);
			}
		}
	}
	//������������G���[���o��
	if (combo_X_ifs.fail())
	{
		assert("combo_X_ifs failled");
	}
	//�t�@�C�������
	combo_X_ifs.close();


	//Y�{�^���R���{���X�g��ǂݍ���
	std::string ComboY_level = f_level + chara_name + std::string("/Combo_Y.txt");
	std::ifstream combo_Y_ifs(ComboY_level);
	int size_y = -1;
	int  com_y = 0;
	combo_Y_ifs >> size_y;
	if (size_y > 0)
	{
		combolist_Y.combolist.resize(size_y);

		if (!combolist_Y.combolist.empty())
		{
			for (int list = 0; list < combolist_Y.combolist.size(); list++)
			{
				combo_Y_ifs >> com_y;
				combolist_Y.combolist[list] = static_cast<AttackState>(com_y);
			}
		}
	}
	//������������G���[���o��
	if (combo_Y_ifs.fail())
	{
		assert("combo_Y_ifs failled");
	}
	//�t�@�C�������
	combo_Y_ifs.close();


	//B�{�^���R���{���X�g��ǂݍ���
	std::string ComboB_level = f_level + chara_name + std::string("/Combo_B.txt");
	std::ifstream combo_B_ifs(ComboB_level);
	int size_b = -1;
	int  com_b = 0;
	combo_B_ifs >> size_b;
	if (size_b > 0)
	{
		combolist_B.combolist.resize(size_b);

		if (!combolist_B.combolist.empty())
		{
			for (int list = 0; list < combolist_B.combolist.size(); list++)
			{
				combo_B_ifs >> com_b;
				combolist_B.combolist[list] = static_cast<AttackState>(com_b);
			}
		}
	}
	//������������G���[���o��
	if (combo_B_ifs.fail())
	{
		assert("combo_B_ifs failled");
	}
	//�t�@�C�������
	combo_B_ifs.close();


	//�����蔻�蒲���l��ǂݍ���
	std::string Hit_level = f_level + chara_name + std::string("/HitParam.txt");
	std::ifstream hit_ifs(Hit_level);
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			//�s���������蔻��ǂݍ���
			hit_ifs >> hitparam_list[list].act_parameter[act].distance.x;
			hit_ifs >> hitparam_list[list].act_parameter[act].distance.y;
			hit_ifs >> hitparam_list[list].act_parameter[act].size.x;
			hit_ifs >> hitparam_list[list].act_parameter[act].size.y;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			/*if (atk == scastI(AttackState::BURST) || atk == scastI(AttackState::COMBOBURST))
			{
				continue;
			}*/
			//�U���������蔻��ǂݍ���
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].distance.y;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.x;
			hit_ifs >> hitparam_list[list].attack_parameter[atk].size.y;
		}
	}
	//������������G���[���o��
	if (hit_ifs.fail())
	{
		assert("hit_ifs failled");
	}
	//�t�@�C�������
	hit_ifs.close();

	//�L�����X�e�[�^�X��ǂݍ���
	chara_state.Load(this->chara_name);

	return true;
}




//�e�L�X�g�f�[�^�������o��
bool Player::AttackWrite()
{
	//AttackState�̏��ɕۑ�����
	std::string f_level = std::string("./Data/CharaParameter/");
	std::string chara_name = GetName().chara_name_list[scastI(this->chara_name)];

	std::string AP_level = f_level + chara_name + std::string("/AttackParam.txt");
	std::ofstream outputfile(AP_level);

	outputfile << jump_later << std::endl;
	outputfile << dash_later << std::endl;

	for (int list = 0; list < attack_list.size(); list++)
	{
		outputfile << attack_list[list].later << std::endl;
		outputfile << attack_list[list].attack_max << std::endl;
		outputfile << scastI(attack_list[list].linkage_button) << std::endl;
		outputfile << scastI(attack_list[list].linkage_command) << std::endl;
		outputfile << scastI(attack_list[list].ground_on) << std::endl;
		outputfile << attack_list[list].squat_on << std::endl;
		outputfile << attack_list[list].need_power << std::endl;
		outputfile << scastI(attack_list[list].linkage_stick) << std::endl;
		outputfile << scastI(attack_list[list].aid_attack_name) << std::endl;
		outputfile << scastI(attack_list[list].real_attack) << std::endl;
		outputfile << attack_list[list].speed_on << std::endl;
		outputfile << attack_list[list].speed.x << std::endl;
		outputfile << attack_list[list].speed.y << std::endl;
		outputfile << attack_list[list].speed.z << std::endl;
		outputfile << attack_list[list].advance_speed << std::endl;
		outputfile << scastI(attack_list[list].combo) << std::endl;
		outputfile << scastI(attack_list[list].conditions_hit) << std::endl;
		outputfile << attack_list[list].timer << std::endl;
		outputfile << attack_list[list].traject_on << std::endl;

		outputfile << scastI(attack_list[list].anim_kind) << std::endl;
		outputfile << scastI(attack_list[list].function_num) << std::endl;

		//�U�����X�g�̃G�t�F�N�g�����l
		outputfile << scastI(attack_list[list].effect_param.effect_kind) << std::endl;
		outputfile << attack_list[list].effect_param.distance.x << std::endl;
		outputfile << attack_list[list].effect_param.distance.y << std::endl;
		outputfile << attack_list[list].effect_param.distance.z << std::endl;
		outputfile << attack_list[list].effect_param.scale.x << std::endl;
		outputfile << attack_list[list].effect_param.scale.y << std::endl;
		outputfile << attack_list[list].effect_param.scale.z << std::endl;
		outputfile << attack_list[list].effect_param.axis.x << std::endl;
		outputfile << attack_list[list].effect_param.axis.y << std::endl;
		outputfile << attack_list[list].effect_param.axis.z << std::endl;
		outputfile << attack_list[list].effect_param.angle << std::endl;
		outputfile << attack_list[list].effect_param.rightORleft << std::endl;

		//�U���񐔂��Ƃ̃p�����[�^�����o��
		if (!attack_list[list].attack_single.empty())
		{
			for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
			{
				outputfile << attack_list[list].attack_single[sin].fream << std::endl;
				outputfile << attack_list[list].attack_single[sin].quantity << std::endl;
				if (!attack_list[list].attack_single[sin].parameter.empty())
				{
					for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
					{
						//�U����������p�����[�^�[�����o��
						outputfile << attack_list[list].attack_single[sin].parameter[para].distance.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].distance.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].size.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].size.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].timer << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.damage << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.HB_timer << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.hitback.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.hitback.y << std::endl;
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].type) << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].knockback << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].gaugeout << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.steal_timer << std::endl;
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].HS_timer) << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_back.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_back.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_shaving << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].param.guard_timer << std::endl;

						outputfile << attack_list[list].attack_single[sin].parameter[para].param.combo_correction << std::endl;
						if (!attack_list[list].attack_single[sin].parameter[para].gaugeout)
						{
							outputfile << attack_list[list].attack_single[sin].parameter[para].gauge_get << std::endl;
						}
						else
						{
							outputfile << float(0.0) << std::endl;
						}


						//�G�t�F�N�g�����l�����o��
						outputfile << scastI(attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind) << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.distance.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.distance.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.distance.z << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.axis.x << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.axis.y << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.axis.z << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.angle << std::endl;
						outputfile << attack_list[list].attack_single[sin].parameter[para].effect_param.rightORleft << std::endl;

					}
				}
			}
		}
	}

	outputfile.close();

	//�A�j���[�V���������l�������o��
	std::string Anim_level = f_level + chara_name + std::string("/AnimParam.txt");
	std::ofstream animout(Anim_level);
	for (int list = 0; list < ac_attack.size(); list++)
	{
		animout << ac_attack[list].fream << std::endl;
		animout << ac_attack[list].timer << std::endl;
		animout << ac_attack[list].later << std::endl;
	}

	animout.close();

	std::string Anim_Act_level = f_level + chara_name + std::string("/AnimActParam.txt");
	std::ofstream animactout(Anim_Act_level);
	for (int list = 0; list < ac_act.size(); list++)
	{
		animactout << ac_act[list].fream << std::endl;
		animactout << ac_act[list].timer << std::endl;
		animactout << ac_act[list].later << std::endl;
	}

	animactout.close();

	//�����蔻�蒲���l�������o��
	std::string Hit_level = f_level + chara_name + std::string("/HitParam.txt");
	std::ofstream hitout(Hit_level);
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			//�s���������蔻�菑������
			hitout << hitparam_list[list].act_parameter[act].distance.x << std::endl;
			hitout << hitparam_list[list].act_parameter[act].distance.y << std::endl;
			hitout << hitparam_list[list].act_parameter[act].size.x << std::endl;
			hitout << hitparam_list[list].act_parameter[act].size.y << std::endl;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			//�U���������蔻�菑������
			hitout << hitparam_list[list].attack_parameter[atk].distance.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].distance.y << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.x << std::endl;
			hitout << hitparam_list[list].attack_parameter[atk].size.y << std::endl;
		}
	}
	hitout.close();

	std::string ComboX_level = f_level + chara_name + std::string("/Combo_X.txt");
	std::ofstream combo_X_out(ComboX_level);
	if (!combolist_X.combolist.empty())
	{
		combo_X_out << combolist_X.combolist.size() << std::endl;
		for (int list = 0; list < combolist_X.combolist.size(); list++)
		{
			//X�{�^���R���{���X�g��������
			combo_X_out << scastI(combolist_X.combolist[list]) << std::endl;
		}
	}
	combo_X_out.close();


	std::string ComboY_level = f_level + chara_name + std::string("/Combo_Y.txt");
	std::ofstream combo_Y_out(ComboY_level);
	if (!combolist_Y.combolist.empty())
	{
		combo_Y_out << combolist_Y.combolist.size() << std::endl;
		for (int list = 0; list < combolist_Y.combolist.size(); list++)
		{
			//Y�{�^���R���{���X�g��������
			combo_Y_out << scastI(combolist_Y.combolist[list]) << std::endl;
		}
	}
	combo_Y_out.close();


	std::string ComboB_level = f_level + chara_name + std::string("/Combo_B.txt");
	std::ofstream combo_B_out(ComboB_level);
	if (!combolist_B.combolist.empty())
	{
		combo_B_out << combolist_B.combolist.size() << std::endl;
		for (int list = 0; list < combolist_B.combolist.size(); list++)
		{
			//B�{�^���R���{���X�g��������
			combo_B_out << scastI(combolist_B.combolist[list]) << std::endl;
		}
	}
	combo_B_out.close();

	//�L�����X�e�[�^�X�������o��
	chara_state.Save(this->chara_name);

	return true;
}




//�p�����[�^�[��S�ď���������
bool Player::AttackClean()
{
	jump_later = 1.0f;
	dash_later = 1.0f;

	//�U���p�����[�^�[��S�ď���������
	for (int list = 0; list < attack_list.size(); list++)
	{
		attack_list[list].later = 0.1f;
		attack_list[list].attack_max = 1;
		attack_list[list].attack_single.resize(attack_list[list].attack_max);
		ac_attack[list].fream = 1.0f;
		ac_attack[list].timer = 1.0f;
		ac_attack[list].later = 1.0f;
		attack_list[list].now_attack_num = 0;
		attack_list[list].linkage_button = PAD::BUTTOM_END;
		attack_list[list].linkage_command = Command::NOCOMMAND;
		attack_list[list].ground_on = attack_list[list].ground_on;
		attack_list[list].squat_on = false;
		attack_list[list].need_power = 0;
		attack_list[list].linkage_stick = PAD::BUTTOM_END;
		attack_list[list].aid_attack_name = AttackState::NONE;
		attack_list[list].attack_name = static_cast<AttackState>(list);
		attack_list[list].real_attack = attack_list[list].attack_name;
		attack_list[list].speed_on = false;
		attack_list[list].speed = YR_Vector3(0.0f, 0.0f);
		attack_list[list].advance_speed = 0.0f;
		attack_list[list].combo = attack_list[list].attack_name;
		attack_list[list].conditions_hit = HitResult::HIT;
		attack_list[list].timer = 0.0f;
		attack_list[list].traject_on = true;
		//�U���񐔂��Ƃ̃p�����[�^������
		for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
		{
			attack_list[list].attack_single[sin].fream = 0.1f;
			attack_list[list].attack_single[sin].quantity = 1;
			attack_list[list].attack_single[sin].parameter.resize(attack_list[list].attack_single[sin].quantity);
			for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
			{
				//�U����������p�����[�^�[������
				attack_list[list].attack_single[sin].parameter[para].distance.x = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].distance.y = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].size.x = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].size.y = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].timer = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].param.damage = 1.0f;
				attack_list[list].attack_single[sin].parameter[para].param.HB_timer = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].param.hitback.x = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].param.hitback.y = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].type = AttackKind::MIDDLE;
				attack_list[list].attack_single[sin].parameter[para].knockback = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].gaugeout = false;
				attack_list[list].attack_single[sin].parameter[para].param.steal_timer = 0.1f;
				attack_list[list].attack_single[sin].parameter[para].HS_timer = HitStopTime::NORMAL;
				attack_list[list].attack_single[sin].parameter[para].gauge_get = 0.1f;
			}
		}
	}

	//�A�j���[�V���������l��S�ď���������
	for (int list = 0; list < ac_attack.size(); list++)
	{
		ac_attack[list].fream = 1.0f;
		ac_attack[list].timer = 1.0f;
		ac_attack[list].later = 1.0f;
	}
	for (int list = 0; list < ac_act.size(); list++)
	{
		ac_act[list].fream = 1.0f;
		ac_act[list].timer = 1.0f;
		ac_act[list].later = 1.0f;
	}

	//�����蔻�蒲���l���S�ď���������
	for (int list = 0; list < hitparam_list.size(); list++)
	{
		for (int act = 0; act < hitparam_list[list].act_parameter.size(); act++)
		{
			hitparam_list[list].act_parameter[act].distance = YR_Vector3(0.0f, 0.0f);
			hitparam_list[list].act_parameter[act].size = YR_Vector3(1.0f, 1.0f);
			//hitparam_list[list].act_parameter[act].state = HitBoxState::NOGUARD;
		}
		for (int atk = 0; atk < hitparam_list[list].attack_parameter.size(); atk++)
		{
			hitparam_list[list].attack_parameter[atk].distance = YR_Vector3(0.0f, 0.0f);
			hitparam_list[list].attack_parameter[atk].size = YR_Vector3(1.0f, 1.0f);
			//hitparam_list[list].attack_parameter[atk].state = HitBoxState::NOGUARD;
		}
	}
	return true;
}




//�f�o�b�O�p�c�[���\��
void Player::DrawDEBUG(
	YRShader* geoshader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	//Debug�\���֐�

#ifdef EXIST_IMGUI
	if (Get_Debug_Draw())
	{

		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].Draw(geoshader, view, projection, light_direction, light_color, ambient_color);
		}

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

		if (!projectile_atk.empty())
		{
			for (int i = 0; i < projectile_atk.size(); i++)
			{
				if (projectile_atk[i].attack)
				{
					if (projectile_atk[i].hit_ok)
					{
						projectile_atk[i].Draw(geoshader, view, projection, light_direction, light_color, ambient_color);
					}
				}
			}
		}
	}

	//�U�����X�g�쐬�p����
	if (Get_Use_ImGui())
	{

		std::string p1_hp = std::to_string(hp);

		FRAMEWORK.font->Begin(FRAMEWORK.context.Get());
		if (now_player > 1)
		{
			FRAMEWORK.font->Draw(
				static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f) + 200.0f,
				100.0f, p1_hp.c_str());
		}
		else
		{
			FRAMEWORK.font->Draw(
				static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f) - 500.0f,
				100.0f, p1_hp.c_str());
		}

		FRAMEWORK.font->End(FRAMEWORK.context.Get());

		std::string now_play = std::to_string(now_player);
		now_play += std::string("P : ") + GetName().chara_name_list[scastI(this->chara_name)];
		ImGui::Begin(now_play.c_str());


		//��������U�����蒲��
		static float save_timer = 0.0f;
		static float init_timer = 0.0f;
		static float load_timer = 0.0f;
		if (ImGui::Button(u8"�Z�[�u"))
		{
			ImGui::OpenPopup("Save");
		}ImGui::SameLine();
		if (ImGui::BeginPopup("Save"))
		{
			ImGui::Text(u8"�Z�[�u���܂����H");
			if (ImGui::Button(u8"�͂�"))
			{
				if (AttackWrite())
				{
					save_timer = 2.0f;
					ImGui::CloseCurrentPopup();
				}
			}
			if (ImGui::Button(u8"������"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		/*if (pad->x_input[scastI(PAD::SELECT)] == 1)
		{
			ImGui::OpenPopup("Load");
		}*/

		if (ImGui::Button(u8"���[�h"))
		{
			ImGui::OpenPopup("Load");
		}ImGui::SameLine();
		if (ImGui::BeginPopup("Load"))
		{
			ImGui::Text(u8"���[�h���܂����H");
			if (ImGui::Button(u8"�͂�") || pad->x_input[scastI(PAD::X)] == 1)
			{
				if (AttackLoad())
				{
					load_timer = 2.0f;
					ImGui::CloseCurrentPopup();
				}
			}
			if (ImGui::Button(u8"������"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button(u8"�p�����[�^�[������"))
		{
			ImGui::OpenPopup("New");
		}
		if (ImGui::BeginPopup("New"))
		{
			ImGui::Text(u8"�{���ɏ��������܂����H");
			if (ImGui::Button(u8"�͂�"))
			{
				if (AttackClean())
				{
					init_timer = 2.0f;
					ImGui::CloseCurrentPopup();
				}
			}
			if (ImGui::Button(u8"������"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (save_timer > 0.0f)
		{
			ImGui::Text(u8"�Z�[�u���܂���");
			save_timer -= elapsed_time;
		}
		if (load_timer > 0.0f)
		{
			ImGui::Text(u8"���[�h���܂���");
			load_timer -= elapsed_time;
		}
		if (init_timer > 0.0f)
		{
			ImGui::Text(u8"���������܂���");
			init_timer -= elapsed_time;
		}

		if (ImGui::BeginTabBar(u8"�ݒ�pImGui",ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
		{
			if (ImGui::BeginTabItem(u8"�L�����X�e�[�^�X"))
			{
				if (ImGui::BeginTabBar(u8"�L�����X�e�[�^�XTabBar", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
				{
					if (ImGui::BeginTabItem(u8"�X�e�[�^�X��{���"))
					{
						ImGui::Text(u8"�s���X�e�[�g");
						ImGui::Text(GetName().act_name_list[scastI(act_state)].c_str());
						ImGui::Text(u8"�U���X�e�[�g");
						ImGui::Text(GetName().attack_name_list[scastI(attack_state)].c_str());

						ImGui::SliderFloat(u8"�W�����v�̒��n�X�L", &jump_later, 0.0f, 2.0f);
						ImGui::SliderFloat(u8"�_�b�V���̒��n�X�L", &dash_later, 0.0f, 2.0f);
						ImGui::Text("fream : "); ImGui::SameLine(); ImGui::Text("%f", fream);
						ImGui::Text("timer : "); ImGui::SameLine(); ImGui::Text("%f", timer);
						ImGui::Text("later : "); ImGui::SameLine(); ImGui::Text("%f", later);
						ImGui::Text("knocktimer : "); ImGui::SameLine(); ImGui::Text("%f", knocktimer);


						//ImGui::Text("later : "); ImGui::SameLine(); ImGui::Text("%f", later);



						chara_state.Draw();

						//ImGui::InputFloat("eye_offset.x", &eye_offset.x, 0.01f, 0.01f);
						//ImGui::InputFloat("eye_offset.y", &eye_offset.y, 0.01f, 0.01f);
						//ImGui::InputFloat("mouse_offset.x", &mouth_offset.x, 0.01f, 0.01f);
						//ImGui::InputFloat("mouse_offset.y", &mouth_offset.y, 0.01f, 0.01f);
						ImGui::Text("player.x:%f", pos.x);
						ImGui::Text("player.y:%f", pos.y);
						ImGui::Text("command_timer:%f", pad->com_list.command_timer);
						ImGui::Text("track:%d", trackgauge);
						ImGui::Text("hitState : "); ImGui::SameLine();
						ImGui::Text(GetName().hitstate_name_list[scastI(hit[0].state)].c_str());
						if (ImGui::TreeNode("ModelParameter"))
						{
							ImGui::SliderFloat("scale_x", &scale.x, -10.0f, 10.0f);
							ImGui::SliderFloat("scale_y", &scale.y, -10.0f, 10.0f);
							ImGui::SliderFloat("scale_z", &scale.z, -10.0f, 10.0f);
							ImGui::SliderFloat("angle_x", &angle.x, -10.0f, 10.0f);
							ImGui::SliderFloat("angle_y", &angle.y, -10.0f, 10.0f);
							ImGui::SliderFloat("angle_z", &angle.z, -10.0f, 10.0f);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("StepParameter"))
						{
							ImGui::Text("speed.x:%f", speed.x);
							//ImGui::SliderFloat("speed", &backstepS, 0.0f, 500.0f);
							//ImGui::SliderFloat("down", &backstepD, 0.0f, 500.0f);
							//ImGui::SliderFloat("air_speed", &stepspeed, 0.0f, 500.0f);
							//ImGui::SliderFloat("air_down", &stepD, 0.0f, 500.0f);
							//ImGui::SliderFloat("passive_speed_X", &passive_speed.x, 0.0f, 500.0f);
							//ImGui::SliderFloat("passive_speed_Y", &passive_speed.y, 0.0f, 500.0f);
							//ImGui::SliderFloat("passive_break", &passive_brake, 0.0f, 500.0f);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("JumpParameter"))
						{
							static float s = 0.0f;


							if (jumpflag)
							{
								if (!max_jump_flag)
								{
									s = 0.0f;
								}
								s = min(speed.y, s);
							}

							ImGui::Checkbox("jump_flug", &jumpflag);
							ImGui::Checkbox("max_jump_flug", &max_jump_flag);
							ImGui::Text("speed.y:%f", speed.y);
							ImGui::Text("speed_X:%f", speed_X.speed);
							ImGui::Text("speed_Y:%f", speed_Y.speed);
							ImGui::Text("down_force:%f", down_force);
							ImGui::Text("s:%f", s);
							//ImGui::SliderFloat("jump_max", &jump_max, 0.0f, 500.0f);
							ImGui::SliderFloat("down_force", &down_force, 0.0f, 1000.0f);
							//ImGui::SliderFloat("high_jump_max", &high_jump_max, 0.0f, 1000.0f);
							//ImGui::SliderFloat("jump_speed", &jump_speed, 0.0f, 5000.0f);
							//ImGui::SliderFloat("high_jump_speed", &high_jump_speed, 0.0f, 5000.0f);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Light"))
						{
							ImGui::SliderFloat("light_direction.x", &this->light_direction.x, -1.0f, 1.0f);
							ImGui::SliderFloat("light_direction.y", &this->light_direction.y, -1.0f, 1.0f);
							ImGui::SliderFloat("light_direction.z", &this->light_direction.z, -1.0f, 1.0f);
							ImGui::SliderFloat("light_direction.w", &this->light_direction.w, -1.0f, 1.0f);
							ImGui::TreePop();
						}

						ImGui::Text(u8"�����Ă�������F"); ImGui::SameLine();
						if (rightOrleft > 0)
						{
							ImGui::Text(u8"�E");
						}
						else
						{
							ImGui::Text(u8"��");
						}

						/*if (ImGui::BeginTabBar("testBar"))
						{
							for (int i = 0; i < 100; i++)
							{
								if (ImGui::BeginTabItem(std::to_string(i).data()))
								{
									{
										ImGui::Text(std::to_string(i).data());
										ImGui::EndTabItem();
									}
								}
							}
							ImGui::EndTabBar();
						}*/

						ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

						if (ImGui::TreeNode(u8"�����蔻��"))
						{
							if (!atk.empty())
							{
								for (int i = 0; i < atk.size(); i++)
								{
									ImGui::InputFloat(".x", &atk[i].pos.x, 0.01f, 0.01f);
									ImGui::InputFloat(".x", &atk[i].pos.y, 0.01f, 0.01f);
								}
							}
							ImGui::TreePop();
						}
						ImGui::EndTabItem();
					}
					//�����蔻�蒲���֐�
					DrawDEBUGHitParam();
					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}



			//�U�������S�ďo��
			int at_num_tab = -1;
			for (int list = 0; list < attack_list.size(); list++)
			{
				std::string tab_num = std::string("attack_");

				if (list % 5 == 0)
				{
					//5�Â����ă^�u������
					at_num_tab++;
				}

				tab_num += std::to_string(at_num_tab);

				if (ImGui::BeginTabItem(tab_num.data()))
				{
					if (ImGui::BeginTabBar(u8"���育��", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
					{
						if (ImGui::BeginTabItem(GetName().attack_name_list[list].c_str()))
						{
							if (ImGui::BeginTabBar(u8"�U�����e", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
							{
								bool real_attack_not_now = false;
								if (ImGui::BeginTabItem(u8"��{���"))
								{
									int real = scastI(attack_list[list].real_attack);
									int pad = scastI(attack_list[list].linkage_button);
									ImGui::SliderInt(u8"�{�^��", &pad, 0, scastI(PAD::PAD_END)-1);
									attack_list[list].linkage_button = static_cast<PAD>(pad);
									ImGui::Text(GetName().pad_name_list[pad].c_str());
									int com = scastI(attack_list[list].linkage_command);
									ImGui::SliderInt(u8"�R�}���h", &com, 0, scastI(Command::END)-1);
									attack_list[list].linkage_command = static_cast<Command>(com);
									ImGui::Text(GetName().command_name_list[com].c_str());
									int ground = scastI(attack_list[list].ground_on);
									ImGui::SliderInt(u8"�U���͂ǂ��ŏo���邩", &ground, 0, scastI(Ground_C::END) - 1);
									attack_list[list].ground_on = static_cast<Ground_C>(ground);
									ImGui::Text(GetName().ground_name_list[ground].c_str());
									int next = scastI(attack_list[list].combo);
									ImGui::SliderInt(u8"���̍U��", &next, 0, scastI(AttackState::ATTACK_END) - 1);
									attack_list[list].combo = static_cast<AttackState>(next);
									ImGui::Text(GetName().attack_name_list[scastI(attack_list[list].combo)].c_str());
									ImGui::Checkbox(u8"���Ⴊ�ݍU��", &attack_list[list].squat_on);
									ImGui::Checkbox(u8"���̋O�Ղ�\������", &attack_list[list].traject_on);

									int anim_k = scastI(attack_list[list].anim_kind);
									ImGui::SliderInt(u8"�g�p�A�j���[�V����", &anim_k, 0, scastI(AttackState::NORMAL_ATTACK_END) - 1);
									attack_list[list].anim_kind = static_cast<AttackState>(anim_k);
									ImGui::Text(GetName().attack_name_list[anim_k].c_str());

									int function_n = scastI(attack_list[list].function_num);
									ImGui::SliderInt(u8"�g�p����֐�", &function_n, 0, scastI(AT_Function_List::AT_END) - 1);
									attack_list[list].function_num = static_cast<AT_Function_List>(function_n);
									ImGui::Text(GetName().function_name_list[function_n].c_str());

									if (attack_list[list].linkage_stick != PAD::BUTTOM_END)
									{
										GetName().linkage_stick_on[list] = true;
									}

									ImGui::Checkbox(u8"�X�e�B�b�N�̓��͂�K�v�Ƃ���", &GetName().linkage_stick_on[list]);
									if (GetName().linkage_stick_on[list])
									{
										int stick = scastI(attack_list[list].linkage_stick);
										ImGui::SliderInt(u8"�X�e�B�b�N", &stick, scastI(PAD::STICK_R), scastI(PAD::STICK_D));
										ImGui::Text(GetName().pad_name_list[stick].c_str());
										attack_list[list].linkage_stick = static_cast<PAD>(stick);
									}
									else
									{
										attack_list[list].linkage_stick = PAD::BUTTOM_END;
									}

									int effect_k_h = scastI(attack_list[list].effect_param.effect_kind);
									ImGui::SliderInt(u8"�G�t�F�N�g�̎��", &effect_k_h, 0, scastI(EffectKind::END) - 1);
									ImGui::Text(GetName().effect_kind_name_list[scastI(attack_list[list].effect_param.effect_kind)].c_str());
									attack_list[list].effect_param.effect_kind = static_cast<EffectKind>(effect_k_h);

									if (attack_list[list].effect_param.effect_kind != EffectKind::NONE)
									{
										if (ImGui::TreeNode(u8"�G�t�F�N�g�ڍאݒ�"))
										{
											ImGui::SliderFloat(u8"���WX", &attack_list[list].effect_param.distance.x, -50.0f, 50.0f);
											ImGui::SliderFloat(u8"���WY", &attack_list[list].effect_param.distance.y, -50.0f, 50.0f);
											ImGui::SliderFloat(u8"���WZ", &attack_list[list].effect_param.distance.z, -50.0f, 50.0f);
											//float scale_all = attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x;
											//ImGui::SliderFloat(u8"�傫���S��", &scale_all, 0.0f, 50.0f);
											ImGui::SliderFloat(u8"�傫��X", &attack_list[list].effect_param.scale.x, 0.0f, 50.0f);
											ImGui::SliderFloat(u8"�傫��Y", &attack_list[list].effect_param.scale.y, 0.0f, 50.0f);
											ImGui::SliderFloat(u8"�傫��Z", &attack_list[list].effect_param.scale.z, 0.0f, 50.0f);
											/*if (attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z)
											{
												attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x = scale_all;
												attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y = scale_all;
												attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z = scale_all;
											}*/
											ImGui::SliderFloat(u8"����X", &attack_list[list].effect_param.axis.x, 0.0, 1.0f);
											ImGui::SliderFloat(u8"����Y", &attack_list[list].effect_param.axis.y, 0.0, 1.0f);
											ImGui::SliderFloat(u8"����Z", &attack_list[list].effect_param.axis.z, 0.0, 1.0f);
											ImGui::SliderFloat(u8"�p�x", &attack_list[list].effect_param.angle, -360.0f, 360.0f);
											ImGui::Checkbox(u8"�p�x���v���C���[�ˑ��ɂ��邩", &attack_list[list].effect_param.rightORleft);

											ImGui::TreePop();
										}
									}

									ImGui::SliderInt(u8"�U�����e", &real, 0, scastI(AttackState::ATTACK_END) - 1);
									attack_list[list].real_attack = static_cast<AttackState>(real);


									if (attack_list[list].attack_name != attack_list[list].real_attack)
									{
										ImGui::Text(GetName().attack_name_list[scastI(attack_list[list].real_attack)].c_str());
										real_attack_not_now = true;
									}
									else
									{
										int result = scastI(attack_list[list].conditions_hit);
										ImGui::InputFloat(u8"��X�L", &attack_list[list].later, 0.01f, 0.1f);
										ImGui::InputInt(u8"�U����", &attack_list[list].attack_max, 1, 10);
										ImGui::InputFloat(u8"���[�V�������x:����", &ac_attack[list].fream, 0.01f, 0.1f);
										ImGui::InputFloat(u8"���[�V�������x:����", &ac_attack[list].timer, 0.01f, 0.1f);
										ImGui::InputFloat(u8"���[�V�������x:��X�L", &ac_attack[list].later, 0.01f, 0.1f);
										ImGui::InputFloat(u8"�O�i���鋗��", &attack_list[list].advance_speed, 0.01f, 0.1f);
										ImGui::SliderInt(u8"�U���J�ڏ���", &result, 0, scastI(HitResult::END) - 1);
										attack_list[list].conditions_hit = static_cast<HitResult>(result);
										ImGui::Text(GetName().result_name_list[scastI(attack_list[list].conditions_hit)].c_str());
										ImGui::Checkbox(u8"�����蔻��ɃX�s�[�h��t�^����", &attack_list[list].speed_on);
										if (attack_list[list].speed_on)
										{
											ImGui::InputFloat(u8"���Z�X�s�[�hX", &attack_list[list].speed.x, 0.01f, 0.1f);
											ImGui::InputFloat(u8"���Z�X�s�[�hY", &attack_list[list].speed.y, 0.01f, 0.1f);
											ImGui::InputFloat(u8"�v���C���[�̎����t���[��", &attack_list[list].timer, 0.01f, 0.1f);
										}
										else
										{
											attack_list[list].speed = YR_Vector3(0.0f, 0.0f);
											attack_list[list].timer = 0.0f;
										}
										ImGui::InputInt(u8"�K�v�ȃQ�[�W��", &attack_list[list].need_power, 1.0f, 10.0f);
										if (attack_list[list].need_power >= 1)
										{
											int aid_attack = scastI(attack_list[list].aid_attack_name);
											ImGui::SliderInt(u8"�Q�[�W������Ȃ��ꍇ�̍U��", &aid_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
											ImGui::Text(GetName().attack_name_list[aid_attack].c_str());
											attack_list[list].aid_attack_name = static_cast<AttackState>(aid_attack);
										}
										else
										{
											attack_list[list].aid_attack_name = AttackState::NONE;
										}


										attack_list[list].attack_single.resize(attack_list[list].attack_max);

										real_attack_not_now = false;
									}
									ImGui::EndTabItem();
								}

								if(!real_attack_not_now)
								{
									//�U���񐔂��Ƃ̃p�����[�^�\��
									if (!attack_list[list].attack_single.empty())
									{
										if (ImGui::Button(u8"�U�����e����"))
										{
											AttackSingle back = attack_list[list].attack_single.back();
											attack_list[list].attack_single.push_back(back);
											attack_list[list].attack_max++;
										}ImGui::SameLine();
										if (ImGui::Button(u8"�U�����e�Ō���폜"))
										{
											int size = attack_list[list].attack_single.size() - 1;
											attack_list[list].attack_single.resize(size);
											attack_list[list].attack_max--;
										}
										for (int sin = 0; sin < attack_list[list].attack_single.size(); sin++)
										{
											std::string now_sin = std::string("ATK_Single:") + std::to_string(sin);
											if (ImGui::BeginTabItem(now_sin.c_str()))
											{
												ImGui::Text(u8"�U���񐔂��Ƃ̃p�����[�^");
												ImGui::InputFloat(u8"�����t���[��", &attack_list[list].attack_single[sin].fream, 0.01f, 0.1f);
												ImGui::InputInt(u8"�����蔻��̐�", &attack_list[list].attack_single[sin].quantity, 1, 10);
												attack_list[list].attack_single[sin].parameter.resize(attack_list[list].attack_single[sin].quantity);
												if (!attack_list[list].attack_single[sin].parameter.empty())
												{
													if (ImGui::Button(u8"���蕡��"))
													{
														AttackParameter back = attack_list[list].attack_single[sin].parameter.back();
														attack_list[list].attack_single[sin].parameter.push_back(back);
														attack_list[list].attack_single[sin].quantity++;
													}ImGui::SameLine();
													if (ImGui::Button(u8"����Ō���폜"))
													{
														int size = attack_list[list].attack_single[sin].parameter.size() - 1;
														attack_list[list].attack_single[sin].parameter.resize(size);
														attack_list[list].attack_single[sin].quantity--;
													}
													if (ImGui::BeginTabBar(u8"����", ImGuiTabBarFlags_::ImGuiTabBarFlags_FittingPolicyScroll))
													{
														for (int para = 0; para < attack_list[list].attack_single[sin].parameter.size(); para++)
														{
															//�U����������p�����[�^�[�\��
															std::string now_para = u8"����";
															now_para += std::to_string(para);
															if (ImGui::BeginTabItem(now_para.c_str()))
															{
																int effect_k = scastI(attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind);
																ImGui::SliderInt(u8"�G�t�F�N�g�̎��", &effect_k, 0, scastI(EffectKind::END) - 1);
																ImGui::Text(GetName().effect_kind_name_list[scastI(attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind)].c_str());
																attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind = static_cast<EffectKind>(effect_k);

																if (attack_list[list].attack_single[sin].parameter[para].effect_param.effect_kind != EffectKind::NONE)
																{
																	if (ImGui::TreeNode(u8"�G�t�F�N�g�ڍאݒ�"))
																	{
																		ImGui::SliderFloat(u8"���WX", &attack_list[list].attack_single[sin].parameter[para].effect_param.distance.x, -50.0f, 50.0f);
																		ImGui::SliderFloat(u8"���WY", &attack_list[list].attack_single[sin].parameter[para].effect_param.distance.y, -50.0f, 50.0f);
																		ImGui::SliderFloat(u8"���WZ", &attack_list[list].attack_single[sin].parameter[para].effect_param.distance.z, -50.0f, 50.0f);
																		//float scale_all = attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x;
																		//ImGui::SliderFloat(u8"�傫���S��", &scale_all, 0.0f, 50.0f);
																		ImGui::SliderFloat(u8"�傫��X", &attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x, 0.0f, 50.0f);
																		ImGui::SliderFloat(u8"�傫��Y", &attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y, 0.0f, 50.0f);
																		ImGui::SliderFloat(u8"�傫��Z", &attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z, 0.0f, 50.0f);
																		/*if (attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y == attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z)
																		{
																			attack_list[list].attack_single[sin].parameter[para].effect_param.scale.x = scale_all;
																			attack_list[list].attack_single[sin].parameter[para].effect_param.scale.y = scale_all;
																			attack_list[list].attack_single[sin].parameter[para].effect_param.scale.z = scale_all;
																		}*/
																		ImGui::SliderFloat(u8"����X", &attack_list[list].attack_single[sin].parameter[para].effect_param.axis.x, 0.0, 1.0f);
																		ImGui::SliderFloat(u8"����Y", &attack_list[list].attack_single[sin].parameter[para].effect_param.axis.y, 0.0, 1.0f);
																		ImGui::SliderFloat(u8"����Z", &attack_list[list].attack_single[sin].parameter[para].effect_param.axis.z, 0.0, 1.0f);
																		ImGui::SliderFloat(u8"�p�x", &attack_list[list].attack_single[sin].parameter[para].effect_param.angle, -360.0f, 360.0f);
																		ImGui::Checkbox(u8"�p�x���v���C���[�ˑ��ɂ��邩", &attack_list[list].attack_single[sin].parameter[para].effect_param.rightORleft);

																		ImGui::TreePop();
																	}
																}

																ImGui::SliderFloat(u8"�v���C���[�Ƃ̋���X", &attack_list[list].attack_single[sin].parameter[para].distance.x, -10.0f, 10.0f);
																ImGui::SliderFloat(u8"�v���C���[�Ƃ̋���Y", &attack_list[list].attack_single[sin].parameter[para].distance.y, -10.0f, 10.0f);
																ImGui::SliderFloat(u8"�T�C�YX", &attack_list[list].attack_single[sin].parameter[para].size.x, 0.0f, 20.0f);
																ImGui::SliderFloat(u8"�T�C�YY", &attack_list[list].attack_single[sin].parameter[para].size.y, 0.0f, 20.0f);
																ImGui::SliderFloat(u8"�����t���[��", &attack_list[list].attack_single[sin].parameter[para].timer, 0.0f, 2.0f);
																ImGui::SliderFloat(u8"�_���[�W", &attack_list[list].attack_single[sin].parameter[para].param.damage, 0.0f, 300.0f);
																ImGui::SliderFloat(u8"�̂����莞��", &attack_list[list].attack_single[sin].parameter[para].param.HB_timer, 0.0f, 10.0f);
																ImGui::SliderFloat(u8"�������X", &attack_list[list].attack_single[sin].parameter[para].param.hitback.x, -300.0f, 300.0f);
																ImGui::SliderFloat(u8"�������Y", &attack_list[list].attack_single[sin].parameter[para].param.hitback.y, -300.0f, 300.0f);
																int type = scastI(attack_list[list].attack_single[sin].parameter[para].type);
																ImGui::SliderInt(u8"�U���^�C�v", &type, 0, scastI(AttackKind::END) - 1);
																attack_list[list].attack_single[sin].parameter[para].type = static_cast<AttackKind>(type);
																ImGui::Text(GetName().attack_kind_name_list[type].c_str());
																ImGui::SliderFloat(u8"�m�b�N�o�b�N(X�̂�)", &attack_list[list].attack_single[sin].parameter[para].knockback, -100.0f, 100.0f);
																ImGui::Checkbox(u8"�Q�[�W���l�����Ȃ�", &attack_list[list].attack_single[sin].parameter[para].gaugeout);
																ImGui::SliderFloat(u8"���ݔ�������鎞��", &attack_list[list].attack_single[sin].parameter[para].param.steal_timer, 0.0f, 2.0f);
																int hit_stop_time = scastI(attack_list[list].attack_single[sin].parameter[para].HS_timer);
																ImGui::SliderInt(u8"�q�b�g�X�g�b�v����", &hit_stop_time, 0, scastI(HitStopTime::END) - 1);
																ImGui::Text(u8"0:�Z���B1:���ʁB2:�����B3:�Y�[���X�g�b�v�B4:�قږ����B5:������������");
																attack_list[list].attack_single[sin].parameter[para].HS_timer = static_cast<HitStopTime>(hit_stop_time);
																ImGui::SliderFloat(u8"�K�[�h�o�b�NX", &attack_list[list].attack_single[sin].parameter[para].param.guard_back.x, -100.0f, 100.0f);
																ImGui::SliderFloat(u8"�K�[�h�o�b�NY", &attack_list[list].attack_single[sin].parameter[para].param.guard_back.y, -100.0f, 100.0f);
																ImGui::SliderFloat(u8"�K�[�h���", &attack_list[list].attack_single[sin].parameter[para].param.guard_shaving, 0.0f, 100.0f);
																ImGui::SliderFloat(u8"�K�[�h�d��", &attack_list[list].attack_single[sin].parameter[para].param.guard_timer, 0.0f, 10.0f);
																ImGui::SliderFloat(u8"�R���{�␳�l", &attack_list[list].attack_single[sin].parameter[para].param.combo_correction, 0.0f, 10.0f);

																if (!attack_list[list].attack_single[sin].parameter[para].gaugeout)
																{
																	ImGui::InputFloat(u8"�Q�[�W�l����", &attack_list[list].attack_single[sin].parameter[para].gauge_get, 0.01f, 0.1f);
																}
																ImGui::EndTabItem();
															}
														}
														ImGui::EndTabBar();
													}
												}

												ImGui::EndTabItem();
											}
										}
									}
								}
								ImGui::EndTabBar();
							}
							ImGui::EndTabItem();
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::End();


		static int combo_flg = 0;


		std::string now_com = std::to_string(now_player);
		now_com += std::string(":X_Combolist");
		ImGui::Begin(now_com.c_str());

		ImGui::RadioButton(u8"X�R���{", &combo_flg, 0); ImGui::SameLine();
		ImGui::RadioButton(u8"Y�R���{", &combo_flg, 1); ImGui::SameLine();
		ImGui::RadioButton(u8"B�R���{", &combo_flg, 2);

		switch (combo_flg)
		{
		case 0:
		{
			//X�R���{���X�g�쐬
			static AttackState at_state = AttackState::NONE;
			int com_attack = scastI(at_state);
			ImGui::SliderInt(u8"�ǉ�����R���{�U��", &com_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
			ImGui::Text(GetName().attack_name_list[com_attack].c_str());
			at_state = static_cast<AttackState>(com_attack);

			if (ImGui::Button(u8"�R���{�ǉ�"))
			{
				combolist_X.combolist.push_back(AttackState());
				combolist_X.combolist.back() = at_state;
			}ImGui::SameLine();
			if (ImGui::Button(u8"�R���{�폜"))
			{
				if (!combolist_X.combolist.empty())
				{
					int size = combolist_X.combolist.size() - 1;
					combolist_X.combolist.resize(size);
				}
			}

			if (!combolist_X.combolist.empty())
			{
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
				for (int i = 0; i < combolist_X.combolist.size(); ++i) {
					ImGui::Text(GetName().attack_name_list[scastI(combolist_X.combolist[i])].c_str());
				}
				ImGui::EndChild();
			}
		}
		break;
		case 1:
		{
			//Y�R���{���X�g�쐬
			static AttackState at_state = AttackState::NONE;
			int com_attack = scastI(at_state);
			ImGui::SliderInt(u8"�ǉ�����R���{�U��", &com_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
			ImGui::Text(GetName().attack_name_list[com_attack].c_str());
			at_state = static_cast<AttackState>(com_attack);

			if (ImGui::Button(u8"�R���{�ǉ�"))
			{
				combolist_Y.combolist.push_back(AttackState());
				combolist_Y.combolist.back() = at_state;
			}ImGui::SameLine();
			if (ImGui::Button(u8"�R���{�폜"))
			{
				if (!combolist_Y.combolist.empty())
				{
					int size = combolist_Y.combolist.size() - 1;
					combolist_Y.combolist.resize(size);
				}
			}

			if (!combolist_Y.combolist.empty())
			{
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
				for (int i = 0; i < combolist_Y.combolist.size(); ++i) {
					ImGui::Text(GetName().attack_name_list[scastI(combolist_Y.combolist[i])].c_str());
				}
				ImGui::EndChild();
			}
		}
		break;
		case 2:
		{
			//B�R���{���X�g�쐬
			static AttackState at_state = AttackState::NONE;
			int com_attack = scastI(at_state);
			ImGui::SliderInt(u8"�ǉ�����R���{�U��", &com_attack, scastI(AttackState::NONE), scastI(AttackState::ATTACK_END) - 1);
			ImGui::Text(GetName().attack_name_list[com_attack].c_str());
			at_state = static_cast<AttackState>(com_attack);

			if (ImGui::Button(u8"�R���{�ǉ�"))
			{
				combolist_B.combolist.push_back(AttackState());
				combolist_B.combolist.back() = at_state;
			}ImGui::SameLine();
			if (ImGui::Button(u8"�R���{�폜"))
			{
				if (!combolist_B.combolist.empty())
				{
					int size = combolist_B.combolist.size() - 1;
					combolist_B.combolist.resize(size);
				}
			}

			if (!combolist_B.combolist.empty())
			{
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
				for (int i = 0; i < combolist_B.combolist.size(); ++i) {
					ImGui::Text(GetName().attack_name_list[scastI(combolist_B.combolist[i])].c_str());
				}
				ImGui::EndChild();
			}
		}
		break;
		default:
			break;
		}


		ImGui::End();

		std::string now_pro = std::to_string(now_player);
		now_pro += std::string(u8"�F��ѓ����");
		ImGui::Begin(now_pro.c_str());

		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(500, 500), ImGuiWindowFlags_NoTitleBar);

		if (!projectile_atk.empty())
		{
			for (int i = 0; i < projectile_atk.size(); i++)
			{
				ImGui::Text("pro[%d]:", i); ImGui::SameLine();
				ImGui::Text("pos_X : %f", projectile_atk[i].pos.x);
			}
		}

		ImGui::EndChild();
		ImGui::End();

		std::string sp_st = std::to_string(now_player) + std::string(u8" P : ") + GetName().chara_name_list[scastI(this->chara_name)] + std::string(u8" : ���K�E�C�x���g");
		special_event.DrawTimeLine(sp_st,chara_name);
	}


#endif // USE_IMGUI
}


//�f�o�b�O�������蔻�蒲���c�[���`��
void Player::DrawDEBUGHitParam()
{
	//�I�[�o�[���C�h���Ďg�p����
}