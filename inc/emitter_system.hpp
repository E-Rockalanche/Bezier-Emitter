#ifndef EMITTER_SYSTEM
#define EMITTER_SYSTEM

class EmitterSystem;

#include <vector>
#include "emitter.hpp"
#include "vec3.hpp"

struct Particle {
	Vec3 position;
	Vec3 velocity;
	float lifetime;
	float time;

	bool isDead() { return time >= lifetime; }
};

struct Vortex : Particle {
	Vec3 angular_velocity;
};

class EmitterSystem {
public:

	void add(Emitter& emitter);
	void update(float dt);
	void render(Vec3 cam_up, Vec3 cam_right);
	void setWind(Vec3 wind);
	void addVortex(Vec3 position, Vec3 velocity, Vec3 angular_velocity, float lifetime);

private:
	std::vector<Emitter*> emitters;
	std::vector<Vortex> vortices;
};

#endif