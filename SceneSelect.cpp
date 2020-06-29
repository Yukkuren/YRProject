#include "Scene.h"
#include "framework.h"

//-------------------------------------------------------------
// **�V�[���T�v**
//�E�Ó]����X�^�[�g�B�摜�����[�h�������ʕ\��
//�E1p,2p�������I�����I���ăX�^�[�g�{�^������������
//�@�Ó]����SceneLoad�ɑJ��
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
	fedo_start = false;
	fedo_alpha = 1.0f;

	//�����I�ɏ���
	select_p1 = scastI(PLSELECT::KNIGHT);
	select_p2 = scastI(PLSELECT::KNIGHT);
	//-------------------------------------
	p1Enter = false;
	p2Enter = false;
	end = false;
	timer = 0.0f;

	spriteShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SPRITE);
	spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
}

void SceneSelect::LoadData()
{
	if (back_img == nullptr)
	{
		back_img = std::make_unique<Sprite>(L"./Data/Image/BG/select.png", 1920.0f, 1080.0f);
		//back_img->LoadGraph(1920.0f, 1080.0f);
	}
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

	load_state = 3;
}


void SceneSelect::UnInit()
{
	knight_icon.reset();
	knight_icon = nullptr;
	back_img.reset();
	back_img = nullptr;
	ken_icon.reset();
	ken_icon = nullptr;
	select_img.reset();
	select_img = nullptr;
}

void SceneSelect::Update(float elapsedTime)
{
	//���[�h�I��
	if (load_fin)
	{
#if USE_IMGUI
		if (pKeyState.nflg == 1)
		{
			select_p1 = scastI(PLSELECT::KNIGHT);
			select_p2 = scastI(PLSELECT::KNIGHT);
			//�t�F�[�h�A�E�g���I������烍�[�h��ʂ�
			FRAMEWORK.SetScene(SCENE_LOAD);
			UnInit();
			return;
		}
#endif // USE_IMGUI

		//timer�̓t���[���Ȃ��悤�Ƀ��Z�b�g����
		if (timer < 1000.0f)
		{
			timer += elapsedTime;
		}
		else
		{
			timer = 0;
		}

		FRAMEWORK.scenegame.pad1->Update();
		FRAMEWORK.scenegame.pad2->Update();

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
				if (select_p1 > scastI(PLSELECT::KEN))
				{
					select_p1 = scastI(PLSELECT::KEN);
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
				if (select_p2 > scastI(PLSELECT::KEN))
				{
					select_p2 = scastI(PLSELECT::KEN);
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
			fedo_start = true;
		}
		if (fedo_start)
		{
			if (FedoOut(elapsedTime))
			{
				//�t�F�[�h�A�E�g���I������烍�[�h��ʂ�
				FRAMEWORK.SetScene(SCENE_LOAD);
				//FRAMEWORK.SetScene(SCENE_TITLE);
				UnInit();
				return;
			}
		}
		else
		{
			//�t�F�[�h�A�E�g���X�^�[�g���ĂȂ��ꍇ�͉�ʂ��f��
			if (fedo_alpha > 0.0f)
			{
				fedo_alpha -= FEDO_MIX(elapsedTime);
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
#if USE_IMGUI
	//ImGui
	{
		ImGui::Text("time : %f", timer);
		ImGui::Text("select");
	}
#endif
	//�摜�̓��[�h���I���܂ŕ`�悵�Ȃ�
	if (load_fin)
	{
		//�e�X�g�`��Ȃ̂Ō�ō폜
		//�w�i
		back_img->DrawRotaGraph(
			spriteShader.get(),
			FRAMEWORK.SCREEN_WIDTH / 2.0f,
			FRAMEWORK.SCREEN_HEIGHT / 2.0f,
			0.0f,
			1.0f);
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
					DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
				);
			}
		}
	}
	//�t�F�[�h�p�摜
	FRAMEWORK.fedo_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fedo_alpha));
}

YR_Vector3 SceneSelect::PosSet(int select)
{
	switch (select)
	{
	case scastI(PLSELECT::KNIGHT):
		return knight_pos;
		break;
	case scastI(PLSELECT::KEN):
		return kenpos;
		break;
	}
	return YR_Vector3(0.0f, 0.0f);
}

bool SceneSelect::FedoOut(float elapsed_time)
{
	fedo_alpha += FEDO_MIX(elapsed_time);

	if (fedo_alpha > 1.0f)
	{
		return true;
	}

	return false;
}