#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "PlayerBase.h"
#include "YR_VectorMaster.h"
#include "AttackBox.h"
#include "HitBox.h"
#include <vector>
#include "YRModelAnim.h"


enum class KNIGHTHIT :int
{
	BODY,
	LEG,
	END
};

enum class KNIGHTATK :int
{
	ONE,
	TWO,
	END,
	HADOU,
	THU_HADOU,
	KYO_HADOU,
	ATKEND
};

//�ʃ��[�V�����f�[�^�i�[�\����
struct Model_MotionData
{
	std::shared_ptr<Model>			wait_R = nullptr;
	std::shared_ptr<Model>			wait_L = nullptr;
	std::shared_ptr<Model>			guard_R = nullptr;
	std::shared_ptr<Model>			guard_L = nullptr;
	std::shared_ptr<Model>			slid_R = nullptr;
	std::shared_ptr<Model>			slid_L = nullptr;
	std::shared_ptr<Model>			air_back_R = nullptr;
	std::shared_ptr<Model>			air_back_L = nullptr;
	std::shared_ptr<Model>			air_dash_R = nullptr;
	std::shared_ptr<Model>			air_dash_L = nullptr;
	std::shared_ptr<Model>			passive_R = nullptr;
	std::shared_ptr<Model>			passive_L = nullptr;
	std::shared_ptr<Model>			squat_R = nullptr;
	std::shared_ptr<Model>			squat_L = nullptr;
	std::shared_ptr<Model>			walk_R = nullptr;
	std::shared_ptr<Model>			walk_L = nullptr;
	std::shared_ptr<Model>			back_R = nullptr;
	std::shared_ptr<Model>			back_L = nullptr;
	std::shared_ptr<Model>			dash_R = nullptr;
	std::shared_ptr<Model>			dash_L = nullptr;
	std::shared_ptr<Model>			backstep_R = nullptr;
	std::shared_ptr<Model>			backstep_L = nullptr;
	std::shared_ptr<Model>			jump_R = nullptr;
	std::shared_ptr<Model>			jump_L = nullptr;
	std::shared_ptr<Model>			air_jump_R = nullptr;
	std::shared_ptr<Model>			air_jump_L = nullptr;
	std::shared_ptr<Model>			intro_R = nullptr;
	std::shared_ptr<Model>			intro_L = nullptr;
	std::shared_ptr<Model>			win_R = nullptr;
	std::shared_ptr<Model>			win_L = nullptr;
	std::shared_ptr<Model>			damage_R_g_u = nullptr;
	std::shared_ptr<Model>			damage_L_g_u = nullptr;
	std::shared_ptr<Model>			jaku_R = nullptr;
	std::shared_ptr<Model>			jaku_L = nullptr;
	std::shared_ptr<Model>			thu_R = nullptr;
	std::shared_ptr<Model>			thu_L = nullptr;
	std::shared_ptr<Model>			kyo_R = nullptr;
	std::shared_ptr<Model>			kyo_L = nullptr;
	std::shared_ptr<Model>			d_jaku_R = nullptr;
	std::shared_ptr<Model>			d_jaku_L = nullptr;
	std::shared_ptr<Model>			d_thu_R = nullptr;
	std::shared_ptr<Model>			d_thu_L = nullptr;
	std::shared_ptr<Model>			u_kyo_R = nullptr;
	std::shared_ptr<Model>			u_kyo_L = nullptr;
	std::shared_ptr<Model>			a_jaku_R = nullptr;
	std::shared_ptr<Model>			a_jaku_L = nullptr;
	std::shared_ptr<Model>			a_thu_R = nullptr;
	std::shared_ptr<Model>			a_thu_L = nullptr;
	std::shared_ptr<Model>			a_kyo_R = nullptr;
	std::shared_ptr<Model>			a_kyo_L = nullptr;
	std::shared_ptr<Model>			a_ukyo_R = nullptr;
	std::shared_ptr<Model>			a_ukyo_L = nullptr;
	std::shared_ptr<Model>			steal_R = nullptr;
	std::shared_ptr<Model>			steal_L = nullptr;
	std::shared_ptr<Model>			slow_R = nullptr;
	std::shared_ptr<Model>			slow_L = nullptr;
	std::shared_ptr<Model>			track_R = nullptr;
	std::shared_ptr<Model>			track_L = nullptr;
	std::shared_ptr<Model>			jaku_rh_R = nullptr;
	std::shared_ptr<Model>			jaku_rh_L = nullptr;
	std::shared_ptr<Model>			thu_rh_R = nullptr;
	std::shared_ptr<Model>			thu_rh_L = nullptr;
	std::shared_ptr<Model>			kyo_rh_R = nullptr;
	std::shared_ptr<Model>			kyo_rh_L = nullptr;
	std::shared_ptr<Model>			jaku_lh_R = nullptr;
	std::shared_ptr<Model>			jaku_lh_L = nullptr;
	std::shared_ptr<Model>			thu_lh_R = nullptr;
	std::shared_ptr<Model>			thu_lh_L = nullptr;
	std::shared_ptr<Model>			kyo_lh_R = nullptr;
	std::shared_ptr<Model>			kyo_lh_L = nullptr;
	std::shared_ptr<Model>			special_R = nullptr;
	std::shared_ptr<Model>			special_L = nullptr;
	std::shared_ptr<Model>			disire_s_R = nullptr;
	std::shared_ptr<Model>			disire_s_L = nullptr;
	std::shared_ptr<Model>			disire_m_R = nullptr;
	std::shared_ptr<Model>			disire_m_L = nullptr;
	std::shared_ptr<Model>			extend_R = nullptr;
	std::shared_ptr<Model>			extend_L = nullptr;

public:
	std::vector <std::shared_ptr<Model>> model_R;
	std::vector <std::shared_ptr<Model>> model_L;
};



//�i�C�g�N���X(�L�����{��)
class Knight : public Player
{
private:
	const float walkspeed = 10.1f;		//�������x(���)
	const float dashspeed = 40.1f;		//�_�b�V�����x
	const float backstepS = 116.0f;		//�o�b�N�X�e�b�v�̑��x
	const float backstepD = 500.0f;		//�o�b�N�X�e�b�v�̌������x
	const float stepspeed = 100.0f;		//�󒆃X�e�b�v�̑��x
	const float stepD = 500.0f;			//�󒆃X�e�b�v�̌������x
	const float jump_max = 108.0f;		//�W�����v�̍ő呬�x(������ƌ������n�ߗ�����)
	const float high_jump_max = 47.0f;	//�n�C�W�����v�̍ő呬�x(������ƌ������n�ߗ�����)
	const float jump_speed = 108.0f;		//���t���[���W�����v�̑��x�ɉ��Z���鐔�l
	const float high_jump_speed = 139.0f;	//���t���[���n�C�W�����v�̑��x�ɉ��Z���鐔�l
	const float brake_speed =10000.0f;		//��~���ɂ�����u���[�L(��{�s�^�b�Ǝ~�܂�̂Ő��l�͑傫��)
	const float track_speed = 100.0f;		//�z�[�~���O�_�b�V���̑��x
	const YR_Vector3 passive_speed = { 70.0f,30.0f };	//�󂯐g��ԂɃX�s�[�h�ɑ�����鑬�x
	const float passive_brake = 100.0f;		//�󂯐g���Ɍ������鑬�x

	float		production_time = 0.0f;	//�J�������o�Ɏg���ϐ�
	
	std::shared_ptr<Model>			main = nullptr;	//���f�����C���f�[�^(���b�V���̂݁B�A�j���[�V��������)
	std::unique_ptr<ModelAnim>		anim = nullptr;	//���f���`��&�A�j���[�V�����K�p�ϐ�

	//�ʃ��[�V�����p�ϐ�
	Model_MotionData				model_motion;

public:

	bool fast;
	YR_Vector3 FastPos;
	YR_Vector3 hadou;
	float hadouspeed;

	Model::Material_Attribute		lumi_material;
	
	//std::array< HitBox, static_cast<int>(KNIGHTHIT::END)> hit;
	//std::array<YR_Vector3, scastI(KNIGHTHIT::END)> Hitplus;
	//std::array<YR_Vector3, scastI(KNIGHTHIT::END)> HitSize;
	
	//�J���[�`�F���W�p�e�N�X�`��
	std::shared_ptr<Texture>	color_texture_main = nullptr;
	std::shared_ptr<Texture>	color_texture_face = nullptr;

	~Knight();
	void Init(YR_Vector3 InitPos);
	void Uninit();
	void LoadData(int color_number);
	void Update(float decision, float elapsed_time);
	void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);

	void DrawDEBUG(
		YRShader* geoshader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);
	//float GetPosX();

	void Move(float decision);
	bool Step(float elapsed_time);
	void AirDash(float elapsed_time);
	void MoveAnimSet();
	void Jump();
	void JumpUpdate(float decision, float elapsed_time);
	void DamageCheck(float decision);
	void KnockUpdate(float elapsed_time);
	void DownHitUpdate(float elapsed_time);
	void SlamUpdate(float elapsed_time);
	void Guard(float decision);
	void GuardBack(float elapsed_time);
	void Squat();
	void FallUpdate(float elapsed_time);
	void DownUpdate();
	void PassiveUpdate(float elapsed_time);
	void WakeUp();
	void GaugeUp(float add);
	void AttackDetailsSet(const AttackState &attack_state);

	void HitBoxTransition(HitBoxState state);

	void GuardAnimSet();
	void WaitAnimSet();
	void WinAnimSet();
	bool WinPerformance(float elapsed_time);


	void AttackInput();	//�U������{�^���������ꂽ���m�F���A������Ă���΂��̍s����������
	void Attack(float decision, float elapsed_time);		//�ȉ��̊֐��𐧌䂷��
	void AttackSwitch(float decision, float elapsed_time, AttackState attack_state = AttackState::NONE);	//attack_state�ɂ���Ă��ꂼ��قȂ�֐��𓮂���
	void AttackDefault(float elapsed_time);//����ȋL�q�̂���U���ȊO�͂��̊֐����g�p����
	void AttackProjectileDefault(float elapsed_time);//����ȋL�q�̂����ѓ���U���ȊO�͂��̊֐����g�p����


	//----------------------------------//
	//			�U���֐�					//
	//----------------------------------//
	void Jaku(float elapsed_time);
	void Thu(float elapsed_time);
	void Kyo(float elapsed_time);
	void U_Kyo(float elapsed_time);
	void D_Jaku(float elapsed_time);
	void D_Thu(float elapsed_time);
	void A_Jaku(float elapsed_time);
	void A_Thu(float elapsed_time);
	void A_Kyo(float elapsed_time);
	void A_UKyo(float elapsed_time);
	void Jaku_Rhurf(float elapsed_time);
	void Thu_Rhurf(float elapsed_time);
	void Kyo_Rhurf(float elapsed_time);
	
	void TrackDash(float decision, float elapsed_time);
	void Steal(float elapsed_time);
	void Slow(float elapsed_time);
	void SpecialAttack(float elapsed_time);

	void Jaku_Lhurf(float elapsed_time);
	void Thu_Lhurf(float elapsed_time);
	void Kyo_Lhurf(float elapsed_time);

	void A_Jaku_Lhurf(float elapsed_time);
	void ExtendATK(float elapsed_time);

	void StateNone(float elapsed_time);


	void DrawFastMove(YR_Vector3 position);
	void FastSet(YR_Vector3 position);



	//�p��
	float Getapply(float n);
	HitBox* GetHit();
	AttackBox* GetAttack();
	int GetMax(int n);
	//float& GetPosX2();

	void NoneChange();

	void StopUpdate();
	void StopEnd();

	bool Intro(float elapsed_time);

	bool AttackLoad();			//�������U���p�����[�^�[��ǂݍ���
	bool AttackWrite();			//�f�o�b�N���p�����[�^�[�������o��
	bool AttackClean();			//�f�o�b�O���p�����[�^�[��S�ď���������
	bool DEBUGAttackLoad();		//�f�o�b�O���p�����[�^�[�𐶐�����

	bool AttackEndCheck();		//�U�������蔻�肪�S�ďI�����Ă��邩�m�F����
	void EndAttackErase();		//�I�������U�������蔻���S�ď�������B
	void AllAttackClear();		//�S�Ă̍U�������蔻�����������
	void AttackUpdate(float elapsed_time);		//�U�����肪���݂��鎞�̂ݍX�V

	void ReadySet();			//�C���g����A�Q�[���J�n�܂ł̐ݒ���s��

	bool ComboSet();
	void ComboUpdate();

	void ComboX(float decision, float elapsed_time);	//X�{�^���R���{�֐�
	void ComboY(float decision, float elapsed_time);	//X�{�^���R���{�֐�
	void ComboB(float decision, float elapsed_time);	//X�{�^���R���{�֐�

	bool StealRangeCheck();

	//�C���g���p��
	enum class INTRO_KNIGHT : int
	{
		SET = 0,	//�Z�b�g����
		WAIT,		//�ォ�猩��
		ZOOM_SET,	//�Z�b�g����
		ZOOM,		//���p�N������
		PULL,		//��������
		PUSH,		//�������
		PULL_2,		//��������
		PUSH_2,		//�������
		FIN_SET,	//�Z�b�g
		FIN,		//�΂ߌ�납�猩��
		FINISH,
	};
	INTRO_KNIGHT	intro_state;

	float			intro_timer;		//�C���g���Ŏg�p����

	YR_Vector3 eye_plus;				//�J�����̍��W�Ƀv���X����l
	YR_Vector3 focus_plus;				//�J�����̒����_�Ƀv���X����l

	void IntroDEBUG();

	//�������o�p��
	enum class WIN_PERFORMANCE_KNIGHT : int
	{
		SET = 0,
		ZOOM,
		PULL,
		STOP,
		FINISH,
	};
	WIN_PERFORMANCE_KNIGHT	win_state;

	float					win_timer;

	void WinDEBUG();

	//����Z�J���������p��
	enum class CAMERA_STATE_KNIGHT : int
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,
		ELEVEN,
		TWELVE,
		THIRTEEN,
		STATE_END,
	}; CAMERA_STATE_KNIGHT camera_state_knight;



public:
	//�e�N�X�`���A�j���[�V�����p

	//���݂̊�̏��
	enum class FaceAnim : int
	{
		NORMAL = 0,
		NORMAL_LIP_SYNC,
		WINK,
		Damage,
		YARUKI,
		KOUHUN,
		TOZI,
		YEAH,
	};

	//�ڂ̎��ʗp��
	enum class FaceEye_Num : int
	{
		NORMAL_EYE = 0,
		WINK1,
		WINK2,
		KIRAME,
		CLOSE,
		TURI,
		KAOMOJI,
		EYE_MAX,
	};

	DirectX::XMFLOAT2	face_eye_offset[scastI(FaceEye_Num::EYE_MAX)] =
	{
		{ 0.0f,0.0f },
		{ 0.2f,0.0f },
		{ 0.4f,0.0f },
		{ 0.605f,0.0f },
		{ 0.0f,0.1f },
		{ 0.2f,0.1f },
		{ 0.39f,0.1f }
	};

	//���̗�
	enum class FaceMouth_Num : int
	{
		NORMAL_MOUSE = 0,
		TOZI,
		KUMON,
		KURI,
		OOGUTI,
		OTYOBO,
		POKAN,
		MOUTH_MAX,
	}; FaceMouth_Num face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;

	static constexpr std::array<FaceMouth_Num,4> lip_sync_can =
	{
		FaceMouth_Num::NORMAL_MOUSE,
		FaceMouth_Num::TOZI,
		FaceMouth_Num::OOGUTI,
		FaceMouth_Num::KURI,
	};

	DirectX::XMFLOAT2	face_mouth_offset[scastI(FaceMouth_Num::MOUTH_MAX)] =
	{
		{ 0.0f,0.0f },
		{ 0.2f,0.0f },
		{ 0.4f,0.0f },
		{ 0.0f,0.1f },
		{ 0.2f,0.1f },
		{ 0.0f,0.09f },
		{ 0.0f,0.11f }
	};

	FaceAnim face_anim = FaceAnim::NORMAL;
	float face_wink_time = 0.0f;				//�E�C���N�����p�ϐ�
	float face_wink_interval_timer = 0.0f;		//�E�C���N�̊Ԋu�𒲐�����ϐ�
	float wink_interval = 3.0f;					//�E�C���N�̊Ԋu
	float lip_sync_time = 0.0f;					//���p�N�����p�ϐ�

	//�܂΂����p��
	enum class Wink_State : int
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOURTH,
		FIVE,
		SIX,
	};Wink_State wink_state = Wink_State::FIRST;


	DirectX::XMFLOAT2	eye_offset = { 0.0f,0.0f };
	DirectX::XMFLOAT2	mouth_offset = { 0.0f,0.0f };

	void FaceAnimation(float elapsed_time);				//�\��̃A�j���[�V��������
	void FaceWink(float elapsed_time);					//�E�C���N�̏���
	bool FaceWink_bool(float elapsed_time);				//�E�C���N�̏���(���p�N�Ȃǂƍ��킹��p)
	void FaceLipSync(float elapsed_time);				//���p�N�̏���

	void ChangeFace(FaceAnim anim);						//�\���ς���֐�(enum�Œ�`)

	std::wstring RandTextSelect();						//�����_���ł��炩���ߐݒ肳�ꂽ�e�L�X�g��I�����ĕԂ�

	std::wstring lip_text;								//�\������e�L�X�g
	bool		text_on = false;						//true�Ȃ�e�L�X�g��\������

	void TextDraw();									//�e�L�X�g��`�悷��
	enum class TextList : int
	{
		NORMAL,
		WARLIKE,
		CRIOSITY,
		TEXT_END,
	};
};

#endif // !_KNIGHT_H_

