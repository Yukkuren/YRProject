#include "Scene.h"
#include "framework.h"

//-------------------------------------------------------------
// **�V�[���T�v**
//�E�{�^���������ƃR���g���[���[�ݒ肪�o��悤�ɂ���
//�@1P,2P�̓��͂����肷��
//�E�ݒ肵����Ó]�BSceneSelect�֑J�ڂ���
//-------------------------------------------------------------

void SceneTitle::Init()
{
	load_fin = false;
	load_state = 0;
	p1_pos = { 680.0f,500.0f };
	p2_pos = { 1100.0f,500.0f };
	none_pos = { 900.0f,700.0f };
	p1 = none_pos;
	p2 = none_pos;
	Rato = 3.0f;
	g1.Init();
	g2.Init();
	fedo_start = false;
	fedo_alpha = 1.0f;

	//�����I�ɏ���
	select_p1 = scastI(INPUT_PLAYER::NONE);
	select_p2 = scastI(INPUT_PLAYER::NONE);
	//-------------------------------------
	p1Enter = false;
	p2Enter = false;
	end = false;
	timer = 0.0f;
}

void SceneTitle::LoadData()
{
	if (test == nullptr)
	{
		test = std::make_unique<Sprite>(L"./Data/Image/BG/mementos.png", 1920.0f, 1080.0f);
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

void SceneTitle::UnInit()
{
	test.reset();
	test = nullptr;
	knight_icon.reset();
	knight_icon = nullptr;
	ken_icon.reset();
	ken_icon = nullptr;
	select_img.reset();
	select_img = nullptr;
}

void SceneTitle::Update(float elapsed_time)
{
	//���[�h�I��
	if (load_fin)
	{
		//timer�̓t���[���Ȃ��悤�Ƀ��Z�b�g����
		if (timer < 1000.0f)
		{
			timer += elapsed_time;
		}
		else
		{
			timer = 0;
		}

		g1.Update();
		g2.Update();

		//�v���C���[1�̃J�[�\���ړ�����
		if (p1Enter)
		{
			if (g1.x_input[scastI(PAD::B)] == 1)
			{
				p1Enter = false;
			}
		}
		else
		{
			if (g1.x_input[scastI(PAD::STICK_R)] == 1)
			{
				select_p1++;
				if (select_p1 > scastI(INPUT_PLAYER::P2))
				{
					select_p1 = scastI(INPUT_PLAYER::P2);
				}
			}
			if (g1.x_input[scastI(PAD::STICK_L)] == 1)
			{
				select_p1--;
				if (select_p1 < 0)
				{
					select_p1 = scastI(INPUT_PLAYER::P1);
				}
			}

			if (g1.x_input[scastI(PAD::A)] == 1&&select_p1!=scastI(INPUT_PLAYER::NONE))
			{
				if (select_p2 != select_p1)
				{
					p1Enter = true;
				}
				//PlaySE(SE_ENTER);
			}
		}

		//�v���C���[2�̃J�[�\���ړ�����
		if (p2Enter)
		{
			if (g2.x_input[scastI(PAD::B)] == 1)
			{
				p2Enter = false;
			}
		}
		else
		{
			if (g2.x_input[scastI(PAD::STICK_R)] == 1)
			{
				select_p2++;
				if (select_p2 > scastI(INPUT_PLAYER::P2))
				{
					select_p2 = scastI(INPUT_PLAYER::P2);
				}
			}
			if (g2.x_input[scastI(PAD::STICK_L)] == 1)
			{
				select_p2--;
				if (select_p2 < 0)
				{
					select_p2 = scastI(INPUT_PLAYER::P1);
				}
			}
			if (g2.x_input[scastI(PAD::A)] == 1 && select_p2 != scastI(INPUT_PLAYER::NONE))
			{
				if (select_p1 != select_p2)
				{
					p2Enter = true;
				}
				//PlaySE(SE_ENTER);
			}
		}


		p1 = PosSet(select_p1);
		p2 = PosSet(select_p2);

		//�����̃v���C���[�����肵����
		if (p1Enter && p2Enter)
		{
			end = true;
		}
		if (end)
		{
			//�t�F�[�h�A�E�g���X�^�[�g������
			fedo_start = true;
		}
		if (fedo_start)
		{
			if (FedoOut(elapsed_time))
			{
				//�t�F�[�h�A�E�g���I�������Z���N�g��ʂ�
				//�����œ��͂�ۑ�����
				//�֐��ɂ���
				FRAMEWORK.scenegame.PadSet(select_p1);
				FRAMEWORK.SetScene(SCENE_SELECT);
				UnInit();
				return;
			}
		}
		else
		{
			//�t�F�[�h�A�E�g���X�^�[�g���ĂȂ��ꍇ�͉�ʂ��f��
			if (fedo_alpha > 0.0f)
			{
				fedo_alpha -= FEDO_MIX(elapsed_time);
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
			t = new std::thread(&SceneTitle::LoadData, this);
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

void SceneTitle::Draw(float elapsed_time)
{
	if (load_fin)
	{
		test->DrawGraph(FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f);
		
		//�A�C�R���`��
		knight_icon->DrawRotaGraph(
			p1_pos.x,
			p1_pos.y,
			0.0f,
			Rato
		);
		ken_icon->DrawRotaGraph(
			p2_pos.x,
			p2_pos.y,
			0.0f,
			Rato
		);

		//�I���摜�`��
		//1P
		if (p1Enter)
		{
			select_img->DrawRotaGraph
			(
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
					p2.x,
					p2.y,
					0.0f,
					Rato,
					DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
				);
			}
		}
	}

	FRAMEWORK.fedo_img->DrawRotaGraph(FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fedo_alpha));

#if USE_IMGUI
	//ImGui
	{
		ImGui::Text("title");
		ImGui::Text("time : %f",timer);
		ImGui::Text("load_state : %d", load_state);
	}
#endif
}

bool SceneTitle::FedoOut(float elapsed_time)
{
	fedo_alpha += FEDO_MIX(elapsed_time);

	if (fedo_alpha > 1.0f)
	{
		return true;
	}

	return false;
}

YR_Vector3 SceneTitle::PosSet(int select)
{
	switch (select)
	{
	case scastI(INPUT_PLAYER::P1):
		return p1_pos;
		break;
	case scastI(INPUT_PLAYER::P2):
		return p2_pos;
		break;
	case scastI(INPUT_PLAYER::NONE):
		return none_pos;
		break;
	}
	return YR_Vector3(0.0f, 0.0f);
}