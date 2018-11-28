#include <vector>
#include "emiutter_system.hpp"
#include "emitter.hpp"
#include "vec3.hpp"

void EmitterSystem::add(Emitter& emitter) {
	emitters.push_back(&emitter);
	emitter.system = this;
}

void EmitterSystem::update(float dt) {
	for(int i = 0; i < emitters.size(); ++i) {
		Emitter* e = emitters[i];
		if (e->isDead()) {
			delete e;
			emitters[i--] = emitters.back();
			emitters.pop_back();
		} else {
			e->applyWind(wind);
			for(int v = 0; v < (int)vortices.size(); ++v) {
				e->applyVortex(vortices[v]);
			}
			e->update(dt);
		}
	}
}

void EmitterSystem::render(Vec3 cam_up, Vec3 cam_right) {
	for(auto &e : emitters) {
		e.render(cam_up, cam_right);
	}
}


void EmitterSystem::drawVortex(const Vortex& v) {
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
		Vec3 vertex = v.position + (4.0 * i / (loops * segments)) * radius * (std::cos(theta) * x_vec + std::sin(theta) * y_vec);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}