#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "vec3.hpp"

struct Particle {
	Vec3 position;
	Vec3 velocity;

	float lifetime;
	float time;
};

#endif