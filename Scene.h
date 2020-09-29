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

constexpr float start_time = 0.0f;

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
	YR_Vector3			pos1P{ -8.0f,0.0f };
	YR_Vector3			pos2P{ 8.0f,0.0f };

	
};



class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;
	bool camera_move_debug;
public:
	float	timer = 0.0f;
	int		sco[6] = { 0,0,0,0,0,0 };

	float p1_elapsed_time = 0.0f;
	float p2_elapsed_time = 0.0f;

	float hit_stop_elapsed;				//�q�b�g�X�g�b�v���Ɏg�p

	float game_speed;					//�Q�[���S�̂̃X�s�[�h��ύX����ϐ�

	//�J�����X�e�[�g�����C���̎��̃J�������W
	YR_Vector3	Scene_eye;
	YR_Vector3	Scene_focus;
	YR_Vector3	Scene_up;
	float		Scene_fov = 0.0f;		//��p
	float		Scene_aspect = 0.0f;	//�A�X�y�N�g��
	float		Scene_nearZ = 0.0f;		//�j�A�N���b�v�ʂ܂ł̋���
	float		Scene_farZ = 0.0f;		//�t�@�[�N���b�v�ʂ܂ł̋���

	YR_Vector3	Start_Scene_eye;		//�Q�[���J�n���̏����J�������W
	YR_Vector3	Start_Scene_focus;		//�Q�[���J�n���̏����J��������

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

	std::unique_ptr<geometric_primitive>	geo = nullptr;		//�|���f�[�^�B�����蔻��̕`��Ɏg��
	//std::unique_ptr<Skinned_mesh>	skin = nullptr;

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
	//std::unique_ptr<Skinned_mesh> box = nullptr;
	//std::shared_ptr<Texture> box_texture = nullptr;
	//MeshMotion motion;
	//std::shared_ptr<Texture> knight2P_texture = nullptr;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;
	std::unique_ptr<YRShader> ParallelToonShader = nullptr;
	std::unique_ptr<YRShader> ToonShader = nullptr;


	//�v���C���[�Ǘ��n
	PlayerALL						PL;
	std::unique_ptr<Player>			player1p	= nullptr;
	std::unique_ptr<Player>			player2p	= nullptr;
	std::unique_ptr<GamepadBase>	pad1		= nullptr;
	std::unique_ptr<GamepadBase>	pad2		= nullptr;

	//�Q�[�����[�v����ϐ�
	bool			pause = false;						//�|�[�Y��
	bool			start = false;						//�ΐ�J�n
	JUDGE_VICTORY	judge = JUDGE_VICTORY::NO_VICTORY;	//���s�̗L��
	float			start_timer = 0.0f;					//�J�n�܂ł̎��ԁBAre You Ready?
	bool			end = false;						//���s������
	bool			fin = false;						//�S�ďI�����
	float			endtimer = 0.0f;					//���s�����Ă��珟����ʂɈڍs����܂łɎg�p
	float			mix_fedo = 0.0f;					//�t�F�[�h�C���̑��x�ύX�p

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
	void	SetPlayerCharacter(std::unique_ptr<Player>& player, int select);	//select�ɂ���ăv���C���[�𐶐�����
	void	PauseUpdate();		//�|�[�Y���s������
	void	TrackSet();			//�z�[�~���O�_�b�V���p�̒l��ύX����
	void	FinUpdate();		//�Q�[���I����̏���
	void	StartSet();			//�C���g���I����̃Q�[����ʂ̃Z�b�g(�J����)
	void	FinSet();			//�Q�[���I����̃Q�[����ʂ̃Z�b�g(�J����)
	void	CameraUpdate();		//�J�����̃X�e�[�g��MAIN�ɂ���ꍇ�̃J�����������s��
	void	CameraRequest(float elapsed_time);	//�J�����̃��N�G�X�g���m�F���A���N�G�X�g������Ώ������s��
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


	std::unique_ptr<board_primitive>		bisuko = nullptr;
	std::unique_ptr<board_primitive>		cube = nullptr;
	std::unique_ptr<board_primitive>		plane = nullptr;

	std::shared_ptr<Texture> bisuko_normal_texture = nullptr;
	std::shared_ptr<Texture> bisuko_specular_texture = nullptr;
	std::shared_ptr<Texture> test_texture = nullptr;
	std::shared_ptr<Texture> test_normal_texture = nullptr;
	std::shared_ptr<Texture> scorpion_specular_texture = nullptr;
	std::shared_ptr<Texture> specular_texture = nullptr;


	//Gbuffer�p�X�v���C�g
	std::unique_ptr<Sprite>	sprite = nullptr;

	//�T���v���[
	std::shared_ptr<Sampler> sampler_wrap = nullptr;
	std::shared_ptr<Sampler> sampler_clamp = nullptr;


	//�V�F�[�_�[
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;
	std::unique_ptr<YRShader> boardShader = nullptr;
	std::unique_ptr<YRShader> animShader = nullptr;
<<<<<<< HEAD
	std::unique_ptr<YRShader> toonShader = nullptr;
=======
	std::unique_ptr<YRShader> toGbuffer = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;
>>>>>>> Shader

	//�e�N�X�`��
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> normal_texture = nullptr;
	std::unique_ptr<Texture> position_texture = nullptr;
	

	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

public:
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);

	void RenderTexture(
		const DirectX::XMMATRIX&	view,
		const DirectX::XMMATRIX&	projection,
		const DirectX::XMFLOAT4&	light_direction,
		const DirectX::XMFLOAT4&	light_color,
		const DirectX::XMFLOAT4&	ambient_color,
		float						elapsed_time);
public:
	struct CB_Multi_Render_Target
	{
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
	};
};