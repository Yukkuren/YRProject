#pragma once
#include <DirectXMath.h>
#include <array>
#include "YR_VectorMaster.h"

//------------------------------------------------
//		�P��K�E�X�ڂ����u���[���p�p�����[�^�[
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

	//YR_Vector3��xy�̒l�̂ݎg�p����
	void CalcBlurParam(float width, float height, YR_Vector3 dir, float deviation);
};


