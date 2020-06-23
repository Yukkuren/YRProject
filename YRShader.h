#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include<wrl.h>
using namespace DirectX;

class YRShader
{
public:
private:
	bool motion_on = false;
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		VSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		PSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	GSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11HullShader>		HSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		DSShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		VertextLayout = nullptr;
	
public:
	//YRShader() {};
	YRShader(bool motion_on) : motion_on(motion_on) {};
	virtual ~YRShader();

	bool Create(const char* VS_cso_file, const char* PS_cso_file);
	bool Create(const char* VS_cso_file, const char* PS_cso_file, const char* GS_cso_file);
	bool Create(const char* VS_cso_file, const char* PS_cso_file, const char* DS_cso_file, const char* HS_cso_file);
	bool Create(const char* VS_cso_file, const char* PS_cso_file, const char* GS_cso_file, const char* DS_cso_file, const char* HS_cso_file);
	
	void Acivate();		//ìÆçÏ
	void Inactivate();	//í‚é~
};
