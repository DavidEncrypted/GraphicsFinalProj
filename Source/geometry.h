#pragma once


class Vector4;
class Matrix4;


class Vector4
{
friend class Matrix4;
public:
	Vector4();
	Vector4(float x, float y);
	Vector4(float x, float y, float z);
	Vector4(float x, float y, float z, float w);

	float & x();
	float & y();
	float & z();
	float & w();

	float x() const;
	float y() const;
	float z() const;
	float w() const;

	const float * elements() const;

	Vector4 operator+(const Vector4 & v) const;
	Vector4 operator-(const Vector4 & v) const;
	Vector4 operator-() const;
	float operator*(const Vector4 & v) const;
	Vector4 operator^(const Vector4 & v) const;
	Vector4 operator*(float s) const;
	Vector4 operator/(float s) const;
	
	Vector4 & operator+=(const Vector4 & v);
	Vector4 & operator-=(const Vector4 & v);
	Vector4 & operator*=(float s);
	Vector4 & operator/=(float s);

	float length() const;
	Vector4 normalize() const;

	Vector4 rotateX(float rad) const;
	Vector4 rotateY(float rad) const;
	Vector4 rotateZ(float rad) const;
	Vector4 rotate(const Vector4 & rot) const;

private:
	float e[4];
};


class Matrix4
{
public:
	Matrix4();

	const float * elements() const;

	Matrix4 operator*(const Matrix4 & m);
	
	Vector4 operator*(const Vector4 & v);

	static Matrix4 TranslationMatrix(const Vector4 & v);
	
	static Matrix4 LookAtMatrix(const Vector4 & pos, const Vector4 & rot);

	static Matrix4 PerspectiveMatrix(float fovy, float aspect, float znear, float zfar);

	Vector4 RightVector();
	Vector4 UpVector();
	Vector4 ForwardVector();

private:
	float e[16];
};
