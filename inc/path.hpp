#ifndef PATH_HPP
#define PATH_HPP

#include <vector>
#include "lerp.hpp"

class Path {
public:
	enum Type {
		LERP,
		BEZC1,
		BEZC2,
		CATMULROM,

		NUM_TYPES
	};

	class Iterator {
	public:
		Iterator(Path* path = NULL);
		void setSpeed(float speed);
		void update(float time);
		void operator+=(float inc);
		void setPosition(float t);
		void reset();
		Vec3 getPosition(float t);
		Vec3 getPosition();
		
	private:
		float t;
		float speed;
		Path* my_path;
	};

	Path(Type type = BEZC2);
	Path(const Path& other);
	Path(Vec3* points, int length, Type type = BEZC2);

	void setType(Type type);
	void setClosed(bool closed);
	void addPoint(Vec3 point);
	void reset();
	Iterator begin();

	Vec3& operator[](int index);
	Vec3 operator[](int index) const;

	int size();
	Vec3* data();

private:
	std::vector<Vec3> points;
	Type type;
	bool closed;
	float speed;

	Vec3 getPosition(int index, float t);
	Vec3 getLerpPoint(int index, float t);
	Vec3 getBezierC1Point(int index, float t);
	Vec3 getBezierC2Point(int index, float t);
	Vec3 getCatmulRomPoint(int index, float t);
};

#endif