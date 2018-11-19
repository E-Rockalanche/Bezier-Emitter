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

void buildClosedBezierC2(const std::vector<Vec3>& control_points, std::vector<Vec3>& c2_points) {
	c2_points.clear();
	const int num_points = control_points.size();
	for(int i = 0; i < num_points; i++) {
		const int j = (i + 1) % num_points;
		const int k = (i + 2) % num_points;

		const Vec3& p0 = control_points[i];
		const Vec3& p1 = control_points[j];
		const Vec3& p2 = control_points[k];

		c2_points.push_back((p0 + p1) / 2);
		c2_points.push_back((p0 + 2 * p1) / 3);
		c2_points.push_back((2 * p1 + p2) / 3);
	}
}