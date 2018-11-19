#include "path.hpp"

Path::Path(Type type) {
	this->type = type;
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
}

void Path::setType(Type type) {
	this->type = type;
}

void Path::addPoint(Vec3 point) {
	points.push_back(point);
}

Vec3 Path::getPoint(int index, float t) {
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
	return points[index];
}

Vec3 Path::operator[](int index) const {
	return points[index];
}

int Path::size() {
	return points.size();
}

Vec3* Path::data() {
	return points.data();
}

Path::Iterator::Iterator(Path* path) {
	t = 0;
	index = 0;
	my_path = path;
}

void Path::Iterator::operator+=(float inc) {
	t += inc;
	if (t >= 1) {
		while(t >= 1) {
			t -= 1;
		}
		int index_increment;

		switch(my_path->type) {
			case LERP:
			case CATMULROM:
			default:
				index_increment = 1;
				break;

			case BEZC1:
				index_increment = 2;
				break;

			case BEZC2:
				index_increment = 4;
				break;
		}

		index = (index + index_increment) % my_path->points.size();
	}
}

void Path::Iterator::reset() {
	t = 0;
	index = 0;
}

Vec3 Path::Iterator::getPoint() {
	return my_path->getPoint(index, t);
}