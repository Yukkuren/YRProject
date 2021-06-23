#pragma once

#include <string>
#include "../../framework.h"

#ifdef EXIST_IMGUI

//Imgui�ŕ\�����邽�߂ɕ������ۑ����Ă���

struct Name_list
{
public:

	//�L���������X�g(�t�@�C���ǂݍ��݂Ɏg�p����ׁA�ǂݍ��ރt�@�C���̖��O�Ɠ����ɂ��邱��)
	std::array<std::string, scastI(PLSELECT::PLSELECT_END)> chara_name_list =
	{
		u8"Knight",
		u8"Neru",
	};

	//-------------------------------------------------------
	//**	�R�����g�����Ă��镔���͉��̃��f�������Ă��镔��

	//�t�@�C���ǂݍ��݂Ɏg�p����z��
	std::array<std::string, scastI(AttackState::NORMAL_ATTACK_END)> attack_file_list =
	{
		u8"wait",
		u8"jaku",
		u8"d_thu",
		u8"kyo",
		u8"d_jaku",
		u8"d_thu",
		u8"u_kyo",
		u8"air_jaku",
		u8"air_thu",
		u8"air_kyo",
		u8"air_ukyo",
		u8"steal",
		u8"throw",
		u8"track",
		u8"Rhurf",
		u8"Rhurf",		//�O���K�E
		u8"Rhurf",		//�O���K�E
		u8"Lhurf",
		u8"Lhurf",		//�㒆�K�E
		u8"Lhurf",		//�㋭�K�E
		u8"special",
		u8"special",	//�㒴�K�E
		u8"special",	//�����Z
		u8"special",	//���G�U��
		u8"thu",
		u8"kyo",
		u8"Rhurf",		//�󒆑O��K�E
		u8"jaku",		//�󒆑O���K�E
		u8"jaku",		//�󒆑O���K�E
		u8"Lhurf",
		u8"jaku",		//�󒆌㒆�K�E
		u8"jaku",		//�󒆌㋭�K�E
		u8"jaku",		//�󒆑O���K�E
		u8"jaku",		//�󒆌㒴�K�E
	};



	//�t�@�C���ǂݍ��݂Ɏg�p����z��
	std::array<std::string, scastI(ActState::ACT_END)> act_file_list =
	{
		u8"wait",
		u8"wait",
		u8"guard",
		u8"squat",
		u8"back",
		//u8"�_�~�[",
		u8"dash",
		u8"backstep",
		u8"jump",
		u8"air_dash",
		u8"air_back",
		u8"damage",		//�X�e�[�g��D��ꂽ���
		u8"passive",	//�N���オ��(����͉��ł͂Ȃ��m��)
		u8"passive",
		u8"down",		//�_�E��
		u8"slid",		//�󒆃_�E��
		u8"damage",
		u8"slid",
		u8"damage",		//�_�E���U�����󂯂�
		u8"wait",
	};


	//�f�o�b�O�p
	std::array<std::string, scastI(AttackState::ATTACK_END)> attack_name_list =
	{
		u8"�U������()�̒��g�͍U�����e",
		u8"��",
		u8"��(�����U��)",
		u8"����(���̎��ɏo�鋭�U��)",
		u8"����",
		u8"����",
		u8"�㋭(��яオ��)",
		u8"���U��",
		u8"�󒆍U��",
		u8"�󋭍U��",
		u8"��㋭�U��(�ł��グ�U��)",
		u8"����",
		u8"����",
		u8"�z�[�~���O�_�b�V��",
		u8"�O��K�E",
		u8"�O���K�E",
		u8"�O���K�E",
		u8"���K�E",
		u8"�㒆�K�E",
		u8"�㋭�K�E",
		u8"�O���K�E",
		u8"�㒴�K�E",
		u8"�����Z",
		u8"���G�U��",
		u8"��̎��ɏo�钆�U��",
		u8"���̎��ɏo�鋭�U��",
		u8"�󒆑O��K�E",
		u8"�󒆑O���K�E",
		u8"�󒆑O���K�E",
		u8"�󒆌��K�E",
		u8"�󒆌㒆�K�E",
		u8"�󒆌㋭�K�E",
		u8"�󒆑O���K�E",
		u8"�󒆌㒴�K�E",
		u8"�U���̍ő吔",
		u8"X�{�^���R���{",
		u8"Y�{�^���R���{",
		u8"B�{�^���R���{",
		u8"A�{�^���������Əo��U��(�v���C���[�I��)",
		u8"A�{�^���������Əo��󒆍U��(�v���C���[�I��)",
	};

	std::array<std::string, scastI(ActState::ACT_END)> act_name_list =
	{
		u8"�����Ȃ�",
		u8"�ҋ@",
		u8"�K�[�h",
		u8"���Ⴊ��",
		u8"���",
		//u8"�_�~�[",
		u8"�_�b�V��",
		u8"�o�b�N�X�e�b�v",
		u8"�W�����v",
		u8"�󒆑O�_�b�V��",
		u8"�󒆌�_�b�V��",
		u8"�X�e�[�g��D��ꂽ���",
		u8"�N���オ��",
		u8"�󂯐g",
		u8"�_�E��",
		u8"�󒆃_�E��",
		u8"�̂�����",
		u8"�@������ꒆ",
		u8"�_�E���U�����󂯂�",
		u8"�U����",
	};

	std::array<std::string, scastI(PAD::PAD_END)> pad_name_list =
	{
		u8"UP",
		u8"DOWN",
		u8"LEFT",
		u8"RIGHT",
		u8"START",
		u8"SELECT",
		u8"L3",
		u8"R3",
		u8"LB�{�^��",
		u8"RB�{�^��",
		u8"A�{�^��",
		u8"B�{�^��",
		u8"X�{�^��",
		u8"Y�{�^��",
		u8"BUTTOM_END",
		u8"L_TRIGGER",
		u8"R_TRIGGER",
		u8"STICK_R",
		u8"STICK_L",
		u8"STICK_U",
		u8"STICK_D",
		u8"STICK_RDown",
		u8"STICK_LDown",
		u8"STICK_RUp",
		u8"STICK_LUp",
		u8"HIGH_UP",
		u8"HIGH_UP_R",
		u8"HIGH_UP_L",
		u8"R_DASH",
		u8"L_DASH",
	};


	std::array < std::string, scastI(Command::END) > command_name_list =
	{
		u8"�R�}���h����",
		u8"236�R�}���h",
		u8"214�R�}���h"
	};

	std::array<bool, scastI(AttackState::ATTACK_END)> linkage_stick_on = { false };


	std::array<std::string, scastI(HitBoxState::END)> hitstate_name_list =
	{
		u8"�K�[�h���Ȃ�",
		u8"���i�K�[�h",
		u8"���i�K�[�h",
		u8"���G",
		u8"�󒆃K�[�h",
		u8"��i�U���ɑ΂��Ė��G",
	};

	std::array<std::string, scastI(HitResult::END)> result_name_list =
	{
		u8"�܂��������ĂȂ�",
		u8"�������ĂȂ�",
		u8"�K�[�h���ꂽ",
		u8"��������",
		u8"�L�����Z���ł��Ȃ�",
	};

	std::array<std::string, scastI(Ground_C::END)> ground_name_list =
	{
		u8"��",
		u8"�n��",
		u8"�ǂ���ł�",
	};

	std::array<std::string, scastI(AttackKind::END)> attack_kind_name_list =
	{
		u8"��i",
		u8"���i",
		u8"���i",
		u8"����",
		u8"��������(���������Ȃ犊���Ԃɂ���)",
		u8"���b�N�Z",
		u8"��ѓ���",
		u8"�z�[�~���O�_�b�V��",
		u8"���E���Ȃ��U��",
		u8"��i�U���Ƒ��E���Ȃ��U��",
		u8"�_�E���U��",
	};


	std::array<std::string, scastI(EffectKind::END)> effect_kind_name_list =
	{
		u8"�����Ȃ�",
		u8"�K�[�h",
		u8"����",
		u8"���q�b�g�G�t�F�N�g",
		u8"�h����",
		u8"���E",
		u8"�Ԃ��h����",
		u8"�΂̃h����",
		u8"�����h����",
		u8"�΂̗���",
		u8"�Ռ��g",
		u8"����",
		u8"�_���[�W�G�t�F�N�g",
		u8"�z�[�~���O�_�b�V�����G�t�F�N�g",
		u8"�ǌ���",
		u8"�ːi�I�[��",
		u8"���[�U�[(��)",
	};


	static Name_list& getInstance()
	{
		static Name_list instance;
		return instance;
	}
};

inline Name_list& GetName()
{
	return Name_list::getInstance();
}
#endif // USE_IMGUI
