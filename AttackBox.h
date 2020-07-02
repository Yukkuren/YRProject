#ifndef _ATTACKBOX_H_
#define _ATTACKBOX_H_
#include "YR_VECTOR3.h"
#include "YRShader.h"

class AttackBox
{
public:
	YR_Vector3	center;		//�U������̒��S���W
	YR_Vector3	size;		//�U������̑傫��
	float		timer;		//�����t���[��
	bool		start;		//�J�n�t���O(�ŏ��̂�true�B�����ݒ�I�����false)
	bool		fin;		//�I���t���O(true�ōU������I��)
	float		fream;		//�����t���[��
	float		later;		//�㌄�t���[��
	bool		attack;		//TRUE�Ȃ�U�����肪�o�Ă���
	bool		hit_ok;		//�U�����ł��Ƃ��̂�true�ɂ��ē���������false
	float		damege;		//���̍U���ŗ^����_���[�W
	float		HB_timer;	//�U���𓖂Ă����̑���̂̂����莞��
	YR_Vector3	hitback;	//�U���𓖂Ă����̑���̐�����уx�N�g��
	int			type;		//�U���̑���(��i�E���i�E���i)
	float		knockback;	//�m�b�N�o�b�N(X�x�N�g���̂�)
	bool		knock_start;//����������true
	bool		gaugeout;	//false�Ȃ�Q�[�W�l���U��
	float		stealtimer; //�݂͂ʂ�����鎞��

	void Init();			//�S��false&�[���N���A
	void Draw(YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);			//DEBUG_MODE��true�̎��̂ݕ`��
	//�ʒu�A�傫���A�����A�����A�㌄�A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�m�b�N�o�b�N
	void Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock,float elapsed_time);
	//Update�ɖ���l�����邱�Ƃɂ���č��W�𓮂����Ă���Btimer��late���͍U���������̂ݒl����������
	//�ʒu�A�傫���A�����A�З́A�̂����莞�ԁA������΂��x�N�g���A�i
	void UpdateMissile(YR_Vector3 cent, YR_Vector3 range, float time, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float elapsed_time);

	//�ʒu�A�傫���A�����A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�Q�[�W���l�����Ȃ�
	void UpdateMissile(YR_Vector3 cent, YR_Vector3 range, float time, float dage, int hitbackT, YR_Vector3 divhitback, int kind, int gauge, float elapsed_time);

	//�ʒu�A�傫���A�����A�����A�㌄�A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�m�b�N�o�b�N,�U�����p������
	bool Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock, bool fin, float elapsed_time);


	//�ʒu�A�傫���A�����A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�Q�[�W���l�����Ȃ�
	void Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock, int gauge, float elapsed_time);

	enum
	{
		UP,
		MIDDLE,
		DOWN,
		STEAL,
	};
};
#endif // !_ATTACKBOX_H_


