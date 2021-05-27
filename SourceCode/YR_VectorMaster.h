#pragma once

#include<DirectXMath.h>
#include <cereal/cereal.hpp>
using namespace DirectX;


//既存のXMFLOAT型を使いやすくするためにラップした構造体
//GetDXFLOAT関数でDirectXの関数に対応できる
struct YR_Vector3
{
public:
	float x;
	float y;
	float z;
public:
	YR_Vector3() :x(0.0f), y(0.0f), z(0.0f) {};
	YR_Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {};
	YR_Vector3(const YR_Vector3& v) : x(v.x), y(v.y), z(v.z) {};
	YR_Vector3(const XMFLOAT3& v) : x(v.x), y(v.y), z(v.z) {};
	YR_Vector3(float xx, float yy) : x(xx), y(yy), z(0.0f) {};
	explicit YR_Vector3(_In_reads_(3) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}


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

	// 前置
	YR_Vector3& operator ++();
	YR_Vector3& operator --();
	// 後置
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
	const XMFLOAT3 GetDXFLOAT3() const
	{
		return XMFLOAT3(x, y, z);
	}

	XMFLOAT2 GetDXFLOAT2()
	{
		return XMFLOAT2(x, y);
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z));
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


struct YR_Vector4 : public XMFLOAT4
{
public:

	constexpr YR_Vector4() : XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {};
	constexpr YR_Vector4(XMFLOAT4 vec) : XMFLOAT4(vec) {};
	constexpr YR_Vector4(float xx, float yy, float zz, float ww) : XMFLOAT4(xx, yy, zz, ww) {};

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w));
	}
};

struct YR_Vector2 : public XMFLOAT2
{
public:
	constexpr YR_Vector2() : XMFLOAT2(0.0f,0.0f) {};
	constexpr YR_Vector2(XMFLOAT2 vec) : XMFLOAT2(vec) {};
	constexpr YR_Vector2(float xx,float yy) : XMFLOAT2(xx, yy) {};

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(x), CEREAL_NVP(y));
	}
};

struct YR_Vector4X4 : public XMFLOAT4X4
{
public:
	constexpr YR_Vector4X4() : XMFLOAT4X4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f) {};
	constexpr YR_Vector4X4(XMFLOAT4X4 vec) : XMFLOAT4X4(vec) {};

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(_11), CEREAL_NVP(_12), CEREAL_NVP(_13), CEREAL_NVP(_14),
			CEREAL_NVP(_21), CEREAL_NVP(_22), CEREAL_NVP(_23), CEREAL_NVP(_24),
			CEREAL_NVP(_31), CEREAL_NVP(_32), CEREAL_NVP(_33), CEREAL_NVP(_34),
			CEREAL_NVP(_41), CEREAL_NVP(_42), CEREAL_NVP(_43), CEREAL_NVP(_44));
	}
};

struct YR_INT4 : public XMINT4
{
public:
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z), CEREAL_NVP(w));
	}
};
