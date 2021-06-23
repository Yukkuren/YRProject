#pragma once

#include "../YR_VectorMaster.h"

//----------------------------------------------------------------
//	BoxParameter
//----------------------------------------------------------------
//�EHitBox�AAttackBox�����Ŏg�p���鋤�ʂ̃p�����[�^�[��ێ�����\����
//�E���ꂼ��̃N���X�ɕϐ��Ƃ��Ď�������
//----------------------------------------------------------------

struct BoxParameter
{
	float		damage;			//�^����(�󂯂�)�_���[�W
	float		HB_timer;		//�U���𓖂Ă�(�󂯂�)���̂̂����莞��
	YR_Vector3	hitback;		//�U���𓖂Ă����̑���̐�����уx�N�g��
	float		steal_timer;	//�݂͂ʂ�����鎞��
	float		guard_shaving;	//�K�[�h���l
	YR_Vector3	guard_back;		//�K�[�h���ꂽ(����)���̐�����уx�N�g��
	float		guard_timer;	//�K�[�h���ꂽ(����)���̃K�[�h�d������

	BoxParameter() :damage(0.0f), HB_timer(0.0f), hitback(0.0f, 0.0f, 0.0f),
		guard_shaving(0.0f), steal_timer(0.0f), guard_back(0.0f, 0.0f, 0.0f), guard_timer(0.1f) {};

	void Reset()
	{
		damage = 0.0f;
		//HB_timer = 0.0f;
		hitback = YR_Vector3(0.0f, 0.0f, 0.0f);
		steal_timer = 0.0f;
		guard_shaving = 0.0f;
		guard_back = YR_Vector3(0.0f, 0.0f, 0.0f);
		guard_timer = 0.0f;
	}
};
