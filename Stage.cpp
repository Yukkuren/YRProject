#include "Scene.h"
#include "framework.h"

void Stage::Init(StageType type)
{
	switch (type)
	{
	case Stage::StageType::NORMAL:
		{
			if (skyShader == nullptr)
			{
				skyShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SKY);
				skyShader->Create("./Data/Shader/SkyMapShader_vs.cso", "./Data/Shader/SkyMapShader_ps.cso");
			}

			if (sky == nullptr)
			{
				sky = std::make_unique<Skinned_mesh>("./Data/FBX/SKY/skyblock_re.fbx");
			} 
			if (stage_data == nullptr)
			{
				stage_data = std::make_unique<Skinned_mesh>("./Data/FBX/SKY/Castle FBX.fbx");
			} 
		}
		break;
	default:
		break;
	}
	Sky_Pos = YR_Vector3(0.0f, -18.0f, 0.0f);
	Sky_Scale = YR_Vector3(0.5f, 0.5f, 0.5f);
	Sky_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	Stage_Pos = YR_Vector3(0.0f, -9.0f, -30.0f);
	Stage_Scale = YR_Vector3(10.5f, 10.5f,10.5f);
	Stage_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);
}

void Stage::Uninit()
{
	skyShader.reset();
	skyShader = nullptr;

	sky.reset();
	sky = nullptr;
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
#ifdef USE_IMGUI
	{
		bool show_another_window = false;
		ImGui::SetNextWindowSize(ImVec2(400, 400), 2);
		ImGui::Begin("SkyMap&Stage", &show_another_window);
		ImGui::SliderFloat(u8"SKY_pos.X", &Sky_Pos.x, -100.0f, 100.0f);
		ImGui::SliderFloat(u8"SKY_pos.Y", &Sky_Pos.y, -100.0f, 100.0f);
		ImGui::SliderFloat(u8"SKY_pos.Z", &Sky_Pos.z, -100.0f, 100.0f);
		ImGui::SliderFloat(u8"SKY_scale.X", &Sky_Scale.x, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"SKY_scale.Y", &Sky_Scale.y, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"SKY_scale.Z", &Sky_Scale.z, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"SKY_angle.X", &Sky_Angle.x, -10.0f, 10.0f);
		ImGui::SliderFloat(u8"SKY_angle.Y", &Sky_Angle.y, -10.0f, 10.0f);
		ImGui::SliderFloat(u8"SKY_angle.Z", &Sky_Angle.z, -10.0f, 10.0f);
		ImGui::SliderFloat(u8"Stage_pos.X", &Stage_Pos.x, -100.0f, 100.0f);
		ImGui::SliderFloat(u8"Stage_pos.Y", &Stage_Pos.y, -100.0f, 100.0f);
		ImGui::SliderFloat(u8"Stage_pos.Z", &Stage_Pos.z, -100.0f, 100.0f);
		ImGui::SliderFloat(u8"Stage_scale.X", &Stage_Scale.x, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"Stage_scale.Y", &Stage_Scale.y, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"Stage_scale.Z", &Stage_Scale.z, 0.0f, 10.0f);
		ImGui::SliderFloat(u8"Stage_angle.X", &Stage_Angle.x, -10.0f, 10.0f);
		ImGui::SliderFloat(u8"Stage_angle.Y", &Stage_Angle.y, -10.0f, 10.0f);
		ImGui::SliderFloat(u8"Stage_angle.Z", &Stage_Angle.z, -10.0f, 10.0f);
		ImGui::End();
	}
#endif // USE_IMGUI

	sky->Render(skyShader.get(),
		Sky_Pos.GetDXFLOAT3(),
		Sky_Scale.GetDXFLOAT3(),
		Sky_Angle.GetDXFLOAT3(),
		view,
		projection,
		light_direction,
		light_color,
		ambient_color,
		elapsed_time,
		0.0f
	);

	stage_data->Render(skyShader.get(),
		Stage_Pos.GetDXFLOAT3(),
		Stage_Scale.GetDXFLOAT3(),
		Stage_Angle.GetDXFLOAT3(),
		view,
		projection,
		light_direction,
		light_color,
		ambient_color,
		elapsed_time,
		0.0f
	);
}