/*
matrix.hpp
Eric Roberts
*/

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <GL/gl.h>
#include "vec3.hpp"

class Matrix {
public:
	GLfloat data[16];
	
	Matrix();
	GLfloat& operator[](int index) { return data[index]; }
	GLfloat operator[](int index) const { return data[index]; }
	friend Matrix operator*(const Matrix& m1, const Matrix& m2);
	friend Vec3 operator*(const Vec3& v, const Matrix& m);
	friend Vec3 operator*(const Matrix& m, const Vec3& v);
};

Matrix calcRotationMatrix(float angle, const Vec3& axis);

#endif
