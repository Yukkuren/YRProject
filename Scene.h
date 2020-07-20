#pragma once

#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_win32.h"
#include "./imgui/imgui_impl_dx11.h"
#include "Key.h"
#include <directxmath.h>
//#include <memory>
#include< Windows.h>
#include "YR_VECTOR3.h"
#include "YRGamePad.h"
#include <thread>
#include "PlayerBase.h"
#include "sprite.h"
#include "YRShader.h"
#include "Geometric_primitive.h"
#include "Board_primitive.h"
#include "Sampler.h"
#include "AnimBoard.h"

enum class INPUT_PLAYER : int
{
	P1 = 0,
	NONE,
	P2
};

class SceneBase
{
public:

	//fedo_alpha�̒l��0.0f~1.0f�ɓ������ăt�F�[�h������
	//fedo_start�̓t�F�[�h�A�E�g���邩�ǂ����̃t���O�Bfalse�̏ꍇ��fedo_alha������������
	float	fedo_alpha;
	bool	fedo_start;

	virtual void Init() {};
	virtual void Update(float elapsed_time) {};
	virtual void Draw(float elapsed_time) {};
};

//�v���C���[����\����
//{
//	HP_MAX�`�F�v���C���[��HP�̍ő�l
//	ratio�`�FHP�`�掞�Ɏg�p
//	power�`�F�Q�[�W�̐F
//	correction_value�F�Ӗ��͒m���B�Ȃ񂩉摜�̈ʒu�����Ɏg���Ă�
//	gauge�`�F�`�掞�Ɏg�p�B������̓Q�[�W
//	pos�`�F�v���C���[�̏����ʒu
//}
struct PlayerALL
{
	float				HP_MAX1P = 0.0f;
	float				HP_MAX2P = 0.0f;
	float				ratio1P	= 0.0f;
	float				ratio2P = 0.0f;
	DirectX::XMFLOAT4	power1P = { 0.0f,0.0f,0.0f,0.0f };
	DirectX::XMFLOAT4	power2P = { 0.0f,0.0f,0.0f,0.0f };
	float				correction_value = 0.0f;
	float				gauge1P = 0.0f;
	float				gauge2P = 0.0f;
	YR_Vector3			pos1P{ -5.0f,0.0f };
	YR_Vector3			pos2P{ 5.0f,0.0f };

	
};

class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;
public:
	float	timer = 0.0f;
	int		sco[6] = { 0,0,0,0,0,0 };

	enum MAIN_LOOP
	{
		INTRO1P = 0,
		INTRO2P,
		READY,
		MAIN,
		FINISH,
		WIN1P,
		WIN2P,
		GAME_FIN,
	};

	enum JUDGE_VICTORY
	{
		NO_VICTORY = 0,
		VICTORY1P,
		VICTORY2P,
		DROW,
	};

	MAIN_LOOP		main_loop;	//���̕ϐ��ŃQ�[�����C���̑J�ڂ��Ǘ�����

	std::unique_ptr<geometric_primitive>	geo = nullptr;
	std::unique_ptr<Skinned_mesh>	skin = nullptr;

	//�摜�֌W
	std::unique_ptr<Sprite> test = nullptr;
	std::unique_ptr<Sprite> HP_img = nullptr;
	std::unique_ptr<Sprite> win1P_img = nullptr;
	std::unique_ptr<Sprite> win2P_img = nullptr;
	std::unique_ptr<Sprite> draw_img = nullptr;
	std::unique_ptr<Sprite> HPbar_img = nullptr;
	std::unique_ptr<Sprite> KO_img = nullptr;
	std::unique_ptr<Sprite> gauge_img = nullptr;
	std::unique_ptr<Sprite> gaugecase_img = nullptr;
	std::unique_ptr<Sprite> font_img = nullptr;
	std::unique_ptr<Sprite> call_img = nullptr;
	std::unique_ptr<Sprite> effect_img = nullptr;
	std::array<int, 3>		p1combo;
	std::array<int, 3>		p2combo;

	//�摜
	std::unique_ptr<Skinned_mesh> box = nullptr;
	std::shared_ptr<Texture> box_texture = nullptr;
	MeshMotion motion;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;

	//�v���C���[�Ǘ��n
	PlayerALL						PL;
	std::unique_ptr<Player>			player1p	= nullptr;
	std::unique_ptr<Player>			player2p	= nullptr;
	std::unique_ptr<GamepadBase>	pad1		= nullptr;
	std::unique_ptr<GamepadBase>	pad2		= nullptr;

	//�Q�[�����[�v����ϐ�
	bool			pause = false;			//�|�[�Y��
	bool			start = false;			//�ΐ�J�n
	JUDGE_VICTORY	judge = NO_VICTORY;		//���s�̗L��
	float			start_timer = 0.0f;		//�J�n�܂ł̎��ԁBAre You Ready?
	bool			end = false;			//���s������
	bool			fin = false;			//�S�ďI�����
	float			endtimer = 0.0f;		//���s�����Ă��珟����ʂɈڍs����܂łɎg�p
	float			mix_fedo = 0.0f;		//�t�F�[�h�C���̑��x�ύX�p

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	bool				FedoOut(float elapsed_time);

	void				PadSet(int select1);
	void				PadSet(int select1, int select2);
	void				Winjudge();
	DirectX::XMFLOAT4	ColorSet(int power);
	void				ComboImageSet();

	DirectX::XMFLOAT2	Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos);
	void				ScoreImageSet();

public:
	//�Q�[�������֐�
	void	SetPlayerCharacter(std::unique_ptr<Player>& player, int select);
	void	PauseUpdate();
	void	TrackSet();
	void	FinUpdate();
	void	StartSet();
	void	FinSet();
};



class SceneTitle : public SceneBase
{
	
public:
	std::unique_ptr<Sprite> test = nullptr;
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;

	GamePad1			g1;
	GamePad2			g2;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> spriteShader = nullptr;

	//�摜�`��֌W
	YR_Vector3			p1;
	YR_Vector3			p2;
	YR_Vector3			none_pos;
	YR_Vector3			p1_pos;
	YR_Vector3			p2_pos;
	float				Rato = 0.0f;
	bool				p1Enter = false;
	bool				p2Enter = false;
	float				timer = 0.0f;
	bool				end = false;

	int					select_p1 = 0;
	int					select_p2 = 0;

public:
	std::unique_ptr<Sprite>	knight_icon = nullptr;
	std::unique_ptr<Sprite>	ken_icon = nullptr;
	std::unique_ptr<Sprite>	select_img = nullptr;

	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void LoadData();
	void UnInit();

	bool FedoOut(float elapsed_time);
	YR_Vector3			PosSet(int select);
};

class SceneClear : public SceneBase
{
public:
	int sco[6] = { 0,0,0,0,0,0 };
	float timer = 0.0f;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void ScoreImageSet();
};

class SceneOver : public SceneBase
{
public:
	float timer = 0.0f;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
};

class SceneLoad : public SceneBase
{
public:
	//���[�h�֌W
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;
	bool				Game_load_fin = false;

	//���[�h���̉摜
	std::unique_ptr<Sprite>	load_img = nullptr;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> spriteShader = nullptr;

	float timer = 0.0f;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit();
	void LoadData();
	bool FedoOut(float elapsed_time);

};

class SceneSelect : public SceneBase
{
public:
	//���[�h�֌W
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> spriteShader = nullptr;

	//�摜�`��֌W
	YR_Vector3			p1;
	YR_Vector3			p2;
	YR_Vector3			knight_pos;
	YR_Vector3			kenpos;
	float				Rato = 0.0f;
	bool				p1Enter = false;
	bool				p2Enter = false;
	float				timer = 0.0f;
	bool				end = false;
	
	int					select_p1 = 0;
	int					select_p2 = 0;

public:
	std::unique_ptr<Sprite>	back_img = nullptr;
	std::unique_ptr<Sprite>	knight_icon = nullptr;
	std::unique_ptr<Sprite>	ken_icon = nullptr;
	std::unique_ptr<Sprite>	select_img = nullptr;
	//std::unique_ptr<Sprite>	back = nullptr;

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	bool				FedoOut(float elapsed_time);

	YR_Vector3			PosSet(int select);
};

class SceneTest : public SceneBase
{
public:
	float	timer = 0.0f;

	std::unique_ptr<Sprite> test = nullptr;
	std::unique_ptr<Skinned_mesh> box = nullptr;
	std::shared_ptr<Texture> box_texture = nullptr;
	std::shared_ptr<Texture> board_texture = nullptr;
	MeshMotion motion;

	std::unique_ptr<geometric_primitive>	geo = nullptr;
	std::unique_ptr<board_primitive>		board = nullptr;
	std::unique_ptr<AnimBoard>				anim = nullptr;

	//�T���v���[
	std::shared_ptr<Sampler> sampler = nullptr;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;
	std::unique_ptr<YRShader> boardShader = nullptr;
	std::unique_ptr<YRShader> animShader = nullptr;

	//�e�N�X�`��
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> normal_texture = nullptr;
	std::unique_ptr<Texture> position_texture = nullptr;
	

public:
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
};