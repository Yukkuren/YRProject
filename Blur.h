#pragma once
#include <DirectXMath.h>
#include <array>
#include "YR_VectorMaster.h"

//------------------------------------------------
//		単一ガウスぼかしブルーム用パラメーター
//------------------------------------------------



class GaussParamManager
{
private:
	float GaussianDistribution(const YR_Vector3& pos, float rho);
public:
	struct GaussBlurParam
	{
		int					SampleCount;
		int					Dummy[3];
		DirectX::XMFLOAT4	Offset[16];
	};
	GaussBlurParam param;

	//YR_Vector3はxyの値のみ使用する
	void CalcBlurParam(float width, float height, YR_Vector3 dir, float deviation);
};


