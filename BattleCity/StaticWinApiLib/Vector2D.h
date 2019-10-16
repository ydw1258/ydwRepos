#pragma once
template<typename T>
class Vector2D
{
public:
	T x;
	T y;
	
	Vector2D(const Vector2D& _Other)
	{
		x = _Other.x;
		y = _Other.y;
	}
	Vector2D operator + (const Vector2D& _Other)
	{
		return{ x + _Other.x, y + _Other.y };
	}

	Vector2D& operator = (const Vector2D& _Value)
	{
		x = _Value.x;
		y = _Value.y;
		return *this;
	}

	Vector2D& operator = (const POINT& _Value)
	{
		x = (float)_Value.x;
		y = (float)_Value.y;
		return *this;
	}

	Vector2D operator + (const Vector2D& _Value) const
	{
		return Vector2D(x + _Value.x, y + _Value.y);
	}
	Vector2D operator * (const Vector2D& _Value) const
	{
		return Vector2D(x * _Value.x, y * _Value.y);
	}
	Vector2D operator - (const Vector2D& _Value) const
	{
		return Vector2D(x - _Value.x, y - _Value.y);
	}
	Vector2D operator / (const Vector2D& _Value) const
	{
		return Vector2D(x / _Value.x, y / _Value.y);
	}
	Vector2D& operator += (const Vector2D& _Value)
	{
		x += _Value.x;
		y += _Value.y;

		return *this;
	}
	Vector2D& operator -= (const Vector2D& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;

		return *this;
	}
	Vector2D& operator *= (const Vector2D& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;

		return *this;
	}
	Vector2D& operator /= (const Vector2D& _Value)
	{
		x /= _Value.x;
		y /= _Value.y;

		return *this;
	}

	bool operator == (const Vector2D& _Value) const
	{
		return (x == _Value.x && y == _Value.y);
	}
	bool operator != (const Vector2D& _Value) const
	{
		return (x != _Value.x || y != _Value.y);
	}
	bool operator >= (const Vector2D& _Value) const
	{
		return (x >= _Value.x && y >= _Value.y);
	}
	bool operator <= (const Vector2D& _Value) const
	{
		return (x <= _Value.x && y <= _Value.y);
	}
	bool operator > (const Vector2D& _Value) const
	{
		return (x > _Value.x && y > _Value.y);
	}
	bool operator < (const Vector2D& _Value) const
	{
		return (x < _Value.x && y < _Value.y);
	}

	float distance() const
	{
		return sqrtf((x) * (x)+(y) * (y));
	}

	Vector2D& reverse_ref()
	{
		float Tmp = x;
		x = y;
		y = Tmp;

		return *this;
	}
	Vector2D& abs()
	{
		x = fabsf(x);
		y = fabsf(y);
		return *this;
	}

	Vector2D reverse()
	{
		float Tx = x;
		float Ty = y;
		float Tmp = Tx;
		Tx = Ty;
		Ty = Tmp;

		return Vector2D(Tx, Ty);
	}


	Vector2D normalize()
	{
		float Tx = x;
		float Ty = y;
		Tx /= x + y;
		Ty /= x + y;

		return Vector2D(Tx, Ty);
	}

	Vector2D();
	~Vector2D();
};

