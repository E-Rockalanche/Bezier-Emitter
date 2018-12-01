#include <vector>
#include "lerp.hpp"
#include "vec3.hpp"

Vec3 lerp(const Vec3& p1, const Vec3& p2, float t) {
	return p1 + t * (p2 - p1);
}

Vec3 bezierC1(const Vec3& p1, const Vec3& p2, const Vec3& p3, float t) {
	const Vec3 v2 = 2 * p2 - 2 * p1;
	const Vec3 v3 = p3 - 2 * p2 + p1;
	return p1 + t*v2 + t*t*v3;
}

Vec3 bezierC2(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4, float t) {
	const Vec3 v2 = -3 * p1 + 3 * p2;
	const Vec3 v3 = 3 * p1 - 6 * p2 + 3 * p3;
	const Vec3 v4 = -p1 + 3 * p2 - 3 * p3 + p4;
	return p1 + t*v2 + t*t*v3 + t*t*t*v4;
}

Vec3 catmulRom(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& p3, float t) {
	const Vec3 r1 = (p2 - p0) / 2;
	const Vec3 r2 = (p3 - p1) / 2;
	const Vec3 v3 = 2 * p1 - 2 * p2 + r1 + r2;
	const Vec3 v2 = -3 * p1 + 3 * p2 - 2 * r1 - 1 * r2;
	return p1 + t*r1 + t*t*v2 + t*t*t*v3;
}