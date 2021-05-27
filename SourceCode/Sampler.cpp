#include "Sampler.h"
#include "framework.h"

Sampler::Sampler(D3D11_SAMPLER_DESC sd)
{
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;

	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = FRAMEWORK.device->CreateSamplerState(&sd, sampler_state.GetAddressOf());
	assert(SUCCEEDED(hr));
}

Sampler::Sampler(
	D3D11_FILTER filter,
	D3D11_TEXTURE_ADDRESS_MODE Address_mode,
	FLOAT color[4])
{
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = filter;
	sd.AddressU = Address_mode;
	sd.AddressV = Address_mode;
	sd.AddressW = Address_mode;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	if (color) {
		memcpy(sd.BorderColor, color, sizeof(color));
	}

	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = FRAMEWORK.device->CreateSamplerState(&sd, sampler_state.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Sampler::Set(UINT Slot, BOOL flg)
{
	if (flg == FALSE) {

	}

	if (sampler_state) {
		FRAMEWORK.context->PSSetSamplers(Slot, 1, sampler_state.GetAddressOf());
		FRAMEWORK.context->DSSetSamplers(Slot, 1, sampler_state.GetAddressOf());
	}
}