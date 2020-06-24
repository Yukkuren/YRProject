#include "Scene.h"
#include "framework.h"

//-------------------------------------------------------------
// **シーン概要**
//・暗転からスタート。画像をロードしたら画面表示
//・1p,2p両方が選択を終えてスタートボタンを押したら
//　暗転してSceneLoadに遷移
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

	//将来的に消す
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
	back_img.reset(back_img.get());
	back_img = nullptr;
	knight_icon.reset(knight_icon.get());
	knight_icon = nullptr;
	ken_icon.reset(ken_icon.get());
	ken_icon = nullptr;
	select_img.reset(select_img.get());
	select_img = nullptr;
}

void SceneSelect::Update(float elapsedTime)
{
	if (load_fin)
	{
		if (timer < 1000.0f)
		{
			timer += elapsedTime;
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
			//フェードアウトが終わったらロード画面へ
			//FRAMEWORK.SetScene(SCENE_LOAD);
			FRAMEWORK.SetScene(SCENE_TITLE);
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
#if USE_IMGUI
	//ImGui
	{
		ImGui::Text("time : %d", time);
		ImGui::Text("select");
	}
#endif
	if (load_fin)
	{
		//テスト描画なので後で削除
		//背景
		back_img->DrawRotaGraph(
			FRAMEWORK.SCREEN_WIDTH / 2.0f,
			FRAMEWORK.SCREEN_HEIGHT / 2.0f,
			0.0f,
			1.0f);
		//アイコン描画
		knight_icon->DrawRotaGraph(
			knight_pos.x,
			knight_pos.y,
			0.0f,
			Rato
		);
		ken_icon->DrawRotaGraph(
			kenpos.x,
			kenpos.y,
			0.0f,
			Rato
		);

		//選択画像描画
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