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