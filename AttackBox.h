#ifndef _ATTACKBOX_H_
#define _ATTACKBOX_H_
#include "YR_VectorMaster.h"
#include "YRShader.h"
#include "Effect.h"

//--------------------------------------------------
// **�U���p�����[�^�[��**
//�E�U���̃p�����[�^�[�𓖂��蔻�育�Ƃɕۑ�����\����
//--------------------------------------------------
enum class HitStopTime : int
{
	SHORT=0,
	NORMAL,
	LONG,
	ZOOM,
	END
};

//--------------------------------------------------
// **�����**
//�E�U�����q�b�g���ɔ�e�������A����Ƃ��K�[�h���ꂽ����ۑ������
//--------------------------------------------------
enum class HitResult : int
{

	NOT_OCCURRENCE = 0,	//�U�����܂��o�Ă��Ȃ�
	NONE,				//�������Ă��Ȃ�
	GUARD,				//�K�[�h���ꂽ
	HIT,				//��������
	NO_CANCEL,			//�L�����Z���ł��Ȃ�
	END,
};

//--------------------------------------------------
// **�U�����e��**
//�E�U���̓��e
//--------------------------------------------------
enum class AttackKind : int
{
	UP = 0,			//��i
	MIDDLE,			//���i
	DOWN,			//���i
	STEAL,			//�͂�
	SLAM,			//�@����(���������Ȃ犊���Ԃɂ���)
	LOCK,			//���b�N�Z
	PROJECTILE,		//��ѓ���
	TRACK,			//�z�[�~���O�_�b�V��
	NO_TO_OFFSET,	//���E���Ȃ��U��
	NO_TO_OFFSET_UP,//��i�U���Ƒ��E���Ȃ�
	DOWN_ATTACK,	//�_�E���U��
	END,
};



struct AttackParameter
{
public:
	YR_Vector3	distance;	//�v���C���[�̒��S���W����ǂꂾ������Ă��邩
	YR_Vector3	size;		//�U������̑傫��
	float		timer;		//�����t���[��
	float		damege;		//���̍U���ŗ^����_���[�W
	float		HB_timer;	//�U���𓖂Ă����̑���̂̂����莞��
	YR_Vector3	hitback;	//�U���𓖂Ă����̑���̐�����уx�N�g��
	AttackKind	type;		//�U���̑���(��i�E���i�E���i)
	float		knockback;	//�m�b�N�o�b�N(X�x�N�g���̂�)
	bool		gaugeout;	//false�Ȃ�Q�[�W�l���U��
	float		stealtimer; //�݂͂ʂ�����鎞��
	HitStopTime	HS_timer;	//�q�b�g�X�g�b�v����(3�p�^�[��)
	float		gauge_get;	//�Q�[�W�̊l����
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
	float				rightOrleft;//�E�����Ȃ�*1�������Ȃ�*-1
	HitResult			hit_result;	//�U���q�b�g���̌��ʂ�ۑ�����
	int					attack_name;//�U���ԍ�
	
	AttackParameter		parameter;	//�p�����[�^�[
	YR_Vector3			pos;		//�U���̎��ۂ̒��S
	YR_Vector3			speed;		//�U���̈ړ����x
	YR_Vector3			plus_speed;	//��ѓ���p�̏�ɕt�^����X�s�[�h

	Effekseer::Handle	handle;		//�G�t�F�N�g�̃n���h��
	EffectKind			effect_kind;//�G�t�F�N�g�̎��

	AttackBox()
	{
		effect_kind = EffectKind::NONE;

	};
	~AttackBox();

	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos);
	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed);
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
};
#endif // !_ATTACKBOX_H_


