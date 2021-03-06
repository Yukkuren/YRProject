#include "Scene.h"
#include "framework.h"
#include "YRSound.h"

//-------------------------------------------------------------
// **シーン概要**
//・暗転からスタート。画像をロードしたら画面表示
//・1p,2p両方が選択を終えてスタートボタンを押したら
//　暗転してSceneLoadに遷移
//・キャラが一種類の状態では使用しない
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

	//将来的に消す
	select_p1 = scastI(PLSELECT::KNIGHT);
	select_p2 = scastI(PLSELECT::KNIGHT);
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
	GetSound().BGMPlay(BGMKind::CHARA_SELECT);
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
	spriteShader.reset();
	spriteShader = nullptr;
}

void SceneSelect::Update(float elapsed_time)
{
	//ロード終了
	if (load_fin)
	{
		if (pKeyState.nflg == 1 || FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::START)] == 1)
		{
			select_p1 = scastI(PLSELECT::KNIGHT);
			select_p2 = scastI(PLSELECT::KNIGHT);
			//フェードアウトが終わったらロード画面へ
			GetSound().BGMStop(BGMKind::CHARA_SELECT);
			FRAMEWORK.SetScene(SCENE_TABLE::SCENE_LOAD);
			UnInit();
			return;
		}
#ifdef EXIST_IMGUI

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
		//constexpr float a = 1.0f / 60.0f;
		FRAMEWORK.scenegame.pad1->Update(elapsed_time);
		FRAMEWORK.scenegame.pad2->Update(elapsed_time);

		//プレイヤー1のカーソル移動処理
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

		//プレイヤー2のカーソル移動処理
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

		//両方のプレイヤーが決定したら
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
			//フェードアウトをスタートさせる
			fado_start = true;
		}
		if (fado_start)
		{
			if (FadoOut(elapsed_time))
			{
				//フェードアウトが終わったらロード画面へ
				GetSound().BGMStop(BGMKind::CHARA_SELECT);
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_LOAD);
				//FRAMEWORK.SetScene(SCENE_TITLE);
				UnInit();
				return;
			}
		}
		else
		{
			//フェードアウトがスタートしてない場合は画面を映す
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
	//画像はロードが終わるまで描画しない
	if (load_fin)
	{
		//テスト描画なので後で削除
		//背景
		back_img->DrawRotaGraph(
			spriteShader.get(),
			FRAMEWORK.SCREEN_WIDTH / 2.0f,
			FRAMEWORK.SCREEN_HEIGHT / 2.0f,
			0.0f,
			1.0f);
		//アイコン描画
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
	}
	//フェード用画像
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