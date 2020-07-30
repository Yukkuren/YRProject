#include "Skinned_mesh.h"
#include "framework.h"

//���[�V�����f�[�^���Z�b�g����
void MeshMotion::MeshSet(std::unique_ptr<Skinned_mesh>& mesh)
{
	skin = mesh.get();
}

//�A�j���[�V���������Z�b�g����
void MeshMotion::AnimReset()
{
	anim_timer = 0.0f;
	skin->AllFalse();
}

//���̂ݕ`��(�A�j���[�V�������I����true)
bool MeshMotion::Draw(
	YRShader		*shader,
	DirectX::XMFLOAT3& pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMFLOAT3& angle,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time,
	bool						inversion,
	const DirectX::XMFLOAT4		material_color
)
{
	anim_timer += elapsed_time;
	skin->Render(shader,pos, scale, angle, view, projection, light_direction, light_color, ambient_color, elapsed_time, anim_timer, inversion,material_color);
	if (skin->FinCheck())
	{
		return true;
	}
	return false;
}

//�A���`��
void MeshMotion::DrawContinue(
	YRShader		*shader,
	DirectX::XMFLOAT3& pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMFLOAT3& angle,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time,
	bool						inversion,
	const DirectX::XMFLOAT4		material_color
)
{
	anim_timer += elapsed_time;
	skin->Render(shader,pos, scale, angle, view, projection, light_direction, light_color, ambient_color, elapsed_time, anim_timer, inversion,material_color);
	if (skin->FinCheck())
	{
		AnimReset();
	}
}

//�A�j���[�V���������p
void MeshMotion::DrawMesh(
	YRShader		*shader,
	DirectX::XMFLOAT3 pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMFLOAT3& angle,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	bool						inversion,
	const DirectX::XMFLOAT4		material_color
)
{
	skin->Render(shader,pos, scale, angle, view, projection, light_direction, light_color, ambient_color, 0.0f, 0.0f,inversion, material_color);
}

//���݂̃A�j���[�V�����̏�ԂŒ�~�`��
void MeshMotion::DrawStop(
	YRShader		*shader,
	DirectX::XMFLOAT3 pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMFLOAT3& angle,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time,
	bool						inversion,
	const DirectX::XMFLOAT4		material_color
)
{
	skin->Render(shader,pos, scale, angle, view, projection, light_direction, light_color, ambient_color, elapsed_time, anim_timer,inversion, material_color);
}


//�p������s��g�p

//���̂ݕ`��(�A�j���[�V�������I����true)
bool MeshMotion::Draw(
	YRShader		*shader,
	DirectX::XMFLOAT3& pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMVECTOR& orientation,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time,
	const DirectX::XMFLOAT4		material_color
)
{
	anim_timer += elapsed_time;
	skin->Render(shader,pos, scale, orientation, view, projection, light_direction, light_color, ambient_color, elapsed_time, anim_timer, material_color);
	if (skin->FinCheck())
	{
		return true;
	}
	return false;
}

//�A���`��
void MeshMotion::DrawContinue(
	YRShader		*shader,
	DirectX::XMFLOAT3& pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMVECTOR& orientation,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time,
	const DirectX::XMFLOAT4		material_color
)
{
	anim_timer += elapsed_time;
	skin->Render(shader,pos, scale, orientation, view, projection, light_direction, light_color, ambient_color, elapsed_time, anim_timer, material_color);
	if (skin->FinCheck())
	{
		AnimReset();
	}
}

//�A�j���[�V���������p
void MeshMotion::DrawMesh(
	YRShader		*shader,
	DirectX::XMFLOAT3 pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMVECTOR& orientation,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	const DirectX::XMFLOAT4		material_color
)
{
	skin->Render(shader,pos, scale, orientation, view, projection, light_direction, light_color, ambient_color, 0.0f, 0.0f, material_color);
}

//���݂̃A�j���[�V�����̏�ԂŒ�~�`��
void MeshMotion::DrawStop(
	YRShader		*shader,
	DirectX::XMFLOAT3 pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMVECTOR& orientation,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time,
	const DirectX::XMFLOAT4		material_color
)
{
	skin->Render(shader,pos, scale, orientation, view, projection, light_direction, light_color, ambient_color, elapsed_time, anim_timer, material_color);
}