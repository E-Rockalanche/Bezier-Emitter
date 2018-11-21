#include <iostream>
#include <cmath>

#include "path.hpp"
#include "assert.hpp"

float realFmod(float num, float den) {
	float result = std::fmod(num, den);
	return (result >= 0) ? result : (den + result);
}

Path::Path(Type type) {
	this->type = type;
	closed = true;
}

Path::Path(const Path& other) {
	points = other.points;
	type = other.type;
}

Path::Path(Vec3* points, int length, Type type) {
	this->points.resize(length);
	for(int i = 0; i < length; i++) {
		this->points[i] = points[i];
	}
	this->type = type;
	this->closed = closed;
}

void Path::setType(Type type) {
	this->type = type;
}

void Path::addPoint(Vec3 point) {
	points.push_back(point);
}

Vec3 Path::getPosition(int index, float t) {
	Vec3 point;

	switch(type) {
		case LERP:
		default:
			point = getLerpPoint(index, t);
			break;

		case BEZC1:
			point = getBezierC1Point(index, t);
			break;

		case BEZC2:
			point = getBezierC2Point(index, t);
			break;

		case CATMULROM:
			point = getCatmulRomPoint(index, t);
			break;
	}

	return point;
}

Vec3 Path::getLerpPoint(int index, float t) {
	const Vec3& p1 = points[index];
	const Vec3& p2 = points[(index + 1) % points.size()];

	return lerp(p1, p2, t);
}

Vec3 Path::getBezierC1Point(int index, float t) {
	const Vec3& p1 = points[index];
	const Vec3& p2 = points[(index + 1) % points.size()];
	const Vec3& p3 = points[(index + 2) % points.size()];

	return bezierC1(p1, p2, p3, t);
}

Vec3 Path::getBezierC2Point(int index, float t) {
	const Vec3& p1 = points[index];
	const Vec3& p2 = points[(index + 1) % points.size()];
	const Vec3& p3 = points[(index + 2) % points.size()];
	const Vec3& p4 = points[(index + 3) % points.size()];
	
	return bezierC2(p1, p2, p3, p4, t);
}

Vec3 Path::getCatmulRomPoint(int index, float t) {
	const Vec3& p0 = points[(index - 1 + points.size()) % points.size()];
	const Vec3& p1 = points[index];
	const Vec3& p2 = points[(index + 1) % points.size()];
	const Vec3& p3 = points[(index + 2) % points.size()];

	return catmulRom(p0, p1, p2, p3, t);
}

Path::Iterator Path::begin() {
	return Iterator(this);
}

Vec3& Path::operator[](int index) {
	assert(index < points.size(), "Path::operator[] index out of bounds");
	return points[index];
}

Vec3 Path::operator[](int index) const {
	assert(index < points.size(), "Path::operator[] index out of bounds");
	return points[index];
}

int Path::size() {
	return points.size();
}

Vec3* Path::data() {
	return points.data();
}

/*
Path::Iterator
*/

Path::Iterator::Iterator(Path* path) {
	t = 0;
	my_path = path;
	speed = 0;
}

void Path::Iterator::setSpeed(float speed) {
	this->speed = speed;
}

#define sign(val) (((val) > 0) - ((val) < 0))

void Path::Iterator::update(float time) {
	static const float EPSILON = 0.0001;
	Vec3 v0 = getPosition(t);
	Vec3 v1 = getPosition(realFmod(t + sign(speed) * EPSILON, 1.0));
	float length = (v1 - v0).length();
	float cur_rate = length / EPSILON;
	*this += speed * time / cur_rate;
}

void Path::Iterator::operator+=(float inc) {
	t = realFmod(t + inc, 1);
}

void Path::Iterator::setPosition(float t) {
	this->t = realFmod(t, 1);
}

void Path::Iterator::reset() {
	t = 0;
}

Vec3 Path::Iterator::getPosition() {
	return getPosition(t);
}

Vec3 Path::Iterator::getPosition(float t) {
	int num_curve_points;

	switch(my_path->type) {
		case LERP:
		case CATMULROM:
		default:
			num_curve_points = 1;
			break;

		case BEZC1:
			num_curve_points = 2;
			break;

		case BEZC2:
			num_curve_points = 3;
			break;
	}

	float path_position = t * my_path->points.size() / num_curve_points;

	int path_index = (int)path_position;
	float inner_t = path_position - path_index;

	return my_path->getPosition(path_index, inner_t);
}