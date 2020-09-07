#pragma once

#include <memory>
#include <d3d11.h>
#include "YRModel.h"
#include "framework.h"

class ModelAnim
{
public:
	ModelAnim(std::shared_ptr<Model>& resource);
	~ModelAnim() {}

	struct Node
	{
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	local_transform;
		DirectX::XMFLOAT4X4	world_transform;
	};

	// アニメーション
	bool IsPlayAnimation() const { return m_current_animation >= 0; }
	void PlayAnimation(int animation_index, bool loop = false);
	void UpdateAnimation(float elapsed_time);

	// 行列計算
	void CalculateLocalTransform();
	void CalculateWorldTransform(
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle);

	const std::vector<Node>& GetNodes() const { return m_nodes; }
	const Model* GetModelResource() const { return m_model_resource.get(); }

	void Draw(
		YRShader* shader,
		const DirectX::XMMATRIX&	view,
		const DirectX::XMMATRIX&	projection,
		const DirectX::XMFLOAT4&	light_direction,
		const DirectX::XMFLOAT4&	light_color,
		const DirectX::XMFLOAT4&	ambient_color,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
		);

private:
	std::shared_ptr<Model>			m_model_resource;
	std::vector<Node>				m_nodes;
	int								m_current_animation = -1;
	float							m_current_seconds = 0.0f;
	bool							m_loop_animation = false;
	bool							m_end_animation = false;
	DirectX::XMMATRIX				world_matrix;

	static const int MaxBones = 32;

	/*struct CbScene
	{
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4	bone_transforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	material_color;
	};*/

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;
		DirectX::XMFLOAT4X4		world;
		DirectX::XMFLOAT4		material_color;
		DirectX::XMFLOAT4		light_direction;
		DirectX::XMFLOAT4		light_color;
		DirectX::XMFLOAT4		ambient_color;
		DirectX::XMFLOAT4X4		bone_transforms[MaxBones];
		DirectX::XMFLOAT4		eyePos;
		DirectX::XMFLOAT4X4		view;
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT3		at;//eyeからfocusに向かう正規化ベクトル
		float					dummy;
		/*int						inverse;
		int		dummy1;
		int		dummy2;
		int		dummy3;*/
	};


	//Microsoft::WRL::ComPtr<ID3D11Buffer>			m_cb_scene;
	//Microsoft::WRL::ComPtr<ID3D11Buffer>			m_cb_mesh;
	//Microsoft::WRL::ComPtr<ID3D11Buffer>			m_cb_subset;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

	//Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertex_shader;
	//Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixel_shader;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_input_layout;

	//Microsoft::WRL::ComPtr<ID3D11BlendState>		m_blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_sampler_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_dummy_srv;

	DirectX::XMFLOAT4X4 coodinate_conversion = {
		1,0,0,0,
		0,0,1,0,
		0,1,0,0,
		0,0,0,1
	};
};
