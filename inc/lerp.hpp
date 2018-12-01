#include <vector>
#include "vec3.hpp"

Vec3 lerp(const Vec3& p1, const Vec3& p2, float t);

Vec3 bezierC1(const Vec3& p1, const Vec3& p2, const Vec3& p3, float t);

Vec3 bezierC2(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec3& p4, float t);

Vec3 catmulRom(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& p3, float t);