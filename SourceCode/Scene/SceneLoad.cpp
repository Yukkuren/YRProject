#include "Scene.h"
#include "../Game/HitCheak.h"
#include "../framework.h"
#include "../YRSound.h"
#include <math.h>
#include "../Chara/PlayerBase/Player_name_list.h"
#include <codecvt>

// w_string型変更用関数
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
	u8"プレイヤー1のみ",
	u8"プレイヤー2までの間",
	u8"プレイヤー2のみ",
	u8"同時までの間",
	u8"同時",
	u8"最後",
};

#endif // EXIST_IMGUI
//-------------------------------------------------------------
// **シーン概要**
//・このシーンではSceneGameのロードを行う
//・SceneSelectから送られてきた情報をもとにデータの読み込みを行い、
//	読み込みがすべて完了したらSceneGameをセットする
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

	//シェーダー作成
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

	//Gbuffer用スプライト
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
	//「ロード画面で表示する画像等」をロードする
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

	//コンスタントバッファ作成
	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(Title_CBuffer));

	load_state = 2;
}

void SceneLoad::UnInit()
{
	//「ロード画面で表示する画像等」のみ解放する
	load_img.reset();
	load_img = nullptr;

	//シェーダーも解放
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
	//タイマーは常に更新する
	timer += elapsedTime;
	GetSound().FadeIn(elapsedTime);
	//「ロード画面で表示する画像等」がロードできた
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

	//「ゲームメイン画面で使用するモデル等」がロードできた
	if (Game_load_fin)
	{
		//ロード画面を挟むため
		if (timer > 1.0f)
		{
			fado_start = true;
		}
		if (fado_start)
		{
			if (FedoOut(elapsedTime))
			{
				//フェードアウトが終わったらゲームメインへ遷移
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
		//		*概要*
		//・「ロード画面で表示する画像等」と「ゲームメイン画面で使用するモデル等」の2つをロードするので
		//	スレッドを2回立てて順番にロードしている
		//・load_fin：「ロード画面で表示する画像等」がロードできた
		//・Game_load_fin：「ゲームメイン画面で使用するモデル等」がロードできた
		//-----------------------------------------------------------------------------------------
		switch (load_state)
		{
		case 0:
			//まずはロード画面で表示する画像等」のロード
			load_state = 1;
			t = new std::thread(&SceneLoad::LoadData, this);
			break;
		case 1:
			//ロードモデルロード中
			break;
		case 2:
			//ロード終了後に何か処理を入れるときのためにあけている
			load_state = 3;
			break;
		case 3:
			//一度スレッドを開放する
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
			//意図的に1フレームずらしている
			load_state = 5;
			break;
		case 5:
			//次に「ゲームメイン画面で使用するモデル等」のロード
			load_state = 6;
			t = new std::thread(&SceneGame::LoadData, &FRAMEWORK.scenegame);
			break;
		case 6:
			//ゲームモデルロード中
			break;
		case 7:
			//ロード終了後に何か処理を入れるときのためにあけている
			load_state = 8;
			break;
		case 8:
			//スレッド解放
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
	//「ロード画面で表示する画像等」がロードできた
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

		//背景の描画
		cbuffer_param.iTime = timer;

		sprite->render(
			titleShader.get(),
			title_texture.get(),
			cbuffer_param,
			sampler_wrap.get(),
			constantBuffer,
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

		//マッチ画面描画
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

	//フェード用画像描画
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
		ImGui::InputFloat(u8"Sinに乗算する値", &plus_match, 1.0f, 10.0f);
		ImGui::InputFloat(u8"最低限進む横の値", &slow_add, 1.0f, 10.0f);
		ImGui::InputFloat(u8"match_timerに乗算する値", &timer_Multiply, 1.0f, 10.0f);
		ImGui::InputFloat(u8"sinの最大値", &sin_max, 1.0f, 10.0f);
		ImGui::InputFloat(u8"間の長さ", &space_time, 0.1f, 1.0f);
		ImGui::InputFloat(u8"ラインに乗算する値", &line_Multiply, 10.0f, 100.0f);
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
		//プレイヤー1だけ表示
	{
		//タイマー加算
		match_timer += elapsed_time * timer_Multiply;

		//sin派を使って速度に緩急をつける
		float sin_match = 0.0f;
		sin_match = std::max(sinf(match_timer), sin_max);

		//速度を加算
		p1_pos_x += ((sin_match * plus_match) + slow_add);

		//ラインは画面外に行くまで加算
		if (line_1p_x < (float)FRAMEWORK.SCREEN_WIDTH)
		{
			line_1p_x += (line_Multiply * elapsed_time);
		}

		//プレイヤー1が所定の場所に到達したら次のステートへ
		if (p1_pos_x > (float)FRAMEWORK.SCREEN_WIDTH + ((float)FRAMEWORK.SCREEN_WIDTH * 0.6f))
		{
			intro_state = IntroState::SPACE1;
			match_timer = 0.0f;
		}
	}
	break;
	case SceneLoad::IntroState::SPACE1:
		//少し間を置く
		match_timer += elapsed_time;

		//一定の速度で画面外に
		p1_pos_x += (plus_match + slow_add);
		line_1p_x += (line_Multiply * elapsed_time);

		//一定時間経ったら次のステートへ
		if (match_timer > space_time)
		{
			match_timer = 0.0f;
			intro_state = IntroState::P2;
		}
		break;
	case SceneLoad::IntroState::P2:
		//プレイヤー2だけ表示
	{
		//タイマー加算
		match_timer += elapsed_time * timer_Multiply;

		//sin派を使って速度に緩急をつける
		float sin_match = 0.0f;
		sin_match = std::max(sinf(match_timer), sin_max);

		//速度を加算
		p2_pos_x -= ((sin_match * plus_match) + slow_add);

		//ラインは画面外に行くまで加算
		if (line_2p_x > 0.0f)
		{
			line_2p_x -= (line_Multiply * elapsed_time);
		}

		//プレイヤー2が所定の場所に到達したら次のステートへ
		if (p2_pos_x < 0.0f - ((float)FRAMEWORK.SCREEN_WIDTH * 0.6f))
		{
			intro_state = IntroState::SPACE2;
			match_timer = 0.0f;
		}
	}
	break;
	case SceneLoad::IntroState::SPACE2:
		//少し間を置く
		match_timer += elapsed_time;

		//一定の速度で画面外に
		p2_pos_x -= (plus_match + slow_add);
		line_2p_x -= (line_Multiply * elapsed_time);

		//光を少しずつ大きくさせながら表示させる
		if (flash_alpha < 1.0f)
		{
			flash_alpha += elapsed_time * 2.0f;
		}
		else
		{
			flash_alpha = 1.0f;
		}

		flash_size += elapsed_time * 10.0f;

		//一定時間経ったら次のステートへ
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
		//両方表示する
	{
		//タイマー加算
		match_timer += elapsed_time * timer_Multiply;

		//sin派を使って速度に緩急をつける
		float sin_match = 0.0f;
		sin_match = sinf(match_timer);

		//光を薄くしていく
		if (flash_alpha > 0.0f)
		{
			flash_alpha -= elapsed_time * 2.0f;
		}
		else
		{
			flash_alpha = 0.0f;
		}

		//sin派が0以下になったら次のステートへ
		if (sin_match < sin_max)
		{
			intro_state = IntroState::FIN;
			match_timer = 0.0f;
		}
		else
		{
			//速度を加算
			p1_pos_x += ((sin_match * plus_match) + slow_add);
			p2_pos_x -= ((sin_match * plus_match) + slow_add);

			//ラインは画面外に行くまで加算
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
		//少しだけ動かしていく
		p1_pos_x += slow_add;
		p2_pos_x -= slow_add;
		break;
	default:
		break;
	}
}

void SceneLoad::MatchDraw(float elapsed_time)
{
	//カットイメージ描画
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


	//名前縁画像描画
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


	//名前画像描画
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



//アイコン画像読み込み
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


	//プレイヤー1の選択キャラの画像参照用文字列作成
	std::string contents_p1_chara = front + GetName().chara_name_list[p1_chara] +
		std::string("/") + GetName().chara_name_list[p1_chara] + cut + std::to_string(p1_color) + png;
	std::wstring icon_name_p1_chara = to_wstring_Load(contents_p1_chara);

	//プレイヤー2の選択キャラの画像参照用文字列作成
	std::string contents_p2_chara = front + GetName().chara_name_list[p2_chara] +
		std::string("/") + GetName().chara_name_list[p2_chara] + cut + std::to_string(p2_color) + png;
	std::wstring icon_name_p2_chara = to_wstring_Load(contents_p2_chara);

	//プレイヤー1の選択キャラの名前画像参照用文字列作成
	std::string contents_p1_name = front + GetName().chara_name_list[p1_chara] +
		std::string("/") + GetName().chara_name_list[p1_chara] + back_name;
	std::wstring name_name_p1 = to_wstring_Load(contents_p1_name);

	//プレイヤー1の選択キャラの名前縁画像参照用文字列作成
	std::string contents_p1_edge = front + GetName().chara_name_list[p1_chara] +
		std::string("/") + GetName().chara_name_list[p1_chara] + back_edge;
	std::wstring edge_name_p1 = to_wstring_Load(contents_p1_edge);

	//プレイヤー2の選択キャラの名前画像参照用文字列作成
	std::string contents_p2_name = front + GetName().chara_name_list[p2_chara] +
		std::string("/") + GetName().chara_name_list[p2_chara] + back_name;
	std::wstring name_name_p2 = to_wstring_Load(contents_p2_name);

	//プレイヤー2の選択キャラの名前縁画像参照用文字列作成
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