#pragma once

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../Key.h"
#include <directxmath.h>
//#include <memory>
#include< Windows.h>
#include "../YR_VectorMaster.h"
#include "../Game/YRGamePad.h"
#include <thread>
#include "../Chara/PlayerBase/PlayerBase.h"
#include "../sprite.h"
#include "../YRShader.h"
#include "../Model/YRModelAnim.h"
#include "../Model/Geometric_primitive.h"
#include "../Model/Board_primitive.h"
#include "../Sampler.h"
#include "../Model/AnimBoard.h"
#include "../Game/collision.h"
#include "../Model/Skinned_mesh.h"


//�قƂ�ǂ̃V�[���Ŏg�p���邽�߃w�b�_�[�ɒ��ڏ����Ă���
constexpr float start_time = 2.0f;			//�Q�[�����J�n����܂ł̎���
constexpr float ready_time = 1.0f;			//Ready�̕\�����o������
constexpr float end_slow_time = 3.0f;		//�����I����X���[�ɂ��鎞��
constexpr float slow_adjust = 0.2f;			//�X���[�␳�l
constexpr float game_end_time = 5.0f;		//�Q�[�����I�����ď�����ʂɈڍs���鎞��
constexpr float HP_Danger_point = 0.3f;		//DANGER��ԂɂȂ�HP�̊�����

//�^�C�g����ʁE�Q�[���V�[���őI���Ɏg�p�ł���{�^���̗���
constexpr std::array<PAD, 10> any_button =
{
	PAD::A,
	PAD::B,
	PAD::X,
	PAD::Y,
	PAD::RB,
	PAD::LB,
	PAD::R_TRIGGER,
	PAD::L_TRIGGER,
	PAD::START,
	PAD::SELECT,
};

//�R���g���[���[�̃v���C���[�ێ����L�^����\���̗�
enum class INPUT_PLAYER : int
{
	NONE = 0,	//�܂����������Ă��Ȃ����
	P1, 		//�v���C���[1
	P2			//�v���C���[2
};


//�S�ẴV�[���̃x�[�X
//���V�����V�[�����쐬����ꍇ�͂��̃N���X���p�����邱��
class SceneBase
{
public:

	//fedo_alpha�̒l��0.0f~1.0f�ɓ������ăt�F�[�h������
	//fedo_start�̓t�F�[�h�A�E�g���邩�ǂ����̃t���O�Bfalse�̏ꍇ��fedo_alha������������
	float	fado_alpha;
	bool	fado_start;

	virtual void Init() {};						//�������B�ǂݍ��ނ̂̓V�F�[�_�[�݂̂ŉ摜�Ȃǂ̓ǂݍ��݂͕ʃX���b�h�𗧂��グ�čs��
	virtual void Update(float elapsed_time) {};	//�X�V����
	virtual void Draw(float elapsed_time) {};	//�`��
	virtual void UnInit() {};					//�������
};

//�v���C���[����\����
//{
//	HP_MAX�`�F�v���C���[��HP�̍ő�l
//	ratio�`�FHP�`�掞�Ɏg�p
//	power�`�F�Q�[�W�̐F
//	correction_value�F�摜�̈ʒu�����Ɏg���Ă�
//	gauge�`�F�`�掞�Ɏg�p�B������̓Q�[�W
//	pos�`�F�v���C���[�̏����ʒu
//}
struct PlayerALL
{
	float				HP_MAX1P = 0.0f;
	float				HP_MAX2P = 0.0f;
	float				ratio1P	= 0.0f;
	float				ratio2P = 0.0f;
	float				Damage_ratio1P = 0.0f;
	float				Damage_ratio2P = 0.0f;
	DirectX::XMFLOAT4	power1P = { 0.0f,0.0f,0.0f,0.0f };
	DirectX::XMFLOAT4	power2P = { 0.0f,0.0f,0.0f,0.0f };
	float				correction_value = 0.0f;
	float				gauge1P = 0.0f;
	float				gauge2P = 0.0f;
	YR_Vector3			pos1P{ -8.0f,0.0f };
	YR_Vector3			pos2P{ 8.0f,0.0f };

};

//--------------------------------------------------
//			�X�e�[�W�N���X
//--------------------------------------------------
//	�X�e�[�W�p�̃��f���A�V�F�[�_�[�����N���X
//	�X�e�[�W�̕`��Ȃǂ����̃N���X�ōs��
//--------------------------------------------------
class Stage
{
public:

	//�X�e�[�W�͂��̍\���̗񋓂Ŕ��ʂ���
	enum class StageType : int
	{
		NORMAL = 0,		//�ʏ�X�e�[�W�B����
	};

public:
	//FBX�f�[�^
	std::unique_ptr<Skinned_mesh> sky_data = nullptr;
	std::shared_ptr<Model> stage_data = nullptr;
	std::unique_ptr<ModelAnim> stage_draw = nullptr;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> skyShader = nullptr;
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> ToonShader = nullptr;

	//��A�X�e�[�W�̈ʒu�����Ɏg�p
	YR_Vector3	Sky_Pos = YR_Vector3(0.0f, 0.0f, 0.0f);
	YR_Vector3	Sky_Scale = YR_Vector3(1.0f, 1.0f, 1.0f);
	YR_Vector3	Sky_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	YR_Vector3	Stage_Pos = YR_Vector3(0.0f, 0.0f, 0.0f);
	YR_Vector3	Stage_Scale = YR_Vector3(1.0f, 1.0f, 1.0f);
	YR_Vector3	Stage_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	void Init(StageType type);
	void Uninit();
	void Draw(const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);
};


//AI�R���g���[���N���X
class AI_Controller
{
public:
	const float max_dis = 16.0f;		//�߂Â����ǂ����̋����ڈ�
	const int attack_probability = 4;	//�U���̊m��
	const int track_probability = 3;	//�z�[�~���O�_�b�V���̊m��
	const int guard_probability = 7;	//�K�[�h�̊m��
	const float timer_max = 2.0f;		//�^�C�}�[�̍ő�l
	const float guard_timer_max = 1.0f;	//�K�[�h�^�C�}�[�̍ő�l

	float timer = 0.0f;

	//AI�̍s�����X�g
	enum class AI_State : int
	{
		INIT = 0,		//�ŏ��̐ݒ�
		RAND_SELECT,	//�����_���ōs�������肷��
		APPROACH,		//�߂Â�
		LEAVE,			//�����
		COMBO,			//X�{�^����A�ł���
		TRACK_DASH,		//�z�[�~���O�_�b�V�����s��
		GUARD,			//�K�[�h���悤�Ƃ���
		KNOCK,			//�̂�������
		DEAPTH,			//�̗͂��Ȃ��Ȃ���
		DOWN,			//�_�E����
		SPECIAL,		//���K��łƂ��Ƃ���
		FOREVER_GUARD,	//�ꐶ�K�[�h����
		END,
	};

	AI_State state = AI_State::INIT;			//���݂̃X�e�[�g
	AI_State before_state = AI_State::INIT;		//�O��̃X�e�[�g
	bool steal_escape = false;					//�͂ݔ��������ꂽ���ǂ����̔���t���O
public:
	//AI�N���X�͊O������S�Ă𓮂������߁A�����őS�Đ錾���Ă���
	void Init()
	{
		state = AI_State::INIT;
		timer = 0.0f;
	}
};


//�R���g���[���[���͓��e�ۑ��E�\���N���X
class InputGamePadDraw
{
public:

	struct Input_list_pad
	{
		std::vector<int> button_input;	//�{�^���̎��
		int stick_input = -1;			//�X�e�B�b�N�̎��
		bool pushed = false;			//������Ă������͂�������ꍇtrue

		Input_list_pad() {
			stick_input = -1;
		};
		~Input_list_pad() {};
	};

	std::list<Input_list_pad>		p1_input_list;
	std::list<Input_list_pad>		p2_input_list;

	//�摜�X�v���C�g
	std::unique_ptr<Sprite> button_img = nullptr;
	std::unique_ptr<Sprite> stick_img = nullptr;

	//�{�^���̃��X�g
	//�E���͂��ꂽ�{�^�����摜�̕��я��ɓo�^���Ă���
	const std::array<int, 8>	button_img_list =
	{
		static_cast<int>(PAD::X),
		static_cast<int>(PAD::Y),
		static_cast<int>(PAD::A),
		static_cast<int>(PAD::B),
		static_cast<int>(PAD::RB),
		static_cast<int>(PAD::LB),
		static_cast<int>(PAD::R_TRIGGER),
		static_cast<int>(PAD::L_TRIGGER)
	};

	//�X�e�B�b�N�̓��̓��X�g
	//�E���͂��ꂽ�X�e�B�b�N�̌������摜�̕��я��ɓo�^���Ă���
	//�Ō��236�A214�̓R�}���h�̖���(�X�e�B�b�N�ɂ͂Ȃ��גǉ����Ă���)
	const std::array<int, 10>	stick_img_list =
	{
		static_cast<int>(PAD::STICK_U),
		static_cast<int>(PAD::STICK_RUp),
		static_cast<int>(PAD::STICK_R),
		static_cast<int>(PAD::STICK_RDown),
		static_cast<int>(PAD::STICK_D),
		static_cast<int>(PAD::STICK_LDown),
		static_cast<int>(PAD::STICK_L),
		static_cast<int>(PAD::STICK_LUp),
		236,
		214
	};

	void Init();
	void Load();
	void Update(Player* player1p, Player* player2p);	//�v���C���[�̏�񂪕K�v�ɂȂ邽�߈����Ń|�C���^���󂯎���Ă���
	void Draw(YRShader* shader);

	int GetButtonNum(int button);						//�{�^���̗������L�^���Ă���
	int GetStickNum(int stick);							//�X�e�B�b�N�̗������L�^���Ă���

	void UnInit();
};


//�Q�[���V�[��
class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;						//�}�E�X�̍��W��ۑ�����ϐ�
	bool camera_move_debug = false;			//�J�������f�o�b�O�@�\�Ƃ��ē������ꍇ��true
	const float Icon_distance = 20.0f;		//�A�C�R����\�����鍂��

	//�[�x�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state2;
public:

	YR_Vector2	sprite_debug_pos = { 0.0f,0.0f };
	float	sprite_debug_scale = 0.0f;

	//2P�̍s�������
	enum class Player2PControl : int
	{
		OPERATION,	//����
		SUSPENSION,	//�����Ȃ�
		AI,			//AI
		INVINCIVLE,	//��ɖ��G�œ����Ȃ�
		END,		//�I���R�}���h
	}; Player2PControl pl2_con = Player2PControl::OPERATION;


	InputGamePadDraw	input_pad;		//���͕\���N���X

	float	timer = 0.0f;				//�Q�[�����^�C�}�[

	float p1_elapsed_time = 0.0f;
	float p2_elapsed_time = 0.0f;

	float hit_stop_elapsed;				//�q�b�g�X�g�b�v���Ɏg�p

	float game_speed;					//�Q�[���S�̂̃X�s�[�h��ύX����ϐ�

	//�J�����X�e�[�g�����C���̎��̃J�������W
	YR_Vector3	Scene_eye;
	YR_Vector3	Scene_focus;
	YR_Vector3	Scene_up;

	//�������ɃJ�����̍��W��ۑ�����ϐ�
	YR_Vector3	Scene_End_eye;

	float		Scene_fov = 0.0f;		//��p
	float		Scene_aspect = 0.0f;	//�A�X�y�N�g��
	float		Scene_nearZ = 0.0f;		//�j�A�N���b�v�ʂ܂ł̋���
	float		Scene_farZ = 0.0f;		//�t�@�[�N���b�v�ʂ܂ł̋���

	float image_alpha = 0.0f;			//�摜�\�����Ɏg�p����A���t�@�l
	float image_size = 5.0f;			//�摜�\�����Ɏg�p����g�嗦

	YR_Vector3	Start_Scene_eye;		//�Q�[���J�n���̏����J�������W
	YR_Vector3	Start_Scene_focus;		//�Q�[���J�n���̏����J��������

	Title_CBuffer cbuffer_param;

	YR_Vector3	old_pos_p1 = { 0.0f,0.0f,0.0f };	//1�t���[���O�̃v���C���[1�̍��W
	YR_Vector3	old_pos_p2 = { 0.0f,0.0f,0.0f };	//1�t���[���O�̃v���C���[2�̍��W

	enum class MAIN_LOOP :int
	{
		INTRO1P = 0,
		INTRO2P,
		READY,
		MAIN,
		FINISH,
		WIN1P,
		WIN2P,
		DRAW,
		GAME_FIN,
	};

	enum class JUDGE_VICTORY : int
	{
		NO_VICTORY = 0,
		VICTORY1P,
		VICTORY2P,
		DRAW,
	};

	//�������J������]�p�X�e�[�g
	enum class FIN_CAMERA_STATE : int
	{
		ROLL = 0,
		STOP,
	};
	FIN_CAMERA_STATE fin_camera_state = FIN_CAMERA_STATE::ROLL;

	MAIN_LOOP		main_loop;	//���̕ϐ��ŃQ�[�����C���̑J�ڂ��Ǘ�����

	std::unique_ptr<geometric_primitive>	geo = nullptr;		//�|���f�[�^�B�����蔻��̕`��Ɏg��
	//std::unique_ptr<Skinned_mesh>	skin = nullptr;

	Stage					stage;				//�X�e�[�W�̎��́B���f���̃f�[�^�A�V�F�[�_�[������

	//�摜�֌W
	std::unique_ptr<Sprite> test = nullptr;
	std::unique_ptr<Sprite> HP_img = nullptr;
	std::unique_ptr<Sprite> win1P_img = nullptr;
	std::unique_ptr<Sprite> win2P_img = nullptr;
	std::unique_ptr<Sprite> draw_img = nullptr;
	std::unique_ptr<Sprite> HPbar_img = nullptr;
	std::unique_ptr<Sprite> HPDamagebar_img = nullptr;
	std::unique_ptr<Sprite> KO_img = nullptr;
	std::unique_ptr<Sprite> font_img = nullptr;
	std::unique_ptr<Sprite> combo_img = nullptr;
	std::unique_ptr<Sprite> desastal_case = nullptr;
	std::unique_ptr<Sprite> desastal_flash = nullptr;
	std::unique_ptr<Sprite> desastal_img = nullptr;
	std::unique_ptr<Sprite> call_img = nullptr;
	std::unique_ptr<Sprite> effect_img = nullptr;
	std::unique_ptr<Sprite> pause_img = nullptr;
	std::unique_ptr<Sprite> Danger_img = nullptr;
	std::array<int, 3>		p1combo;
	std::array<int, 3>		p2combo;
	std::shared_ptr<Texture> p1_icon_img = nullptr;
	std::shared_ptr<Texture> p2_icon_img = nullptr;
	std::shared_ptr<Texture> arrow_icon_img = nullptr;

	//�|��
	std::unique_ptr<AnimBoard>	p1_icon_board = nullptr;
	std::unique_ptr<AnimBoard>	p2_icon_board = nullptr;
	std::unique_ptr<AnimBoard>	arrow_icon_board = nullptr;

	std::unique_ptr<Sprite> gauge_img = nullptr;
	std::unique_ptr<Sprite> gauge_case_img = nullptr;
	std::unique_ptr<Sprite> gauge_case_mask = nullptr;
	std::unique_ptr<Sprite> gauge_anim1p = nullptr;
	std::unique_ptr<Sprite> gauge_anim2p = nullptr;

	std::unique_ptr<Sprite> HPbar_base = nullptr;
	std::unique_ptr<Sprite> HPbar_case = nullptr;
	std::unique_ptr<Sprite> HPbar_mask = nullptr;
	std::unique_ptr<Sprite> HPbar_fedo = nullptr;
	std::unique_ptr<Sprite> HPbar_design = nullptr;


	//��ʕ`��p�e�N�X�`��
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> UI_texture = nullptr;
	std::unique_ptr<Texture> HP_texture = nullptr;
	//std::unique_ptr<Texture> normal_texture = nullptr;
	//std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;
	std::unique_ptr<Texture> multi_blur_texture = nullptr;
	std::array<std::unique_ptr<Texture>, 10> blur_texture = { nullptr };

	//Gbuffer�p�X�v���C�g
	std::unique_ptr<Sprite>	sprite = nullptr;

	//�T���v���[
	std::shared_ptr<Sampler> sampler_clamp = nullptr;
	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> animShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;
	std::unique_ptr<YRShader> ParallelToonShader = nullptr;
	std::unique_ptr<YRShader> ToonShader = nullptr;
	std::unique_ptr<YRShader> gaussShader = nullptr;
	std::unique_ptr<YRShader> multi_gaussShader = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;
	std::unique_ptr<YRShader> concentrationShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	//�v���C���[�Ǘ��n
	PlayerALL						PL;
	std::unique_ptr<Player>			player1p = nullptr;
	std::unique_ptr<Player>			player2p = nullptr;
	std::unique_ptr<GamepadBase>	pad1 = nullptr;
	std::unique_ptr<GamepadBase>	pad2 = nullptr;

	float							pl1_before_hp = 0.0f;
	float							pl2_before_hp = 0.0f;

	int							pl1_before_power = 0;
	int							pl2_before_power = 0;

	YR_Vector2		Danger_pos_p1 = { 0.0f,0.0f };		//1P��DANGER�\�L���W
	YR_Vector2		Danger_pos_p2 = { 0.0f,0.0f };		//2P��DANGER�\�L���W
	float			Danger_size = 0.0f;					//DANGER�\�L�̑傫��
	float			Danger_alpha = 0.0f;				//DANGER�\�L�̃A���t�@�l
	float			desastal_size_p1 = 0.0f;			//1P�̃f�B�U�X�^���������̃T�C�Y
	float			desastal_size_p2 = 0.0f;			//2P�̃f�B�U�X�^���������̃T�C�Y
	float			desastal_alpha_p1 = 0.0f;			//1P�̃f�B�U�X�^���������̃A���t�@�l
	float			desastal_alpha_p2 = 0.0f;			//2P�̃f�B�U�X�^���������̃A���t�@�l

	//�Q�[�����[�v����ϐ�
	bool			pause = false;						//�|�[�Y��
	bool			start = false;						//�ΐ�J�n
	JUDGE_VICTORY	judge = JUDGE_VICTORY::NO_VICTORY;	//���s�̗L��
	float			start_timer = 0.0f;					//�J�n�܂ł̎��ԁBAre You Ready?
	bool			end = false;						//���s������
	bool			fin = false;						//�S�ďI�����
	float			endtimer = 0.0f;					//���s�����Ă��珟����ʂɈڍs����܂łɎg�p
	float			roll_timer = -1.0f;					//������ɃJ�������񂷍ۂɎg�p����
	float			mix_fade = 0.0f;					//�t�F�[�h�C���̑��x�ύX�p
	bool			blur_on = false;					//�u���[����������t���O


	AI_Controller	AI2P;								//2P��AI���

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	bool				FadeOut(float elapsed_time);

	void				PadSet(int select1);
	void				PadSet(int select1, int select2);
	void				Winjudge();
	DirectX::XMFLOAT4	ColorSet(int power);
	DirectX::XMFLOAT4	HPColorSet(float hp, float max_hp);
	void				ComboImageSet();

	DirectX::XMFLOAT2	Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos);

	void				Control2PState(float elapsed_time);
	void				HPBar_Draw(	const DirectX::XMMATRIX& view,
									const DirectX::XMMATRIX& projection,
									const DirectX::XMFLOAT4& light_direction,
									const DirectX::XMFLOAT4& light_color,
									const DirectX::XMFLOAT4& ambient_color,
									float elapsed_time);
	void				UI_Draw(float elapsed_time);
	void				SetUITexture();
	void				SetHPTexture();

	void				PlayerMoveReq(int req_player_num, YR_Vector3 pos);
	void				PlayerMoveReq(int req_player_num, YR_Vector3 vec, float speed);

	void				IconDraw(
							const DirectX::XMMATRIX& view,
							const DirectX::XMMATRIX& projection,
							float					elapsed_time);

public:
	//�Q�[�������֐�
	void	SetPlayerCharacter(std::unique_ptr<Player>* player, int select);	//select�ɂ���ăv���C���[�𐶐�����
	void	PauseUpdate();		//�|�[�Y���s������
	void	TrackSet();			//�z�[�~���O�_�b�V���p�̒l��ύX����
	void	FinUpdate();		//�Q�[���I����̏���
	void	StartSet();			//�C���g���I����̃Q�[����ʂ̃Z�b�g(�J����)
	void	FinSet();			//�Q�[���I����̃Q�[����ʂ̃Z�b�g(�J����)
	void	CameraUpdate(float elapsed_time);		//�J�����̃X�e�[�g��MAIN�ɂ���ꍇ�̃J�����������s��
	void	CameraRequest(float elapsed_time);	//�J�����̃��N�G�X�g���m�F���A���N�G�X�g������Ώ������s��

	void SetRenderTexture();

	void NullSetRenderTexture();


	void RenderTexture();

	void RenderBlur();

	void RenderUI();

	void RenderHP();

	void AIControll(float elapsed_time);

	void EndORGameCameraSet(float elapsed_time);

	void DangerDraw(float elapsed_time);

	void DangerSound();

	void DesastalFlash(float elapsed_time);

	void ComboDraw();

	/*struct CB_Multi_Render_Target
	{
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
	};*/
};


//�^�C�g���V�[��
class SceneTitle : public SceneBase
{

public:
	std::unique_ptr<Sprite> test = nullptr;
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;
	std::vector<float> testevent;

	GamePad1			g1;
	GamePad2			g2;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> titleShader = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;

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
	std::unique_ptr<Sprite>	title_img = nullptr;
	std::unique_ptr<Sprite> choice_img = nullptr;
	std::unique_ptr<Sprite>	choice_mask = nullptr;
	std::unique_ptr<Sprite>	choice_anim = nullptr;


	//��ʕ`��p�e�N�X�`��
	std::unique_ptr<Texture> color_texture = nullptr;
	//std::unique_ptr<Texture> normal_texture = nullptr;
	//std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;

	std::unique_ptr<Texture> title_texture = nullptr;

	//Gbuffer�p�X�v���C�g
	std::unique_ptr<Sprite>	sprite = nullptr;

	//�T���v���[
	std::shared_ptr<Sampler> sampler_clamp = nullptr;
	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	Title_CBuffer cbuffer_param;

	void LoadData();

	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit();

	bool FedoOut(float elapsed_time);
	YR_Vector3			PosSet(int select);


	void SetRenderTexture();

	void NullSetRenderTexture();

	void RenderTexture(float elapsed_time);


	enum class VS_MODE : int
	{
		PLAYER = 0,
		CPU,
	};

	VS_MODE vs_mode = VS_MODE::CPU;

	enum class STATE : int
	{
		HOME,
		SELECT,
		END,
	};
	STATE state = STATE::HOME;
};


//���[�h�V�[��
//�}�b�`��ʂ̃C���g�����Đ�����V�[��
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
	//std::unique_ptr<Sprite>	load_bg = nullptr;

	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> spriteShader = nullptr;

	float match_timer = 0.0f;

	float plus_match = 0.0f;

	float slow_add = 0.0f;

	float timer_Multiply = 0.0f;

	float sin_max = 0.0f;

	float space_time = 0.0f;

	float VS_size = 0.0f;

	float VS_alpha = 0.0f;

	float flash_size = 0.0f;
	float flash_alpha = 0.0f;

	float p1_pos_x = 0.0f;
	float p2_pos_x = 0.0f;

	float line_1p_x = 0.0f;
	float line_2p_x = 0.0f;

	//���C���ɏ�Z����l
	float line_Multiply = 0.0f;

	std::unique_ptr<Sprite> flash = nullptr;

	enum class IntroState : int
	{
		P1,			//�v���C���[1
		SPACE1,		//�����Ԃ�����
		P2,			//�v���C���[2
		SPACE2,		//�����Ԃ�u��
		P1P2,		//�����ɏo��
		FIN,		//sin�l��max�ɒB�����炱���Ɉړ�����(�����Ȃ�����)
	};

	IntroState intro_state = IntroState::P1;

	std::unique_ptr<Sprite> p1_cut = nullptr;
	std::unique_ptr<Sprite> p2_cut = nullptr;
	std::unique_ptr<Sprite> p1_name = nullptr;
	std::unique_ptr<Sprite> p2_name = nullptr;
	std::unique_ptr<Sprite> p1_name_edge = nullptr;
	std::unique_ptr<Sprite> p2_name_edge = nullptr;
	std::unique_ptr<Sprite> Box = nullptr;
	std::unique_ptr<Sprite> VS_Image = nullptr;

	std::unique_ptr<YRShader> titleShader = nullptr;
	std::unique_ptr<Texture> title_texture = nullptr;

	//Gbuffer�p�X�v���C�g
	std::unique_ptr<Sprite>	sprite = nullptr;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	Title_CBuffer cbuffer_param;

	float				timer = 0.0f;
	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	bool				FedoOut(float elapsed_time);
	void				MatchStart();
	void				MatchUpdate(float elapsed_time);
	void				MatchDraw(float elapsed_time);
	void				IconLoad();
};


//�Z���N�g�V�[��
class SceneSelect : public SceneBase
{
public:
	//���[�h�֌W
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;

	//�V�F�[�_�[
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> selectShader = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;

	//��ʕ`��p�e�N�X�`��
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;

	std::unique_ptr<Texture> select_texture = nullptr;

	//�T���v���[
	std::shared_ptr<Sampler> sampler_clamp = nullptr;
	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	//Gbuffer�p�X�v���C�g
	std::unique_ptr<Sprite>	sprite = nullptr;

	//�摜�`��֌W
	YR_Vector3			p1;					//�v���C���[1�̃J�[�\���̈ʒu
	YR_Vector3			p2;					//�v���C���[2�̃J�[�\���̈ʒu
	YR_Vector3			knight_icon_pos;	//�i�C�g�̃A�C�R���̈ʒu
	YR_Vector3			p1_cut_pos;			//�v���C���[�P�̃L�����摜�̈ʒu
	YR_Vector3			p2_cut_pos;			//�v���C���[�P�̃L�����摜�̈ʒu
	YR_Vector3			name_distance;		//���O�̈ʒu(�J�b�g�C������ǂꂾ��������)
	float				sx = 0.0f;			//�J�b�g�C���摜�̕`��w��ʒu
	float				sw = 0.0f;			//�J�b�g�C���摜�̕`��w��傫��
	float				icon_range = 0.0f;	//�A�C�R�����m�̋���
	float				Rato = 0.0f;		//�A�C�R���̑傫��
	float				case_rato = 0.0f;	//�A�C�R���P�[�X�̑傫��
	float				cursor_rato = 0.0f;	//�I�������L�����摜�̑傫��
	float				cut_rato = 0.0f;	//�J�[�\���̑傫��
	float				name_rato = 0.0f;	//���O�̑傫��
	bool				p1Enter = false;	//�v���C���[1�����肵����true
	bool				p2Enter = false;	//�v���C���[1�����肵����true
	float				timer = 0.0f;
	bool				end = false;
	float				cursor_speed = 1.0f;//�J�[�\���̑��x
	float				image_alpha = 0.0f;	//�摜�̃A���t�@�l
	float				p1_chara_alpha = 0.0f;//�L�����摜�������Ɏg�p
	float				p2_chara_alpha = 0.0f;//�L�����摜�������Ɏg�p

	YR_Vector2			ready_pos = { 0.0f,0.0f };	//Ready to Start�摜�̍��W
	float				ready_rato = 1.0f;			//Ready to Start�摜�̑傫��

	//�L�����I��
	int					select_p1 = -1;		//�v���C���[1�̃L�����ԍ�
	int					select_p2 = -1;		//�v���C���[2�̃L�����ԍ�
	int					old_select_p1 = -1;	//�v���C���[1�̑O�t���[���̃L�����ԍ�
	int					old_select_p2 = -1;	//�v���C���[2�̑O�t���[���̃L�����ԍ�
	PLCOLOR				color_p1 = PLCOLOR::ORIGINAL;	//�v���C���[1�̃J���[�ԍ�
	PLCOLOR				color_p2 = PLCOLOR::ORIGINAL;	//�v���C���[2�̃J���[�ԍ�
	PLCOLOR				old_color_p1 = PLCOLOR::ORIGINAL;	//�v���C���[1�̑O�t���[���̃J���[�ԍ�
	PLCOLOR				old_color_p2 = PLCOLOR::ORIGINAL;	//�v���C���[2�̑O�t���[���̃J���[�ԍ�

	//���C�����W(�K����ʍ�����o�Ă���ׁA�E�[�̍��W�̂�)
	YR_Vector2			line_red = { 0.0f, 0.0f };
	YR_Vector2			line_blue = { 0.0f, 0.0f };

	//���C���ɏ�Z����l
	float line_Multiply = 0.0f;

	//Ready�ɏ�Z����l
	float ready_Multiply = 0.0f;

	//���C���̑���(�c)
	float line_breadth = 1.0f;

public:

	//std::unique_ptr<Sprite>	select_img = nullptr;
	//std::unique_ptr<Sprite>	back = nullptr;

	//�A�C�R���摜
	//std::array<std::unique_ptr<Sprite>, scastI(PLSELECT::PLSELECT_END)> icon_image;
	std::unique_ptr<Sprite> chara_case = nullptr;
	std::unique_ptr<Sprite> select_point = nullptr;
	std::unique_ptr<Sprite> white_box = nullptr;
	std::unique_ptr<Sprite> Box_sprite = nullptr;
	std::unique_ptr<Sprite> ready_to_start = nullptr;

	//�L�����I�𔻒�p��
	enum class Select_P : int
	{
		NONE,
		P1,
		P2,
		ALL,
		P_END
	};
	std::array<std::unique_ptr<Sprite>, scastI(Select_P::P_END)> select_img;

	//�L�����摜�`��p�\����
	struct Select_Chara_Case
	{
		YR_Vector3					pos;							//���W
		Select_P					select = Select_P::NONE;		//�I�����
		std::unique_ptr<Sprite>		name_image = nullptr;			//���O�摜
		std::unique_ptr<Sprite>		edge_image = nullptr;			//���O���摜
		std::array<std::unique_ptr<Sprite>,scastI(PLCOLOR::COLOR_END)>	icon_image;			//�A�C�R���摜
	};
	std::array<Select_Chara_Case, scastI(PLSELECT::PLSELECT_END)> select_p;

	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	Title_CBuffer cbuffer_param;

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	void				IconLoad();
	bool				FadoOut(float elapsed_time);
	void				SetRenderTexture();
	void				NullSetRenderTexture();
	void				RenderTexture();

	void				PosSet();
	void				SelectCheck();
	void				DrawSelect(int num);
	void				DrawChara();

	void				Color_Add(PLCOLOR& color);
	void				Color_Sub(PLCOLOR& color);

	void				ColorChange();
	void				ColorAdjustment();

	void				ReadyStep(float elapsed_time);
	void				DrawReady();
};


//�e�X�g�V�[��
class SceneTest : public SceneBase
{
public:
	float	timer = 0.0f;

	bool cut_in = false;
	bool cut_fin = false;
	float cut_timer = 0.0f;
	bool timer_start = false;

	bool pause = false;

	bool match_on = false;
	float match_timer = 0.0f;

	float plus_match = 0.0f;

	float slow_add = 0.0f;

	float timer_Multiply = 0.0f;

	float sin_max = 0.0f;

	float space_time = 0.0f;

	float VS_size = 0.0f;

	float VS_alpha = 0.0f;

	float flash_size = 0.0f;
	float flash_alpha = 0.0f;

	float knight_1p_pos_x = 0.0f;
	float knight_2p_pos_x = 0.0f;

	float line_1p_x = 0.0f;
	float line_2p_x = 0.0f;

	float line_Multiply = 0.0f;

	std::unique_ptr<Sprite> flash = nullptr;

	enum class IntroState : int
	{
		P1,			//�v���C���[1
		SPACE1,		//�����Ԃ�����
		P2,			//�v���C���[2
		SPACE2,		//�����Ԃ�u��
		P1P2,		//�����ɏo��
		FIN,		//sin�l��max�ɒB�����炱���Ɉړ�����(�����Ȃ�����)
	};

	IntroState intro_state = IntroState::P1;

	std::unique_ptr<Sprite> test = nullptr;

	std::unique_ptr<Sprite> cutFrame = nullptr;
	std::unique_ptr<Sprite> cutMask = nullptr;
	std::unique_ptr<Sprite> cutIn = nullptr;
	std::unique_ptr<Sprite> knight_1p_cut = nullptr;
	std::unique_ptr<Sprite> knight_2p_cut = nullptr;
	std::unique_ptr<Sprite> knight_name = nullptr;
	std::unique_ptr<Sprite> Box = nullptr;
	std::unique_ptr<Sprite> VS_Image = nullptr;


	DirectX::XMFLOAT3	eye_cut = { 34.156f,5.693,-14.232 };
	DirectX::XMFLOAT3	focus_cut = { 8.539f,5.693f,2.898f };
	float				fov_cut = 50.0f;
	float				near_cut = 1.4f;
	float				far_cut = 100000.0f;


	//�J���[�`�F���W�p�e�N�X�`��
	std::shared_ptr<Texture>	color_texture_main = nullptr;
	std::shared_ptr<Texture>	color_texture_face = nullptr;

	DirectX::XMFLOAT2	eye_offset = { 0.0f,0.0f };
	DirectX::XMFLOAT2	mouth_offset = { 0.0f,0.0f };

	std::shared_ptr<Model> knight = nullptr;
	std::shared_ptr<Model> earth = nullptr;
	std::shared_ptr<Model> wait_R = nullptr;
	std::shared_ptr<Model> special_R = nullptr;
	std::shared_ptr<Texture> board_texture = nullptr;
	std::unique_ptr<ModelAnim> motion = nullptr;
	std::unique_ptr<ModelAnim> earth_motion = nullptr;
	std::unique_ptr<Skinned_mesh> sky = nullptr;

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
	std::shared_ptr<Texture> knight_normal_map = nullptr;
	std::shared_ptr<Texture> knight_height_map = nullptr;

	std::shared_ptr<Texture> earth_normal_map = nullptr;
	std::shared_ptr<Texture> earth_height_map = nullptr;


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
	std::unique_ptr<YRShader> toonShader = nullptr;
	std::unique_ptr<YRShader> paralleltoonShader = nullptr;
	std::unique_ptr<YRShader> toGbuffer = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;
	std::unique_ptr<YRShader> flatShader = nullptr;
	std::unique_ptr<YRShader> gaussShader = nullptr;
	std::unique_ptr<YRShader> multi_gaussShader = nullptr;
	std::unique_ptr<YRShader> furShader = nullptr;
	std::unique_ptr<YRShader> skyShader = nullptr;
	std::unique_ptr<YRShader> TessellationShader = nullptr;

	//�e�N�X�`��
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> normal_texture = nullptr;
	std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;
	std::unique_ptr<Texture> multi_blur_texture = nullptr;
	std::array<std::unique_ptr<Texture>,10> blur_texture = { nullptr };
	std::unique_ptr<Texture> fur = nullptr;

	std::shared_ptr<Model> Neru = nullptr;
	std::unique_ptr<ModelAnim> Neru_base = nullptr;
	std::vector<std::shared_ptr<Model>> Neru_anims;

	Collision circle;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer_Gauss = nullptr;

	DirectX::XMFLOAT3 knight_angle = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 knight_scale = { 0.1f,0.1f,0.1f };
	float	knight_scale_all = 0.1f;
	DirectX::XMFLOAT3 knight_pos = { 0.0f,0.0f,20.0f };

public:
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit() {
		knight.reset();
	};

	void RenderTexture(
		const DirectX::XMMATRIX&	view,
		const DirectX::XMMATRIX&	projection,
		const DirectX::XMFLOAT4&	light_direction,
		const DirectX::XMFLOAT4&	light_color,
		const DirectX::XMFLOAT4&	ambient_color,
		float						elapsed_time);

	void RenderBlur(
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);

	void MatchStart();

	void MatchUpdate(float elapsed_time);


	void NeruLoad();

	void NeruImGui();

public:
	struct CB_Multi_Render_Target
	{
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
		float Density = 0.0f;		//�т̖��x
		float Distance = 0.0f;		//�т̒���
		float dummy1 = 0.0f;
		float dummy2 = 0.0f;
	};

	struct ConstantBufferForPerFrame
	{
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
		float wave_time = 0.0f; //�g�� UV �X�N���[���p����
		float divide = 0.0f; //�������i�O�Ɠ��j
		float dummy1 = 0.0f;
		float dummy2 = 0.0f;
	};
};