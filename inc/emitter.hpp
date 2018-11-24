#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "particle.hpp"
#include "path.hpp"

class Emitter {
public:
	Emitter();
	void setVelocity(float min_velocity, float max_velocity);
	void setLifetime(float min_lifetime, float max_lifetime);
	void setSize(float size1, float size2);
	void setColour(Vec3 colour1, Vec3 colour2);
	void setPath(Path::Iterator it);
	void setPathSpeed(float speed);
	void setEmissionRate(float rate);
	void render(Vec3 camera_up = Vec3(0, 1, 0), Vec3 camera_right = Vec3(1, 0, 0));
	void update(float time);
	void setTextureAtlas(int handle, int images, int h_images);
	void setAccelerationFunc(Vec3 (*func)(Vec3));
	void setGravity(float gravity);
	bool loadFromFile(std::string filename, std::string path);

private:
	static const int MAX_PARTICLES = 256;

	std::vector<Particle> particles;
	int num_particles;
	Path::Iterator path_iterator;
	float emission_rate;
	float time_since_emission;
	Vec3 (*acceleration_func)(Vec3);

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
	int images;
	int h_images;
};

#endif