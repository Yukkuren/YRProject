#ifndef _HITBOX_H_
#define _HITBOX_H_

#include "YR_VECTOR3.h"

class HitBox
{
public:
	YR_Vector3	center;			//�����蔻��̒��S���W
	YR_Vector3	size;			//�����蔻��̃T�C�Y
	bool		hit;			//�������Ă�����TRUE
	float		damege;			//�󂯂��_���[�W
	//bool		hitother;		//����Ɠ������Ă�����true
	int			timer;			//�U�����󂯂����̂̂����莞��
	YR_Vector3	hitback;		//�U�����󂯂����̐�����уx�N�g��
	int			guard;			//�K�[�h���Ă���ꍇ�Ή��������l������
	bool		guard_ok;		//�K�[�h�����Ȃ�true
	bool		steal;			//�͂܂ꂽ��true
	int			steal_timer;	//���������\����


	void Init(YR_Vector3 cent, YR_Vector3 range);		//�S�ă[���N���A(�v���C���[�̏����ʒu�ɓ����蔻������邽�߃v���C���[�̍��W��������Ă���)
	void Draw();								//DEBUG_MODE��true�̎��̂ݕ`��
	void Update(YR_Vector3 cent, YR_Vector3 range);	//������W�ƃT�C�Y���󂯎���čX�V

	enum
	{
		NOGUARD,
		MIDDLE,
		DOWN,
		INVINCIBLE,
		ALL
	};
};

#endif // !_HITBOX_H_
