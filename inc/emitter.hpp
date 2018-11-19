#ifndef EMITTER_HPP
#define EMITTER_HPP

class Emitter {
public:
	void setPath(const Path& path);
	void setPathSpeed(float speed);

	void setTexture(int texture_handle);
	void setTextureUVs(float* uvs); // 4 coordinates to form a quad

	void setInitialColour(Vec3 colour);
	void setEndColour(Vec3 colour);

	void set

	void setNumParticles(int particles);

	int getNumParticles();
	Particle* data();

private:
	std::vector<Particle> particles;
};

#endif