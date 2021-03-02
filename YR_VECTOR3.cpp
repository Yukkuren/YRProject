#include "YR_VectorMaster.h"

bool YR_Vector3::operator == (const YR_Vector3& R)
{
	if (x == R.x)
	{
		if (y == R.y)
		{
			if (z == R.z)
			{
				return true;
			}
		}
	}
	return false;
}
bool YR_Vector3::operator == (const XMFLOAT3& R)
{
	if (x == R.x)
	{
		if (y == R.y)
		{
			if (z == R.z)
			{
				return true;
			}
		}
	}
	return false;
}




YR_Vector3& YR_Vector3::operator = (const YR_Vector3& R)
{
	x = R.x;
	y = R.y;
	z = R.z;
	return *this;
}
YR_Vector3& YR_Vector3::operator = (const XMFLOAT3& R)
{
	x = R.x;
	y = R.y;
	z = R.z;
	return *this;
}


YR_Vector3 YR_Vector3::operator + (const YR_Vector3& R)
{
	return YR_Vector3(x + R.x, y + R.y, z + R.z);
}
YR_Vector3 YR_Vector3::operator - (const YR_Vector3& R)
{
	return YR_Vector3(x - R.x, y - R.y, z - R.z);
}
YR_Vector3 YR_Vector3::operator + (const XMFLOAT3& R)
{
	return YR_Vector3(x + R.x, y + R.y, z + R.z);
}
YR_Vector3 YR_Vector3::operator - (const XMFLOAT3& R)
{
	return YR_Vector3(x - R.x, y - R.y, z - R.z);
}


YR_Vector3& YR_Vector3::operator += (const YR_Vector3& R)
{
	x += R.x;
	y += R.y;
	z += R.z;
	return *this;
}
YR_Vector3& YR_Vector3::operator -= (const YR_Vector3& R)
{
	x -= R.x;
	y -= R.y;
	z -= R.z;
	return *this;
}
YR_Vector3& YR_Vector3::operator += (const XMFLOAT3& R)
{
	x += R.x;
	y += R.y;
	z += R.z;
	return *this;
}
YR_Vector3& YR_Vector3::operator -= (const XMFLOAT3& R)
{
	x -= R.x;
	y -= R.y;
	z -= R.z;
	return *this;
}


YR_Vector3 YR_Vector3::operator + (const float& R)
{
	return YR_Vector3(x + R, y + R, z + R);
}
YR_Vector3 YR_Vector3::operator - (const float& R)
{
	return YR_Vector3(x - R, y - R, z - R);
}
YR_Vector3 YR_Vector3::operator * (const float& R)
{
	return YR_Vector3(x * R, y * R, z * R);
}
YR_Vector3 YR_Vector3::operator / (const float& R)
{
	return YR_Vector3(x / R, y / R, z / R);
}

YR_Vector3& YR_Vector3::operator += (const float& R)
{
	x += R;
	y += R;
	z += R;
	return *this;
}
YR_Vector3& YR_Vector3::operator -= (const float& R)
{
	x -= R;
	y -= R;
	z -= R;
	return *this;
}
YR_Vector3& YR_Vector3::operator *= (const float& R)
{
	x *= R;
	y *= R;
	z *= R;
	return *this;
}
YR_Vector3& YR_Vector3::operator /= (const float& R)
{
	x /= R;
	y /= R;
	z /= R;
	return *this;
}

// �O�u
YR_Vector3& YR_Vector3::operator ++()
{
	++x;
	++y;
	++z;
	return *this;
}
YR_Vector3& YR_Vector3::operator --()
{
	--x;
	--y;
	--z;
	return *this;
}
// ��u
YR_Vector3 YR_Vector3::operator ++(int)		// int�^�łȂ���΂Ȃ�Ȃ��C�u��u'operator++'�̗]���ȃp�����[�^�[��'int'�^�ł���K�v������܂��v�Əo��
{
	const YR_Vector3 tmp = *this;
	++(*this);					//��L�̑O�u�ł��g���Ď������邱�ƂŁC�������e�̐H���Ⴂ������C�ێ琫�����コ����
	return tmp;					//�ԋp�p�̃C���X�^���X�𐶐����C�����Ԃ����ƂɂȂ�̂ŁC�߂�l�͎��̂ƂȂ�
								//�G���[���b�Z�[�W�F�u�^ "YR_Vector3 &" �̎Q�Ƃ��^ "const YR_Vector3" �̏������q�Ƀo�C���h����Ƃ��ɏC���q���h���b�v����܂����v
}
YR_Vector3 YR_Vector3::operator --(int)
{
	const YR_Vector3 tmp = *this;
	--(*this);
	return tmp;
}

YR_Vector3& YR_Vector3::Normalize()
{
	float len = Length();

	// 0 ��������C���̂܂ܕԂ�
	if (-1.192092896e-07F < len && len < 1.192092896e-07F)
	{
		return *this;
	}

	// �����̋t����������
	*this *= (1.0f / len);
	return *this;
}

YR_Vector3 YR_Vector3::Ortho() const
{
	return YR_Vector3(y, -x);
}

void YR_Vector3::Set(float xx, float yy)
{
	x = xx;
	y = yy;
}
void YR_Vector3::Set(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}
void YR_Vector3::Set(XMFLOAT3 v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}


void YR_Vector3::Rotate(float deg)
{
	float theta = deg / 180 * 3.14159265359f;
	float c = cosf(theta);
	float s = sinf(theta);

	float tx = (x * c) - (y * s);
	float ty = (x * s) + (y * c);

	x = tx;
	y = ty;
}

void YR_Vector3::Truncate(float length)
{
	float angle = atan2f(y, x);

	x = length * cosf(angle);
	y = length * sinf(angle);
}

float YR_Vector3::Distance(const YR_Vector3& R) const
{
	YR_Vector3 d(R.x - x, R.y - y, R.z - z);
	return d.Length();
}
float YR_Vector3::Length() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

YR_Vector3 operator + (const YR_Vector3& L, const YR_Vector3& R)
{
	return YR_Vector3(L.x + R.x, L.y + R.y, L.z + R.z);
}
YR_Vector3 operator + (const XMFLOAT3& L, const YR_Vector3& R)
{
	return YR_Vector3(L.x + R.x, L.y + R.y, L.z + R.z);
}
YR_Vector3 operator + (const YR_Vector3& L, const XMFLOAT3& R)
{
	return YR_Vector3(L.x + R.x, L.y + R.y, L.z + R.z);
}
YR_Vector3 operator + (const YR_Vector3& L, const float& R)
{
	return YR_Vector3(L.x + R, L.y + R, L.z + R);
}
YR_Vector3 operator + (const float& L, const YR_Vector3& R)
{
	return YR_Vector3(L + R.x, L + R.y, L + R.z);
}



YR_Vector3 operator - (const YR_Vector3& L, const YR_Vector3& R)
{
	return YR_Vector3(L.x - R.x, L.y - R.y, L.z - R.z);
}
YR_Vector3 operator - (const XMFLOAT3& L, const YR_Vector3& R)
{
	return YR_Vector3(L.x - R.x, L.y - R.y, L.z - R.z);
}
YR_Vector3 operator - (const YR_Vector3& L, const XMFLOAT3& R)
{
	return YR_Vector3(L.x - R.x, L.y - R.y, L.z - R.z);
}
YR_Vector3 operator - (const YR_Vector3& L, const float& R)
{
	return YR_Vector3(L.x - R, L.y - R, L.z - R);
}
YR_Vector3 operator - (const float& L, const YR_Vector3& R)
{
	return YR_Vector3(L - R.x, L - R.y, L - R.z);
}



YR_Vector3 operator * (const YR_Vector3& L, const float& R)
{
	return YR_Vector3(L.x * R, L.y * R, L.z * R);
}
YR_Vector3 operator * (const float& L, const YR_Vector3& R)
{
	return YR_Vector3(L * R.x, L * R.y, L * R.z);
}
YR_Vector3 operator / (const YR_Vector3& L, const float& R)
{
	return YR_Vector3(L.x / R, L.y / R, L.z / R);
}
YR_Vector3 operator / (const float& L, const YR_Vector3& R)
{
	return YR_Vector3(L / R.x, L / R.y, L / R.z);
}