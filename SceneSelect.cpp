#include "Scene.h"
#include "framework.h"
#include "YRSound.h"

//-------------------------------------------------------------
// **�V�[���T�v**
//�E�Ó]����X�^�[�g�B�摜�����[�h�������ʕ\��
//�E1p,2p�������I�����I���ăX�^�[�g�{�^������������
//�@�Ó]����SceneLoad�ɑJ��
//�E�L���������ނ̏�Ԃł͎g�p���Ȃ�
//-------------------------------------------------------------


void SceneSelect::Init()
{
	load_fin = false;
	load_state = 0;
	knight_pos = { 680.0f,500.0f };
	kenpos = { 1100.0f,500.0f };
	p1 = knight_pos;
	p2 = kenpos;
	Rato = 3.0f;
	FRAMEWORK.scenegame.pad1->Init();
	FRAMEWORK.scenegame.pad2->Init();
	fado_start = false;
	fado_alpha = 1.0f;

	//�����I�ɏ���
	select_p1 = scastI(PLSELECT::KNIGHT);
	select_p2 = scastI(PLSELECT::KNIGHT);
	//-------------------------------------
	p1Enter = false;
	p2Enter = false;
	end = false;
	timer = 0.0f;

	//�^�C�g���V�F�[�_�[�p�p�����[�^
	cbuffer_param.Resolution = { 1920.0f,1080.0f,(1920.0f / 1080.0f) };
	cbuffer_param.brightness = 15.0f;
	cbuffer_param.gamma = 13;
	cbuffer_param.spot_brightness = 1.5f;
	cbuffer_param.ray_density = 1.5f;
	cbuffer_param.curvature = 90.0f;
	cbuffer_param.red = 10.0f;
	cbuffer_param.green = 2.8f;
	cbuffer_param.blue = 4.0f;
	cbuffer_param.material_color = { 1.0f,1.0f,1.0f,1.0f };
	cbuffer_param.dummy1 = 0.0f;
	cbuffer_param.dummy2 = 0.0f;
	cbuffer_param.dummy3 = 0.0f;

	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}
	GetSound().BGMPlay(BGMKind::CHARA_SELECT);
}

void SceneSelect::LoadData()
{
	//if (back_img == nullptr)
	//{
	//	back_img = std::make_unique<Sprite>(L"./Data/Image/BG/select.png", 1920.0f, 1080.0f);
	//	//back_img->LoadGraph(1920.0f, 1080.0f);
	//}

	//�T���v���[����
	if (sampler_clamp == nullptr)
	{
		sampler_clamp = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
	}
	if (sampler_wrap == nullptr)
	{
		sampler_wrap = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	//Gbuffer�p�X�v���C�g
	if (sprite == nullptr)
	{
		sprite = std::make_unique<Sprite>();
	}

	//�摜�ǂݍ���
	if (knight_icon == nullptr)
	{
		knight_icon = std::make_unique<Sprite>(L"./Data/Image/Character/Ryu/icon.png", 64.0f, 64.0f);
		//knight_icon->LoadGraph(64.0f, 64.0f);
	}
	if (ken_icon == nullptr)
	{
		ken_icon = std::make_unique<Sprite>(L"./Data/Image/Character/Ken/icon.png", 64.0f, 64.0f);
		//ken_icon->LoadGraph(64.0f, 64.0f);
	}
	if (select_img == nullptr)
	{
		select_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/select.png", 64.0f, 64.0f);
		//select_img->LoadGraph(64.0f, 64.0f);
	}

	//�R���X�^���g�o�b�t�@�쐬
	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(Title_CBuffer));

	//�V�F�[�_�[�ǂݍ���
	if (spriteEx == nullptr)
	{
		spriteEx = std::make_unique<YRShader>(ShaderType::SPRITE_EX);
		spriteEx->Create("./Data/Shader/SpriteEx_vs.cso", "./Data/Shader/SpriteEx_ps.cso");
	}
	if (selectShader == nullptr)
	{
		selectShader = std::make_unique<YRShader>(ShaderType::TITLE);
		selectShader->Create("./Data/Shader/CharaSelectShader_vs.cso", "./Data/Shader/CharaSelectShader_ps.cso");
	}

	//�e�N�X�`�����[�h
	if (color_texture == nullptr)
	{
		color_texture = std::make_unique<Texture>();
		color_texture->Create(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM);
		color_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}

	if (luminance_texture == nullptr)
	{
		luminance_texture = std::make_unique<Texture>();
		luminance_texture->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
		luminance_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}

	//�^�C�g���V�F�[�_�[�ɑ���摜(�m�C�Y�摜��ǂݍ���ł���)
	if (select_texture == nullptr)
	{
		select_texture = std::make_unique<Texture>(L"./Data/Shader/noise.png");
	}

	load_state = 3;
}


void SceneSelect::UnInit()
{
	knight_icon.reset();
	knight_icon = nullptr;
	ken_icon.reset();
	ken_icon = nullptr;
	select_img.reset();
	select_img = nullptr;
	spriteShader.reset();
	spriteShader = nullptr;
	select_texture.reset();
	select_texture = nullptr;

	sampler_clamp.reset();
	sampler_clamp = nullptr;
	sampler_wrap.reset();
	sampler_wrap = nullptr;

	color_texture.reset();
	color_texture = nullptr;
	luminance_texture.reset();
	luminance_texture = nullptr;

	constantBuffer.Reset();
	constantBuffer = nullptr;

	selectShader.reset();
	selectShader = nullptr;
}

void SceneSelect::Update(float elapsed_time)
{
	//���[�h�I��
	if (load_fin)
	{
		if (pKeyState.nflg == 1 || FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::START)] == 1)
		{
			select_p1 = scastI(PLSELECT::KNIGHT);
			select_p2 = scastI(PLSELECT::KNIGHT);
			//�t�F�[�h�A�E�g���I������烍�[�h��ʂ�
			GetSound().BGMStop(BGMKind::CHARA_SELECT);
			FRAMEWORK.SetScene(SCENE_TABLE::SCENE_LOAD);
			UnInit();
			return;
		}
#ifdef EXIST_IMGUI

#endif // USE_IMGUI

		//timer�̓t���[���Ȃ��悤�Ƀ��Z�b�g����
		if (timer < 1000.0f)
		{
			timer += elapsed_time;
		}
		else
		{
			timer = 0;
		}
		//constexpr float a = 1.0f / 60.0f;
		FRAMEWORK.scenegame.pad1->Update(elapsed_time);
		FRAMEWORK.scenegame.pad2->Update(elapsed_time);

		//�v���C���[1�̃J�[�\���ړ�����
		if (p1Enter)
		{
			if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::B)] == 1)
			{
				p1Enter = false;
			}
		}
		else
		{
			if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_R)] == 1)
			{
				select_p1++;
				if (select_p1 > scastI(PLSELECT::NERU))
				{
					select_p1 = scastI(PLSELECT::NERU);
				}
			}
			if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_L)] == 1)
			{
				select_p1--;
				if (select_p1 < 0)
				{
					select_p1 = scastI(PLSELECT::KNIGHT);
				}
			}

			if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::A)] == 1)
			{
				p1Enter = true;
				//PlaySE(SE_ENTER);
			}
		}

		//�v���C���[2�̃J�[�\���ړ�����
		if (p2Enter)
		{
			if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::B)] == 1)
			{
				p2Enter = false;
			}
		}
		else
		{
			if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_R)] == 1)
			{
				select_p2++;
				if (select_p2 > scastI(PLSELECT::NERU))
				{
					select_p2 = scastI(PLSELECT::NERU);
				}
			}
			if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_L)] == 1)
			{
				select_p2--;
				if (select_p2 < 0)
				{
					select_p2 = scastI(PLSELECT::KNIGHT);
				}
			}
			if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::A)] == 1)
			{
				p2Enter = true;
				//PlaySE(SE_ENTER);
			}
		}


		p1 = PosSet(select_p1);
		p2 = PosSet(select_p2);

		//�����̃v���C���[�����肵����
		if (p1Enter && p2Enter)
		{
			if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::START)] == 1 ||
				FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::START)] == 1)
			{
				end = true;
				//PlaySE(SE_ENTER);
			}
		}
		if (end)
		{
			//�t�F�[�h�A�E�g���X�^�[�g������
			fado_start = true;
		}
		if (fado_start)
		{
			if (FadoOut(elapsed_time))
			{
				//�t�F�[�h�A�E�g���I������烍�[�h��ʂ�
				GetSound().BGMStop(BGMKind::CHARA_SELECT);
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_LOAD);
				//FRAMEWORK.SetScene(SCENE_TITLE);
				UnInit();
				return;
			}
		}
		else
		{
			//�t�F�[�h�A�E�g���X�^�[�g���ĂȂ��ꍇ�͉�ʂ��f��
			if (fado_alpha > 0.0f)
			{
				fado_alpha -= FADE_MIX(elapsed_time);
			}
		}

	}
	else
	{

		switch (load_state)
		{
		case 0:
			if (1/*Fedo::getInstance().FedoIn()*/)
			{
				load_state = 1;
			}
			break;
		case 1:
			load_state = 2;
			t = new std::thread(&SceneSelect::LoadData, this);
		case 2:
			break;
		case 3:
			if (1/*Fedo::getInstance().FedoOut()*/)
			{
				load_state = 4;
				//pFedo.Set();
			}
			break;
		case 4:
			t->join();
			delete t;
			load_fin = true;
			break;
		default:
			break;
		}
	}
}

void SceneSelect::Draw(float elapsedTime)
{
#ifdef EXIST_IMGUI
	//ImGui
	if(Get_Use_ImGui())
	{
		ImGui::Text("time : %f", timer);
		ImGui::Text("select");
	}
#endif
	//�摜�̓��[�h���I���܂ŕ`�悵�Ȃ�
	if (load_fin)
	{
		SetRenderTexture();

		//�e�X�g�`��Ȃ̂Ō�ō폜
		//�w�i
		/*back_img->DrawRotaGraph(
			spriteShader.get(),
			FRAMEWORK.SCREEN_WIDTH / 2.0f,
			FRAMEWORK.SCREEN_HEIGHT / 2.0f,
			0.0f,
			1.0f);*/

		//�w�i�X�v���C�g�`��
		sprite->render(
			selectShader.get(),
			select_texture.get(),
			cbuffer_param,
			sampler_wrap.get(),
			constantBuffer,
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

		//�A�C�R���`��
		knight_icon->DrawRotaGraph(
			spriteShader.get(),
			knight_pos.x,
			knight_pos.y,
			0.0f,
			Rato
		);
		ken_icon->DrawRotaGraph(
			spriteShader.get(),
			kenpos.x,
			kenpos.y,
			0.0f,
			Rato
		);

		//�I���摜�`��
		//1P
		if (p1Enter)
		{
			select_img->DrawRotaGraph
			(
				spriteShader.get(),
				p1.x,
				p1.y,
				0.0f,
				Rato,
				false,
				SpriteMask::NONE,
				DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
			);
		}
		else
		{
			if (static_cast<int>(timer * 10.0f) % 5 > 0)
			{
				select_img->DrawRotaGraph
				(
					spriteShader.get(),
					p1.x,
					p1.y,
					0.0f,
					Rato,
					false,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
				);
			}
		}

		//2P
		if (p2Enter)
		{
			select_img->DrawRotaGraph
			(
				spriteShader.get(),
				p2.x,
				p2.y,
				0.0f,
				Rato,
				false,
				SpriteMask::NONE,
				DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
			);
		}
		else
		{
			if (static_cast<int>(timer * 10.0f) % 5 > 0)
			{
				select_img->DrawRotaGraph
				(
					spriteShader.get(),
					p2.x,
					p2.y,
					0.0f,
					Rato,
					false,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
				);
			}
		}

		NullSetRenderTexture();
		RenderTexture();
		FRAMEWORK.framebuffer.Deactivate();
	}

	//if (load_fin)
	//{
	//	NullSetRenderTexture();
	//	RenderTexture(elapsed_time);
	//	FRAMEWORK.framebuffer.Deactivate();
	//	//framebuffer::ResetRenderTargetViews();

	//	//SceneRender(elapsed_time);
	//}
	//�t�F�[�h�p�摜
	FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, false, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));
}

YR_Vector3 SceneSelect::PosSet(int select)
{
	switch (select)
	{
	case scastI(PLSELECT::KNIGHT):
		return knight_pos;
		break;
	case scastI(PLSELECT::NERU):
		return kenpos;
		break;
	}
	return YR_Vector3(0.0f, 0.0f);
}


bool SceneSelect::FadoOut(float elapsed_time)
{
	fado_alpha += FADE_MIX(elapsed_time);

	if (fado_alpha > 1.0f)
	{
		return true;
	}

	return false;
}


void SceneSelect::SetRenderTexture()
{
	FRAMEWORK.framebuffer.GetDefaultRTV();
	FRAMEWORK.framebuffer.SetRenderTexture(color_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(normal_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(position_texture->GetRenderTargetView());
	FRAMEWORK.framebuffer.SetRenderTexture(luminance_texture->GetRenderTargetView());

	ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

	//��ʃN���A
	FRAMEWORK.framebuffer.Clear();
	FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//�r���[�X�V
	YRCamera.Active();

	//�r���[�|�[�g�ݒ�
	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f, dsv);

	//�u�����h�X�e�[�g�ݒ�
	FRAMEWORK.BlendSet(Blend::ALPHA);
	//���X�^���C�U�[�ݒ�
	FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
	//�f�v�X�X�e���V���X�e�[�g�ݒ�
	FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

	//�T���v���[�ݒ�
	sampler_clamp->Set(0);
}


void SceneSelect::NullSetRenderTexture()
{
	//�����_�[�^�[�Q�b�g�̉�
	//FRAMEWORK.context.Get()->OMSetRenderTargets(testrtv.size(), testrtv.data(), FRAMEWORK.depth.Get());
	FRAMEWORK.framebuffer.ResetRenderTexture();
	FRAMEWORK.framebuffer.SetDefaultRTV();
}


void SceneSelect::RenderTexture()
{
	//Gbuffer�`��
	sprite->render(
		spriteEx.get(),
		color_texture.get(),
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);
}