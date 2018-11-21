#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "particle.hpp"
#include "path.hpp"

class Emitter {
public:
	Emitter();
	void setPath(Path::Iterator it);
	void render();
	void update(float time);

private:
	static const int MAX_PARTICLES = 100;

	std::vector<Particle> particles;
	int num_particles;
	Path::Iterator path_iterator;
	float path_speed;
	float emission_rate;
	float emission_time;
	float min_angle;
	float max_angle;
	float velocity;
	Vec3 colour1;
	Vec3 colour2;
	float min_lifetime;
	float max_lifetime;
	float size1;
	float size2;
};

#endif