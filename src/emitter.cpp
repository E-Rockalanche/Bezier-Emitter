#include "emitter.hpp"

Emitter::Emitter() {
	rate = 1.0/6.0;

	mass = 1.0;
	drag = 0.0;

	min_velocity = max_velocity = 0.0;

	min_lifetime = max_lifetime = 1.0;

	colour1 = colour2 = Vec3(1, 1, 1);

	size1 = size2 = 1.0;
}

Emitter::~Emitter() {};

void Emitter::setEmitterPosition(Vec3 position) {
	has_path = false;
	emitter_position = position;
}

void Emitter::setEmitterVelocity(Vec3 velocity) {
	has_path = false;
	emitter_velocity = velocity;
}

void Emitter::setEmitterGravity(Vec3 gravity) {
	has_path = false;
	emitter_gravity = gravity;
}

void Emitter::setPath(Path::Iterator it, float speed) {
	has_path = true;
	path_iterator = it;
	path_iterator.setSpeed(speed);
}

void Emitter::setPathSpeed(float speed) {
	has_path = true;
	path_iterator.setSpeed(speed);
}

void Emitter::setEmitterLifetime(float time) {
	lifetime = time;
	has_lifetime = true;
}

bool Emitter::isDead() {
	return (has_lifetime && (lifetime <= 0));
}

void Emitter::setDebugMode(bool debug = true) {
	this->debug = debug;
}

void Emitter::setRate(float rate) {
	this->rate = rate;
}

void Emitter::setSpawnMultiple(int min_multiple, int max_multiple) {
	this->min_multiple = min_multiple;
	this->max_multiple = max_multiple;
}

void Emitter::setParticleVelocity(float min_velocity, float max_velocity) {
	this->min_velocity = min_velocity;
	this->max_velocity = max_velocity;
}

void Emitter::setParticleDirection(Vec3 min_dir, Vec3 max_dir) {
	this->min_direction = min_direction;
	this->max_direction = max_direction;
}

void Emitter::setParticleGravity(Vec3 gravity) {
	particle_gravity = gravity;
}

void Emitter::setParticleLifetime(float min_lifetime, float max_lifetime) {
	this->min_lifetime = min_lifetime;
	this->max_lifetime = max_lifetime;
}

void Emitter::setMass(float mass) {
	this->mass = mass;
}

void Emitter::setDrag(float drag) {
	this->drag = drag;
}