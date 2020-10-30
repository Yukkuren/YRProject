#ifndef _HITBOX_H_
#define _HITBOX_H_

#include "YR_VectorMaster.h"
#include "YRShader.h"


constexpr float hitback_Adjustment = 300.0f;

enum class HitBoxState : int
{
	NOGUARD = 0,	//�K�[�h���Ȃ�
	MIDDLE,			//���i�K�[�h
	DOWN,			//���i�K�[�h
	INVINCIBLE,		//���G
	ALL,			//�󒆃K�[�h(�͂݁A�K�[�h�s�ȊO��S�ăK�[�h�ł���)
	END,
};

struct HitParameter
{
	YR_Vector3	distance = { 0.0f,0.0f };		//�v���C���[�̒��S���W����ǂꂾ������Ă��邩
	YR_Vector3	size = { 0.0f,0.0f };			//�����蔻��̃T�C�Y
	HitBoxState	state = HitBoxState::NOGUARD;	//���݂̓����蔻��̏��
};

class HitBox
{
public:
	//�U�����󂯂����ɒ��g����������ϐ�
	YR_Vector3	center;			//�����蔻��̒��S���W
	bool		hit;			//�������Ă�����TRUE
	float		damege;			//�󂯂��_���[�W
	//bool		hitother;		//����Ɠ������Ă�����true
	float		timer;			//�U�����󂯂����̂̂����莞��
	YR_Vector3	hitback;		//�U�����󂯂����̐�����уx�N�g��
	//int			guard;			//�K�[�h���Ă���ꍇ�Ή��������l������
	bool		guard_ok;		//�K�[�h�����Ȃ�true
	bool		steal;			//�͂܂ꂽ��true
	float		steal_timer;	//���������\����

	//��ɕϓ�����ϐ�
	HitParameter	parameter;		//�����蔻��̃p�����[�^


	void Init();		//�S�ă[���N���A(�v���C���[�̏����ʒu�ɓ����蔻������邽�߃v���C���[�̍��W��������Ă���)
	void Draw(
		YRShader *shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);		//DEBUG_MODE��true�̎��̂ݕ`��
	//������W�ƃT�C�Y���󂯎���čX�V
	void Update(YR_Vector3 pl_pos,HitParameter& param,float elapsed_time);
	void Update(YR_Vector3 pl_pos, YR_Vector3& distance, YR_Vector3& size , float elapsed_time);

};

#endif // !_HITBOX_H_
