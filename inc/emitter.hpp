#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "path.hpp"

class Emitter {
public:
	Emitter();
	virtual ~Emitter();
	virtual void render(Vec3 cam_up, Vec3 cam_right) = 0;
	virtual void update(float dt) = 0;
	virtual bool loadFromFile(std::string filename, std::string path = "") = 0;
	virtual void applyWind(Vec3 wind);
	virtual void applyVortex(const Vortex& vortex);

	void setEmitterPosition(Vec3 position);
	void setEmitterVelocity(Vec3 position);
	void setEmitterGravity(Vec3 gravity);

	void setPath(Path::Iterator it, float speed);
	void setPathSpeed(float speed);

	void setEmitterLifetime(float time);
	bool isDead();

	void setDebugMode(bool debug = true);

	void setRate(float rate);
	void setSpawnMultiple(int min_multiple, int max_multiple);

	void setParticleVelocity(float min_velocity, float max_velocity);
	void setParticleDirection(Vec3 min_dir, Vec3 max_dir);
	void setParticleGravity(Vec3 gravity);

	void setParticleLifetime(float min_time, float max_time);

	void setMass(float mass);
	void setDrag(float drag);

protected:
	static const float wind_pressure_coef = 0.613 / 60.0;

	float emission_rate;
	float time_since_emission;
	int min_multiple, max_multiple;

	bool has_lifetime;
	float lifetime;

	bool follow_path;
	Path::Iterator it;

	Vec3 emitter_position;
	Vec3 emitter_velocity;
	Vec3 emitter_gravity;

	Vec3 min_offset, max_offset;
	Vec3 min_direction, max_direction;
	Vec3 particle_gravity;
	float min_velocity, max_velocity;
	float min_lifetime, max_lifetime;

	bool debug;
};

#endif