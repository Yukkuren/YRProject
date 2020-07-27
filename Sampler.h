#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include<wrl.h>

class Sampler
{
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state = nullptr;

public:
	Sampler(D3D11_SAMPLER_DESC sd);
	Sampler(D3D11_FILTER filter,
		D3D11_TEXTURE_ADDRESS_MODE Address_mode,
		FLOAT color[4] = { 0 });
	void Set(UINT Slot = 0, BOOL flg = TRUE);
	ID3D11SamplerState* GetSamplerState() { return sampler_state.Get(); }
};
