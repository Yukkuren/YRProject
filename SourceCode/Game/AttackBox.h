#ifndef _ATTACKBOX_H_
#define _ATTACKBOX_H_
#include "../YR_VectorMaster.h"
#include "../YRShader.h"
#include "../Effect.h"

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

	AttackParameter() : distance(0.0f, 0.0f, 0.0f), size(0.0f, 0.0f, 0.0f), timer(0.0f), damege(0.0f), HB_timer(0.0f), hitback(0.0f, 0.0f, 0.0f),
		type(AttackKind::MIDDLE), knockback(0.0f), gaugeout(false), stealtimer(0.0f), HS_timer(HitStopTime::SHORT), gauge_get(0.0f) {};
};


struct EffectParameter
{
	EffectKind			effect_kind;	//�G�t�F�N�g�̎��
	YR_Vector3			distance;		//�U������Ƃ̍��W�̈ʒu�̍�
	YR_Vector3			scale;			//�傫��
	YR_Vector3			axis;			//�p�x�̕���
	float				angle;			//�p�x
	bool				rightORleft;	//�p�x���v���C���[�Ɉˑ������邩

	EffectParameter() : effect_kind(EffectKind::NONE), distance(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), axis(0.0f, 0.0f, 0.0f),
	angle(0.0f),rightORleft(false){}
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

	EffectParameter		effect_param;//�G�t�F�N�g�̃p�����[�^�[
	Effekseer::Handle	handle;		//�G�t�F�N�g�̃n���h��

	AttackBox() : fin(false), attack(false), hit_ok(true), knock_start(false), plus(false), rightOrleft(0.0f), hit_result(HitResult::NONE), attack_name(0),
		parameter(), pos(0.0f, 0.0f, 0.0f), speed(0.0f, 0.0f, 0.0f), plus_speed(0.0f, 0.0f, 0.0f), effect_param(), handle() {};
	~AttackBox();

	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, EffectParameter effect_param);
	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed, EffectParameter effect_param);
	void Update(YR_Vector3 pl_pos, float elapsed_time, YR_Vector3 effect_pos = YR_Vector3(0.0f, 0.0f));
	void Draw(YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);			//DEBUG_MODE��true�̎��̂ݕ`��
	void SpeedPlus(YR_Vector3 plus_speed,float elapsed_time);
};
#endif // !_ATTACKBOX_H_


