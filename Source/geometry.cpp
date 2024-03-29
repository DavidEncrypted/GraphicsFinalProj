#include "geometry.h"

#include <cmath>
#include <iostream>



Vector4::Vector4()
{
	e[0] = 0.0f;
	e[1] = 0.0f;
	e[2] = 0.0f;
	e[3] = 1.0f;
}

Vector4::Vector4(float x, float y)
{
	e[0] = x;
	e[1] = y;
	e[2] = 0.0f;
	e[3] = 1.0f;
}

Vector4::Vector4(float x, float y, float z)
{
	e[0] = x;
	e[1] = y;
	e[2] = z;
	e[3] = 1.0f;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	e[0] = x;
	e[1] = y;
	e[2] = z;
	e[3] = w;
}


float & Vector4::x()
{
	return e[0];
}
float & Vector4::y()
{
	return e[1];
}
float & Vector4::z()
{
	return e[2];
}
float & Vector4::w()
{
	return e[3];
}

float Vector4::x() const
{
	return e[0];
}
float Vector4::y() const
{
	return e[1];
}
float Vector4::z() const
{
	return e[2];
}
float Vector4::w() const
{
	return e[3];
}


const float * Vector4::elements() const
{
	return e;
}


Vector4 Vector4::operator+(const Vector4 & v) const
{
	return Vector4(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2], 1.0f);
}

Vector4 Vector4::operator-(const Vector4 & v) const
{
	return Vector4(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2], 1.0f);
}

Vector4 Vector4::operator-() const
{
	return Vector4(-e[0], -e[1], -e[2], e[3]);
}

float Vector4::operator*(const Vector4 & v) const
{
	return (e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2]);
}

Vector4 Vector4::operator^(const Vector4 & v) const
{
	return Vector4(e[1] * v.e[2] - e[2] * v.e[1], e[2] * v.e[0] - e[0] * v.e[2], e[0] * v.e[1] - e[1] * v.e[0], 1.0f);
}

Vector4 Vector4::operator*(float s) const
{
	return Vector4(e[0] * s, e[1] * s, e[2] * s, e[3]);
}

Vector4 Vector4::operator/(float s) const
{
	return Vector4(e[0] / s, e[1] / s, e[2] / s, e[3]);
}

Vector4 & Vector4::operator+=(const Vector4 & v)
{
	return (*this = *this + v);
}

Vector4 & Vector4::operator-=(const Vector4 & v)
{
	return (*this = *this - v);
}

Vector4 & Vector4::operator*=(float s)
{
	return (*this = *this * s);
}

Vector4 & Vector4::operator/=(float s)
{
	return (*this = *this / s);
}


float Vector4::length() const
{
	return sqrt(*this * *this);
}

Vector4 Vector4::normalize() const
{
	return *this / length();
}


Vector4 Vector4::rotateX(float rad) const
{
	return Vector4(e[0], e[1] * cos(rad) - e[2] * sin(rad), e[2] * cos(rad) + e[1] * sin(rad), e[3]);
}
Vector4 Vector4::rotateY(float rad) const
{
	return Vector4(e[0] * cos(rad) + e[2] * sin(rad), e[1], e[2] * cos(rad) - e[0] * sin(rad), e[3]);
}
Vector4 Vector4::rotateZ(float rad) const
{
	return Vector4(e[0] * cos(rad) - e[1] * sin(rad), e[1] * cos(rad) + e[0] * sin(rad), e[2], e[3]);
}
Vector4 Vector4::rotate(const Vector4 & rot) const
{
	return this->rotateX(rot.e[0]).rotateY(rot.e[1]).rotateZ(rot.e[2]);
}



Matrix4::Matrix4()
{
	e[0] = 1.0f; e[1] = 0.0f; e[2] = 0.0f; e[3] = 0.0f;
	e[4] = 0.0f; e[5] = 1.0f; e[6] = 0.0f; e[7] = 0.0f;
	e[8] = 0.0f; e[9] = 0.0f; e[10] = 1.0f; e[11] = 0.0f;
	e[12] = 0.0f; e[13] = 0.0f; e[14] = 0.0f; e[15] = 1.0f;
}


const float * Matrix4::elements() const
{
	return e;
}


Matrix4 Matrix4::operator*(const Matrix4 & m)
{
	Matrix4 rm;

	rm.e[0] = e[0] * m.e[0] + e[1] * m.e[4] + e[2] * m.e[8] + e[3] * m.e[12];
	rm.e[1] = e[0] * m.e[1] + e[1] * m.e[5] + e[2] * m.e[9] + e[3] * m.e[13];
	rm.e[2] = e[0] * m.e[2] + e[1] * m.e[6] + e[2] * m.e[10] + e[3] * m.e[14];
	rm.e[3] = e[0] * m.e[3] + e[1] * m.e[7] + e[2] * m.e[11] + e[3] * m.e[15];
	
	rm.e[4] = e[4] * m.e[0] + e[5] * m.e[4] + e[6] * m.e[8] + e[7] * m.e[12];
	rm.e[5] = e[4] * m.e[1] + e[5] * m.e[5] + e[6] * m.e[9] + e[7] * m.e[13];
	rm.e[6] = e[4] * m.e[2] + e[5] * m.e[6] + e[6] * m.e[10] + e[7] * m.e[14];
	rm.e[7] = e[4] * m.e[3] + e[5] * m.e[7] + e[6] * m.e[11] + e[7] * m.e[15];
	
	rm.e[8] = e[8] * m.e[0] + e[9] * m.e[4] + e[10] * m.e[8] + e[11] * m.e[12];
	rm.e[9] = e[8] * m.e[1] + e[9] * m.e[5] + e[10] * m.e[9] + e[11] * m.e[13];
	rm.e[10] = e[8] * m.e[2] + e[9] * m.e[6] + e[10] * m.e[10] + e[11] * m.e[14];
	rm.e[11] = e[8] * m.e[3] + e[9] * m.e[7] + e[10] * m.e[11] + e[11] * m.e[15];
	
	rm.e[12] = e[12] * m.e[0] + e[13] * m.e[4] + e[14] * m.e[8] + e[15] * m.e[12];
	rm.e[13] = e[12] * m.e[1] + e[13] * m.e[5] + e[14] * m.e[9] + e[15] * m.e[13];
	rm.e[14] = e[12] * m.e[2] + e[13] * m.e[6] + e[14] * m.e[10] + e[15] * m.e[14];
	rm.e[15] = e[12] * m.e[3] + e[13] * m.e[7] + e[14] * m.e[11] + e[15] * m.e[15];

	return rm;
}

Vector4 Matrix4::operator*(const Vector4 & v)
{
	Vector4 rv;

	rv.e[0] = e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2] + e[3] * v.e[3];
	rv.e[1] = e[4] * v.e[0] + e[5] * v.e[1] + e[6] * v.e[2] + e[7] * v.e[3];
	rv.e[2] = e[8] * v.e[0] + e[9] * v.e[1] + e[10] * v.e[2] + e[11] * v.e[3];
	rv.e[3] = e[12] * v.e[0] + e[13] * v.e[1] + e[14] * v.e[2] + e[15] * v.e[3];

	return rv;
}


Matrix4 Matrix4::TranslationMatrix(const Vector4 & v)
{
	Matrix4 rm;

	rm.e[0] = 1.0f; rm.e[1] = 0.0f; rm.e[2] = 0.0f; rm.e[3] = v.e[0];
	rm.e[4] = 0.0f; rm.e[5] = 1.0f; rm.e[6] = 0.0f; rm.e[7] = v.e[1];
	rm.e[8] = 0.0f; rm.e[9] = 0.0f; rm.e[10] = 1.0f; rm.e[11] = v.e[2];
	rm.e[12] = 0.0f; rm.e[13] = 0.0f; rm.e[14] = 0.0f; rm.e[15] = 1.0f;

	return rm;
}

Matrix4 Matrix4::LookAtMatrix(const Vector4 & pos, const Vector4 & rot)
{
	Matrix4 rm;

	Vector4 f = Vector4(1.0f, 0.0f, 0.0f).rotate(rot);
	Vector4 s = (f ^ Vector4(0.0f, 0.0f, 1.0f)).normalize();
	Vector4 u = s ^ f;

	rm.e[0] = s.e[0];  rm.e[1] = s.e[1];  rm.e[2] = s.e[2];   rm.e[3] = 0.0f;
	rm.e[4] = u.e[0];  rm.e[5] = u.e[1];  rm.e[6] = u.e[2];   rm.e[7] = 0.0f;
	rm.e[8] = -f.e[0]; rm.e[9] = -f.e[1]; rm.e[10] = -f.e[2]; rm.e[11] = 0.0f;
	rm.e[12] = 0.0f;   rm.e[13] = 0.0f;   rm.e[14] = 0.0f;    rm.e[15] = 1.0f;

	return rm * TranslationMatrix(-pos);
}

Matrix4 Matrix4::PerspectiveMatrix(float fovy, float aspect, float znear, float zfar)
{
	Matrix4 rm;

	float f = 1.0f / atan(fovy / 2.0f);
	float e10 = (zfar + znear) / (znear - zfar);
	float e11 = (2.0f * zfar * znear) / (znear - zfar);

	rm.e[0] = f / aspect; rm.e[1] = 0.0f;  rm.e[2] = 0.0f;   rm.e[3] = 0.0f;
	rm.e[4] = 0.0f;       rm.e[5] = f;     rm.e[6] = 0.0f;   rm.e[7] = 0.0f;
	rm.e[8] = 0.0f;       rm.e[9] = 0.0f;  rm.e[10] = e10;   rm.e[11] = e11;
	rm.e[12] = 0.0f;      rm.e[13] = 0.0f; rm.e[14] = -1.0f; rm.e[15] = 0.0f;

	return rm;
}

Vector4 Matrix4::RightVector(){
	Vector4 vec = Vector4(e[0], e[1], e[2]);
	return vec;
}

Vector4 Matrix4::UpVector(){
	Vector4 vec = Vector4(e[4], e[5], e[6]);
 	return vec;
}

Vector4 Matrix4::ForwardVector(){
	Vector4 vec = Vector4(e[8], e[9], e[10]);
	return vec;
}