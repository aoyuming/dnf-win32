#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_

#include <math.h>
#include <assert.h>

#define _IS_ZERO(n)  ((n) > -0.000000001f && (n) < 0.000000001f)

class CVector2D
{
public:
	float x,y;
	CVector2D(float X = 0.0f,float Y = 0.0f)
		:
	x(X),
	y(Y)
	{}
	//制空
	void Zero()
	{
		x = y = 0.0f;
	}
	//长度
	float Length() const
	{
		return sqrt(x * x + y * y);
	}

	//单位化-得到单位向量即向量的方向
	CVector2D Normalize()
	{
		float len = sqrt(x * x + y * y);
		assert(!_IS_ZERO(len));
		CVector2D v(x / len , y / len);
		return v;
	}
	//向量加法
	CVector2D operator + (const CVector2D& that)
	{
		CVector2D v(x + that.x, y + that.y);
		return v;
	}

	CVector2D& operator += (const CVector2D& that)
	{
		x += that.x;
		y += that.y;
		return *this;
	}
	//向量减法
	CVector2D operator - (const CVector2D& that)
	{
		CVector2D v(x - that.x, y - that.y);
		return v;
	}

	CVector2D& operator -= (const CVector2D& that)
	{
		x -= that.x;
		y -= that.y;
		return *this;
	}
	//逆向量 
	CVector2D operator - ()
	{
		CVector2D v(-x,-y);
		return v;
	}
	//向量的数乘 (向量 * 标量 -》向量)
	CVector2D operator * (float f)
	{
		CVector2D v(x * f, y * f);
		return v;
	}

	CVector2D& operator *= (float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	//向量的数除 (向量 / 标量 -》向量)
	CVector2D operator / (float f)
	{
		assert(!_IS_ZERO(f));
		CVector2D v(x / f, y / f);
		return v;
	}

	CVector2D& operator /= (float f)
	{
		assert(!_IS_ZERO(f));
		x /= f;
		y /= f;
		return *this;
	}
	//向量的点乘 (向量 * 向量-》标量)
	float operator * (const CVector2D& that)
	{
		return x * that.x + y * that.y;
	}

	//投影//this在that上的的投影
	CVector2D Shadow(const CVector2D& that)
	{
		//this和that的点乘
		float f1 = x * that.x + y * that.y;
		//this的长度
		//float f2 = sqrt(x * x + y * y);
		float f2 = that.Length();
		f2 *= f2;
		float f3 = f1 / f2;
		//CVector2D v(x * f3, y * f3);
		CVector2D v(that.x * f3, that.y * f3);
		return v;
	}
	//顺时针向量
	CVector2D Clockwize()
	{
		CVector2D v(y,-x);
		return v;
	}
	//逆时针向量
	CVector2D CounterClockwize()
	{
		CVector2D v(-y,x);
		return v;
	}
};

//标量 * 向量
inline CVector2D operator * (float f,CVector2D v)
{
	CVector2D u(f * v.x ,f * v.y);
	return u;
}

#endif