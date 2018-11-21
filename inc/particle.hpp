#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "vec3.hpp"

struct Particle {
	Vec3 position;
	Vec3 velocity;
	Vec3 acceleration;

	float lifetime;
	float time;

	inline void update(float time) {
		velocity += time * acceleration;
		position += time * velocity;
		this->time += time;
	}

	inline bool isDead() const {
		return (time >= lifetime);
	}
};

#endif