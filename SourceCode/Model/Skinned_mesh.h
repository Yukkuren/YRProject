#pragma once

#include<Windows.h>
#include<directxmath.h>
#include "../YRShader.h"
#include<array>
#include<memory>
#include "../Texture.h"

//-----------------------------------------------------------------
//			Skinned_Meshクラス
//-----------------------------------------------------------------
//・初期に制作した3Dモデルを扱うクラス(fbx)
//・アニメーションをFBXモデルに依存させる形でデータを保持するので
//  必然的にメモリを多大に確保してしまい、モデルの容量も大きくなってしまう
//・現在はModelクラスがある為、アニメーションのないモデルのみ使用する
//-----------------------------------------------------------------

#define MAX_BONE_INFLUENCES 4		//ボーンの影響度の最大数
#define BONE_MAX	(128)			//ボーンの最大数

struct bone_influence
{
	int index;		//インデックス
	float weight;	//重き
};
typedef std::vector<bone_influence> bone_influences_per_control_point;


class Skinned_mesh
{

protected:
	//テクスチャ利用
	std::shared_ptr<Texture> texture = nullptr;
public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		line_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		filling_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		filling_state_inverse;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depth_state;
	D3D11_TEXTURE2D_DESC texture2d_desc;
	unsigned int indexsize;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;


	//Blenderなどと座標軸が違うためこの行列を掛けて矯正する
	DirectX::XMFLOAT4X4 coodinate_conversion = {
		1,0,0,0,
		0,0,1,0,
		0,1,0,0,
		0,0,0,1
	};
	//Y軸とZ軸を入れ替える

	Skinned_mesh(const char* fbx_filename, std::shared_ptr<Texture> tex, u_int sampling_rate = 0);

	Skinned_mesh(const char *fbx_filename, u_int sampling_rate = 0);

	bool Load(const char* fbx_filename, u_int sampling_rate = 0);
	bool Load(const char* fbx_filename, std::shared_ptr<Texture> tex, u_int sampling_rate = 0);

	~Skinned_mesh(){}


	//基本的にこっちを使う(position,scale,angle直入力型)
	void Render(
		YRShader					*shader,
		const DirectX::XMFLOAT3			&pos,
		const DirectX::XMFLOAT3			&scale,
		const DirectX::XMFLOAT3			&angle,
		const DirectX::XMMATRIX		&view,
		const DirectX::XMMATRIX		&projection,
		const DirectX::XMFLOAT4		&light_direction,
		const DirectX::XMFLOAT4		&light_color,
		const DirectX::XMFLOAT4		&ambient_color,
		//UNIT.23
		float						elapsed_time,
		float						anime_count,		//アニメのカウント(通常は1.0f、早くしたい場合は減らす(？))
		bool						inversion = false,	//反転させるかどうか
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);


	//こちらはテスト版
	void Render(
		YRShader					*shader,
		DirectX::XMFLOAT3			&pos,
		DirectX::XMFLOAT3			&scale,
		DirectX::XMVECTOR			&orientation,
		const DirectX::XMMATRIX		&view,
		const DirectX::XMMATRIX		&projection,
		const DirectX::XMFLOAT4		&light_direction,
		const DirectX::XMFLOAT4		&light_color,
		const DirectX::XMFLOAT4		&ambient_color,
		//UNIT.23
		float						elapsed_time,
		float						anime_count,		//アニメのカウント
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);


	struct Vertex
	{
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
		DirectX::XMFLOAT2		texcoord;
		FLOAT bone_weights[MAX_BONE_INFLUENCES] = { 1,0,0,0 };
		INT bone_indices[MAX_BONE_INFLUENCES] = {};
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;
		DirectX::XMFLOAT4X4		world;
		DirectX::XMFLOAT4		material_color;
		DirectX::XMFLOAT4		light_direction;
		DirectX::XMFLOAT4		light_color;
		DirectX::XMFLOAT4		ambient_color;
		DirectX::XMFLOAT4X4		bone_transforms[BONE_MAX];
		DirectX::XMFLOAT4		eyePos;
		DirectX::XMFLOAT4X4		view;
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT3		at;//eyeからfocusに向かう正規化ベクトル
		float					dummy;
	};

	struct material
	{
		DirectX::XMFLOAT4 color = { 0.8f,0.8f,0.8f,1.0f };
		ID3D11ShaderResourceView *shader_resource_view;
		~material()
		{
			if (shader_resource_view != nullptr)
			{
				shader_resource_view->Release();
			}
		}
	};
	//material diffuse;
	struct subset
	{
		u_int index_start = 0;	//	start number of index buffer
		u_int index_count = 0;	//	number of vertics (indices)
		material diffuse;
	};
	//std::vector<subset> subsets;

	//UNIT.22
	struct bone
	{
		DirectX::XMFLOAT4X4 transform;
	};
	//UNIT.23
	typedef std::vector<bone> skeletal;
	struct Skeletal_animation : public std::vector<skeletal>
	{
		float sampling_time = 1 / 24.0f;
		float animation_tick = 0.0f;
		//bool anim_fin = false;
	};

	struct mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer>vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>index_buffer;
		std::vector<subset> subsets;
		DirectX::XMFLOAT4X4 global_transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		//UNIT.22
		//std::vector<Skinned_mesh::bone> skeletal;
		//UNIT.23
		Skeletal_animation skeletal_animation;
	};
	std::vector<mesh>meshes;

	//アニメーション管理用変数

	bool FinCheck()
	{
		for (auto& mesh : meshes)
		{
			int frame = static_cast<int>(mesh.skeletal_animation.animation_tick / mesh.skeletal_animation.sampling_time);
			if (frame <= static_cast<int>(mesh.skeletal_animation.size()))
			{
				return false;
			}
		}
		return true;
	}

	void AllFalse()
	{
		for (auto& mesh : meshes)
		{
			mesh.skeletal_animation.animation_tick = 0;
			//mesh.skeletal_animation.anim_fin = false;
		}
	}
};


//メッシュアニメーション管理クラス
class MeshMotion
{
public:
	Skinned_mesh*	skin;		//モデル付け替え用アドレス
	float			anim_timer = 0.0f;	//モーション用タイマー
public:
	//モーションデータをセットする
	void MeshSet(std::unique_ptr<Skinned_mesh>& mesh);

	//アニメーションをリセットする
	void AnimReset();

	//一回のみ描画(アニメーションが終わるとtrue)
	bool Draw(
		YRShader			*shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time,
		bool						inversion = true,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//連続描画
	void DrawContinue(
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
		bool						inversion = true,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//アニメーション無し用
	void DrawMesh(
		YRShader		*shader,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		bool						inversion = true,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//現在のアニメーションの状態で停止描画
	void DrawStop(
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
		bool						inversion = true,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);


	//姿勢制御行列使用

	//一回のみ描画(アニメーションが終わるとtrue)
	bool Draw(
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
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//連続描画
	void DrawContinue(
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
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//アニメーション無し用
	void DrawMesh(
		YRShader* shader,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMVECTOR& orientation,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//現在のアニメーションの状態で停止描画
	void DrawStop(
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
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);
};