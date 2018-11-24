#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "emitter.hpp"
#include "path.hpp"
#include "parse_path.hpp"
#include "load_texture.hpp"

Emitter::Emitter() {
	particles.resize(MAX_PARTICLES);
	num_particles = 0;
	emission_rate = 0.1;
	time_since_emission = 0;

	// environment variables

	gravity = Vec3(0, 0, 0);
	wind = Vec3(0, 0, 0);
	drag_coef = 0;

	texture_handle = 0;
	images = 0;
	h_images = 1;

	// random initial variables

	min_offset = Vec3(0, 0, 0);
	max_offset = Vec3(0, 0, 0);

	min_velocity = 0;
	max_velocity = 0;

	min_direction = Vec3(0, 0, 0);
	max_direction = Vec3(0, 0, 0);

	min_lifetime = 1;
	max_lifetime = 1;

	// interpolated variables

	colour1 = Vec3(1, 1, 1);
	colour2 = Vec3(1, 1, 1);

	size1 = 1;
	size2 = 1;
}

void Emitter::setVelocity(float min_velocity, float max_velocity) {
	this->min_velocity = min_velocity;
	this->max_velocity = max_velocity;
}

void Emitter::setDirection(Vec3 min_direction, Vec3 max_direction) {
	this->min_direction = min_direction;
	this->max_direction = max_direction;
}

void Emitter::setLifetime(float min_lifetime, float max_lifetime) {
	this->min_lifetime = min_lifetime;
	this->max_lifetime = max_lifetime;
}

void Emitter::setSize(float size1, float size2) {
	this->size1 = size1;
	this->size2 = size2;
}

void Emitter::setColour(Vec3 colour1, Vec3 colour2) {
	this->colour1 = colour1;
	this->colour2 = colour2;
}

void Emitter::setPath(Path::Iterator it) {
	path_iterator = it;
}

void Emitter::setPathSpeed(float speed) {
	path_iterator.setSpeed(speed);
}

void Emitter::setEmissionRate(float rate) {
	emission_rate = rate;
}

void Emitter::setTextureAtlas(int texture_handle, int images, int h_images) {
	this->texture_handle = texture_handle;
	this->images = images;
	this->h_images = h_images;
}

void Emitter::setGravity(Vec3 gravity) {
	this->gravity = gravity;
}

void Emitter::setWind(Vec3 wind, float drag, float mass) {
	this->wind = wind;
	drag_coef = drag;
	this->mass = mass;
}

void Emitter::setSpawnOffset(Vec3 min_offset, Vec3 max_offset) {
	this->min_offset = min_offset;
	this->max_offset = max_offset;
}

#define LERP(x, y, t) ((x) + (t) * ((y) - (x)))

void Emitter::render(Vec3 camera_up, Vec3 camera_right) {
	// top left, top right, bottom right, bottom left
	static const float quad_x[] = { -0.5, 0.5, 0.5, -0.5 };
	static const float quad_y[] = { 0.5, 0.5, -0.5, -0.5 };

	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	int v_images = images / h_images + (bool)(images % h_images);
	for(int i = 0; i < num_particles; ++i) {
		const Particle& p = particles[i];

		float t = p.time / p.lifetime;
		Vec3 colour = LERP(colour1, colour2, t);
		float size = LERP(size1, size2, t);

		int image_index = std::max(0, std::min((int)(t * images), images-1));
		float tex_left = (float)(image_index % h_images) / (float)h_images;
		float tex_top = (float)(image_index / h_images) / (float)v_images;

		glColor3f(colour.x, colour.y, colour.z);
		for(int j = 0; j < 4; ++j) {
			float tx = tex_left + (quad_x[j] + 0.5) / (float)h_images;
			float ty = tex_top + (0.5 - quad_y[j]) / (float)v_images;
			glTexCoord2d(tx, ty);

			Vec3 vertex = p.position + size * (quad_x[j] * camera_right + quad_y[j] * camera_up);
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

#define random(value) (((float)std::rand() / (float)RAND_MAX) * (value))
#define randomRange(low, high) ((low) + random((high) - (low)))
#define sqr(value) ((value) * (value))

void Emitter::update(float time) {
	path_iterator.update(time);
	time_since_emission += time;

	static const float wind_pressure_coef = 0.613;
	bool apply_wind = drag_coef > 0 && mass > 0;

	int i = 0;
	while(i < num_particles) {
		Particle& p = particles[i];
		p.time += time;
		if (p.time >= p.lifetime) {
			// replace with last particle
			particles[i] = particles[--num_particles];
		} else {
			if (apply_wind) {
				Vec3 cur_wind = wind - p.velocity;
				float pressure = wind_pressure_coef * Vec3::dotProduct(cur_wind, cur_wind);
				float cur_size = size1 + p.time/p.lifetime * (size2 - size1);
				float wind_force = sqr(cur_size) * pressure * drag_coef;

				p.velocity += time * wind_force / mass * cur_wind.normalize();
			}
			p.velocity += time * gravity;
			p.position += time * p.velocity;

			++i;
		}
	}

	if (time_since_emission >= emission_rate) {
		createParticle();
		time_since_emission = 0;
	}
}

void Emitter::createParticle() {
	int index;
	if (num_particles < (int)particles.size()) {
		index = num_particles++;
	} else {
		// overrite a random particle to make emitter look consistent
		index = std::rand() % particles.size();
	}

	Particle& p = particles[index];

	Vec3 direction;
	Vec3 offset;
	for(int i = 0; i < 3; ++i) {
		direction[i] = randomRange(min_direction[i], max_direction[i]);
		offset[i] = randomRange(min_offset[i], max_offset[i]);
	}
	direction.normalize();

	p.position = path_iterator.getPosition() + offset;
	p.velocity = randomRange(min_velocity, max_velocity) * direction;
	p.lifetime = randomRange(min_lifetime, max_lifetime);
	p.time = 0;
}

bool Emitter::loadFromFile(std::string filename, std::string path){
	parsePath(filename, filename, path);
	std::ifstream fin((path + filename).c_str());
	while(!fin.eof() && fin.good()) {
		std::string var;
		fin >> var;
		if (var.size()) {
			if (var == "vel") {
				float velocity;
				fin >> velocity;
				min_velocity = max_velocity = velocity;
			} else if (var == "min_vel") {
				fin >> min_velocity;
			} else if (var == "max_vel") {
				fin >> max_velocity;
			} else if (var == "dir") {
				Vec3 direction;
				fin >> direction;
				min_direction = max_direction = direction;
			} else if (var == "min_dir") {
				fin >> min_direction;
			} else if (var == "max_dir") {
				fin >> max_direction;
			} else if (var == "offset") {
				Vec3 offset;
				fin >> offset;
				min_offset = max_offset = offset;
			} else if (var == "min_offset") {
				fin >> min_offset;
			} else if (var == "max_offset") {
				fin >> max_offset;
			} else if (var == "lif") {
				float lifetime;
				fin >> lifetime;
				min_lifetime = max_lifetime = lifetime;
			} else if (var == "min_lif") {
				fin >> min_lifetime;
			} else if (var == "max_lif") {
				fin >> max_lifetime;
			} else if (var == "col") {
				Vec3 colour;
				fin >> colour;
				colour1 = colour2 = colour;
			} else if (var == "col1") {
				fin >> colour1;
			} else if (var == "col2") {
				fin >> colour2;
			} else if (var == "size") {
				float size;
				fin >> size;
				size1 = size2 = size;
			} else if (var == "size1") {
				fin >> size1;
			} else if (var == "size2") {
				fin >> size2;
			} else if (var == "grav") {
				fin >> gravity;
			} else if (var == "wind") {
				fin >> wind;
			} else if (var == "drag") {
				fin >> drag_coef;
			} else if (var == "mass") {
				fin >> mass;
			} else if (var == "rate") {
				fin >> emission_rate;
			} else if (var == "max") {
				int max_particles;
				fin >> max_particles;
				particles.resize(max_particles);
			} else if (var == "tex") {
				std::string tex_filename;
				fin >> tex_filename >> images >> h_images;
				int handle = loadTexture(tex_filename, path);
				if (handle == 0) {
					return false;
				} else {
					texture_handle = handle;
				}
			} else {
				// ignore line
				if (var[0] != '#') {
					std::cout << "warning: unknown token '" << var << "'\n";
				}
				std::getline(fin, var);
			}
		}
	}
	return fin.eof();
}