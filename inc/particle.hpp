#ifndef PARTICLE_HPP
#define PARTICLE_HPP

struct Particle {
	float size;
	Vec3 position;
	Vec3 velocity;
	Vec3 colour;
	float lifetime;
	int texture_handle;
	float u[4];
	float v[4];
};

#endif