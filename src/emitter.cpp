#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

#include "emitter.hpp"
#include "path.hpp"
#include "particle.hpp"

Emitter::Emitter() {
	particles.resize(MAX_PARTICLES);
	num_particles = 0;
	emission_rate = 0.1;
	emission_time = 0;
	min_angle = 0;
	max_angle = 2.0 * M_PI;
	colour1 = Vec3(1, 1, 0);
	colour2 = Vec3(1, 0, 0);
	velocity = 0.5;
	min_lifetime = 1;
	max_lifetime = 10;
	size1 = 3;
	size2 = 1;
	path_speed = 2;
}

void Emitter::setPath(Path::Iterator it) {
	path_iterator = it;
}

void Emitter::render() {
	glBegin(GL_QUADS);
	for(int i = 0; i < num_particles; i++) {
		const Particle& p = particles[i];
		const float t = p.time / p.lifetime;
		const Vec3 colour = colour1 + t * (colour2 - colour1);
		const float size = size1 + t * (size2 - size1);

		glColor3f(colour.x, colour.y, colour.z);
		glVertex3f(p.position.x - size/2, p.position.y - size/2, p.position.z);
		glVertex3f(p.position.x + size/2, p.position.y - size/2, p.position.z);
		glVertex3f(p.position.x + size/2, p.position.y + size/2, p.position.z);
		glVertex3f(p.position.x - size/2, p.position.y + size/2, p.position.z);
	}
	glEnd();
}

void Emitter::update(float time) {
	path_iterator += path_speed * time / 100.0;
	emission_time += time;

	for(int i = 0; i < num_particles; i++) {
		Particle& p = particles[i];
		p.update(time);
		if (p.isDead()) {
			particles[i] = particles[num_particles - 1];
			num_particles--;
		}
	}

	if (emission_time >= emission_rate && num_particles < MAX_PARTICLES) {
		Particle& p = particles[num_particles++];
		emission_time = 0;

		float angle = min_angle + (max_angle - min_angle) * std::rand() / RAND_MAX;
		float z = (float)std::rand() / RAND_MAX;
		float x = std::sqrt(1 - z*z) * std::cos(angle);
		float y = std::sqrt(1 - z*z) * std::sin(angle);

		p.position = path_iterator.getPosition();
		p.velocity = velocity * Vec3(x, y, z);
		p.acceleration = Vec3(0, 0.2, 0);
		p.lifetime = min_lifetime + (max_lifetime - min_lifetime) * std::rand() / RAND_MAX;
		p.time = 0;
	}
}