#include "Scene.h"
#include "../framework.h"
#include "../YRSound.h"
#include "../Chara/PlayerBase/Player_name_list.h"
#include <codecvt>

// w_string型変更用関数
using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter_Icon;

std::string to_string_Icon(std::wstring wstr)
{
	return strconverter_Icon.to_bytes(wstr);
}

std::wstring to_wstring_Icon(std::string str)
{
	return strconverter_Icon.from_bytes(str);
}

//-------------------------------------------------------------
// **シーン概要**
//・暗転からスタート。画像をロードしたら画面表示
//・1p,2p両方が選択を終えてスタートボタンを押したら
//　暗転してSceneLoadに遷移
//・キャラが一種類の状態では使用しない
//-------------------------------------------------------------


void SceneSelect::Init()
{
	//各数値はImGuiにて設定済み

	load_fin = false;
	load_state = 0;
	knight_icon_pos = { 630.0f,360.0f };
	p1_cut_pos = { 480.0f,785.0f };
	p2_cut_pos = { 1500.0f,785.0f };
	name_distance = { 0.0f,175.0f };
	Rato = 1.5f;
	case_rato = 0.4f;
	cursor_rato = 0.4f;
	cut_rato = 1.7f;
	icon_range = 630.0f;
	name_rato = 1.0f;
	sx = 135.0f;
	sw = 310.0f;

	p1_chara_alpha = 0.0f;
	p2_chara_alpha = 0.0f;

	FRAMEWORK.scenegame.pad1->Init();
	FRAMEWORK.scenegame.pad2->Init();
	fado_start = false;
	fado_alpha = 1.0f;
	image_alpha = 0.0f;

	//初期値として-1を入れておく
	select_p1 = -1;
	select_p2 = -1;

	p1Enter = false;
	p2Enter = false;
	end = false;
	timer = 0.0f;

	cursor_speed = 15.0f;

	//タイトルシェーダー用パラメータ
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

	for (int i = 0; i < select_p.size(); i++)
	{
		select_p[i].pos = YR_Vector3(knight_icon_pos.x + (static_cast<float>(i) * icon_range), knight_icon_pos.y);
		select_p[i].select = Select_P::NONE;
	}

	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}

	//初期位置
	p1 = select_p[scastI(PLSELECT::KNIGHT)].pos;
	p2 = select_p[scastI(PLSELECT::NERU)].pos;

	color_p1 = PLCOLOR::ORIGINAL;
	color_p2 = PLCOLOR::ORIGINAL;

	old_select_p1 = select_p1;
	old_select_p2 = select_p2;

	old_color_p1 = color_p1;
	old_color_p2 = color_p2;

	ready_pos = { -(FRAMEWORK.SCREEN_WIDTH * 0.5f),(FRAMEWORK.SCREEN_HEIGHT * 0.5f) };

	ready_rato = 1.0f;

	line_red = { 0.0f,490.0f };
	line_blue = { 0.0f,605.0f };

	line_Multiply = 5000.0f;
	ready_Multiply = 4000.0f;

	line_breadth = 20.0f;
}

void SceneSelect::LoadData()
{
	//if (back_img == nullptr)
	//{
	//	back_img = std::make_unique<Sprite>(L"./Data/Image/BG/select.png", 1920.0f, 1080.0f);
	//	//back_img->LoadGraph(1920.0f, 1080.0f);
	//}

	//サンプラー生成
	if (sampler_clamp == nullptr)
	{
		sampler_clamp = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
	}
	if (sampler_wrap == nullptr)
	{
		sampler_wrap = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	//Gbuffer用スプライト
	if (sprite == nullptr)
	{
		sprite = std::make_unique<Sprite>();
	}

	//画像読み込み
	IconLoad();

	//NONEは使用しないが万が一アクセスした場合のことを考えて1Pの画像を読み込ませている
	if (select_img[scastI(Select_P::NONE)] == nullptr)
	{
		select_img[scastI(Select_P::NONE)] = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/select1P.png", 1280.0f, 1024.0f);
	}

	if (select_img[scastI(Select_P::P1)] == nullptr)
	{
		select_img[scastI(Select_P::P1)] = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/select1P.png", 1280.0f, 1024.0f);
	}

	if (select_img[scastI(Select_P::P2)] == nullptr)
	{
		select_img[scastI(Select_P::P2)] = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/select2P.png", 1280.0f, 1024.0f);
	}

	if (select_img[scastI(Select_P::ALL)] == nullptr)
	{
		select_img[scastI(Select_P::ALL)] = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/selectall.png", 1280.0f, 1024.0f);
	}

	if (chara_case == nullptr)
	{
		chara_case = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/chara_case.png", 1280.0f, 1024.0f);
	}

	if (select_point == nullptr)
	{
		select_point = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/select_point.png", 640.0f, 640.0f);
	}

	if (ready_to_start == nullptr)
	{
		ready_to_start = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/Ready_to_Start.png", 1280.0f, 360.0f);
	}

	if (white_box == nullptr)
	{
		white_box = std::make_unique<Sprite>(L"./Data/Shader/decoi.png", 1024.0f, 1024.0f);
	}

	if (Box_sprite == nullptr)
	{
		Box_sprite = std::make_unique<Sprite>(L"./Data/Image/UI/GameLoad/Box.png", 640.0f, 640.0f);
	}

	//コンスタントバッファ作成
	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(Title_CBuffer));

	//シェーダー読み込み
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

	//テクスチャロード
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

	//タイトルシェーダーに送る画像(ノイズ画像を読み込んでいる)
	if (select_texture == nullptr)
	{
		select_texture = std::make_unique<Texture>(L"./Data/Shader/noise.png");
	}

	load_state = 3;
}


void SceneSelect::UnInit()
{
	/*knight_icon.reset();
	knight_icon = nullptr;
	ken_icon.reset();
	ken_icon = nullptr;*/

	for (int i = 0; i < select_p.size(); i++)
	{
		select_p[i].name_image.reset();
		select_p[i].name_image = nullptr;
		select_p[i].edge_image.reset();
		select_p[i].edge_image = nullptr;
		for (int n = 0; n < select_p[i].icon_image.size(); n++)
		{
			select_p[i].icon_image[n].reset();
			select_p[i].icon_image[n] = nullptr;
		}
	}

	for (int i = 0; i < select_img.size(); i++)
	{
		select_img[i].reset();
		select_img[i] = nullptr;
	}

	chara_case.reset();
	chara_case = nullptr;
	select_point.reset();
	select_point = nullptr;

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

	Box_sprite.reset();
	Box_sprite = nullptr;
}

void SceneSelect::Update(float elapsed_time)
{
	//ロード終了
	if (load_fin)
	{
		//if (pKeyState.nflg == 1 || FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::START)] == 1)
		//{
		//	select_p1 = scastI(PLSELECT::KNIGHT);
		//	select_p2 = scastI(PLSELECT::KNIGHT);
		//	//フェードアウトが終わったらロード画面へ
		//	GetSound().BGMStop(BGMKind::CHARA_SELECT);
		//	FRAMEWORK.SetScene(SCENE_TABLE::SCENE_LOAD);
		//	UnInit();
		//	return;
		//}

		//アルファ値増加
		if (image_alpha < 1.0f)
		{
			image_alpha += elapsed_time;
		}
		else
		{
			image_alpha = 1.0f;
		}

		//アルファ値減少
		if (p1_chara_alpha > 0.0f)
		{
			p1_chara_alpha -= elapsed_time;
		}
		else
		{
			p1_chara_alpha = 0.0f;
		}

		if (p2_chara_alpha > 0.0f)
		{
			p2_chara_alpha -= elapsed_time;
		}
		else
		{
			p2_chara_alpha = 0.0f;
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

		//プレイヤーのカーソル移動処理
		PosSet();

		//カーソルの判定処理
		SelectCheck();

		//カラーを調整する
		ColorChange();

		//カラー被りを無くす
		ColorAdjustment();

		//このフレームのキャラ情報を保存
		old_select_p1 = select_p1;
		old_select_p2 = select_p2;

		//このフレームのカラー情報を保存
		old_color_p1 = color_p1;
		old_color_p2 = color_p2;

		/*p1 = PosSet(select_p1);
		p2 = PosSet(select_p2);*/

		ReadyStep(elapsed_time);

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
				GetSound().BGMPlay(BGMKind::CHARA_SELECT);
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
		ImGui::SliderFloat(u8"画像のサイズ", &Rato, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"キャラケースの大きさ", &case_rato, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"カーソルの大きさ", &cursor_rato, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"選択キャラ大きさ", &cut_rato, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"名前の大きさ", &name_rato, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"Readyの大きさ", &ready_rato, 0.0f, 10.0f);
		ImGui::Text(u8"------------------------------------------------");
		ImGui::SliderFloat(u8"画像の位置X", &knight_icon_pos.x, 0.0f, (float)FRAMEWORK.SCREEN_WIDTH);
		ImGui::SliderFloat(u8"画像の位置Y", &knight_icon_pos.y, 0.0f, (float)FRAMEWORK.SCREEN_HEIGHT);
		ImGui::SliderFloat(u8"画像の距離", &icon_range, 0.0f, 800.0f);
		ImGui::SliderFloat(u8"1P選択キャラの位置X", &p1_cut_pos.x, 0.0f, (float)FRAMEWORK.SCREEN_WIDTH);
		ImGui::SliderFloat(u8"2P選択キャラの位置X", &p2_cut_pos.x, 0.0f, (float)FRAMEWORK.SCREEN_WIDTH);
		ImGui::SliderFloat(u8"選択キャラの位置Y", &p1_cut_pos.y, 0.0f, (float)FRAMEWORK.SCREEN_HEIGHT);
		ImGui::SliderFloat(u8"名前の距離X", &name_distance.x, -500.0f, 500.0f);
		ImGui::SliderFloat(u8"名前の距離Y", &name_distance.y, -500.0f, 500.0f);
		ImGui::SliderFloat(u8"Readyの位置X", &ready_pos.x, -(float)FRAMEWORK.SCREEN_WIDTH, (float)FRAMEWORK.SCREEN_WIDTH);
		ImGui::SliderFloat(u8"Readyの位置Y", &ready_pos.y, -(float)FRAMEWORK.SCREEN_HEIGHT, (float)FRAMEWORK.SCREEN_HEIGHT);
		p2_cut_pos.y = p1_cut_pos.y;
		ImGui::SliderFloat("sx", &sx, 0.0f, 640.0f);
		ImGui::SliderFloat("sw", &sw, 0.0f, 640.0f);
		ImGui::SliderFloat(u8"カーソルの速度", &cursor_speed, 0.0f, 50.0f);
		ImGui::Text(u8"------------------------------------------------");
		ImGui::SliderFloat(u8"ラインの右端座標X", &line_red.x, 0.0f, (float)FRAMEWORK.SCREEN_WIDTH);
		ImGui::SliderFloat(u8"ラインの立幅", &line_breadth, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"赤ラインの座標Y", &line_red.y, 0.0f, (float)FRAMEWORK.SCREEN_HEIGHT);
		ImGui::SliderFloat(u8"青ラインの座標Y", &line_blue.y, 0.0f, (float)FRAMEWORK.SCREEN_HEIGHT);
		ImGui::SliderFloat(u8"ラインの移動速度に乗算する値", &line_Multiply, 0.0f, 10000.0f);
		ImGui::SliderFloat(u8"Readyの移動速度に乗算する値", &ready_Multiply, 0.0f, 10000.0f);
		ImGui::Text(u8"------------------------------------------------");
		for (int i = 0; i < select_p.size(); i++)
		{
			select_p[i].pos = YR_Vector3(knight_icon_pos.x + (static_cast<float>(i) * icon_range), knight_icon_pos.y);
		}
	}
#endif
	//画像はロードが終わるまで描画しない
	if (load_fin)
	{
		SetRenderTexture();

		//テスト描画なので後で削除
		//背景
		/*back_img->DrawRotaGraph(
			spriteShader.get(),
			FRAMEWORK.SCREEN_WIDTH / 2.0f,
			FRAMEWORK.SCREEN_HEIGHT / 2.0f,
			0.0f,
			1.0f);*/

		//背景スプライト描画
		cbuffer_param.iTime = timer;

		sprite->render(
			selectShader.get(),
			select_texture.get(),
			cbuffer_param,
			sampler_wrap.get(),
			constantBuffer,
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

		//アイコン描画
		/*knight_icon->DrawRotaGraph(
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
		);*/


		for (int i = 0; i < select_p.size(); i++)
		{

			select_p[i].icon_image[scastI(PLCOLOR::ORIGINAL)]->DrawRotaSetGraph(
				spriteShader.get(),
				select_p[i].pos.x,
				select_p[i].pos.y,
				sx,
				sw,
				0.0f,
				Rato,
				false,
				SpriteMask::NONE,
				DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
			);

			//選択状態を描画
			DrawSelect(i);

			chara_case->DrawRotaGraph(
				spriteShader.get(),
				select_p[i].pos.x,
				select_p[i].pos.y,
				0.0f,
				case_rato,
				false,
				SpriteMask::NONE,
				DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
			);
		}

		//選択されたキャラの描画
		DrawChara();

		//カーソルの描画

		//1P
		if (!p1Enter)
		{
			select_point->DrawRotaGraph(
				spriteShader.get(),
				p1.x,
				p1.y + (sinf(timer * 10.0f) * 5.0f),
				0.0f,
				cursor_rato,
				false,
				SpriteMask::NONE,
				DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, image_alpha)
			);
		}

		//2P
		if (!p2Enter)
		{
			select_point->DrawRotaGraph(
				spriteShader.get(),
				p2.x,
				p2.y + (sinf(timer * 10.0f) * 5.0f),
				0.0f,
				cursor_rato,
				false,
				SpriteMask::NONE,
				DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, image_alpha)
			);
		}


		if (p1Enter && p2Enter)
		{
			//両方決定していた場合「Ready to Start」を表示する
			DrawReady();
		}



		//選択画像描画
		//1P
		//if (p1Enter)
		//{
		//	select_img->DrawRotaGraph
		//	(
		//		spriteShader.get(),
		//		p1.x,
		//		p1.y,
		//		0.0f,
		//		Rato,
		//		false,
		//		SpriteMask::NONE,
		//		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
		//	);
		//}
		//else
		//{
		//	if (static_cast<int>(timer * 10.0f) % 5 > 0)
		//	{
		//		select_img->DrawRotaGraph
		//		(
		//			spriteShader.get(),
		//			p1.x,
		//			p1.y,
		//			0.0f,
		//			Rato,
		//			false,
		//			SpriteMask::NONE,
		//			DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
		//		);
		//	}
		//}

		////2P
		//if (p2Enter)
		//{
		//	select_img->DrawRotaGraph
		//	(
		//		spriteShader.get(),
		//		p2.x,
		//		p2.y,
		//		0.0f,
		//		Rato,
		//		false,
		//		SpriteMask::NONE,
		//		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
		//	);
		//}
		//else
		//{
		//	if (static_cast<int>(timer * 10.0f) % 5 > 0)
		//	{
		//		select_img->DrawRotaGraph
		//		(
		//			spriteShader.get(),
		//			p2.x,
		//			p2.y,
		//			0.0f,
		//			Rato,
		//			false,
		//			SpriteMask::NONE,
		//			DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
		//		);
		//	}
		//}

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
	//フェード用画像
	FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, false, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));
}



void SceneSelect::DrawSelect(int num)
{
	switch (select_p[num].select)
	{
	case SceneSelect::Select_P::NONE:
		//何も描画しない
		break;
	case SceneSelect::Select_P::P1:
		select_img[scastI(Select_P::P1)]->DrawRotaGraph(
			spriteShader.get(),
			select_p[num].pos.x,
			select_p[num].pos.y,
			0.0f,
			case_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);
		break;
	case SceneSelect::Select_P::P2:
		select_img[scastI(Select_P::P2)]->DrawRotaGraph(
			spriteShader.get(),
			select_p[num].pos.x,
			select_p[num].pos.y,
			0.0f,
			case_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);
		break;
	case SceneSelect::Select_P::ALL:
		select_img[scastI(Select_P::ALL)]->DrawRotaGraph(
			spriteShader.get(),
			select_p[num].pos.x,
			select_p[num].pos.y,
			0.0f,
			case_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);
		break;
	case SceneSelect::Select_P::P_END:
		//何も描画しない
		break;
	default:
		break;
	}
}




void SceneSelect::PosSet()
{
	//プレイヤー1のカーソル移動処理
	if (p1Enter)
	{
		//キャンセル
		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::B)] == 1)
		{
			p1Enter = false;
			p1_chara_alpha = 0.0f;
			GetSound().SESinglePlay(SEKind::SELECT_CANCEL);
		}
	}
	else
	{
		//決定していない場合のみ移動する
		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_R)] > 0)
		{
			p1.x += cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_L)] > 0)
		{
			p1.x -= cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_U)] > 0)
		{
			p1.y -= cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_D)] > 0)
		{
			p1.y += cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_LDown)] > 0)
		{
			p1.y += cursor_speed;
			p1.x -= cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::STICK_RDown)] > 0)
		{
			p1.y += cursor_speed;
			p1.x += cursor_speed;
		}

		if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::A)] == 1)
		{
			if (select_p1 != -1)
			{
				//キャラが選択されたときのみ
				p1Enter = true;
				GetSound().SESinglePlay(SEKind::OFFSET);
				p1_chara_alpha = 1.0f;
			}
			//PlaySE(SE_ENTER);
		}
	}


	//プレイヤー2のカーソル移動処理
	if (p2Enter)
	{
		//キャンセル
		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::B)] == 1)
		{
			p2Enter = false;
			GetSound().SESinglePlay(SEKind::SELECT_CANCEL);
			p2_chara_alpha = 0.0f;
		}
	}
	else
	{
		//決定していない場合のみ移動する
		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_R)] > 0)
		{
			p2.x += cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_L)] > 0)
		{
			p2.x -= cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_U)] > 0)
		{
			p2.y -= cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_D)] > 0)
		{
			p2.y += cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_LDown)] > 0)
		{
			p2.y += cursor_speed;
			p2.x -= cursor_speed;
		}
		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::STICK_RDown)] > 0)
		{
			p2.y += cursor_speed;
			p2.x += cursor_speed;
		}

		if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::A)] == 1)
		{
			if (select_p2 != -1)
			{
				//キャラが選択されたときのみ
				p2Enter = true;
				GetSound().SESinglePlay(SEKind::OFFSET);
				p2_chara_alpha = 1.0f;
				//PlaySE(SE_ENTER);
			}
		}
	}

	//移動制限
	const float margin = 100.0f;

	if (p1.x < margin)
	{
		p1.x = margin;
	}
	if (p2.x < margin)
	{
		p2.x = margin;
	}

	if (p1.x > (float)FRAMEWORK.SCREEN_WIDTH - margin)
	{
		p1.x = (float)FRAMEWORK.SCREEN_WIDTH - margin;
	}
	if (p2.x > (float)FRAMEWORK.SCREEN_WIDTH - margin)
	{
		p2.x = (float)FRAMEWORK.SCREEN_WIDTH - margin;
	}

	if (p1.y < margin)
	{
		p1.y = margin;
	}
	if (p2.y < margin)
	{
		p2.y = margin;
	}

	if (p1.y > (float)FRAMEWORK.SCREEN_HEIGHT - margin)
	{
		p1.y = (float)FRAMEWORK.SCREEN_HEIGHT - margin;
	}
	if (p2.y > (float)FRAMEWORK.SCREEN_HEIGHT - margin)
	{
		p2.y = (float)FRAMEWORK.SCREEN_HEIGHT - margin;
	}


	//カラー変更
	if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::LB)] == 1)
	{
		Color_Sub(color_p1);
	}

	if (FRAMEWORK.scenegame.pad1->x_input[scastI(PAD::RB)] == 1)
	{
		Color_Add(color_p1);
	}

	if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::LB)] == 1)
	{
		Color_Sub(color_p2);
	}

	if (FRAMEWORK.scenegame.pad2->x_input[scastI(PAD::RB)] == 1)
	{
		Color_Add(color_p2);
	}
}



void SceneSelect::SelectCheck()
{
	//各プレイヤーがどのキャラを選択しているかを判定する

	//処理前に選択状態を初期化する
	select_p1 = -1;
	select_p2 = -1;

	for (int i = 0; i < select_p.size(); i++)
	{
		Select_P p = Select_P::NONE;

		if (p1.x >= (select_p[i].pos.x - ((sw * 0.5f) * Rato)) && p1.x <= (select_p[i].pos.x + ((sw * 0.5f) * Rato)))
		{
			if (p1.y >= (select_p[i].pos.y - ((select_p[i].icon_image[scastI(PLCOLOR::ORIGINAL)]->sh * 0.5f) * Rato)) && p1.y <= (select_p[i].pos.y + ((select_p[i].icon_image[scastI(PLCOLOR::ORIGINAL)]->sh * 0.5f) * Rato)))
			{
				//1Pが選択している
				p = Select_P::P1;
				//1Pの選択情報を保存する
				select_p1 = i;
			}
		}

		if (p2.x >= (select_p[i].pos.x - ((sw * 0.5f) * Rato)) && p2.x <= (select_p[i].pos.x + ((sw * 0.5f) * Rato)))
		{
			if (p2.y >= (select_p[i].pos.y - ((select_p[i].icon_image[scastI(PLCOLOR::ORIGINAL)]->sh * 0.5f) * Rato)) && p2.y <= (select_p[i].pos.y + ((select_p[i].icon_image[scastI(PLCOLOR::ORIGINAL)]->sh * 0.5f) * Rato)))
			{
				//2Pの選択情報を保存する
				select_p2 = i;

				if (p == Select_P::P1)
				{
					//両方選択している
					p = Select_P::ALL;
				}
				else
				{
					//2Pが選択している
					p = Select_P::P2;
				}
			}
		}

		select_p[i].select = p;
	}
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

	//画面クリア
	FRAMEWORK.framebuffer.Clear();
	FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//ビュー更新
	YRCamera.Active();

	//ビューポート設定
	//レンダーターゲットビューの設定
	FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f, dsv);

	//ブレンドステート設定
	FRAMEWORK.BlendSet(Blend::ALPHA);
	//ラスタライザー設定
	FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
	//デプスステンシルステート設定
	FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

	//サンプラー設定
	sampler_clamp->Set(0);
}


void SceneSelect::NullSetRenderTexture()
{
	//レンダーターゲットの回復
	//FRAMEWORK.context.Get()->OMSetRenderTargets(testrtv.size(), testrtv.data(), FRAMEWORK.depth.Get());
	FRAMEWORK.framebuffer.ResetRenderTexture();
	FRAMEWORK.framebuffer.SetDefaultRTV();
}


void SceneSelect::RenderTexture()
{
	//Gbuffer描画
	sprite->render(
		spriteEx.get(),
		color_texture.get(),
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);
}


//アイコン画像読み込み
void SceneSelect::IconLoad()
{
	std::string front = std::string("./Data/Image/Character/");
	std::string back = std::string("_cut0.png");
	std::string color2 = std::string("_cut1.png");
	std::string back_name = std::string("_name.png");
	std::string back_edge = std::string("_name_edge.png");

	for (int icon_num = 0; icon_num < select_p.size(); icon_num++)
	{
		std::string contents = front + GetName().chara_name_list[icon_num] +
			std::string("/") + GetName().chara_name_list[icon_num] + back;
		std::wstring icon_name = to_wstring_Icon(contents);

		std::string contents_color2 = front + GetName().chara_name_list[icon_num] +
			std::string("/") + GetName().chara_name_list[icon_num] + color2;
		std::wstring color2_name = to_wstring_Icon(contents_color2);

		std::string contents_name = front + GetName().chara_name_list[icon_num] +
			std::string("/") + GetName().chara_name_list[icon_num] + back_name;
		std::wstring name_name = to_wstring_Icon(contents_name);

		std::string contents_edge = front + GetName().chara_name_list[icon_num] +
			std::string("/") + GetName().chara_name_list[icon_num] + back_edge;
		std::wstring edge_name = to_wstring_Icon(contents_edge);

		if (select_p[icon_num].icon_image[scastI(PLCOLOR::ORIGINAL)] == nullptr)
		{
			select_p[icon_num].icon_image[scastI(PLCOLOR::ORIGINAL)] = std::make_unique<Sprite>(icon_name.data(), 640.0f, 192.0f);
		}

		if (select_p[icon_num].icon_image[scastI(PLCOLOR::COLOR_2)] == nullptr)
		{
			select_p[icon_num].icon_image[scastI(PLCOLOR::COLOR_2)] = std::make_unique<Sprite>(color2_name.data(), 640.0f, 192.0f);
		}

		if (select_p[icon_num].name_image == nullptr)
		{
			select_p[icon_num].name_image = std::make_unique<Sprite>(name_name.data(), 640.0f, 320.0f);
		}

		if (select_p[icon_num].edge_image == nullptr)
		{
			select_p[icon_num].edge_image = std::make_unique<Sprite>(edge_name.data(), 640.0f, 320.0f);
		}
	}
}


//キャラを選択した時にカットイン画像を下に描画する
void SceneSelect::DrawChara()
{
	if (select_p1 != -1)
	{
		//キャラアイコン
		select_p[select_p1].icon_image[scastI(color_p1)]->DrawRotaGraph(
			spriteShader.get(),
			p1_cut_pos.x,
			p1_cut_pos.y,
			0.0f,
			cut_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);

		//名前画像(縁)
		select_p[select_p1].edge_image->DrawRotaGraph(
			spriteShader.get(),
			p1_cut_pos.x + name_distance.x,
			p1_cut_pos.y + name_distance.y,
			0.0f,
			name_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);

		//名前画像(中)
		select_p[select_p1].name_image->DrawRotaGraph(
			spriteShader.get(),
			p1_cut_pos.x + name_distance.x,
			p1_cut_pos.y + name_distance.y,
			0.0f,
			name_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, image_alpha)
		);


		//マスク値設定
		select_p[select_p1].icon_image[scastI(color_p1)]->DrawRotaGraph(
			spriteShader.get(),
			p1_cut_pos.x,
			p1_cut_pos.y,
			0.0f,
			cut_rato,
			false,
			SpriteMask::WRITE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);

		select_p[select_p1].edge_image->DrawRotaGraph(
			spriteShader.get(),
			p1_cut_pos.x + name_distance.x,
			p1_cut_pos.y + name_distance.y,
			0.0f,
			name_rato,
			false,
			SpriteMask::WRITE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);


		//マスク内に白画像描画
		white_box->DrawRotaGraph(
			spriteShader.get(),
			p1_cut_pos.x,
			p1_cut_pos.y,
			0.0f,
			0.8f,
			false,
			SpriteMask::INDRAW,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, p1_chara_alpha)
		);
	}

	if (select_p2 != -1)
	{
		//キャラアイコン
		select_p[select_p2].icon_image[scastI(color_p2)]->DrawRotaGraph(
			spriteShader.get(),
			p2_cut_pos.x,
			p2_cut_pos.y,
			0.0f,
			cut_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);

		//名前画像(縁)
		select_p[select_p2].edge_image->DrawRotaGraph(
			spriteShader.get(),
			p2_cut_pos.x + name_distance.x,
			p2_cut_pos.y + name_distance.y,
			0.0f,
			name_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);

		//名前画像(中)
		select_p[select_p2].name_image->DrawRotaGraph(
			spriteShader.get(),
			p2_cut_pos.x + name_distance.x,
			p2_cut_pos.y + name_distance.y,
			0.0f,
			name_rato,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, image_alpha)
		);

		//マスク値書き込み
		select_p[select_p2].icon_image[scastI(color_p2)]->DrawRotaGraph(
			spriteShader.get(),
			p2_cut_pos.x,
			p2_cut_pos.y,
			0.0f,
			cut_rato,
			false,
			SpriteMask::WRITE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);

		select_p[select_p2].edge_image->DrawRotaGraph(
			spriteShader.get(),
			p2_cut_pos.x + name_distance.x,
			p2_cut_pos.y + name_distance.y,
			0.0f,
			name_rato,
			false,
			SpriteMask::WRITE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
		);

		//マスク内に白画像描画
		white_box->DrawRotaGraph(
			spriteShader.get(),
			p2_cut_pos.x,
			p2_cut_pos.y,
			0.0f,
			0.8f,
			false,
			SpriteMask::INDRAW,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, p2_chara_alpha)
		);
	}
}



void SceneSelect::Color_Add(PLCOLOR& color)
{
	//カラー番号を増やす(最大数より増えたらオリジナルカラーに戻す)
	int color_int = scastI(color) + 1;

	if (color_int >= scastI(PLCOLOR::COLOR_END))
	{
		color_int = scastI(PLCOLOR::ORIGINAL);
	}

	color = static_cast<PLCOLOR>(color_int);
}

void SceneSelect::Color_Sub(PLCOLOR& color)
{
	//カラー番号を減らす(オリジナルより減ったら一番後ろに戻す)
	int color_int = scastI(color) - 1;

	if (color_int < scastI(PLCOLOR::ORIGINAL))
	{
		color_int = scastI(PLCOLOR::COLOR_END) - 1;
	}

	color = static_cast<PLCOLOR>(color_int);
}



void SceneSelect::ColorChange()
{
	//前フレームと選択されたキャラが違う場合、カラーをオリジナルに戻す
	if (old_select_p1 != select_p1)
	{
		color_p1 = PLCOLOR::ORIGINAL;
	}

	if (old_select_p2 != select_p2)
	{
		color_p2 = PLCOLOR::ORIGINAL;
	}
}


void SceneSelect::ColorAdjustment()
{
	//同じキャラ、同じカラーを選択していた場合、後から選択したほうのカラーを強制的に変える
	if (select_p1 == select_p2)
	{
		if (color_p1 == color_p2)
		{
			if (old_select_p1 == old_select_p2)
			{
				//キャラを選択している状態からカラーを変更する場合
				if (color_p1 == old_color_p1)
				{
					//1Pが既にそのカラーを選択していた
					Color_Add(color_p2);
				}
				else if (color_p2 == old_color_p2)
				{
					//2Pが既にそのカラーを選択していた
					Color_Add(color_p1);
				}
				else
				{
					//両方同時にカラーを変更した場合
					Color_Add(color_p2);
				}
			}
			else
			{
				//キャラを選択していない場合
				if (select_p1 == old_select_p1)
				{
					//1Pが先に選択していた
					Color_Add(color_p2);
				}
				else if (select_p2 == old_select_p2)
				{
					//2Pが先に選択していた
					Color_Add(color_p1);
				}
				else
				{
					//両方同じタイミングなら
					Color_Add(color_p2);
				}
			}
		}
	}
}


//Ready表示の動き
void SceneSelect::ReadyStep(float elapsed_time)
{
	if (p1Enter && p2Enter)
	{
		//両方決定した場合横移動させる
		line_red.x += line_Multiply * elapsed_time;
		ready_pos.x += ready_Multiply * elapsed_time;

		//上限設定
		if (line_red.x > (float)FRAMEWORK.SCREEN_WIDTH)
		{
			line_red.x = (float)FRAMEWORK.SCREEN_WIDTH;
		}
		if (ready_pos.x > (float)FRAMEWORK.SCREEN_WIDTH * 0.5f)
		{
			ready_pos.x = (float)FRAMEWORK.SCREEN_WIDTH * 0.5f;
		}
	}
	else
	{
		line_red.x = 0.0f;
		ready_pos.x = 0.0f;
	}

	//ラインのX座標は同期させる
	line_blue.x = line_red.x;
}

//Ready表示
void SceneSelect::DrawReady()
{
	//赤ラインの描画
	Box_sprite->DrawExtendGraph(
		spriteShader.get(),
		0.0f,
		line_red.y - line_breadth,
		line_red.x,
		line_red.y + line_breadth,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
	);

	//青ラインの描画
	Box_sprite->DrawExtendGraph(
		spriteShader.get(),
		0.0f,
		line_blue.y - line_breadth,
		line_blue.x,
		line_blue.y + line_breadth,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
	);

	//Readyの描画
	ready_to_start->DrawRotaGraph(
		spriteShader.get(),
		ready_pos.x,
		ready_pos.y,
		0.0f,
		ready_rato
	);
}
