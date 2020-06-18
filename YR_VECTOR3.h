#pragma once
#include<DirectXMath.h>
using namespace DirectX;

class YR_Vector3
{	
public:
	float x;
	float y;
	float z;
public:
	YR_Vector3() :x(0.0f), y(0.0f), z(0.0f) {};
	YR_Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {};
	YR_Vector3(const YR_Vector3 &v) : x(v.x), y(v.y), z(v.z) {};
	YR_Vector3(const XMFLOAT3& v) : x(v.x), y(v.y), z(v.z) {};
	YR_Vector3(float xx, float yy) : x(xx), y(yy), z(0.0f) {};

	YR_Vector3& operator = (const YR_Vector3& R);
	YR_Vector3& operator = (const XMFLOAT3& R);

	YR_Vector3 operator + (const YR_Vector3& R);
	YR_Vector3 operator - (const YR_Vector3& R);

	YR_Vector3 operator + (const XMFLOAT3& R);
	YR_Vector3 operator - (const XMFLOAT3& R);

	YR_Vector3& operator += (const YR_Vector3& R);
	YR_Vector3& operator -= (const YR_Vector3& R);

	YR_Vector3& operator += (const XMFLOAT3& R);
	YR_Vector3& operator -= (const XMFLOAT3& R);

	YR_Vector3 operator + (const float& R);
	YR_Vector3 operator - (const float& R);
	YR_Vector3 operator * (const float& R);
	YR_Vector3 operator / (const float& R);

	YR_Vector3& operator += (const float& R);
	YR_Vector3& operator -= (const float& R);
	YR_Vector3& operator *= (const float& R);
	YR_Vector3& operator /= (const float& R);

	bool operator == (const YR_Vector3& R);
	bool operator == (const XMFLOAT3& R);

	// ‘O’u
	YR_Vector3& operator ++();
	YR_Vector3& operator --();
	// Œã’u
	YR_Vector3 operator ++(int);
	YR_Vector3 operator --(int);

	YR_Vector3& Normalize();

	YR_Vector3 Ortho() const;


	void Set(float xx, float yy);
	void Set(float xx, float yy, float zz);
	void Set(XMFLOAT3 v);

	void Rotate(float deg);

	void Truncate(float length);

	float Distance(const YR_Vector3& R) const;
	float Length() const;

public:
	XMFLOAT3 GetDXFLOAT3()
	{
		return XMFLOAT3(x, y, z);
	}

	XMFLOAT2 GetDXFLOAT2()
	{
		return XMFLOAT2(x, y);
	}
};

	YR_Vector3 operator + (const YR_Vector3& L, const YR_Vector3& R);
	YR_Vector3 operator + (const XMFLOAT3& L, const YR_Vector3& R);
	YR_Vector3 operator + (const YR_Vector3& L, const XMFLOAT3& R);
	YR_Vector3 operator + (const YR_Vector3& L, const float& R);
	YR_Vector3 operator + (const float& L, const YR_Vector3& R);

	YR_Vector3 operator - (const YR_Vector3& L, const YR_Vector3& R);
	YR_Vector3 operator - (const XMFLOAT3& L, const YR_Vector3& R);
	YR_Vector3 operator - (const YR_Vector3& L, const XMFLOAT3& R);
	YR_Vector3 operator - (const YR_Vector3& L, const float& R);
	YR_Vector3 operator - (const float& L, const YR_Vector3& R);

	//Vector2 operator * ( const Vector2	&L, const Vector2	&R );
	YR_Vector3 operator * (const YR_Vector3& L, const float& R);
	YR_Vector3 operator * (const float& L, const YR_Vector3& R);

	//Vector2 operator / ( const Vector2	&L, const Vector2	&R );
	YR_Vector3 operator / (const YR_Vector3& L, const float& R);
	YR_Vector3 operator / (const float& L, const YR_Vector3& R);