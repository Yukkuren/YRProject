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
	g1.Init();
	g2.Init();

	//�����I�ɏ���
	select_p1 = scastI(PLSELECT::KNIGHT);
	select_p2 = scastI(PLSELECT::KNIGHT);
	//-------------------------------------
	p1Enter = false;
	p2Enter = false;
	end = false;
	Fedotimer = 255;
}

void SceneSelect::LoadData()
{
	if (back_img == nullptr)
	{
		back_img = std::make_unique<Sprite>(L"./Data/Image/BG/select.png");
		back_img->LoadGraph(1920.0f, 1080.0f);
	}
	if (knight_icon == nullptr)
	{
		knight_icon = std::make_unique<Sprite>(L"./Data/Image/Character/Ryu/icon.png");
		knight_icon->LoadGraph(64.0f, 64.0f);
	}
	if (ken_icon == nullptr)
	{
		ken_icon = std::make_unique<Sprite>(L"./Data/Image/Character/Ken/icon.png");
		ken_icon->LoadGraph(64.0f, 64.0f);
	}
	if (select_img == nullptr)
	{
		select_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/select.png");
		select_img->LoadGraph(64.0f, 64.0f);
	}

	load_state = 3;
}

void SceneSelect::Update(float elapsedTime)
{
	if (load_fin)
	{
		if (timer < 1000)
		{
			timer++;
		}
		else
		{
			timer = 0;
		}

		g1.Update();
		g2.Update();

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
				if (select_p1 > scastI(PLSELECT::KEN))
				{
					select_p1 = scastI(PLSELECT::KEN);
				}
			}
			if (g1.x_input[scastI(PAD::STICK_L)] == 1)
			{
				select_p1--;
				if (select_p1 < 0)
				{
					select_p1 = scastI(PLSELECT::KNIGHT);
				}
			}

			if (g1.x_input[scastI(PAD::A)] == 1)
			{
				p1Enter = true;
				//PlaySE(SE_ENTER);
			}
		}

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
				if (select_p2 > scastI(PLSELECT::KEN))
				{
					select_p2 = scastI(PLSELECT::KEN);
				}
			}
			if (g2.x_input[scastI(PAD::STICK_L)] == 1)
			{
				select_p2--;
				if (select_p2 < 0)
				{
					select_p2 = scastI(PLSELECT::KNIGHT);
				}
			}
			if (g2.x_input[scastI(PAD::A)] == 1)
			{
				p2Enter = true;
				//PlaySE(SE_ENTER);
			}
		}


		p1 = PosSet(select_p1);
		p2 = PosSet(select_p2);

		if (p1Enter && p2Enter)
		{
			if (g1.x_input[scastI(PAD::START)] == 1 || g2.x_input[scastI(PAD::START)] == 1)
			{
				end = true;
				//PlaySE(SE_ENTER);
			}
		}
		if (end)
		{
			//�t�F�[�h�A�E�g���I������烍�[�h��ʂ�
			FRAMEWORK.SetScene(SCENE_LOAD);
			//if (Fedo())
			//{
			//	/*SetDrawBright(255, 255, 255);
			//	SetScene(SCENE_GAME);
			//	Uninit();*/
			//}
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
			t = new std::thread(&SceneSelect::LoadData, this);
			load_state = 2;
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