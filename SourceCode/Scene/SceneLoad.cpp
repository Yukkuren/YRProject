#include "Scene.h"
#include "../Game/HitCheak.h"
#include "../framework.h"
#include "../YRSound.h"
#include <math.h>
#include "../Chara/PlayerBase/Player_name_list.h"
#include <codecvt>

// w_string�^�ύX�p�֐�
using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter_Load;

std::string to_string_Load(std::wstring wstr)
{
	return strconverter_Load.to_bytes(wstr);
}

std::wstring to_wstring_Load(std::string str)
{
	return strconverter_Load.from_bytes(str);
}

#undef max

#ifdef EXIST_IMGUI

static std::array<std::string, scastI(SceneTest::IntroState::FIN) + 1> intro_list =
{
	u8"�v���C���[1�̂�",
	u8"�v���C���[2�܂ł̊�",
	u8"�v���C���[2�̂�",
	u8"�����܂ł̊�",
	u8"����",
	u8"�Ō�",
};

#endif // EXIST_IMGUI
//-------------------------------------------------------------
// **�V�[���T�v**
//�E���̃V�[���ł�SceneGame�̃��[�h���s��
//�ESceneSelect���瑗���Ă����������ƂɃf�[�^�̓ǂݍ��݂��s���A
//	�ǂݍ��݂����ׂĊ���������SceneGame���Z�b�g����
//-------------------------------------------------------------
float Hitcheak::timer = 0.0f;
bool Hitcheak::hit = false;
bool Hitcheak::stop1p = false;
bool Hitcheak::stop2p = false;
float Hitcheak::add1P = 0.0f;
float Hitcheak::add2P = 0.0f;
bool Hitcheak::clash = false;
YR_Vector3 Hitcheak::clashpos = { 0.0f,0.0f };
YR_Vector3 Hitcheak::effectpos = { 0.0f,0.0f,-5.0f };
int Hitcheak::effecttimer = 0;
//int Hitcheak::hei = 0;

void SceneLoad::Init()
{
	timer = 0;
	load_fin = false;
	load_state = 0;
	fado_alpha = 1.0f;
	fado_start = false;
	Game_load_fin = false;

	//�V�F�[�_�[�쐬
	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}
	if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/LoadShader_vs.cso", "./Data/Shader/LoadShader_ps.cso");
	}

	//Gbuffer�p�X�v���C�g
	if (sprite == nullptr)
	{
		sprite = std::make_unique<Sprite>();
	}

	//GetSound().BGMPlay(BGMKind::LOAD);

	match_timer = 0.0f;
	plus_match = 60.0f;

	slow_add = 0.5f;

	timer_Multiply = 3.5f;

	sin_max = 0.05f;

	space_time = 0.8f;

	VS_size = 5.0f;

	VS_alpha = 0.0f;

	flash_size = 0.0f;
	flash_alpha = 0.0f;

	line_1p_x = 0.0f;
	line_2p_x = 0.0f;

	line_Multiply = 10000.0f;

	cbuffer_param.Resolution = { 1920.0f ,1080.0f ,((1920.0f) / (1080.0f)) };
	cbuffer_param.brightness = 15.0f;
	cbuffer_param.gamma = 13;
	cbuffer_param.spot_brightness = 1.5f;
	cbuffer_param.ray_density = 1.5f;
	cbuffer_param.curvature = 90.0f;
	cbuffer_param.red = 1.0f;
	cbuffer_param.green = 1.0f;
	cbuffer_param.blue = 1.0f;
	cbuffer_param.material_color = { 1.0f,1.0f,1.0f,1.0f };
	cbuffer_param.dummy1 = 0.0f;
	cbuffer_param.dummy2 = 0.0f;
	cbuffer_param.dummy3 = 0.0f;

	MatchStart();
}

void SceneLoad::LoadData()
{
	//�u���[�h��ʂŕ\������摜���v�����[�h����
	if (load_img == nullptr)
	{
		load_img = std::make_unique<Sprite>(
			L"./Data/Image/UI/GameLoad/load_image.png",
			300.0f,
			400.0f,
			3,
			4,
			100.0f,
			100.0f);
	}

	/*if (load_bg == nullptr)
	{
		load_bg = std::make_unique<Sprite>(
			L"./Data/Image/BG/Load_BG.png",
			3840.0f,
			2160.0f);
	}*/


	IconLoad();

	if (Box == nullptr)
	{
		Box = std::make_unique<Sprite>(L"./Data/Image/UI/GameLoad/Box.png", 640.0f, 640.0f);
	}
	if (VS_Image == nullptr)
	{
		VS_Image = std::make_unique<Sprite>(L"./Data/Image/UI/GameLoad/VS.png", 640.0f, 320.0f);
	}
	if (flash == nullptr)
	{
		flash = std::make_unique<Sprite>(L"./Data/Image/UI/GameLoad/flash.png", 640.0f, 640.0f);
	}
	if (title_texture == nullptr)
	{
		title_texture = std::make_unique<Texture>(L"./Data/Shader/noise.png");
		//title_texture = std::make_unique<Texture>(L"./Data/Image/UI/GameTitle/Title.png");
	}

	if (sampler_wrap == nullptr)
	{
		sampler_wrap = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	//�R���X�^���g�o�b�t�@�쐬
	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(Title_CBuffer));

	load_state = 2;
}

void SceneLoad::UnInit()
{
	//�u���[�h��ʂŕ\������摜���v�̂݉������
	load_img.reset();
	load_img = nullptr;

	//�V�F�[�_�[�����
	spriteShader.reset();
	spriteShader = nullptr;

	p1_cut.reset();
	p1_cut = nullptr;
	p2_cut.reset();
	p2_cut = nullptr;

	p1_name.reset();
	p1_name = nullptr;
	p2_name.reset();
	p2_name = nullptr;

	p1_name_edge.reset();
	p1_name_edge = nullptr;
	p2_name_edge.reset();
	p2_name_edge = nullptr;

	Box.reset();
	Box = nullptr;
	VS_Image.reset();
	VS_Image = nullptr;
	flash.reset();
	flash = nullptr;

	if (load_state < 8)
	{
		if (t->joinable())
		{
			t->join();
		}
		delete t;
	}
}

void SceneLoad::Update(float elapsedTime)
{
	//�^�C�}�[�͏�ɍX�V����
	timer += elapsedTime;
	GetSound().FadeIn(elapsedTime);
	//�u���[�h��ʂŕ\������摜���v�����[�h�ł���
	if (load_fin)
	{
		if (!fado_start)
		{
			if (fado_alpha > 0.0f)
			{
				fado_alpha -= FADE_MIX(elapsedTime);
			}
		}
	}

	//�u�Q�[�����C����ʂŎg�p���郂�f�����v�����[�h�ł���
	if (Game_load_fin)
	{
		//���[�h��ʂ����ނ���
		if (timer > 1.0f)
		{
			fado_start = true;
		}
		if (fado_start)
		{
			if (FedoOut(elapsedTime))
			{
				//�t�F�[�h�A�E�g���I�������Q�[�����C���֑J��
				//FRAMEWORK.SetScene(SCENE_TABLE::SCENE_GAME);
				GetSound().BGMStop(BGMKind::LOAD);
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_GAME);
				UnInit();
				return;
			}
		}
	}
	else
	{
		//-----------------------------------------------------------------------------------------
		//		*�T�v*
		//�E�u���[�h��ʂŕ\������摜���v�Ɓu�Q�[�����C����ʂŎg�p���郂�f�����v��2�����[�h����̂�
		//	�X���b�h��2�񗧂Ăď��ԂɃ��[�h���Ă���
		//�Eload_fin�F�u���[�h��ʂŕ\������摜���v�����[�h�ł���
		//�EGame_load_fin�F�u�Q�[�����C����ʂŎg�p���郂�f�����v�����[�h�ł���
		//-----------------------------------------------------------------------------------------
		switch (load_state)
		{
		case 0:
			//�܂��̓��[�h��ʂŕ\������摜���v�̃��[�h
			load_state = 1;
			t = new std::thread(&SceneLoad::LoadData, this);
			break;
		case 1:
			//���[�h���f�����[�h��
			break;
		case 2:
			//���[�h�I����ɉ�������������Ƃ��̂��߂ɂ����Ă���
			load_state = 3;
			break;
		case 3:
			//��x�X���b�h���J������
			if (t->joinable())
			{
				t->join();
			}
			delete t;
			load_fin = true;
			load_state = 4;
			GetSound().BGMPlay(BGMKind::LOAD);
			break;
		case 4:
			//�Ӑ}�I��1�t���[�����炵�Ă���
			load_state = 5;
			break;
		case 5:
			//���Ɂu�Q�[�����C����ʂŎg�p���郂�f�����v�̃��[�h
			load_state = 6;
			t = new std::thread(&SceneGame::LoadData, &FRAMEWORK.scenegame);
			break;
		case 6:
			//�Q�[�����f�����[�h��
			break;
		case 7:
			//���[�h�I����ɉ�������������Ƃ��̂��߂ɂ����Ă���
			load_state = 8;
			break;
		case 8:
			//�X���b�h���
			if (t->joinable())
			{
				t->join();
			}
			delete t;
			load_state = 9;
			break;
		case 9:
#ifdef EXIST_IMGUI
			if (Get_Use_ImGui())
			{
				Game_load_fin = true;
			}
#endif // EXIST_IMGUI

			if (timer > 10.0f)
			{
				Game_load_fin = true;
			}
			break;
		default:
			break;
		}
	}

	if (load_fin)
	{
		MatchUpdate(elapsedTime);
	}
}

void SceneLoad::Draw(float elapsedTime)
{
	//�u���[�h��ʂŕ\������摜���v�����[�h�ł���
	if (load_fin)
	{
		/*load_bg->DrawRotaGraph(
			spriteShader.get(),
			static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
			static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f,
			0.0f,
			0.5f
		);*/
		/*knight_1p_cut->DrawRotaGraph(
			spriteShader.get(),
			static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
			static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f,
			0.0f,
			0.5f
		);*/

		//�w�i�̕`��
		cbuffer_param.iTime = timer;

		sprite->render(
			titleShader.get(),
			title_texture.get(),
			cbuffer_param,
			sampler_wrap.get(),
			constantBuffer,
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

		//�}�b�`��ʕ`��
		MatchDraw(elapsedTime);

		if (load_state < 8)
		{
			load_img->DrawRotaDivGraph(
				spriteShader.get(),
				FRAMEWORK.SCREEN_WIDTH * 0.92f,
				FRAMEWORK.SCREEN_HEIGHT * 0.85f + (sinf(timer) * 20.0f),
				0.0f,
				2.0f,
				0.05f,
				elapsedTime);
		}
	}

	//�t�F�[�h�p�摜�`��
	FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f,false,SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));

#ifdef EXIST_IMGUI
	//ImGui
	if(Get_Use_ImGui())
	{
		ImGui::Text("Load");
		ImGui::Text("time : %f", timer);
		ImGui::Text("load_state : %d", load_state);
		ImGui::SliderFloat("knight1p_pos_x", &p1_pos_x, -960.0f, 1920.0f + 960.0f);
		ImGui::SliderFloat("knight2p_pos_x", &p2_pos_x, -960.0f, 1920.0f + 960.0f);
		ImGui::InputFloat(u8"Sin�ɏ�Z����l", &plus_match, 1.0f, 10.0f);
		ImGui::InputFloat(u8"�Œ���i�މ��̒l", &slow_add, 1.0f, 10.0f);
		ImGui::InputFloat(u8"match_timer�ɏ�Z����l", &timer_Multiply, 1.0f, 10.0f);
		ImGui::InputFloat(u8"sin�̍ő�l", &sin_max, 1.0f, 10.0f);
		ImGui::InputFloat(u8"�Ԃ̒���", &space_time, 0.1f, 1.0f);
		ImGui::InputFloat(u8"���C���ɏ�Z����l", &line_Multiply, 10.0f, 100.0f);
		ImGui::Text(intro_list[scastI(intro_state)].c_str());
		ImGui::Text("line1p : %f", line_1p_x);
		ImGui::Text("line2p : %f", line_2p_x);
	}
#endif
}

bool SceneLoad::FedoOut(float elapsed_time)
{
	fado_alpha += FADE_MIX(elapsed_time);

	if (fado_alpha > 1.0f)
	{
		return true;
	}

	return false;
}

void SceneLoad::MatchStart()
{
	p1_pos_x = -(float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
	p2_pos_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
	line_1p_x = -(float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
	line_2p_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
	match_timer = 0.0f;
	flash_alpha = 0.0f;
	flash_size = 0.0f;
	intro_state = IntroState::P1;
	/*GetSound().BGMStop(BGMKind::LOAD);
	GetSound().BGMPlay(BGMKind::LOAD);*/
}

void SceneLoad::MatchUpdate(float elapsed_time)
{
	//knight_1p_pos_x += ((sin_match * plus_match) + slow_add);
	//knight_2p_pos_x -= ((sin_match * plus_match) + slow_add);

	switch (intro_state)
	{
	case SceneLoad::IntroState::P1:
		//�v���C���[1�����\��
	{
		//�^�C�}�[���Z
		match_timer += elapsed_time * timer_Multiply;

		//sin�h���g���đ��x�Ɋɋ}������
		float sin_match = 0.0f;
		sin_match = std::max(sinf(match_timer), sin_max);

		//���x�����Z
		p1_pos_x += ((sin_match * plus_match) + slow_add);

		//���C���͉�ʊO�ɍs���܂ŉ��Z
		if (line_1p_x < (float)FRAMEWORK.SCREEN_WIDTH)
		{
			line_1p_x += (line_Multiply * elapsed_time);
		}

		//�v���C���[1������̏ꏊ�ɓ��B�����玟�̃X�e�[�g��
		if (p1_pos_x > (float)FRAMEWORK.SCREEN_WIDTH + ((float)FRAMEWORK.SCREEN_WIDTH * 0.6f))
		{
			intro_state = IntroState::SPACE1;
			match_timer = 0.0f;
		}
	}
	break;
	case SceneLoad::IntroState::SPACE1:
		//�����Ԃ�u��
		match_timer += elapsed_time;

		//���̑��x�ŉ�ʊO��
		p1_pos_x += (plus_match + slow_add);
		line_1p_x += (line_Multiply * elapsed_time);

		//��莞�Ԍo�����玟�̃X�e�[�g��
		if (match_timer > space_time)
		{
			match_timer = 0.0f;
			intro_state = IntroState::P2;
		}
		break;
	case SceneLoad::IntroState::P2:
		//�v���C���[2�����\��
	{
		//�^�C�}�[���Z
		match_timer += elapsed_time * timer_Multiply;

		//sin�h���g���đ��x�Ɋɋ}������
		float sin_match = 0.0f;
		sin_match = std::max(sinf(match_timer), sin_max);

		//���x�����Z
		p2_pos_x -= ((sin_match * plus_match) + slow_add);

		//���C���͉�ʊO�ɍs���܂ŉ��Z
		if (line_2p_x > 0.0f)
		{
			line_2p_x -= (line_Multiply * elapsed_time);
		}

		//�v���C���[2������̏ꏊ�ɓ��B�����玟�̃X�e�[�g��
		if (p2_pos_x < 0.0f - ((float)FRAMEWORK.SCREEN_WIDTH * 0.6f))
		{
			intro_state = IntroState::SPACE2;
			match_timer = 0.0f;
		}
	}
	break;
	case SceneLoad::IntroState::SPACE2:
		//�����Ԃ�u��
		match_timer += elapsed_time;

		//���̑��x�ŉ�ʊO��
		p2_pos_x -= (plus_match + slow_add);
		line_2p_x -= (line_Multiply * elapsed_time);

		//�����������傫�������Ȃ���\��������
		if (flash_alpha < 1.0f)
		{
			flash_alpha += elapsed_time * 2.0f;
		}
		else
		{
			flash_alpha = 1.0f;
		}

		flash_size += elapsed_time * 10.0f;

		//��莞�Ԍo�����玟�̃X�e�[�g��
		if (match_timer > space_time)
		{
			match_timer = 0.0f;
			p1_pos_x = -(float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			p2_pos_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			line_1p_x = -(float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			line_2p_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			intro_state = IntroState::P1P2;
			cbuffer_param.red = 1.0f;
			cbuffer_param.green = 0.2f;
			cbuffer_param.blue = 0.4f;
		}
		break;
	case SceneLoad::IntroState::P1P2:
		//�����\������
	{
		//�^�C�}�[���Z
		match_timer += elapsed_time * timer_Multiply;

		//sin�h���g���đ��x�Ɋɋ}������
		float sin_match = 0.0f;
		sin_match = sinf(match_timer);

		//���𔖂����Ă���
		if (flash_alpha > 0.0f)
		{
			flash_alpha -= elapsed_time * 2.0f;
		}
		else
		{
			flash_alpha = 0.0f;
		}

		//sin�h��0�ȉ��ɂȂ����玟�̃X�e�[�g��
		if (sin_match < sin_max)
		{
			intro_state = IntroState::FIN;
			match_timer = 0.0f;
		}
		else
		{
			//���x�����Z
			p1_pos_x += ((sin_match * plus_match) + slow_add);
			p2_pos_x -= ((sin_match * plus_match) + slow_add);

			//���C���͉�ʊO�ɍs���܂ŉ��Z
			if (line_1p_x < (float)FRAMEWORK.SCREEN_WIDTH)
			{
				line_1p_x += (line_Multiply * elapsed_time);
			}
			if (line_2p_x > 0.0f)
			{
				line_2p_x -= (line_Multiply * elapsed_time);
			}
		}

	}
	break;
	case SceneLoad::IntroState::FIN:
		//���������������Ă���
		p1_pos_x += slow_add;
		p2_pos_x -= slow_add;
		break;
	default:
		break;
	}
}

void SceneLoad::MatchDraw(float elapsed_time)
{
	//�J�b�g�C���[�W�`��
	p1_cut->DrawRotaGraph(
		spriteShader.get(),
		p1_pos_x,
		265.0f,
		0.0f,
		2.8f);

	p2_cut->DrawRotaGraph(
		spriteShader.get(),
		p2_pos_x,
		810.0f,
		0.0f,
		2.8f,
		true);


	//���O���摜�`��
	p1_name_edge->DrawRotaGraph(
		spriteShader.get(),
		p1_pos_x - 680.0f,
		265.0f + 100.0f,
		0.0f,
		1.0f);

	p2_name_edge->DrawRotaGraph(
		spriteShader.get(),
		p2_pos_x + 680.0f,
		810.0f + 100.0f,
		0.0f,
		1.0f);


	//���O�摜�`��
	p1_name->DrawRotaGraph(
		spriteShader.get(),
		p1_pos_x - 680.0f,
		265.0f + 100.0f,
		0.0f,
		1.0f,
		false,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f));

	p2_name->DrawRotaGraph(
		spriteShader.get(),
		p2_pos_x + 680.0f,
		810.0f + 100.0f,
		0.0f,
		1.0f,
		false,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	Box->DrawExtendGraph(
		spriteShader.get(),
		line_1p_x - 2500.0f,
		515.0f,
		line_1p_x,
		540.0f,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(1.0f, 0.1f, 0.1f, 1.0f)
	);
	Box->DrawExtendGraph(
		spriteShader.get(),
		line_2p_x,
		540.0f,
		line_2p_x + 2500.0f,
		565.0f,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f)
	);

	flash->DrawRotaGraph(
		spriteShader.get(),
		(float)FRAMEWORK.SCREEN_WIDTH / 2.0f,
		(float)FRAMEWORK.SCREEN_HEIGHT / 2.0f,
		0.0f,
		flash_size,
		false,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, flash_alpha)
	);

	if (intro_state == IntroState::FIN || intro_state == IntroState::P1P2)
	{
		if (VS_alpha < 1.0f)
		{
			VS_alpha += elapsed_time * 10.0f;
		}
		else
		{
			VS_alpha = 1.0f;
		}

		if (VS_size > 1.0f)
		{
			VS_size -= elapsed_time * 10.0f;
		}
		else
		{
			VS_size = 1.0f;
		}

		VS_Image->DrawRotaGraph(
			spriteShader.get(),
			(float)FRAMEWORK.SCREEN_WIDTH / 2.0f,
			(float)FRAMEWORK.SCREEN_HEIGHT / 2.0f,
			0.0f,
			VS_size,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, VS_alpha)
		);
	}
	else
	{
		VS_alpha = 0.0f;
		VS_size = 3.0f;
	}
}



//�A�C�R���摜�ǂݍ���
void SceneLoad::IconLoad()
{
	std::string front = std::string("./Data/Image/Character/");
	std::string cut = std::string("_cut");
	std::string png = std::string(".png");
	std::string back_name = std::string("_name.png");
	std::string back_edge = std::string("_name_edge.png");


	int p1_chara = FRAMEWORK.sceneselect.select_p1;
	int p2_chara = FRAMEWORK.sceneselect.select_p2;

	int p1_color = scastI(FRAMEWORK.sceneselect.color_p1);
	int p2_color = scastI(FRAMEWORK.sceneselect.color_p2);


	//�v���C���[1�̑I���L�����̉摜�Q�Ɨp������쐬
	std::string contents_p1_chara = front + GetName().chara_name_list[p1_chara] +
		std::string("/") + GetName().chara_name_list[p1_chara] + cut + std::to_string(p1_color) + png;
	std::wstring icon_name_p1_chara = to_wstring_Load(contents_p1_chara);

	//�v���C���[2�̑I���L�����̉摜�Q�Ɨp������쐬
	std::string contents_p2_chara = front + GetName().chara_name_list[p2_chara] +
		std::string("/") + GetName().chara_name_list[p2_chara] + cut + std::to_string(p2_color) + png;
	std::wstring icon_name_p2_chara = to_wstring_Load(contents_p2_chara);

	//�v���C���[1�̑I���L�����̖��O�摜�Q�Ɨp������쐬
	std::string contents_p1_name = front + GetName().chara_name_list[p1_chara] +
		std::string("/") + GetName().chara_name_list[p1_chara] + back_name;
	std::wstring name_name_p1 = to_wstring_Load(contents_p1_name);

	//�v���C���[1�̑I���L�����̖��O���摜�Q�Ɨp������쐬
	std::string contents_p1_edge = front + GetName().chara_name_list[p1_chara] +
		std::string("/") + GetName().chara_name_list[p1_chara] + back_edge;
	std::wstring edge_name_p1 = to_wstring_Load(contents_p1_edge);

	//�v���C���[2�̑I���L�����̖��O�摜�Q�Ɨp������쐬
	std::string contents_p2_name = front + GetName().chara_name_list[p2_chara] +
		std::string("/") + GetName().chara_name_list[p2_chara] + back_name;
	std::wstring name_name_p2 = to_wstring_Load(contents_p2_name);

	//�v���C���[2�̑I���L�����̖��O���摜�Q�Ɨp������쐬
	std::string contents_p2_edge = front + GetName().chara_name_list[p2_chara] +
		std::string("/") + GetName().chara_name_list[p2_chara] + back_edge;
	std::wstring edge_name_p2 = to_wstring_Load(contents_p2_edge);

	if (p1_cut == nullptr)
	{
		p1_cut = std::make_unique<Sprite>(icon_name_p1_chara.data(), 640.0f, 192.0f);
	}
	if (p2_cut == nullptr)
	{
		p2_cut = std::make_unique<Sprite>(icon_name_p2_chara.data(), 640.0f, 192.0f);
	}
	if (p1_name == nullptr)
	{
		p1_name = std::make_unique<Sprite>(name_name_p1.data(), 640.0f, 320.0f);
	}
	if (p2_name == nullptr)
	{
		p2_name = std::make_unique<Sprite>(name_name_p2.data(), 640.0f, 320.0f);
	}
	if (p1_name_edge == nullptr)
	{
		p1_name_edge = std::make_unique<Sprite>(edge_name_p1.data(), 640.0f, 320.0f);
	}
	if (p2_name_edge == nullptr)
	{
		p2_name_edge = std::make_unique<Sprite>(edge_name_p2.data(), 640.0f, 320.0f);
	}
}