#ifndef _KNIGHT_H_
#define _KNIGHT_H_

#include "PlayerBase.h"
#include "YR_VECTOR3.h"
#include "AttackBox.h"
#include "HitBox.h"


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

class Knight : public Player
{
private:
	const float walkspeed = 0.05f;
	const float dashspeed = 0.1f;
	const float backstepS = 0.5f;
	const float stepspeed = 0.5f;
	const float jump_max = 0.5f;
	const float high_jump_max = 0.5f;
public:
	bool fast;
	YR_Vector3 FastPos;
	YR_Vector3 hadou;
	float hadouspeed;
	
	std::array<AttackBox, static_cast<int>(KNIGHTATK::ATKEND)> atk;
	std::array< HitBox, static_cast<int>(KNIGHTHIT::END)> hit;
	std::array<YR_Vector3, scastI(KNIGHTHIT::END)> Hitplus;
	std::array<YR_Vector3, scastI(KNIGHTHIT::END)> HitSize;
	

	~Knight();
	void Init(YR_Vector3 InitPos);
	void Uninit();
	void LoadData();
	void Update(float decision, float elapsed_time);
	void Draw(
		YRShader				*shader,
		YRShader				*geoshader,
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
	void JumpUpdate();
	void DamageCheck();
	void KnockUpdate();
	void Guard(float decision);
	void GuardBack();
	void Squat();
	void FallUpdate();
	void DownUpdate();
	void PassiveUpdate();
	void WakeUp();
	void GaugeUp(float add);
	void CancelList();

	void GuardAnimSet();
	void WaitAnimSet();
	void WinAnimSet();
	bool WinPerformance();


	void Jaku();
	void Thu(int fream);
	void Kyo(int fream);
	void D_Jaku();
	void D_Thu(int fream);
	void U_Kyo(int fream);
	void Hadouken();
	void Thu_Hadouken();
	void Kyo_Hadouken();
	void HadouUpdate();
	void Thu_HadouUpdate();
	void Kyo_HadouUpdate();
	void P_Kyo();
	void TrackDash(float decision);
	void Steal();
	void Slow();
	void Panish_N();
	void Panish_H();

	void ExtendATK();

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


	//�U���ȊO�̃X�e�[�g
	enum
	{
		NONE = 0,
		WAIT,
		STATENONE,
		SLOW,
		KNOCK,
		FALL,
		DOWN,
		WAKE,
		PASSIVE,
		GUARD,
		SQUAT,
		MOVER,
		MOVEL,
		DASH,
		BACK,
		JUMP,
		AIR_F,
		AIR_B,
		TRACK_DASH,
		STEAL,
		JAKU,
		THU,
		KYO,
		D_JAKU,
		D_THU,
		U_KYO,
		HADOUKEN,
		THU_HADOUKEN,
		KYO_HADOUKEN,
		P_KYO,
		PANISH_N,
		PANISH_H,
		EXTENDATK,
	};

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
};

#endif // !_KNIGHT_H_

