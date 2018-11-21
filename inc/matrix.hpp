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
	Matrix operator*(const Matrix& other) const;
};

Matrix calcRotationMatrix(float angle, const Vec3& axis);

#endif
