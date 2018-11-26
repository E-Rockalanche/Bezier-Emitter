#ifndef PARTICLE_EMITTER
#define PARTICLE_EMITTER

#include <vector>
#include "emitter.hpp"
#include "vec3.hpp"

class ParticleEmitter : public Emitter {
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();
	virtual void render(Vec3 cam_up, Vec3 cam_right);
	virtual void update(float dt);
	virtual bool loadFromFile(std::string filename, std::string path = "");

	// interpolated particle variables

	void setSize(float size1, float size2);
	void setColour(Vec3 colour1, Vec3 colour2);
	void setTextureAtlas(int handle, int images, int h_images);

private:
	static const int DEFAULT_MAX_PARTICLES = 256;

	std::vector<Particle> particles;
	int num_particles;

	// interpolated variables

	Vec3 colour1;
	Vec3 colour2;

	float size1;
	float size2;

	int texture_handle;
	int images;
	int h_images;

	void createParticle();
	void initializeParticle(Particle& p);
};

#endif