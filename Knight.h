#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "PlayerBase.h"
#include "YR_VECTOR3.h"
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
	std::shared_ptr<Model>			damage_R_g_u = nullptr;
	std::shared_ptr<Model>			jaku_R_f = nullptr;
	std::shared_ptr<Model>			jaku_R_t = nullptr;
	std::shared_ptr<Model>			jaku_R_l = nullptr;
	std::shared_ptr<Model>			thu_R_f = nullptr;
	std::shared_ptr<Model>			thu_R_t = nullptr;
	std::shared_ptr<Model>			thu_R_l = nullptr;
	std::shared_ptr<Model>			kyo_R_f = nullptr;
	std::shared_ptr<Model>			kyo_R_t = nullptr;
	std::shared_ptr<Model>			kyo_R_l = nullptr;
	std::shared_ptr<Model>			d_jaku_R_f = nullptr;
	std::shared_ptr<Model>			d_jaku_R_t = nullptr;
	std::shared_ptr<Model>			d_jaku_R_l = nullptr;
	std::shared_ptr<Model>			d_thu_R_f = nullptr;
	std::shared_ptr<Model>			d_thu_R_t = nullptr;
	std::shared_ptr<Model>			d_thu_R_l = nullptr;
	std::shared_ptr<Model>			u_kyo_R_f = nullptr;
	std::shared_ptr<Model>			u_kyo_R_t = nullptr;
	std::shared_ptr<Model>			u_kyo_R_l = nullptr;
	std::shared_ptr<Model>			a_jaku_R_f = nullptr;
	std::shared_ptr<Model>			a_jaku_R_t = nullptr;
	std::shared_ptr<Model>			a_jaku_R_l = nullptr;
	std::shared_ptr<Model>			a_thu_R_f = nullptr;
	std::shared_ptr<Model>			a_thu_R_t = nullptr;
	std::shared_ptr<Model>			a_thu_R_l = nullptr;
	std::shared_ptr<Model>			a_kyo_R_f = nullptr;
	std::shared_ptr<Model>			a_kyo_R_t = nullptr;
	std::shared_ptr<Model>			a_kyo_R_l = nullptr;
	std::shared_ptr<Model>			a_ukyo_R_f = nullptr;
	std::shared_ptr<Model>			a_ukyo_R_t = nullptr;
	std::shared_ptr<Model>			a_ukyo_R_l = nullptr;
	std::shared_ptr<Model>			steal_R_f = nullptr;
	std::shared_ptr<Model>			steal_R_t = nullptr;
	std::shared_ptr<Model>			steal_R_l = nullptr;
	std::shared_ptr<Model>			slow_R_f = nullptr;
	std::shared_ptr<Model>			slow_R_t = nullptr;
	std::shared_ptr<Model>			slow_R_l = nullptr;
	std::shared_ptr<Model>			track_R_f = nullptr;
	std::shared_ptr<Model>			track_R_t = nullptr;
	std::shared_ptr<Model>			track_R_l = nullptr;
	std::shared_ptr<Model>			jaku_rh_R_f = nullptr;
	std::shared_ptr<Model>			jaku_rh_R_t = nullptr;
	std::shared_ptr<Model>			jaku_rh_R_l = nullptr;
	std::shared_ptr<Model>			thu_rh_R_f = nullptr;
	std::shared_ptr<Model>			thu_rh_R_t = nullptr;
	std::shared_ptr<Model>			thu_rh_R_l = nullptr;
	std::shared_ptr<Model>			kyo_rh_R_f = nullptr;
	std::shared_ptr<Model>			kyo_rh_R_t = nullptr;
	std::shared_ptr<Model>			kyo_rh_R_l = nullptr;
	std::shared_ptr<Model>			jaku_lh_R_f = nullptr;
	std::shared_ptr<Model>			jaku_lh_R_t = nullptr;
	std::shared_ptr<Model>			jaku_lh_R_l = nullptr;
	std::shared_ptr<Model>			thu_lh_R_f = nullptr;
	std::shared_ptr<Model>			thu_lh_R_t = nullptr;
	std::shared_ptr<Model>			thu_lh_R_l = nullptr;
	std::shared_ptr<Model>			kyo_lh_R_f = nullptr;
	std::shared_ptr<Model>			kyo_lh_R_t = nullptr;
	std::shared_ptr<Model>			kyo_lh_R_l = nullptr;
	std::shared_ptr<Model>			special_R_f = nullptr;
	std::shared_ptr<Model>			special_R_t = nullptr;
	std::shared_ptr<Model>			special_R_l = nullptr;
	std::shared_ptr<Model>			disire_s_R_f = nullptr;
	std::shared_ptr<Model>			disire_s_R_t = nullptr;
	std::shared_ptr<Model>			disire_s_R_l = nullptr;
	std::shared_ptr<Model>			disire_m_R_f = nullptr;
	std::shared_ptr<Model>			disire_m_R_t = nullptr;
	std::shared_ptr<Model>			disire_m_R_l = nullptr;
	std::shared_ptr<Model>			extend_R_f = nullptr;
	std::shared_ptr<Model>			extend_R_t = nullptr;
	std::shared_ptr<Model>			extend_R_l = nullptr;

public:
	std::vector <std::shared_ptr<Model>> model_R;
};



//�i�C�g�N���X(�L�����{��)
class Knight : public Player
{
private:
	const float walkspeed = 10.1f;		//�������x(���)
	const float dashspeed = 30.1f;		//�_�b�V�����x
	const float backstepS = 0.5f;
	const float stepspeed = 0.5f;
	const float jump_max = 100.0f;		//�W�����v�̍ő呬�x(������ƌ������n�ߗ�����)
	const float high_jump_max = 120.0f;	//�n�C�W�����v�̍ő呬�x(������ƌ������n�ߗ�����)
	const float brake_speed =10000.0f;	//��~���ɂ�����u���[�L(��{�s�^�b�Ǝ~�܂�̂Ő��l�͑傫��)

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
	
	std::array< HitBox, static_cast<int>(KNIGHTHIT::END)> hit;
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
		const DirectX::XMFLOAT4& light_direction,
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
	bool Step();
	void MoveAnimSet();
	void Jump();
	void JumpUpdate(float elapsed_time);
	void DamageCheck();
	void KnockUpdate(float elapsed_time);
	void Guard(float decision);
	void GuardBack(float elapsed_time);
	void Squat();
	void FallUpdate();
	void DownUpdate();
	void PassiveUpdate();
	void WakeUp();
	void GaugeUp(float add);
	void CancelList();
	void AttackDetailsSet();

	void GuardAnimSet();
	void WaitAnimSet();
	void WinAnimSet();
	bool WinPerformance();


	void AttackInput();	//�U������{�^���������ꂽ���m�F���A������Ă���΂��̍s����������
	void Attack(float decision, float elapsed_time);		//�ȉ��̊֐��𐧌䂷��


	//----------------------------------//
	//			�U���֐�					//
	//----------------------------------//
	void Jaku(float elapsed_time);
	void Thu(float elapsed_time);
	void Kyo(float fream, float elapsed_time);
	void D_Jaku(float elapsed_time);
	void D_Thu(float fream, float elapsed_time);
	void U_Kyo(float fream, float elapsed_time);
	void Hadouken(float elapsed_time);
	void Thu_Hadouken(float elapsed_time);
	void Kyo_Hadouken(float elapsed_time);
	void HadouUpdate(float elapsed_time);
	void Thu_HadouUpdate(float elapsed_time);
	void Kyo_HadouUpdate(float elapsed_time);
	void P_Kyo(float elapsed_time);
	void TrackDash(float decision, float elapsed_time);
	void Steal(float elapsed_time);
	void Slow(float elapsed_time);
	void SpecialAttack(float elapsed_time);

	void Panish_N(float elapsed_time);
	void Panish_H(float elapsed_time);

	void ExtendATK(float elapsed_time);

	void StateNone();


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

	bool Intro();

	bool AttackLoad();			//�������U���p�����[�^�[��ǂݍ���
	bool AttackWrite();			//�f�o�b�N���p�����[�^�[�������o��
	bool AttackClean();			//�f�o�b�O���p�����[�^�[��S�ď���������
	bool DEBUGAttackLoad();		//�f�o�b�O���p�����[�^�[�𐶐�����

	bool AttackEndCheck();		//�U�������蔻�肪�S�ďI�����Ă��邩�m�F����
	void EndAttackErase();		//�I�������U�������蔻���S�ď�������B
	void AllAttackClear();		//�S�Ă̍U�������蔻�����������
	void AttackUpdate(float elapsed_time);		//�U�����肪���݂��鎞�̂ݍX�V

	//�C���g���p��
	enum class INTRO_KNIGHT : int
	{
		SET = 0,
		CAMERA_ZOOM,
		FINISH,
	};
	INTRO_KNIGHT	intro_state;

	//�������o�p��
	enum class WIN_PERFORMANCE_KNIGHT : int
	{
		CAMERA_ZOOM = 0,
		FINISH,
	};
	WIN_PERFORMANCE_KNIGHT win_state;



public:
	//�e�N�X�`���A�j���[�V�����p

	//���݂̊�̏��
	enum class FaceAnim : int
	{
		NORMAL = 0,
		WINK,
		Damage,
		YARUKI,
		KOUHUN,
	};

	//�ڂ̎��ʗp��
	enum FaceEye_Num : int
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

	DirectX::XMFLOAT2	face_eye_offset[FaceEye_Num::EYE_MAX] =
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
	enum FaceMouse_Num : int
	{
		NORMAL_MOUSE = 0,
		TOZI,
		KUMON,
		KURI,
		OOGUTI,
		OTYOBO,
		POKAN,
		MOUSE_MAX,
	};

	DirectX::XMFLOAT2	face_mouse_offset[FaceMouse_Num::MOUSE_MAX] =
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
	float face_wink_time;

	//�܂΂����p��
	enum Wink_State : int
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOURTH,
		FIVE,
		SIX,
	};

	Wink_State wink_state = Wink_State::FIRST;

	DirectX::XMFLOAT2	eye_offset = { 0.0f,0.0f };
	DirectX::XMFLOAT2	mouse_offset = { 0.0f,0.0f };

	void FaceAnimation(float elapsed_time);
	void FaceWink(float elapsed_time);

	void ChangeFace(FaceAnim anim);
};

#endif // !_KNIGHT_H_

