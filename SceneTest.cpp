#include "Scene.h"
#include "framework.h"

void SceneTest::Init()
{
	timer = 0.0f;

	//シェーダー作成
	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}
	if (skinShader == nullptr)
	{
		skinShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SKIN);
		skinShader->Create("./Data/Shader/Skinned_VS.cso", "./Data/Shader/Skinned_PS.cso");
	}
	if (geoShader == nullptr)
	{
		geoShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::GEO);
		geoShader->Create("./Data/Shader/geometric_primitive_vs.cso", "./Data/Shader/geometric_primitive_ps.cso");
	}
	if (boardShader == nullptr)
	{
		boardShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::BOARD);
		boardShader->Create("./Data/Shader/board_vs.cso", "./Data/Shader/board_ps.cso");
	}
	if (animShader == nullptr)
	{
		animShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::ANIM);
		animShader->Create("./Data/Shader/AnimShader_vs.cso", "./Data/Shader/AnimShader_ps.cso", "./Data/Shader/AnimShader_gs.cso");
	}
	if (toonShader == nullptr)
	{
		toonShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::TOON);
		toonShader->Create("./Data/Shader/ToonShader_vs.cso", "./Data/Shader/ToonShader_ps.cso", "./Data/Shader/ToonShader_gs.cso");
	}

	//カメラ初期設定
	YRCamera.SetEye(DirectX::XMFLOAT3(0.0f, 5.0f, -25));			//視点
	YRCamera.SetFocus(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f));			//注視点
	YRCamera.SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));				//上方向
	YRCamera.SetPerspective(30 * 0.01745f, 1920.0f / 1080.0f, 0.0001f, 1000000);

	if (box_texture == nullptr)
	{
		box_texture = std::make_shared<Texture>(L"./Data/FBX/danbo_fbx/texture/danbo_face_c2.png");
	}
	if (board_texture == nullptr)
	{
		board_texture = std::make_shared<Texture>(L"./Data/Image/UI/GameScene/effect.png");
	}

	if (box == nullptr)
	{
		box = std::make_unique<Skinned_mesh>("./Data/FBX/knight.fbx");
	}
	if (geo == nullptr)
	{
		geo = std::make_unique<geometric_primitive>();
	}
	if (board == nullptr)
	{
		board = std::make_unique<board_primitive>(board_texture);
	}
	if (anim == nullptr)
	{
		anim = std::make_unique<AnimBoard>(board_texture, 6,XMFLOAT2(64.0f,64.0f),XMINT2(3,2),XMFLOAT2(192.0f,128.0f));
	}

	motion.MeshSet(box);
	motion.AnimReset();

	//画像のロード
	if (test == nullptr)
	{
		test = std::make_unique<Sprite>(L"./Data/Image/BG/stage1.png", 3840.0f, 2160.0f);
	}

	//テクスチャロード
	if (color_texture == nullptr)
	{
		color_texture = std::make_unique<Texture>();
		color_texture->Create(1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM);
	}
	if (normal_texture == nullptr)
	{
		normal_texture = std::make_unique<Texture>();
		normal_texture->Create(1280, 720, DXGI_FORMAT_R16G16B16A16_FLOAT);
	}
	if (position_texture == nullptr)
	{
		position_texture = std::make_unique<Texture>();
		position_texture->Create(1280, 720, DXGI_FORMAT_R16G16B16A16_FLOAT);
	}
	if (sampler == nullptr)
	{
		sampler = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	}
}

void SceneTest::Update(float elapsed_time)
{

}

void SceneTest::Draw(float elapsed_time)
{
	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(0, -1, 1, 0);
	static DirectX::XMFLOAT4 ambient_color(0.3f, 0.3f, 0.3f, 0.5f);
	static float anim_count = 0.0f;
	//static DirectX::XMFLOAT3 box_angle = { DirectX::XMConvertToRadians(-90.0f),0.0f,0.0f };
	static DirectX::XMFLOAT3 box_angle = { 0.0f,0.0f,0.0f };

	static float off_x = 0.0f;
	static float off_y = 0.0f;
#if USE_IMGUI
	//ImGui
	if(0){
		DirectX::XMFLOAT3	eye = YRCamera.GetEye();
		DirectX::XMFLOAT3	focus = YRCamera.GetFocus();
		float				fov = YRCamera.GetFov();
		fov /= 0.01745f;
		//ImGui::Text("anim : %f", motion.anim_timer);
		ImGui::Text("SceneTest");
		ImGui::Text("time : %f", timer);
		//ImGui::InputFloat("scroll", &scall, 0.01f, 100.0f);
		//ImGui::SliderFloat("rollX", &roll.x, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollY", &roll.y, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollZ", &roll.z, 0.0f, 30.00f);
		//ImGui::InputFloat("transX", &trans.x, 1.0f, 30.00f);
		//ImGui::InputFloat("transY", &trans.y, 1.0f, 30.00f);
		//ImGui::InputFloat("transZ", &trans.z, 1.0f, 30.00f);
		if (ImGui::TreeNode("light"))
		{
			ImGui::ColorEdit4("light_color", light_color);
			ImGui::InputFloat("light_direction.x", &light_direction.x, 0.1f, 0.1f);
			ImGui::InputFloat("light_direction.y", &light_direction.y, 0.1f, 0.1f);
			ImGui::InputFloat("light_direction.z", &light_direction.z, 0.1f, 0.1f);
			ImGui::InputFloat("light_direction.w", &light_direction.w, 0.1f, 0.1f);
			ImGui::InputFloat("ambient_color.x", &ambient_color.x, 0.01f, 0.01f);
			ImGui::InputFloat("ambient_color.y", &ambient_color.y, 0.01f, 0.01f);
			ImGui::InputFloat("ambient_color.z", &ambient_color.z, 0.01f, 0.01f);
			ImGui::InputFloat("ambient_color.w", &ambient_color.w, 0.01f, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::InputFloat("eye_X", &eye.x, 0.01f, 0.01f);
			ImGui::InputFloat("eye_Y", &eye.y, 0.01f, 0.01f);
			ImGui::InputFloat("eye_Z", &eye.z, 0.01f, 0.01f);
			ImGui::InputFloat("focus_X", &focus.x, 0.01f, 0.01f);
			ImGui::InputFloat("focus_Y", &focus.y, 0.01f, 0.01f);
			ImGui::InputFloat("focus_Z", &focus.z, 0.01f, 0.01f);
			ImGui::InputFloat("fov", &fov, 1.00f, 1.00f);
			ImGui::TreePop();
		}

		YRCamera.SetEye(eye);
		YRCamera.SetFocus(focus);
		YRCamera.SetPerspective(fov * 0.01745f, 1920.0f / 1080.0f, 0.0001f, 1000000.0f);
		ImGui::InputFloat("box_angle.x", &box_angle.x, 0.01f, 0.01f);
		ImGui::InputFloat("box_angle.y", &box_angle.y, 0.01f, 0.01f);
		ImGui::InputFloat("box_angle.z", &box_angle.z, 0.01f, 0.01f);

		ImGui::InputFloat("offset.x", &off_x, 0.01f, 0.01f);
		ImGui::InputFloat("offset.y", &off_y, 0.01f, 0.01f);
	}
#endif
	DirectX::XMFLOAT4 lightColor(light_color[0], light_color[1], light_color[2], light_color[3]);

	//ビュー行列
	DirectX::XMMATRIX V;
	V = DirectX::XMLoadFloat4x4(&YRCamera.GetView());
	//投資投影行列
	DirectX::XMMATRIX P;
	P = DirectX::XMLoadFloat4x4(&YRCamera.GetProjection());

	//材質カラー
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1, 1, 1, 1);


	//仮背景
	//test->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 0.5f);

	motion.DrawContinue(
		toonShader.get(),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
		box_angle,
		V, P, light_direction, lightColor, ambient_color, elapsed_time
	);

	//geo->render(
	//	geoShader.get(),
	//	DirectX::XMFLOAT3(5.0f, 5.0f, 0.0f),
	//	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	//	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	//	V,
	//	P,
	//	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	//);

	//sampler->Set(0);

	//board->render(
	//	boardShader.get(),
	//	DirectX::XMFLOAT3(-5.0f, 3.0f, 0.0f),
	//	DirectX::XMFLOAT3(3.0f, 3.0f, 0.0f),
	//	box_angle,
	//	V,
	//	P,
	//	off_x,
	//	off_y,
	//	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	//);

	////sampler->Set(0);
	//anim->Render(
	//	animShader.get(),
	//	DirectX::XMFLOAT3(5.0f, 3.0f, 0.0f),
	//	DirectX::XMFLOAT2(1.0f, 1.0f),
	//	box_angle,
	//	0.1f,
	//	V, P,
	//	elapsed_time
	//);
}