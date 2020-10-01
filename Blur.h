#pragma once
#include <DirectXMath.h>

class GaussBlurParam
{
private:
	int					SampleCount;
	int					Dummy[3];
	DirectX::XMFLOAT4	Offset[16];
public:
	float GaussianDistribution(const DirectX::XMFLOAT2& pos, float rho);
	void CalcBlurParam(int width, int height, DirectX::XMFLOAT2 dir, float deviation);
};


