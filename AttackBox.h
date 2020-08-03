#ifndef _ATTACKBOX_H_
#define _ATTACKBOX_H_
#include "YR_VECTOR3.h"
#include "YRShader.h"

//--------------------------------------------------
// **�U���p�����[�^�[�\����**
//�E�U���̃p�����[�^�[�𓖂��蔻�育�Ƃɕۑ�����\����
//--------------------------------------------------
struct AttackParameter
{
public:
	YR_Vector3	distance;	//�v���C���[�̒��S���W����ǂꂾ������Ă��邩
	YR_Vector3	size;		//�U������̑傫��
	float		timer;		//�����t���[��
	float		damege;		//���̍U���ŗ^����_���[�W
	float		HB_timer;	//�U���𓖂Ă����̑���̂̂����莞��
	YR_Vector3	hitback;	//�U���𓖂Ă����̑���̐�����уx�N�g��
	int			type;		//�U���̑���(��i�E���i�E���i)
	float		knockback;	//�m�b�N�o�b�N(X�x�N�g���̂�)
	bool		gaugeout;	//false�Ȃ�Q�[�W�l���U��
	float		stealtimer; //�݂͂ʂ�����鎞��
};


class AttackBox
{
public:
	//bool				start;		//�J�n�t���O(�ŏ��̂�true�B�����ݒ�I�����false)
	bool				fin;		//�I���t���O(true�ōU������I��)
	bool				attack;		//TRUE�Ȃ�U�����肪�o�Ă���
	bool				hit_ok;		//�U�����ł��Ƃ��̂�true�ɂ��ē���������false
	bool				knock_start;//����������true
	bool				plus;		//true�Ȃ��ѓ���Bspeed��plus�𑫂��Ă���
	float				rightOrleft;	//�E�����Ȃ�*1�������Ȃ�*-1
	
	AttackParameter		parameter;	//�p�����[�^�[
	YR_Vector3			pos;		//�U���̎��ۂ̒��S
	YR_Vector3			speed;		//�U���̈ړ����x
	YR_Vector3			plus_speed;	//��ѓ���p�̏�ɕt�^����X�s�[�h

	void Init(AttackParameter& param, float rightOrleft);
	void Init(AttackParameter& param, float rightOrleft, YR_Vector3 plus_speed);
	void Update(YR_Vector3 pl_pos, float elapsed_time);
	void Draw(YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);			//DEBUG_MODE��true�̎��̂ݕ`��
	void SpeedPlus(YR_Vector3 plus_speed,float elapsed_time);
	////�ʒu�A�傫���A�����A�����A�㌄�A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�m�b�N�o�b�N
	//void Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock,float elapsed_time);
	////Update�ɖ���l�����邱�Ƃɂ���č��W�𓮂����Ă���Btimer��late���͍U���������̂ݒl����������
	////�ʒu�A�傫���A�����A�З́A�̂����莞�ԁA������΂��x�N�g���A�i
	//void UpdateMissile(YR_Vector3 cent, YR_Vector3 range, float time, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float elapsed_time);

	////�ʒu�A�傫���A�����A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�Q�[�W���l�����Ȃ�
	//void UpdateMissile(YR_Vector3 cent, YR_Vector3 range, float time, float dage, float hitbackT, YR_Vector3 divhitback, int kind, int gauge, float elapsed_time);

	////�ʒu�A�傫���A�����A�����A�㌄�A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�m�b�N�o�b�N,�U�����p������
	//bool Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, bool fin, float elapsed_time);


	////�ʒu�A�傫���A�����A�З́A�̂����莞�ԁA������΂��x�N�g���A�i�A�Q�[�W���l�����Ȃ�
	//void Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, int gauge, float elapsed_time);

	enum
	{
		UP,
		MIDDLE,
		DOWN,
		STEAL,
	};
};
#endif // !_ATTACKBOX_H_


