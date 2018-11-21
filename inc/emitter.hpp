#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "particle.hpp"
#include "path.hpp"

class Emitter {
public:
	Emitter();
	void setPath(Path::Iterator it);
	void setPathSpeed(float speed);
	void setEmissionRate(float rate);
	void render();
	void update(float time);
	void setTextureAtlas(int handle, int h_images, int v_images);

private:
	static const int MAX_PARTICLES = 256;

	std::vector<Particle> particles;
	int num_particles;
	Path::Iterator path_iterator;
	float emission_rate;
	float time_since_emission;

	// random initial variables

	float min_velocity;
	float max_velocity;

	float min_lifetime;
	float max_lifetime;

	// interpolated variables

	Vec3 colour1;
	Vec3 colour2;

	float size1;
	float size2;

	int texture_handle;
	int h_images;
	int v_images;
};

#endif