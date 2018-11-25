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
	particles.resize(DEFAULT_MAX_PARTICLES);

	num_particles = 0;
	min_emission_delay = 0;
	max_emission_delay = 1;
	emission_timer = 0;

	num_vortices = 0;
	min_vortex_emission_delay = 0;
	max_vortex_emission_delay = 1;
	vortex_emission_timer = 0;

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

	debug = false;
}

void Emitter::setPath(Path::Iterator it) {
	path_iterator = it;
}

void Emitter::setPathSpeed(float speed) {
	path_iterator.setSpeed(speed);
}

void Emitter::setEmissionDelay(float min_delay, float max_delay) {
	min_emission_delay = min_delay;
	max_emission_delay = max_delay;
}

void Emitter::setVortexEmissionDelay(float min_delay, float max_delay) {
	min_vortex_emission_delay = min_delay;
	max_vortex_emission_delay = max_delay;
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

void Emitter::setTextureAtlas(int texture_handle, int images, int h_images) {
	this->texture_handle = texture_handle;
	this->images = images;
	this->h_images = std::max(1, h_images);
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

void Emitter::setDebugMode(bool debug) {
	this->debug = debug;
}

void Emitter::drawVortex(const Vortex& v) {
	static const int segments = 16;
	static const int loops = 2;

	float radius = v.angular_velocity.length();

	Vec3 x_vec, y_vec;

	x_vec = Vec3::crossProduct(v.angular_velocity, v.angular_velocity + Vec3(1, 0, 0));
	x_vec.normalize();

	y_vec = -Vec3::crossProduct(v.angular_velocity, x_vec);
	y_vec.normalize();

	float t = v.time / v.lifetime;
	float alpha = (1.0 - t) * t * 4.0;

	glColor4f(1, 0, 0, alpha);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < segments * loops; ++i) {
		float theta = 2.0 * M_PI * i / segments;
		Vec3 vertex = v.position + (i / (2.0 * loops * segments)) * radius * (std::cos(theta) * x_vec + std::sin(theta) * y_vec);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
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

	if (debug) {
		for(int i = 0; i < num_vortices; ++i) {
			const Vortex& v = vortices[i];
			drawVortex(v);
		}
	}
}

#define random(value) (((float)std::rand() / (float)RAND_MAX) * (value))
#define randomRange(low, high) ((low) + random((high) - (low)))
#define sqr(value) ((value) * (value))

void Emitter::update(float time) {
	path_iterator.update(time);
	emission_timer -= time;
	vortex_emission_timer -= time;

	for(int i = 0; i < num_vortices; ++i) {
		Vortex& v = vortices[i];
		v.time += time;
		if (v.isDead()) {
			vortices[i] = vortices[--num_vortices];
			--i;
		} else {
			updateParticle(v, time);
		}
	}

	for(int i = 0; i < num_particles; ++i) {
		Particle& p = particles[i];
		p.time += time;
		if (p.isDead()) {
			particles[i] = particles[--num_particles];
			--i;
		} else {
			updateParticle(p, time);
		}
	}

	if (emission_timer <= 0) {
		createParticle();
		emission_timer = randomRange(min_emission_delay, max_emission_delay);
	}

	if (vortex_emission_timer <= 0) {
		createVortex();
		vortex_emission_timer = randomRange(min_vortex_emission_delay, max_vortex_emission_delay);
	}
}

void Emitter::updateParticle(Particle& p, float time) {
	static const float wind_pressure_coef = 0.613;

	if (drag_coef > 0 && mass > 0) {
		// calculate wind vector
		Vec3 relative_wind = wind - p.velocity;
		
		for(int i = 0; i < num_vortices; ++i) {
			const Vortex& v = vortices[i];
			if (&p != (Particle*)&v) {
				// add vortex wind
				float t = v.time / v.lifetime;
				float life_factor = (1.0 - t) * t * 4;
				Vec3 r = p.position - v.position;
				float scale = life_factor / (1.0 + Vec3::dotProduct(r, r));
				relative_wind += Vec3::crossProduct(v.angular_velocity, r) * scale + v.velocity - p.velocity;
			}
		}

		// calculate wind force
		float pressure = wind_pressure_coef * Vec3::dotProduct(relative_wind, relative_wind);
		float cur_size = size1 + p.time/p.lifetime * (size2 - size1);
		float wind_force = sqr(cur_size) * pressure * drag_coef;

		p.velocity += time * wind_force / mass * relative_wind.normalize();
	}

	p.velocity += time * gravity;
	p.position += time * p.velocity;
}

void Emitter::createParticle() {
	int index;
	if (num_particles < (int)particles.size()) {
		index = num_particles++;
	} else {
		// overrite a random particle to make emitter look consistent
		index = std::rand() % particles.size();
	}

	initializeParticle(particles[index]);
}

void Emitter::createVortex() {
	int index;
	if (num_vortices < (int)vortices.size()) {
		index = num_vortices++;
	} else {
		// overrite a random particle to make emitter look consistent
		index = std::rand() % vortices.size();
	}

	initializeVortex(vortices[index]);
}

void Emitter::initializeParticle(Particle& p) {
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

void Emitter::initializeVortex(Vortex& v) {
	initializeParticle(v);

	Vec3 axis;
	for(int i = 0; i < 3; ++i) {
		axis[i] = randomRange(min_vortex_axis[i], max_vortex_axis[i]);
	}
	axis.normalize();

	float angular_velocity = randomRange(min_vortex_angular_velocity, max_vortex_angular_velocity);
	
	v.angular_velocity = angular_velocity * axis;
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
			}

			else if (var == "dir") {
				Vec3 direction;
				fin >> direction;
				min_direction = max_direction = direction;
			} else if (var == "min_dir") {
				fin >> min_direction;
			} else if (var == "max_dir") {
				fin >> max_direction;
			}

			else if (var == "offset") {
				Vec3 offset;
				fin >> offset;
				min_offset = max_offset = offset;
			} else if (var == "min_offset") {
				fin >> min_offset;
			} else if (var == "max_offset") {
				fin >> max_offset;
			}

			else if (var == "lif") {
				float lifetime;
				fin >> lifetime;
				min_lifetime = max_lifetime = lifetime;
			} else if (var == "min_lif") {
				fin >> min_lifetime;
			} else if (var == "max_lif") {
				fin >> max_lifetime;
			}

			else if (var == "col") {
				Vec3 colour;
				fin >> colour;
				colour1 = colour2 = colour;
			} else if (var == "col1") {
				fin >> colour1;
			} else if (var == "col2") {
				fin >> colour2;
			}

			else if (var == "size") {
				float size;
				fin >> size;
				size1 = size2 = size;
			} else if (var == "size1") {
				fin >> size1;
			} else if (var == "size2") {
				fin >> size2;
			}

			else if (var == "grav") {
				fin >> gravity;
			}

			else if (var == "wind") {
				fin >> wind;
			} else if (var == "drag") {
				fin >> drag_coef;
			} else if (var == "mass") {
				fin >> mass;
			}

			else if (var == "vortex_axis") {
				Vec3 axis;
				fin >> axis;
				min_vortex_axis = max_vortex_axis = axis;
			} else if (var == "min_vortex_axis") {
				fin >> min_vortex_axis;
			} else if (var == "max_vortex_axis") {
				fin >> max_vortex_axis;
			}

			else if (var == "vortex_angular_vel") {
				float vel;
				fin >> vel;
				min_vortex_angular_velocity = max_vortex_angular_velocity = vel;
			} else if (var == "min_vortex_angular_vel") {
				fin >> min_vortex_angular_velocity;
			} else if (var == "max_vortex_angular_vel") {
				fin >> max_vortex_angular_velocity;
			}

			else if (var == "delay") {
				float delay;
				fin >> delay;
				min_emission_delay = max_emission_delay = delay;
			} else if (var == "min_delay") {
				fin >> min_emission_delay;
			} else if (var == "max_delay") {
				fin >> max_emission_delay;
			}

			else if (var == "vortex_delay") {
				float delay;
				fin >> delay;
				min_vortex_emission_delay = max_vortex_emission_delay = delay;
			} else if (var == "min_vortex_delay") {
				fin >> min_vortex_emission_delay;
			} else if (var == "max_vortex_delay") {
				fin >> max_vortex_emission_delay;
			}

			else if (var == "max") {
				int max_particles;
				fin >> max_particles;
				particles.resize(max_particles);
			}

			else if (var == "vortex_max") {
				int max_vortices;
				fin >> max_vortices;
				vortices.resize(max_vortices);
			}

			else if (var == "tex") {
				std::string tex_filename;
				fin >> tex_filename >> images >> h_images;
				int handle = loadTexture(tex_filename, path);
				if (handle == 0) {
					return false;
				} else {
					texture_handle = handle;
				}
			}
			else {
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