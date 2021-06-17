#ifndef _HITBOX_H_
#define _HITBOX_H_

#include "../YR_VectorMaster.h"
#include "../YRShader.h"
#include "BoxParameter.h"


constexpr float hitback_Adjustment = 300.0f;

enum class HitBoxState : int
{
	NOGUARD = 0,		//�K�[�h���Ȃ�
	MIDDLE,				//���i�K�[�h
	DOWN,				//���i�K�[�h
	INVINCIBLE,			//���G
	ALL,				//�󒆃K�[�h(�͂݁A�K�[�h�s�ȊO��S�ăK�[�h�ł���)
	UP_INVINCIBLE,		//��i�U���ɑ΂��Ė��G
	PROJECTILE_ARMOR,	//��ѓ���ɑ΂��ăA�[�}�[
	SLIDE,				//�����Ă�����
	END,
};

//-------------------------------------------------
//		�q�b�g�����U���̑�����
//-------------------------------------------------
//	�E�q�b�g�����U���̑�����ۑ�����
//-------------------------------------------------
enum class HitStateKind : int
{
	NORMAL = 0,		//�ʏ�q�b�g
	STEAL,			//�͂݃q�b�g
	SLAM,			//�@�����q�b�g
	ARMOR,			//�A�[�}�[����������
	DOWN,			//�_�E���q�b�g
};

struct HitParameter
{
	YR_Vector3	distance = { 0.0f,0.0f };		//�v���C���[�̒��S���W����ǂꂾ������Ă��邩
	YR_Vector3	size = { 0.0f,0.0f };			//�����蔻��̃T�C�Y
};

class HitBox
{
public:
	//�U�����󂯂����ɒ��g����������ϐ�
	YR_Vector3	center;							//�����蔻��̒��S���W
	bool		hit;							//�������Ă�����TRUE
	bool		guard_ok;						//�K�[�h�����Ȃ�true
	HitStateKind hit_state;						//�q�b�g�����U���̑�����ۑ�����
	HitBoxState	state = HitBoxState::NOGUARD;	//���݂̓����蔻��̏��
	//��ɕϓ�����ϐ�
	HitParameter	parameter;					//�����蔻��̃p�����[�^
	BoxParameter	param;						//HitBox�ɑ���p�����[�^


	void Init();		//�S�ă[���N���A(�v���C���[�̏����ʒu�ɓ����蔻������邽�߃v���C���[�̍��W��������Ă���)
	void Draw(
		YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);		//DEBUG_MODE��true�̎��̂ݕ`��
	//������W�ƃT�C�Y���󂯎���čX�V
	void Update(YR_Vector3 pl_pos, HitParameter& param, float rightOrleft, float elapsed_time);
	void Update(YR_Vector3 pl_pos, YR_Vector3& distance, YR_Vector3& size, float rightOrleft, float elapsed_time);


	HitBox() : center(0.0f, 0.0f, 0.0f), hit(false), guard_ok(false), hit_state(HitStateKind::NORMAL) {};
};

#endif // !_HITBOX_H_
