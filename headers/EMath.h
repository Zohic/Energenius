#pragma once

#include <math.h> 

const float EPI = 3.14159265f;

struct IVec2d
{
	int x, y;
	IVec2d()
	{
		x = y = 0;
	}
	IVec2d(int ix, int iy)
	{
		x = ix;
		y = iy;
	}
};

struct Vec2d
{
	float x, y;

	Vec2d()
	{
		x = y = 0;
	}

	Vec2d(float v)
	{
		x = y = v;
	}

	Vec2d(float ix, float iy)
	{
		x = ix;
		y = iy;
	}

	operator IVec2d() const
	{
		return IVec2d((int)x, (int)y);
	}

	Vec2d operator +(Vec2d v)
	{
		return Vec2d(x + v.x, y + v.y);
	}
	Vec2d operator -(Vec2d v)
	{
		return Vec2d(x - v.x, y - v.y);
	}
	Vec2d operator *(float v)
	{
		return Vec2d(x * v, y * v);
	}
	Vec2d operator /(float v)
	{
		return Vec2d(x / v, y / v);
	}

	Vec2d operator -()
	{
		return Vec2d(-x, -y);
	}

	bool operator ==(Vec2d v)
	{
		return (x == v.x) && (y == v.y);
	}

	void operator +=(Vec2d v)
	{
		x += v.x;
		y += v.y;
	}
	void operator -=(Vec2d v)
	{
		x -= v.x;
		y -= v.y;
	}
	void operator *=(float v)
	{
		x *= v;
		y *= v;
	}
	void operator /=(float v)
	{
		x /= v;
		y /= v;
	}

	float operator *(Vec2d v)// dot product
	{
		return x * v.x + y * v.y;
	}


	float operator &(Vec2d v)// cross product with vec
	{
		return x * v.y - y * v.y;
	}

	Vec2d operator &(float v)// cross product with float
	{
		return Vec2d(y * v, -x * v);
	}

	// (vec   X float) ^
	// (float X vec) = -(vec X float)


	float Length()
	{
		return sqrtf(x*x+y*y);
	}

	float sLength()
	{
		return x * x + y * y;
	}

	void Normalize()
	{
		float l = Length();
		if (l != 0)
		{
			x = x / l;
			y = y / l;
		}
	}

	Vec2d Normalized()
	{
		float l = Length();
		if(l!=0)
			return Vec2d(x / l, y / l);
		else
			return Vec2d(0, 0);
	}

	Vec2d GetNormal()
	{
		return Vec2d(-y, x);
	}

};

static float DotProduct(Vec2d a, Vec2d b)
{
	return a.x * b.x + a.y * b.y;
}

static float CrossProduct(Vec2d a, Vec2d b)//actually returns z value of perpendicular vector as if it was a 3d crossproduct
{
	return a.x * b.y - a.y * b.y;
}

static Vec2d CrossProduct(Vec2d a, float v)// returns a perpendicular to the given vector multiplied by value
{
	return Vec2d(a.y * v, -a.x * v);
}

static Vec2d CrossProduct(float v, Vec2d a)// returns the same but in opposite direction
{
	return Vec2d(-a.y * v, a.x * v);
}



struct Mat22
{
	float mat[2][2];
	Mat22()
	{
		mat[0][0] = mat[0][1] = mat[1][0] = mat[1][1] = 0;
	}
	Mat22(float m1, float m2, float m3, float m4)
	{
		mat[0][0] = m1;
		mat[0][1] = m2;
		mat[1][0] = m3;
		mat[1][1] = m4;
	}
	Mat22(Vec2d v1, Vec2d v2)
	{
		mat[0][0] = v1.x; mat[0][1] = v1.y;
		mat[1][0] = v2.x; mat[1][1] = v2.y;
	}

	static Mat22 RotationMat(float a)
	{
		float s = sin(a);
		float c = cos(a);
		return Mat22(
			c, -s,
			s, c);
	}

	Vec2d operator *(Vec2d v)
	{
		return Vec2d(v.x * mat[0][0] + v.y * mat[0][1], v.x * mat[1][0] + v.y * mat[1][1]);
	}
	IVec2d operator *(IVec2d v)
	{
		return IVec2d(v.x * mat[0][0] + v.y * mat[0][1], v.x * mat[1][0] + v.y * mat[1][1]);
	}
};

struct Mat33
{
	float mat[3][3];
	Mat33()
	{
		mat[0][0] = mat[0][1] = mat[0][2] = 0;
		mat[1][0] = mat[1][1] = mat[1][2] = 0;
		mat[2][0] = mat[2][1] = mat[2][2] = 0;
	}
	Mat33(float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8,float m9)
	{
		mat[0][0] = m1;	mat[0][1] = m2;	mat[0][2] = m3;
		mat[1][0] = m4;	mat[1][1] = m5;	mat[1][2] = m6;
		mat[2][0] = m7;	mat[2][1] = m8;	mat[2][2] = m9;

	}

	static Mat33 Identity()
	{
		return Mat33(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1);
	}

	static Mat33 TranslateMat2D(float tx,float ty)
	{
		return Mat33(
			1, 0, tx,
			0, 1, ty,
			0, 0,  1);
	}
	static Mat33 RotationMat2D(float a)
	{
		float s = sin(a);
		float c = cos(a);
		return Mat33(
			c, -s, 0,
			s,  c, 0,
			0,  0, 1);
	}
	static Mat33 ScaleMat2D(float sx, float sy)
	{
		return Mat33(
			sx, 0, 0,
			0, sy, 0,
			0, 0,  1);
	}
	static Mat33 ShearMat2D(float shx, float shy)
	{
		return Mat33(
			  1, shx, 0,
			shy,   1, 0,
			  0,   0, 1);
	}


	float GetDet()
	{
		return //constant first row
			mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
			mat[0][1] * (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]) +
			mat[0][2] * (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]);
	}

	Mat33 GetInverse()
	{
		Mat33 out;
		float idet = 1.0f / GetDet();
		out.mat[0][0] =  (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) * idet;
		out.mat[0][1] = -(mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2]) * idet;
		out.mat[0][2] =  (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]) * idet;

		out.mat[1][0] = -(mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]) * idet;
		out.mat[1][1] =  (mat[0][0] * mat[2][2] - mat[2][0] * mat[0][2]) * idet;
		out.mat[1][2] = -(mat[0][0] * mat[1][2] - mat[1][0] * mat[0][2]) * idet;

		out.mat[2][0] =  (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]) * idet;
		out.mat[2][1] = -(mat[0][0] * mat[2][1] - mat[2][0] * mat[0][1]) * idet;
		out.mat[2][2] =  (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]) * idet;

		return out;
	}

	Mat33 operator *(Mat33 b)
	{
		Mat33 out;
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
			{
				out.mat[r][c] = mat[r][0] * b.mat[0][c] + mat[r][1] * b.mat[1][c] + mat[r][2] * b.mat[2][c];
			}
		return out;
	}



	Vec2d operator *(Vec2d v)
	{
		return Vec2d(v.x * mat[0][0] + v.y * mat[0][1] + mat[0][2], v.x * mat[1][0] + v.y * mat[1][1] + mat[1][2]);
	}
	IVec2d operator *(IVec2d v)
	{
		return IVec2d(v.x * mat[0][0] + v.y * mat[0][1] + mat[0][2], v.x * mat[1][0] + v.y * mat[1][1] + mat[1][2]);
	}
};

static int esign(float x)
{
	return (int)(x > 0) - (int)(x < 0);
}