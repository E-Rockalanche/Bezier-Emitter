#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "emitter.hpp"
#include "path.hpp"
#include "particle.hpp"

Emitter::Emitter() {
	particles.resize(MAX_PARTICLES);
	num_particles = 0;
	emission_rate = 0.1;
	time_since_emission = 0;

	// random initial variables

	min_velocity = 0.5;
	max_velocity = 1;

	min_lifetime = 3;
	max_lifetime = 10;

	// interpolated variables

	colour1 = Vec3(1, 1, 0);
	colour2 = Vec3(1, 0, 0);

	size1 = 1;
	size2 = 0.2;

	texture_handle = 0;
	images = 0;
	h_images = 1;
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
	for(int i = 0; i < num_particles; i++) {
		const Particle& p = particles[i];

		float t = p.time / p.lifetime;
		Vec3 colour = LERP(colour1, colour2, t);
		float size = LERP(size1, size2, t);

		int image_index = std::max(0, std::min((int)(t * images), images-1));
		float tex_left = (float)(image_index % h_images) / (float)h_images;
		float tex_top = (float)(image_index / h_images) / (float)v_images;

		glColor3f(colour.x, colour.y, colour.z);
		for(int j = 0; j < 4; j++) {
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

#define random(value) ((float)(value) * std::rand() / RAND_MAX)
#define randomRange(low, high) ((low) + random((high) - (low)))

void Emitter::update(float time) {
	path_iterator.update(time);
	time_since_emission += time;

	for(int i = 0; i < num_particles; i++) {
		Particle& p = particles[i];
		p.update(time);
		if (p.isDead()) {
			particles[i] = particles[num_particles - 1];
			num_particles--;
		}
	}

	while (time_since_emission >= emission_rate && num_particles < MAX_PARTICLES) {
		Particle& p = particles[num_particles++];
		time_since_emission = 0;

		float angle = random(2.0 * M_PI);
		float z = randomRange(-1.0, 1.0);
		float x = std::sqrt(1 - z*z) * std::cos(angle);
		float y = std::sqrt(1 - z*z) * std::sin(angle);

		p.position = path_iterator.getPosition();
		p.velocity = randomRange(min_velocity, max_velocity) * Vec3(x, y, z);
		p.acceleration = Vec3(0, 0.2, 0);
		p.lifetime = randomRange(min_lifetime, max_lifetime);
		p.time = 0;
	}
}