/*
matrix.cpp
Eric Roberts
*/

#include <math.h>
#include "matrix.hpp"
#include "vec3.hpp"

Matrix::Matrix() {
	for(int i = 0; i < 16; i++) {
		data[i] = (i%5 == 0) ? 1.0f : 0.0f;
	}
}

Matrix operator*(const Matrix& m1, const Matrix& m2) {
	Matrix result;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			GLfloat sum = 0;
			for(int k = 0; k < 4; k++) {
				sum += m1.data[i*4 + k] * m2.data[k*4 + j];
			}
			result[i*4 + j] = sum;
		}
	}
	return result;
}

Vec3 operator*(const Vec3& v, const Matrix& m) {
	Vec3 result;
	for(int j = 0; j < 3; j++) {
		GLfloat sum = 0;
		for(int k = 0; k < 3; k++) {
			sum += v[k] * m.data[k*4 + j];
		}
		result[j] = sum;
	}
	return result;
}

Vec3 operator*(const Matrix& m, const Vec3& v) {
	Vec3 result;
	for(int i = 0; i < 3; i++) {
		GLfloat sum = 0;
		for(int k = 0; k < 3; k++) {
			sum += m.data[i*4 + k] * v[k];
		}
		result[i] = sum;
	}
	return result;
}

Matrix calcRotationMatrix(float angle, const Vec3& axis) {
	Matrix matrix;
	float cos = std::cos(angle);
	float omcos = 1.0 - cos;
	float sin = std::sin(angle);
	
	matrix[0] = cos + axis.x * axis.x * omcos;
	matrix[1] = axis.y * axis.x * omcos + axis.z * sin;
	matrix[2] = axis.z * axis.x * omcos - axis.y * sin;
	matrix[3] = 0.0;
	
	matrix[4] = axis.x * axis.y * omcos - axis.z * sin;
	matrix[5] = cos + axis.y * axis.y * omcos;
	matrix[6] = axis.z * axis.y * omcos + axis.x * sin;
	matrix[7] = 0.0;
	
	matrix[8] = axis.x * axis.z * omcos + axis.y * sin;
	matrix[9] = axis.y * axis.z * omcos - axis.x * sin;
	matrix[10] = cos + axis.z * axis.z * omcos;
	matrix[11] = 0.0;
	
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = 0.0;
	matrix[15] = 1.0;
	
	return matrix;
}
