//#pragma once

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "Skinned_mesh.h"

#include "YR_VectorMaster.h"
#include "HitBox.h"
#include "AttackBox.h"
#include "Tracking.h"
#include <memory>
#include "YRGamePad.h"
#include "Texture.h"
#include <vector>
#include <string>
#include "sprite.h"


constexpr float POS_Y = 0.0f;							//�Œ�̍���
constexpr float GAUGE_MAX = 100.0f;						//�Q�[�W�ő��
constexpr int	Track_max = 2;							//�z�[�~���O�_�b�V���ő吔
constexpr float	non_target = 110.0f;					//���̒l������ꂽ�t���[���͏�������O���悤�ɂ���
constexpr float target_max = 100.0f;					//�������ł��̒l�ȏ�͊O���悤�ɂ���
constexpr float attenuation_slam = 8.0f;				//���蒆���x�̌�����
constexpr float draw_guarf_effect_add_pos_x = 3.0f;		//�K�[�h�G�t�F�N�g�`�掞�̈ʒu��Ԓl
constexpr float Reflection_range_min = 100.0f;			//���̐��l�ȏ�̐�����тȂ�ǂŒ��˕Ԃ�
constexpr float Reflection_attenuation_factor = 0.8f;	//�ǔ��ˎ��̌�����
constexpr float cut_in_max_time = 0.5f;					//�J�b�g�C���\������
constexpr float passive_max_time = 0.2f;				//�󂯐g�����G����

//--------------------------------------
//	**�L�������ݒ�
//	�E�V�����L������ǉ�����Ȃ炱���ɋL��
//--------------------------------------
enum class PLSELECT :int
{
	KNIGHT = 0,
	KEN,
	PLSELECT_END,
};


//--------------------------------------
//	**�A�j���[�V�����I��
//	�ENodeChange���ɑ���A�A�j���[�V�������w�肷��
//--------------------------------------
enum class AnimAtk :int
{
	FREAM = 0,	//����
	LATER,		//�㌄
	TIMER,		//����
};



//--------------------------------------
//	**�U�����̃v���C���[�̒n�ʔ����
//--------------------------------------
enum class Ground_C :int
{
	AIR = 0,	//��
	GROUND,		//�n��
	EITHER,		//�ǂ���ł�
	END,
};



//---------------------------------------------
// **�X�s�[�h�N���X**
//�E�v���C���[�ɉ��Z����X,Y�������̗͂���������
//�E�U���֐����s����Set�֐��𓮂����A���x������
//�E�󒆍U�����̃|�b�v�Ȃǂɗ��p����
//----------------------------------------------
class Speed
{
public:
	float speed = 0.0f;
	const float decrease_value = 100.0f;		//�X�s�[�h�̌����l

	void Set(float speed)
	{
		this->speed = speed;
	};
	float Update(float elapsed_time)
	{
		if (speed > 0.0f)
		{
			speed -= (decrease_value * elapsed_time);
		}
		else
		{
			speed = 0.0f;
		}
		return speed;
	};
};


//---------------------------------------------
// **�X�e�[�g�N���X**
//�EActState�͍U���ȊO�̔��f������B���f���Ȃ����̂�case�ɓ���Ȃ�(���̑��̏���������̂��߂Ɏg�p����)
//�EAttackState��ActState��ATTACK�������ꍇ�ǂ̍U�������Ă��邩�𔻒f����
//----------------------------------------------
enum class ActState : int
{
	//�E�����̎�
	NONE = 0,		//�����Ȃ�(default)
	WAIT,			//�ҋ@(default)m
	GUARD,			//�K�[�h(default)
	SQUAT,			//���Ⴊ��(default)m
	RETREAT,		//���(default)m
	DUMMY,			//�_�~�[(���l��ԗp)
	DASH,			//�_�b�V��(default)m
	BACK,			//�o�b�N�X�e�b�v(default)m
	JUMP,			//�W�����v(default)m
	AIR_F,			//�󒆑O�_�b�V��(default)
	AIR_B,			//�󒆌�_�b�V��(default)
	STATENONE,		//�X�e�[�g��D��ꂽ��ԁB����ɒ͂܂�Ă�or���K�H
	WAKE,			//�_�E������N���オ��
	PASSIVE,		//�󂯐g���Ƃ�
	DOWN,			//�_�E�����
	FALL,			//�󒆂Ń_�E�����Ă�����
	KNOCK,			//�U�����󂯂Ă̂�����
	SLAM,			//�@���������
	DOWN_HIT,		//�_�E���U�����󂯂�
	ATTACK,			//�U����
	ACT_END,		//(�I�_)
};

enum class AttackState : int
{
	NONE = 0,		//�U������
	JAKU,			//��m
	THU,			//��(����)m
	D_KYO,			//����(���̎��ɏo�鋭�U��)
	D_JAKU,			//����m
	D_THU,			//����m
	U_KYO,			//�㋭(���U��)m
	A_JAKU,			//���U��m
	A_THU,			//�󒆍U��m
	A_KYO,			//�󋭍U��m
	A_UKYO,			//��㋭�U��(�ł��グ�U��)m
	STEAL,			//�͂�m
	SLOW,			//����m
	TRACK_DASH,		//�z�[�~���O�_�b�V��m
	JAKU_RHURF,		//�O��K�E
	THU_RHURF,		//�O���K�E
	KYO_RHURF,		//�O���K�E
	JAKU_LHURF,		//���K�E
	THU_LHURF,		//�㒆�K�E
	KYO_LHURF,		//�㋭�K�E
	SPECIAL_ATTACK,	//�O���K�Em
	DESIRE_SPECIAL,	//�㒴�K�E
	DESIRE_METEOR,	//�����Z
	EXTENDATK,		//���G�U��
	JAKU_THU,		//��̎��ɏo�钆�U��m
	JAKU_KYO,		//���̎��ɏo�鋭�U��m
	A_JAKU_RHURF,	//�󒆑O��K�E
	A_THU_RHURF,	//�󒆑O���K�E
	A_KYO_RHURF,	//�󒆑O���K�E
	A_JAKU_LHURF,	//�󒆌��K�E
	A_THU_LHURF,	//�󒆌㒆�K�E
	A_KYO_LHURF,	//�󒆌㋭�K�E
	A_SPECIAL_ATTACK,//�󒆑O���K�E
	A_DESIRE_SPECIAL,//�󒆌㒴�K�E

	COMBO_X,		//X�{�^���R���{
	COMBO_Y,		//Y�{�^���R���{
	COMBO_B,		//B�{�^���R���{

	VERSATILE_ATTACK,//A�{�^���������Əo��U��(�v���C���[�I��)
	A_VERSATILE_ATTACK,//A�{�^���������Əo��󒆍U��(�v���C���[�I��)

	ATTACK_END,		//�ő�T�C�Y
};

//Desire(�~�]�A�]��)��Crystal(����)


//---------------------------------------------
// **�󒆃_�b�V���X�e�[�g��**
//�ENONE�ȊO�̏ꍇ�͂��ꂼ��K�p�����������s��
//----------------------------------------------
enum class AirDashState : int
{
	NONE = 0,		//�����Ȃ�
	AIR_F,			//�󒆃_�b�V��
	AIR_B,			//�󒆃o�b�N�_�b�V��
	ACT_END,		//(�I�_)
};


//---------------------------------------------------------------
// **�U�����e�ۑ��\����**
//�E�U���̔����t���[���A�����p�����[�^�ϐ��A���̐�������ۑ�����\����
//---------------------------------------------------------------
struct AttackSingle
{
public:
	std::vector<AttackParameter>	parameter;		//�����p�����[�^�[
	int								quantity = 1;	//��x�̍U���Ő������铖���蔻��̐���
	float							fream = 0.0f;	//�����t���[��
};


//------------------------------------------------------------------------
// **�U�����X�g�N���X**
//�E�U���̃p�����[�^�[���L�����������ɓǂݍ��݁A
//�@�U�����s���ۂɓ����蔻��ɑΉ������p�����[�^�[�𑗂�
//			�ȉ��A�����p
//�P�F�U�������{�^��������
//�Q�F�v���C���[�ɔ����t���[���𑗂�
//�R�F�����t���[���ɂȂ�����SetAttack�֐��œ����蔻��𐶐�
//�S�F�����蔻��̎����͓����蔻��{�̂Œ�������׉����G��Ȃ�
//�T�F�U���������񔭐�����ꍇ��attack_max�ɂ��̉񐔂����Ă��̕����[�v������
//
//		�U�������蔻��
//�P�F��������Init�֐��Ńp�����[�^�[�𑗂�B
//		���̎��Adistance�ɓ����̂̓v���C���[����ǂꂾ������Ă��邩�Ƃ����l
//�Q�FUpdate�֐��̈����Ƀv���C���[��pos��n���A��Ɉʒu���X�V�����悤�ɂ���
//�R�FAttackBox�N���X�Ɂu�U������ɕt�^����X�s�[�h�𑝉�������֐��v�����AUpdate�Œl��ς���
//etc...Update���Fpos=player_pos+distance+speed;
//						speed�͊֐��ŕς����邽�߁Apos�̈ʒu���ς���Ă���
//-------------------------------------------------------------------------
class AttackList
{
public:
	AttackState						attack_name;	//�U����(�v���C���[�������ɕۑ�)
	int								attack_max;		//�U����
	std::vector<AttackSingle>		attack_single;	//�U�����e�ۑ�
	float							later;			//�㌄�t���[��
	int								now_attack_num;	//���ݎ��s���Ă���U���ԍ��͉��Ԃ�������(attack_max�܂ŉ������later���v���C���[��)
	PAD								linkage_button;	//�ǂ̃{�^���ōU���𔭐������邩
	Command							linkage_command;//�ǂ̃R�}���h�ōU���𔭐������邩
	Ground_C						ground_on;		//�U���͋󒆁A�n��A�ǂ���ł����������邩
	bool							squat_on;		//���Ⴊ�ݍU�����ǂ���(true�ł��Ⴊ�ݍU��)
	int								need_power;		//�U�����s���̂ɕK�v�ȃQ�[�W��(1�P�ʂŋL�q)
	PAD								linkage_stick;	//�ǂ̕����ւ̓��͂ōU���𔭐������邩
	AttackState						aid_attack_name;//�Q�[�W������Ȃ������ꍇ�o���Z
	AttackState						real_attack;	//���ۂ̍U��(��{��attack_name�Ɠ������̂����A����̍U���Ɠ����U�����o���ꍇ�͂��̍U����������)
	bool							speed_on;		//�X�s�[�h������ꍇ��true
	YR_Vector3						speed;			//�U������ɉ��Z���鑬�x
	float							advance_speed;	//�U�����O�i���鋗��
	AttackState						combo;			//�A�ł������ɂ���R���{
	HitResult						conditions_hit;	//�L�����Z�����邽�߂̍U���q�b�g����
	float							timer;			//��ѓ���̏ꍇ���g�ɕt�^���鎝������
	bool							traject_on;		//���̋O�Ղ�\������Ȃ�true
public:
	AttackList() : now_attack_num(0), attack_name(AttackState::NONE), later(0.0f),
		attack_max(0), linkage_button(PAD::BUTTOM_END), linkage_command(Command::NOCOMMAND), ground_on(Ground_C::GROUND), squat_on(false),
		need_power(0), linkage_stick(PAD::BUTTOM_END), aid_attack_name(AttackState::NONE), real_attack(attack_name),
		speed_on(false), speed(0.0f, 0.0f, 0.0f), advance_speed(0.0f), combo(AttackState::NONE), conditions_hit(HitResult::HIT),timer(0.0f),traject_on(true) {};
	//�U�������蔻��𐶐�����
	void SetAttack(std::vector<AttackBox> *atk, float rightOrleft, YR_Vector3 pl_pos)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->emplace_back();
			atk->back().Init(scastI(attack_name), attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos);
		}
		now_attack_num++;
	}

	//��ѓ���U�������蔻����Z�b�g����
	void SetAttack(std::vector<AttackBox>* atk, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->emplace_back();
			atk->back().Init(scastI(attack_name), attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos, plus_speed);
		}
		now_attack_num++;
	}
};




//---------------------------------------------------------------------------
// **�����蔻�胊�X�g�N���X**
//�E�����蔻��̃p�����[�^�[���e�s�����Ƃɕۑ�����N���X
//�E�v���C���[�N���X��Update�Ŗ���p�����[�^�[�����Ă���
//�E�����������蔻��̐��̓L�����ɂ���ĕς��ׁA�ϐ���vector�^�Ŏ������A
//�@�L�����������ɕK�v�Ȍ�����������
//----------------------------------------------------------------------------
class HitParameterList
{
public:
	std::array<HitParameter,scastI(ActState::ACT_END)>			act_parameter;		//�U���ȊO�̍s�����̓����蔻��
	std::array<HitParameter, scastI(AttackState::ATTACK_END)>	attack_parameter;	//�U�����̓����蔻��
};




//---------------------------------------------------------------
// **�A�j���[�V���������l�ۑ��\����**
//�E�����A�����A�㌄���ꂼ��̃A�j���[�V�������x�����l��ۑ�����\����
//---------------------------------------------------------------
struct Animation_Coordinate
{
public:
	float							fream = 1.0f;			//����
	float							timer = 1.0f;			//����
	float							later = 1.0f;			//�㌄
};


//---------------------------------------------------------------
// **�R���{�ۑ��\����**
//�E�R���{�̎�ށA���݂̔ԍ���ۑ�����\����
//---------------------------------------------------------------
struct ComboList
{
public:
	std::vector<AttackState>		combolist;		//�R���{���X�g
	int								now_pos = 0;	//���݂̃R���{�̈ʒu
};


class Player
{
protected:
	const float jump_max_time = 0.05f;	//�W�����v������jump_can_timer�ɓ����l
public:
	//�Q�[���������p�ϐ�
	float				hp = 0;
	int					jumpcount = 2;							//�W�����v�̉񐔕ێ�
	int					air_dash_count = 1;						//�󒆃_�b�V���̉񐔕ێ�
	bool				jumpflag = false;						//�W�����v���Ă���
	bool				max_jump_flag = false;					//�W�����v�̍ő�l�𒴂���
	float				jump_can_timer = 0.0f;					//�W�����v�\�ɂȂ�܂ł̎��Ԃ𑪒肷��(0.0�ȉ��ɂȂ�΃W�����v�\)
	bool				ground = true;							//TRUE�Ȃ�n�ʂɂ��Ă���
	bool				attack = false;							//TRUE�Ȃ�U����
	ActState			act_state = ActState::NONE;				//���̍s���B�܂��s���s�Ȃ�ǂ�������Ԃ�
	ActState			rival_state = ActState::NONE;			//����̍��̍s���BSceneGame�ő������
	AttackState			attack_state = AttackState::NONE;		//�����̍U�������Ă��邩
	AttackState			last_attack = AttackState::NONE;		//�Ō�ɂȂ�̍U����������
	AirDashState		air_dash_state = AirDashState::NONE;	//�󒆃_�b�V���X�e�[�g
	float				rightOrleft = 1.0f;						//�E�����Ȃ�*1�������Ȃ�*-1
	bool				moveflag = false;						//TRUE�Ȃ瓮���Ă���
	float				fream = non_target;						//�����t���[��
	float				later = non_target;						//�㌄�t���[��(�����l��non_target�����Ă���)
	float				timer = non_target;						//�����t���[��(�����l��non_target�����Ă���)
	float				knocktimer = 0.0f;						//��炢����
	float				gravity = 0.0f;							//�d�͒l
	float				down_force = 0.0f;						//�W�����v���ɏd�͂Ƃ͕ʂɓ����������̗�
	float				fall_force = 0.0f;						//�󒆃_�E�����ɏd�͂Ƃ͕ʂɓ����������̗�
	bool				finish = false;							//�s�����I�������Ƃ��̂�true
	bool				step = false;							//�X�e�b�v����true(���̏������s��Ȃ�)
	bool				hightrigger = false;					//�n�C�W�����v��true�A�ʏ펞false
	int					trackgauge = 2;							//�ǔ��_�b�V���̎c���
	float				gauge = 0.0f;							//�Q�[�W
	int					power = 1;								//�Q�[�W���ő�ɂȂ��1������B����Z�͂������������
	float				steal_escape = 0.0f;					//���������\����
	int					combo_count = 0;						//�R���{�J�E���g�B�������͍U�����󂯂��ۂɑ����A����ɑ���J�E���g
	int					now_player = 0;							//�ǂ̃v���C���[�����̃L�����𑀍삵�Ă��邩(1:1P�A2:2P)
	float				anim_ccodinate = 1.0f;					//�A�j���[�V�������x�𒲐�����ϐ�
	int					stop_state = 0;							//�q�b�g�X�g�b�v���̏����Ŏg�p
	float				cut_in_timer = 0.0f;					//�J�b�g�C���̕\������
	float				passive_timer = 0.0f;					//�󂯐g���̖��G���ԑ���ϐ�
	int					combo_count_player = 0;					//�R���{�J�E���g�B������͑��肩��󂯎��
	DirectX::XMFLOAT4	light_direction =
		DirectX::XMFLOAT4(-1.0f, -0.1, 1.0f, 0.0f);				//���C�g�̐i�ޕ���
	HitResult			hit_result = HitResult::NOT_OCCURRENCE;	//�U�������������ꍇ�̌��ʂ�ۑ�����
	HitResult			atk_result = HitResult::HIT;			//�U�����莞�ɃL�����Z���p�̏�����ۑ����邽�߂̕ϐ�
	std::vector<AttackBox> atk;									//�U�������蔻��
	std::vector<AttackBox> projectile_atk;						//��ѓ�����蔻��
	std::vector<HitBox>	hit;									//�����蔻��
	bool				traject_on = true;						//���̋O�Ղ�\������ꍇtrue(�U���������Ƀp�����[�^�[����擾����)

	std::array<Animation_Coordinate, scastI(AttackState::ATTACK_END)>	ac_attack;	//�U�����Ƃ̃A�j���[�V���������l
	std::array<Animation_Coordinate, scastI(ActState::ACT_END)>			ac_act;		//�s�����Ƃ̃A�j���[�V���������l

	float												jump_later = 1.0f;			//�W�����v�̒��n��
	float												dash_later = 1.0f;			//�_�b�V���̌㌄

	std::vector<AttackList>								attack_list;	//�U���̃��X�g�B�������ɓǂݍ��݁A�ۑ�����(�U���������Ƀp�����[�^�[�𑗂�)
	std::vector<HitParameterList>						hitparam_list;	//�����蔻��̃��X�g�B�������ɓǂݍ��݁A�ۑ�����(�����蔻��̐�������������)
	ComboList											combolist_X;	//X�{�^���R���{���X�g
	ComboList											combolist_Y;	//Y�{�^���R���{���X�g
	ComboList											combolist_B;	//B�{�^���R���{���X�g

public:
	//���f���p�ϐ�
	//MeshMotion						motion;
	//std::unique_ptr<Skinned_mesh>	base = nullptr;
public:
	//���W�A���f���`��p�ϐ�
	YR_Vector3		pos = { 0.0f,0.0f,0.0f };
	YR_Vector3		scale = { 1.0f,1.0f,1.0f };
	YR_Vector3		angle = { 0.0f,0.0f,0.0f };
	YR_Vector3		speed = { 0.0f,0.0f,0.0f };
	YR_Vector3		stop_pos = { 0.0f,0.0f,0.0f };
	Tracking		tracking;
	Speed			speed_X;
	Speed			speed_Y;
	std::unique_ptr<GamepadBase> pad;

	int				chara_color = 0;		//�L�����̃J���[�ԍ�


public:
	//�摜
	std::unique_ptr<Sprite> cutFrame = nullptr;		//�J�b�g�C���̃t���[��
	std::unique_ptr<Sprite> cutMask = nullptr;		//�J�b�g�C���̃}�X�N�摜
	std::unique_ptr<Sprite> cutIn = nullptr;		//�J�b�g�C���摜

public:
	//��{�����֐�
	virtual void Init(YR_Vector3 InitPos) = 0;
	virtual void LoadData(int color_number) = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision,float elapsed_time) = 0;
	virtual void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time ) = 0;

	virtual void DrawDEBUG(
		YRShader* geoshader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time) = 0;

	virtual void DrawCutIn(
		YRShader* shader,
		float elapsed_time
	) = 0;

	virtual void TextDraw()=0;

	virtual ~Player() = default;

public:
	//�s���n�֐�

	virtual void Move(float decision) = 0;
	virtual void MoveStop() = 0;
	virtual bool Step(float elapsed_time) = 0;
	virtual void AirDash(float elapsed_time) = 0;
	virtual void Jump() = 0;
	virtual void JumpUpdate(float decision, float elapsed_time) = 0;
	virtual void DamageCheck(float decision) = 0;
	virtual void Guard(float decision) = 0;
	virtual void Squat() = 0;
	virtual void GuardBack(float elapsed_time) = 0;
	virtual void FallUpdate(float elapsed_time) = 0;
	virtual void DownUpdate() = 0;
	virtual void PassiveUpdate(float elapsed_time) = 0;
	virtual void WakeUp() = 0;
	virtual void GaugeUp(float add) = 0;
	virtual void StateNone(float elapsed_time) = 0;
	virtual void KnockUpdate(float elapsed_time) = 0;
	virtual void DownHitUpdate(float elapsed_time) = 0;
	virtual void SlamUpdate(float elapsed_time) = 0;

	virtual void HitBoxTransition(HitBoxState state) = 0;

	virtual void AttackInput() = 0;
	virtual void Attack(float decision, float elapsed_time) = 0;
	virtual void AttackSwitch(float decision, float elapsed_time, AttackState attack_state = AttackState::NONE) = 0;

	virtual void WinAnimSet() = 0;

	virtual void NoneChange() = 0;

	virtual void StopUpdate() = 0;
	virtual void StopEnd() = 0;
	virtual void TrackDash(float decision, float elapsed_time) = 0;

	virtual bool Intro(float elapsed_time) = 0;
	virtual void IntroDEBUG() = 0;
	virtual bool WinPerformance(float elapsed_time) = 0;
	virtual void WinDEBUG() = 0;

	virtual void ReadySet() = 0;

	virtual bool AttackLoad() = 0;
	virtual bool AttackWrite() = 0;

	virtual bool ComboSet() = 0;					//�R���{�J�n���A�ǂ̍U�������Ă��邩���m�F���Ă��̌�̃R���{���Z�b�g����
	virtual void ComboUpdate() = 0;					//�R���{�X�V�B�X�e�[�g��ݒ肷��

	virtual void StopHitParamUpdate() = 0;

public:
	virtual bool AttackEndCheck() = 0;								//�U�������蔻�肪�S�ďI�����Ă��邩�m�F����
	virtual void EndAttackErase() = 0;								//�I�������U�������蔻���S�ď�������B
	virtual void AllAttackClear() = 0;								//�S�Ă̍U�������蔻�����������
	virtual void AttackUpdate(float elapsed_time) = 0;				//�U�����肪���݂��鎞�̂ݍX�V
	virtual void HitResultUpdate() = 0;								//�U�����肪���݂��鎞�̂ݍX�V(�����蔻�肪�擾�������ʂ��v���C���[�ɑ���)

};

#endif // !PLAYERBASE_H_

