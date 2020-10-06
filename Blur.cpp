#include "Blur.h"

//------------------------------------------
//		ガウスの重みを計算
//------------------------------------------

float GaussParamManager::GaussianDistribution(const YR_Vector3& pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}

//------------------------------------------
//		ブラーパラメーターの計算
//------------------------------------------

void GaussParamManager::CalcBlurParam(float width, float height, YR_Vector3 dir, float deviation)
{
	param.SampleCount = 15;
	auto tu = 1.0f / width;
	auto tv = 1.0f / height;

	param.Offset[0].z = GaussianDistribution(YR_Vector3(0.0f, 0.0f), deviation);
	auto total_weight = param.Offset[0].z;

	param.Offset[0].x = 0.0f;
	param.Offset[0].y = 0.0f;

	for (auto i = 1; i < 8; ++i)
	{
		param.Offset[i].x = dir.x * i * tu;
		param.Offset[i].y = dir.y * i * tv;
		param.Offset[i].z = GaussianDistribution(dir * float(i), deviation);
		total_weight += param.Offset[i].z * 2.0f;
	}

	for (auto i = 0; i < 8; ++i)
	{
		param.Offset[i].z /= total_weight;
	}

	for (auto i = 8; i < 15; ++i)
	{
		param.Offset[i].x = -param.Offset[i - 7].x;
		param.Offset[i].y = -param.Offset[i - 7].y;
		param.Offset[i].z = -param.Offset[i - 7].z;
	}
}