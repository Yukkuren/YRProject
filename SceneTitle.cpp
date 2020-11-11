#include "Scene.h"
#include "framework.h"
#include "YRSound.h"

//-------------------------------------------------------------
// **シーン概要**
//・ボタンを押すとコントローラー設定が出るようにして
//　1P,2Pの入力を決定する
//・設定したら暗転。SceneSelectへ遷移する
//TODO:出力のDrawエラーを消すにはディファードシェーダーにする必要がある
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
	fado_start = false;
	fado_alpha = 1.0f;

	//将来的に消す
	select_p1 = scastI(INPUT_PLAYER::NONE);
	select_p2 = scastI(INPUT_PLAYER::NONE);
	//-------------------------------------
	p1Enter = false;
	p2Enter = false;
	end = false;
	timer = 0.0f;

	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}

	GetSound().BGMPlay(BGMKind::TITLE);
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
	spriteShader.reset();
	spriteShader = nullptr;
}

void SceneTitle::Update(float elapsed_time)
{
	//ロード終了
	if (load_fin)
	{
		if (pKeyState.aflg > 0)
		{
			GetSound().BGMPause(BGMKind::TITLE);
			GetSound().SEPlay(SEKind::SELECT_ENTER);
		}
		else
		{
			GetSound().BGMPlay(BGMKind::TITLE);
		}

		if (pKeyState.nflg == 1 || g1.x_input[scastI(PAD::START)] == 1)
		{
			select_p1 = scastI(INPUT_PLAYER::P1);
			select_p2 = scastI(INPUT_PLAYER::P2);
			FRAMEWORK.scenegame.PadSet(select_p1);
			UnInit();
			FRAMEWORK.SetScene(SCENE_SELECT);
			return;
		}
#if USE_IMGUI
		

		if (pKeyState.tflg == 1)
		{
			select_p1 = scastI(INPUT_PLAYER::P1);
			select_p2 = scastI(INPUT_PLAYER::P2);
			FRAMEWORK.scenegame.PadSet(select_p1);
			UnInit();
			FRAMEWORK.SetScene(SCENE_TEST);
			return;
		}

#endif // USE_IMGUI

		//timerはフローしないようにリセットする
		if (timer < 1000.0f)
		{
			timer += elapsed_time;
		}
		else
		{
			timer = 0;
		}

		g1.Update(elapsed_time);
		g2.Update(elapsed_time);

		//プレイヤー1のカーソル移動処理
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

		//プレイヤー2のカーソル移動処理
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

		//両方のプレイヤーが決定したら
		if (p1Enter && p2Enter)
		{
			end = true;
		}
		if (end)
		{
			//フェードアウトをスタートさせる
			fado_start = true;
		}
		if (fado_start)
		{
			if (FedoOut(elapsed_time))
			{
				//フェードアウトが終わったらセレクト画面へ
				//ここで入力を保存する
				//関数にする
				FRAMEWORK.scenegame.PadSet(select_p1);
				FRAMEWORK.SetScene(SCENE_SELECT);
				UnInit();
				return;
			}
		}
		else
		{
			//フェードアウトがスタートしてない場合は画面を映す
			if (fado_alpha > 0.0f)
			{
				fado_alpha -= FEDO_MIX(elapsed_time);
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
		test->DrawGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f);
		
		//アイコン描画
		knight_icon->DrawRotaGraph(
			spriteShader.get(),
			p1_pos.x,
			p1_pos.y,
			0.0f,
			Rato
		);
		ken_icon->DrawRotaGraph(
			spriteShader.get(),
			p2_pos.x,
			p2_pos.y,
			0.0f,
			Rato
		);

		//選択画像描画
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

		// フォントの描画
		{
			FRAMEWORK.font->Begin(FRAMEWORK.context.Get());
			FRAMEWORK.font->Draw(static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f)-150.0f, static_cast<float>(FRAMEWORK.SCREEN_HEIGHT)-100.0f, L"abcdefg");
			FRAMEWORK.font->Draw(500, 50, L"あいうえお");
			FRAMEWORK.font->Draw(500, 100, L"カキクケコ");
			FRAMEWORK.font->Draw(500, 150, L"漢字最高！");
			FRAMEWORK.font->Draw(500, 200, L"改行が\nできるよ");
			FRAMEWORK.font->Draw(500, 300, L"色々　改造してね。");
			FRAMEWORK.font->End(FRAMEWORK.context.Get());
		}

	}

	FRAMEWORK.fedo_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));

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
	fado_alpha += FEDO_MIX(elapsed_time);

	if (fado_alpha > 1.0f)
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