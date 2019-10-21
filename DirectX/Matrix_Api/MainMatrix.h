#pragma once
#include <mmsystem.h>

struct MVector4
{
	union
	{
		float s[4];

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};


		struct
		{
			float a1;
			float a2;
			float a3;
			float a4;
		};
	};
};

struct MMatrix
{
	union
	{
		struct
		{
			MVector4 V1;
			MVector4 V2;
			MVector4 V3;
			MVector4 V4;
		};

		float m[4][4];
		float s[16];
	};


	MMatrix operator*(const MMatrix& _Other)
	{
		MMatrix Tmp;

		Tmp.V1.x = (V1.x * _Other.V1.x + V1.y * _Other.V2.x + V1.z * _Other.V3.x + V1.w * _Other.V4.x);
		Tmp.V1.y = (V1.x * _Other.V1.y + V1.y * _Other.V2.y + V1.z * _Other.V3.y + V1.w * _Other.V4.y);
		Tmp.V1.z = (V1.x * _Other.V1.z + V1.y * _Other.V2.z + V1.z * _Other.V3.z + V1.w * _Other.V4.z);
		Tmp.V1.w = (V1.x * _Other.V1.w + V1.y * _Other.V2.w + V1.z * _Other.V3.w + V1.w * _Other.V4.w);

		Tmp.V2.x = (V2.x * _Other.V1.x + V2.y * _Other.V2.x + V2.z * _Other.V3.x + V2.w * _Other.V4.x);
		Tmp.V2.y = (V2.x * _Other.V1.y + V2.y * _Other.V2.y + V2.z * _Other.V3.y + V2.w * _Other.V4.y);
		Tmp.V2.z = (V2.x * _Other.V1.z + V2.y * _Other.V2.z + V2.z * _Other.V3.z + V2.w * _Other.V4.z);
		Tmp.V2.w = (V2.x * _Other.V1.w + V2.y * _Other.V2.w + V2.z * _Other.V3.w + V2.w * _Other.V4.w);

		Tmp.V3.x = (V3.x * _Other.V1.x + V3.y * _Other.V2.x + V3.z * _Other.V3.x + V3.w * _Other.V4.x);
		Tmp.V3.y = (V3.x * _Other.V1.y + V3.y * _Other.V2.y + V3.z * _Other.V3.y + V3.w * _Other.V4.y);
		Tmp.V3.z = (V3.x * _Other.V1.z + V3.y * _Other.V2.z + V3.z * _Other.V3.z + V3.w * _Other.V4.z);
		Tmp.V3.w = (V3.x * _Other.V1.w + V3.y * _Other.V2.w + V3.z * _Other.V3.w + V3.w * _Other.V4.w);

		Tmp.V4.x = (V4.x * _Other.V1.x + V4.y * _Other.V2.x + V4.z * _Other.V3.x + V4.w * _Other.V4.x);
		Tmp.V4.y = (V4.x * _Other.V1.y + V4.y * _Other.V2.y + V4.z * _Other.V3.y + V4.w * _Other.V4.y);
		Tmp.V4.z = (V4.x * _Other.V1.z + V4.y * _Other.V2.z + V4.z * _Other.V3.z + V4.w * _Other.V4.z);
		Tmp.V4.w = (V4.x * _Other.V1.w + V4.y * _Other.V2.w + V4.z * _Other.V3.w + V4.w * _Other.V4.w);

		return Tmp;
	}



	MVector4 operator*(const MVector4 & _Other)
	{
		MVector4 Tmp;

		Tmp.x = (V1.x * _Other.x + V2.x * _Other.x + V3.x * _Other.x + V4.x * _Other.x);
		Tmp.y = (V1.y * _Other.y + V2.y * _Other.y + V3.y * _Other.y + V4.y * _Other.y);
		Tmp.z = (V1.z * _Other.z + V2.z * _Other.z + V3.z * _Other.z + V4.z * _Other.z);
		Tmp.w = (V1.w * _Other.w + V2.w * _Other.w + V3.w * _Other.w + V4.w * _Other.w);

		return Tmp;
	}


	MMatrix Translation(const MVector4 & _Other)
	{
		this->V1.x = 1.0f;
		this->V1.y = .0f;
		this->V1.z = .0f;
		this->V1.w = .0f;

		this->V2.x = .0f;
		this->V2.y = 1.0f;
		this->V2.z = .0f;
		this->V2.w = .0f;

		this->V3.x = .0f;
		this->V3.y = .0f;
		this->V3.z = 1.0f;
		this->V3.w = .0f;

		this->V4.x = _Other.x;
		this->V4.y = _Other.y;
		this->V4.z = _Other.z;
		this->V4.w = 1.0f;

		return *this;
	}

	MMatrix RotX(const float& _Angle)
	{
		this->V1.x = 1.0f;
		this->V1.y = .0f;
		this->V1.z = .0f;
		this->V1.w = .0f;

		this->V2.x = .0f;
		this->V2.y = cos(_Angle);
		this->V2.z = -sin(_Angle);
		this->V2.w = .0f;

		this->V3.x = .0f;
		this->V3.y = sin(_Angle);
		this->V3.z = cos(_Angle);
		this->V3.w = .0f;

		this->V4.x = .0f;
		this->V4.y = .0f;
		this->V4.z = .0f;
		this->V4.w = 1.0f;

		return *this;
	}

	MMatrix RotY(const float& _Angle)
	{
		this->V1.x = cos(_Angle);
		this->V1.y = .0f;
		this->V1.z = sin(_Angle);
		this->V1.w = .0f;

		this->V2.x = .0f;
		this->V2.y = 1.0f;
		this->V2.z = .0f;
		this->V2.w = .0f;

		this->V3.x = -sin(_Angle);
		this->V3.y = .0f;
		this->V3.z = cos(_Angle);
		this->V3.w = .0f;

		this->V4.x = .0f;
		this->V4.y = .0f;
		this->V4.z = .0f;
		this->V4.w = 1.0f;

		return *this;
	}

	MMatrix RotZ(const float& _Angle)
	{
		this->V1.x = cos(_Angle);
		this->V1.y = -sin(_Angle);
		this->V1.z = .0f;
		this->V1.w = .0f;

		this->V2.x = sin(_Angle);
		this->V2.y = cos(_Angle);
		this->V2.z = .0f;
		this->V2.w = .0f;

		this->V3.x = .0f;
		this->V3.y = .0f;
		this->V3.z = 1.0f;
		this->V3.w = .0f;

		this->V4.x = .0f;
		this->V4.y = .0f;
		this->V4.z = .0f;
		this->V4.w = 1.0f;

		return *this;
	}

	MMatrix Scale(const MVector4 & _Other)
	{
		this->V1.x = _Other.x;
		this->V1.y = .0f;
		this->V1.z = .0f;
		this->V1.w = .0f;

		this->V2.x = .0f;
		this->V2.y = _Other.y;
		this->V2.z = .0f;
		this->V2.w = .0f;

		this->V3.x = .0f;
		this->V3.y = .0f;
		this->V3.z = _Other.z;
		this->V3.w = .0f;

		this->V4.x = .0f;
		this->V4.y = .0f;
		this->V4.z = .0f;
		this->V4.w = 1.0f;

		return *this;
	}

	void identity()
	{
		this->V1.x = 1.0f;
		this->V1.y = .0f;
		this->V1.z = .0f;
		this->V1.w = .0f;

		this->V2.x = .0f;
		this->V2.y = 1.0f;
		this->V2.z = .0f;
		this->V2.w = .0f;

		this->V3.x = .0f;
		this->V3.y = .0f;
		this->V3.z = 1.0f;
		this->V3.w = .0f;

		this->V4.x = .0f;
		this->V4.y = .0f;
		this->V4.z = .0f;
		this->V4.w = 1.0f;
	}

	void proj(const float& _Width, const float& _Height, const float& _Near, const float& _Far)
	{
		this->V1.x = (2 * _Near) / _Width;
		this->V1.y = .0f;
		this->V1.z = .0f;
		this->V1.w = .0f;

		this->V2.x = .0f;
		this->V2.y = (2 * _Near) / _Height;
		this->V2.z = .0f;
		this->V2.w = .0f;

		this->V3.x = .0f;
		this->V3.y = .0f;
		this->V3.z = (_Far) / (_Far - _Near);
		this->V3.w = 1.0f;

		this->V4.x = .0f;
		this->V4.y = .0f;
		this->V4.z = (_Far * _Near) / (_Far - _Near) * (-1.0f);
		this->V4.w = .0f;
	}
};