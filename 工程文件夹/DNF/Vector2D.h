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
	//�ƿ�
	void Zero()
	{
		x = y = 0.0f;
	}
	//����
	float Length() const
	{
		return sqrt(x * x + y * y);
	}

	//��λ��-�õ���λ�����������ķ���
	CVector2D Normalize()
	{
		float len = sqrt(x * x + y * y);
		assert(!_IS_ZERO(len));
		CVector2D v(x / len , y / len);
		return v;
	}
	//�����ӷ�
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
	//��������
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
	//������ 
	CVector2D operator - ()
	{
		CVector2D v(-x,-y);
		return v;
	}
	//���������� (���� * ���� -������)
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
	//���������� (���� / ���� -������)
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
	//�����ĵ�� (���� * ����-������)
	float operator * (const CVector2D& that)
	{
		return x * that.x + y * that.y;
	}

	//ͶӰ//this��that�ϵĵ�ͶӰ
	CVector2D Shadow(const CVector2D& that)
	{
		//this��that�ĵ��
		float f1 = x * that.x + y * that.y;
		//this�ĳ���
		//float f2 = sqrt(x * x + y * y);
		float f2 = that.Length();
		f2 *= f2;
		float f3 = f1 / f2;
		//CVector2D v(x * f3, y * f3);
		CVector2D v(that.x * f3, that.y * f3);
		return v;
	}
	//˳ʱ������
	CVector2D Clockwize()
	{
		CVector2D v(y,-x);
		return v;
	}
	//��ʱ������
	CVector2D CounterClockwize()
	{
		CVector2D v(-y,x);
		return v;
	}
};

//���� * ����
inline CVector2D operator * (float f,CVector2D v)
{
	CVector2D u(f * v.x ,f * v.y);
	return u;
}

#endif