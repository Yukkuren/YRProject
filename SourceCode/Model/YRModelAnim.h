#pragma once

#include <memory>
#include <d3d11.h>
#include "YRModel.h"
//#include "framework.h"

//-----------------------------------------------------------------
//			ModelAnimクラス
//-----------------------------------------------------------------
//・モデルデータをアニメーションさせるためのクラス
//・使い方はこのクラスを生成するときにメッシュがあるモデルのModelクラスを渡す
//・その後、メッシュの無いアニメーションのみのModelクラスをセットすることで
//  メッシュのアニメーションが出来る
//-----------------------------------------------------------------

class ModelAnim
{
public:
	ModelAnim(const std::shared_ptr<Model>& resource);
	~ModelAnim() {
		m_model_resource.reset();
		model_resource_anim.reset();
		m_nodes.clear();
	}

	struct Node
	{
		std::string name;
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
	void DebugUpdateAnimation(float timer);

	// 行列計算
	void CalculateLocalTransform();
	void CalculateWorldTransform(
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& angle);

	const std::vector<Node>& GetNodes() const { return m_nodes; }
	const Model* GetModelResource() const { return m_model_resource.get(); }

	DirectX::XMFLOAT4X4 Draw(
		YRShader* shader,
		const DirectX::XMMATRIX&	view,
		const DirectX::XMMATRIX&	projection,
		const DirectX::XMFLOAT4&	light_direction,
		const DirectX::XMFLOAT4&	light_color,
		const DirectX::XMFLOAT4&	ambient_color,
		const DirectX::XMFLOAT2&	off_set_eye = { 0.0f,0.0f },
		const DirectX::XMFLOAT2&	off_set_mouse = { 0.0f,0.0f },
		const Model::Material_Attribute& blur_material= Model::Material_Attribute::SWORD,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
		);



	void Draw(
		YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const D3D_PRIMITIVE_TOPOLOGY& topology,
		const DirectX::XMFLOAT4& ambient_color,
		const DirectX::XMFLOAT2& off_set_eye = { 0.0f,0.0f },
		const DirectX::XMFLOAT2& off_set_mouse = { 0.0f,0.0f },
		const Model::Material_Attribute& blur_material = Model::Material_Attribute::SWORD,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	void NodeChange(const std::shared_ptr<Model>& resource);
	void NodeChange(const std::shared_ptr<Model>& resource,int anim_num);

	//bool AnimCheack(const std::shared_ptr<Model>& resource, int anim_num);

	const bool &GetLoopAnim()const  { return m_loop_animation; };	//再生中のアニメーションがループするかを返す
	const int& GetEndAnim()const { return m_current_animation; };	//再生中のアニメーションが終了したかを返す(-1なら終了)

	const bool& GetIntermediate()const { return model_resource_anim->intermediate; };	//再生中のアニメーションが中間アニメーションかどうかを返す

private:
	std::shared_ptr<Model>			m_model_resource;
	std::shared_ptr<Model>			model_resource_anim = nullptr;
	std::vector<Node>				m_nodes;
	int								m_current_animation = -1;
	float							m_current_seconds = 0.0f;
	bool							m_loop_animation = false;
	bool							m_end_animation = false;
	DirectX::XMMATRIX				world_matrix;

	static const int MaxBones = 128;

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;
		DirectX::XMFLOAT4X4		world;
		DirectX::XMFLOAT4		material_color;
		DirectX::XMFLOAT4		light_direction;
		DirectX::XMFLOAT4		light_color;
		DirectX::XMFLOAT4		ambient_color;
		DirectX::XMFLOAT4X4		bone_transforms[MaxBones];
		DirectX::XMFLOAT4		eyePos;						//カメラの位置
		DirectX::XMFLOAT4X4		view;						//トゥーンシェーダー用view行列
		DirectX::XMFLOAT4X4		projection;					//トゥーンシェーダー用projection行列
		DirectX::XMFLOAT3		at;							//eyeからfocusに向かう正規化ベクトル
		float					lumi_factor;				//輝度調整用(0.0f~1.0f)
		float					Offset_X;					//テクスチャの位置調整X
		float					Offset_Y;					//テクスチャの位置調整Y
		float					dummy00;
		float					dummy01;
		DirectX::XMFLOAT4		dummy02;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

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
