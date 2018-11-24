#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "path.hpp"
#include "matrix.hpp"

class Emitter {
public:
	Emitter();
	void setSpawnOffset(Vec3 min_offset, Vec3 max_offset);
	void setVelocity(float min_velocity, float max_velocity);
	void setDirection(Vec3 min_direction, Vec3 max_direction);
	void setLifetime(float min_lifetime, float max_lifetime);
	void setSize(float size1, float size2);
	void setColour(Vec3 colour1, Vec3 colour2);
	void setPath(Path::Iterator it);
	void setPathSpeed(float speed);
	void setEmissionRate(float rate);
	void setTextureAtlas(int handle, int images, int h_images);
	void setGravity(Vec3 gravity);
	void setWind(Vec3 wind, float drag, float mass);
	void setRotation(Vec3 axis, float angular_speed, int rotation_groups);

	bool loadFromFile(std::string filename, std::string path = "");
	void render(Vec3 camera_up = Vec3(0, 1, 0), Vec3 camera_right = Vec3(1, 0, 0));
	void update(float time);
	void createParticle();

private:
	static const int MAX_PARTICLES = 256;
	// static const int MAX_ROTATION_GROUPS = 16;
	#define MAX_ROTATION_GROUPS 16

	struct Particle {
		Vec3 position;
		Vec3 velocity;

		float lifetime;
		float time;

		int rotation_group;
	};

	std::vector<Particle> particles;
	int num_particles;
	Path::Iterator path_iterator;
	float emission_rate;
	float time_since_emission;

	// environment variables

	Vec3 gravity;
	Vec3 wind;
	float drag_coef;
	float mass;

	float angular_velocity;
	Vec3 rotation_axis;
	float angle;
	int rotation_groups;

	// random initial variables

	Vec3 min_offset;
	Vec3 max_offset;

	float min_velocity;
	float max_velocity;

	Vec3 min_direction;
	Vec3 max_direction;

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