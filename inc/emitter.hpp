#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "path.hpp"

class Emitter {
public:
	Emitter();

	void setPath(Path::Iterator it);
	void setPathSpeed(float speed);

	void setEmissionDelay(float min_delay, float max_delay);
	void setVortexEmissionDelay(float min_delay, float max_delay);

	void setGravity(Vec3 gravity);
	void setWind(Vec3 wind, float drag, float mass);
	void setTextureAtlas(int handle, int images, int h_images);

	void setSpawnOffset(Vec3 min_offset, Vec3 max_offset);
	void setVelocity(float min_velocity, float max_velocity);
	void setDirection(Vec3 min_direction, Vec3 max_direction);
	void setLifetime(float min_lifetime, float max_lifetime);

	void setSize(float size1, float size2);
	void setColour(Vec3 colour1, Vec3 colour2);

	bool loadFromFile(std::string filename, std::string path = "");
	void render(Vec3 camera_up = Vec3(0, 1, 0), Vec3 camera_right = Vec3(1, 0, 0));
	void update(float time);

	void setDebugMode(bool debug = true);

private:
	static const int DEFAULT_MAX_PARTICLES = 256;

	struct Particle {
		Vec3 position;
		Vec3 velocity;

		float lifetime;
		float time;

		Particle() {}
		Particle(Vec3 p, Vec3 v, float lt) : position(p), velocity(v), lifetime(lt), time(0) {}
		bool isDead() { return time >= lifetime; }
	};

	struct Vortex : Particle {
		Vec3 angular_velocity;

		Vortex() {}
		Vortex(Particle p, Vec3 av) : Particle(p), angular_velocity(av) {}
	};

	Path::Iterator path_iterator;

	std::vector<Particle> particles;
	int num_particles;
	float emission_timer;
	float min_emission_delay;
	float max_emission_delay;

	std::vector<Vortex> vortices;
	int num_vortices;
	float vortex_emission_timer;
	float min_vortex_emission_delay;
	float max_vortex_emission_delay;

	// environment variables

	Vec3 gravity;
	Vec3 wind;
	float drag_coef;
	float mass;

	// random initial variables

	Vec3 min_offset;
	Vec3 max_offset;

	float min_velocity;
	float max_velocity;

	Vec3 min_direction;
	Vec3 max_direction;

	float min_lifetime;
	float max_lifetime;

	Vec3 min_vortex_axis;
	Vec3 max_vortex_axis;

	float min_vortex_angular_velocity;
	float max_vortex_angular_velocity;

	// interpolated variables

	Vec3 colour1;
	Vec3 colour2;

	float size1;
	float size2;

	int texture_handle;
	int images;
	int h_images;

	bool debug;

	void createParticle();
	void createVortex();
	void initializeParticle(Particle& p);
	void initializeVortex(Vortex& v);
	void updateParticle(Particle& p, float time);
	void drawVortex(const Vortex& v);
};

#endif