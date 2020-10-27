#include "Scene.h"
#include "framework.h"

void Stage::Init(StageType type)
{
	switch (type)
	{
	case Stage::StageType::NORMAL:
		{
			//シェーダー生成
			if (skyShader == nullptr)
			{
				skyShader = std::make_unique<YRShader>(ShaderType::SKY);
				skyShader->Create("./Data/Shader/SkyMapShader_vs.cso", "./Data/Shader/SkyMapShader_ps.cso");
			}
			if (skinShader == nullptr)
			{
				/*skinShader = std::make_unique<YRShader>(ShaderType::SKIN);
				skinShader->Create("./Data/Shader/Skinned_VS.cso", "./Data/Shader/Skinned_PS.cso");*/
				skinShader = std::make_unique<YRShader>(ShaderType::FLAT);
				skinShader->Create("./Data/Shader/flatShader_vs.cso", "./Data/Shader/flatShader_ps.cso","./Data/Shader/flatShader_gs.cso");
			}
			/*if (ToonShader == nullptr)
			{
				ToonShader = std::make_unique<YRShader>(ShaderType::TOON);
				ToonShader->Create("./Data/Shader/ToonShader_vs.cso", "./Data/Shader/ToonShader_ps.cso", "./Data/Shader/ToonShader_gs.cso");
			}*/

			//FBX読み込み
			/*if (sky == nullptr)
			{
				sky = std::make_shared<Model>("./Data/FBX/SKY/skyblock_re.fbx");
			} */
			if (sky_data == nullptr)
			{
				sky_data = std::make_unique<Skinned_mesh>("./Data/FBX/SKY/skyblock_re.fbx");
			}
			if (stage_data == nullptr)
			{
				stage_data = std::make_shared<Model>("./Data/FBX/SKY/castle.fbx");
			} 

			/*if (sky_draw == nullptr)
			{
				sky_draw = std::make_unique<ModelAnim>(sky);
			}*/
			if (stage_draw == nullptr)
			{
				stage_draw = std::make_unique<ModelAnim>(stage_data);
			}
		}
		break;
	default:
		break;
	}
	Sky_Pos = YR_Vector3(0.0f, -18.0f, 0.0f);
	Sky_Scale = YR_Vector3(1.0f, 1.0f, 1.0f);
	Sky_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	Stage_Pos = YR_Vector3(0.0f, -9.0f, -30.0f);
	Stage_Scale = YR_Vector3(10.5f, 10.5f,10.5f);
	Stage_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);
}

void Stage::Uninit()
{
	//シェーダー解放
	skyShader.reset();
	skyShader = nullptr;
	skinShader.reset();
	skinShader = nullptr;
	/*ToonShader.reset();
	ToonShader = nullptr;*/

	//FBX解放
	/*sky.reset();
	sky = nullptr;*/
	sky_data.reset();
	sky_data = nullptr;
	stage_data.reset();
	stage_data = nullptr;
}

void Stage::Draw(
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	static DirectX::XMFLOAT4 light_direction_stage = DirectX::XMFLOAT4(-0.4f, -0.7f, 0.7f, 0.0f);
	static DirectX::XMFLOAT4 ambient_color_stage(0.0f, 0.0f, 0.0f, 0.5f);
#ifdef USE_IMGUI
	{
		
		bool show_another_window = false;
		ImGui::SetNextWindowSize(ImVec2(400, 400), 2);
		ImGui::Begin("SkyMap&Stage", &show_another_window);
		if (ImGui::TreeNode("SkyMap"))
		{
			ImGui::SliderFloat("SKY_pos.X", &Sky_Pos.x, -100.0f, 100.0f);
			ImGui::SliderFloat("SKY_pos.Y", &Sky_Pos.y, -100.0f, 100.0f);
			ImGui::SliderFloat("SKY_pos.Z", &Sky_Pos.z, -100.0f, 100.0f);
			ImGui::SliderFloat("SKY_scale.X", &Sky_Scale.x, 0.0f, 10.0f);
			ImGui::SliderFloat("SKY_scale.Y", &Sky_Scale.y, 0.0f, 10.0f);
			ImGui::SliderFloat("SKY_scale.Z", &Sky_Scale.z, 0.0f, 10.0f);
			ImGui::SliderFloat("SKY_angle.X", &Sky_Angle.x, -10.0f, 10.0f);
			ImGui::SliderFloat("SKY_angle.Y", &Sky_Angle.y, -10.0f, 10.0f);
			ImGui::SliderFloat("SKY_angle.Z", &Sky_Angle.z, -10.0f, 10.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Stage"))
		{
			ImGui::SliderFloat("Stage_pos.X", &Stage_Pos.x, -100.0f, 100.0f);
			ImGui::SliderFloat("Stage_pos.Y", &Stage_Pos.y, -100.0f, 100.0f);
			ImGui::SliderFloat("Stage_pos.Z", &Stage_Pos.z, -100.0f, 100.0f);
			ImGui::SliderFloat("Stage_scale.X", &Stage_Scale.x, 0.0f, 10.0f);
			ImGui::SliderFloat("Stage_scale.Y", &Stage_Scale.y, 0.0f, 10.0f);
			ImGui::SliderFloat("Stage_scale.Z", &Stage_Scale.z, 0.0f, 10.0f);
			ImGui::SliderFloat("Stage_angle.X", &Stage_Angle.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Stage_angle.Y", &Stage_Angle.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Stage_angle.Z", &Stage_Angle.z, -10.0f, 10.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Light"))
		{
			ImGui::SliderFloat("LightDirection.X", &light_direction_stage.x, -1.0f, 1.0f);
			ImGui::SliderFloat("LightDirection.Y", &light_direction_stage.y, -1.0f, 1.0f);
			ImGui::SliderFloat("LightDirection.Z", &light_direction_stage.z, -1.0f, 1.0f);
			ImGui::SliderFloat("LightDirection.W", &light_direction_stage.w, -1.0f, 1.0f);
			ImGui::SliderFloat("AmbientColor.X", &ambient_color_stage.x, 0.0f, 1.0f);
			ImGui::SliderFloat("AmbientColor.Y", &ambient_color_stage.y, 0.0f, 1.0f);
			ImGui::SliderFloat("AmbientColor.Z", &ambient_color_stage.z, 0.0f, 1.0f);
			ImGui::SliderFloat("AmbientColor.W", &ambient_color_stage.w, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		ImGui::End();
	}
#endif // USE_IMGUI

	/*sky_draw->CalculateLocalTransform();
	sky_draw->CalculateWorldTransform(Sky_Pos.GetDXFLOAT3(), Sky_Scale.GetDXFLOAT3(), Sky_Angle.GetDXFLOAT3());
	sky_draw->Draw(skyShader.get(),
		view,
		projection,
		light_direction,
		light_color,
		ambient_color
	);*/
	sky_data->Render(skyShader.get(),
		Sky_Pos.GetDXFLOAT3(), Sky_Scale.GetDXFLOAT3(), Sky_Angle.GetDXFLOAT3(),
		view,
		projection,
		light_direction,
		light_color,
		ambient_color, elapsed_time, 0.0f);

	stage_draw->CalculateLocalTransform();
	stage_draw->CalculateWorldTransform(Stage_Pos.GetDXFLOAT3(),Stage_Scale.GetDXFLOAT3(),Stage_Angle.GetDXFLOAT3());
	stage_draw->Draw(skinShader.get(),
		view,
		projection,
		light_direction_stage,
		light_color,
		ambient_color_stage
	);
}