#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "../PlayerBase/PlayerBase.h"
#include "../../YR_VectorMaster.h"
#include "../../Game/AttackBox.h"
#include "../../Game/HitBox.h"
#include <vector>
#include "../../Trajectory.h"
#include "../../Model/Geometric_primitive.h"


enum class KNIGHTHIT :int
{
	HEAD,
	BODY,
	LEG,
	END
};



//�i�C�g�N���X(�L�����{��)
class Knight : public Player
{
private:
	const size_t	max_traject_count = 128;	//���̋O�Ղ̍ő�l

public:

	//�J���[�`�F���W�p�e�N�X�`��
	std::shared_ptr<Texture>	color_texture_main = nullptr;
	std::shared_ptr<Texture>	color_texture_face = nullptr;

	//���̋O��
	Trajectory					traject;
	YR_Vector3					sword_head = { 0.0f,0.0f,0.0f };
	YR_Vector3					sword_tail = { 0.0f,0.0f,0.0f };

	~Knight();
	void CharaInit();
	void Uninit();
	void LoadData(PLCOLOR color_number);
	void Update(float decision, float elapsed_time);
	void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);

	void AttackDetailsSet(const AttackState &attack_state)override;

	void WinAnimSet();
	bool WinPerformance(float elapsed_time);


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

	void A_Jaku_Lhurf(float elapsed_time)override;
	void ExtendATK(float elapsed_time);



	//�p��

	bool Intro(float elapsed_time);

	void DrawDEBUGHitParam();

	//bool DEBUGAttackLoad();		//�f�o�b�O���p�����[�^�[�𐶐�����


	void ReadySet();			//�C���g����A�Q�[���J�n�܂ł̐ݒ���s��


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

	void ChangeFace(FaceAnim anim)override;				//�\���ς���֐�(enum�Œ�`)

	std::wstring RandTextSelect();						//�����_���ł��炩���ߐݒ肳�ꂽ�e�L�X�g��I�����ĕԂ�


	enum class TextList : int
	{
		NORMAL,
		WARLIKE,
		CRIOSITY,
		TEXT_END,
	};
};

#endif // !_KNIGHT_H_

