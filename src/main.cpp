#define _USE_MATH_DEFINES

#include <math.h>
#include <string>
#include <iostream>
#include <iomanip>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#if(OS == windows)
	#define GL_CLAMP_TO_EDGE 0x812F
#endif

#include "stb_image.h"
#include "vec3.hpp"
#include "path.hpp"
#include "emitter.hpp"
#include "matrix.hpp"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

Path path(Path::CATMULROM);
bool display_path = false;
bool display_control_points = false;
Emitter emitter;
float path_speed = 0.5;
Matrix rotation_matrix;
int last_mpos[2];
int cur_mpos[2];
bool arcball_on = false;

Vec3 camera_pos(0, 0, 50);
Vec3 camera_up(0, 1, 0);
Vec3 camera_right(1, 0, 0);

void checkGLError(std::string message) {
	GLenum error_enum = glGetError();
	if (GL_NO_ERROR != error_enum) {
		std::cout << "openGL error: " << error_enum << ": " << message << '\n';
		exit(1);
	}
}

Vec3 getArcballVector(int x, int y) {
	Vec3 point = Vec3((float)x/SCREEN_WIDTH - 0.5, 0.5 - (float)y/SCREEN_HEIGHT, 0);
	
	float hyp_sqr = point.x*point.x + point.y*point.y;
	
	if (hyp_sqr <= 1) {
		// mouse is inside arcball
		point.z = std::sqrt(1.0 - hyp_sqr);
	} else {
		// mouse is outside arcball
		point = Vec3::normalize(point);
	}
	return point;
}

void mouseAction(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				arcball_on = true;
				last_mpos[0] = cur_mpos[0] = x;
				last_mpos[1] = cur_mpos[1] = y;
			} else {
				arcball_on = false;
			}
			break;

		case 3: // scroll up
			camera_pos /= 1.1;
			break;

		case 4: // scroll down
			camera_pos *= 1.1;
			break;
			
		default:
			arcball_on = false;
			break;
	}
}

void mouseMotion(int x, int y) {
	if (arcball_on) {
		cur_mpos[0] = x;
		cur_mpos[1] = y;
	}
}

void keyboardInput(unsigned char key, int x, int y) {
	switch(key) {
		case 'd':
			display_path = !display_path;
			display_control_points = !display_control_points;
			break;

		case '+':
			path_speed += 0.1;
			emitter.setPathSpeed(path_speed);
			break;

		case '-':
			path_speed -= 0.1;
			emitter.setPathSpeed(path_speed);
			break;

		case '1':
			emitter.setEmissionRate(0.1);
			break;

		case '2':
			emitter.setEmissionRate(1);
			break;

		case '3':
			emitter.setEmissionRate(10);
			break;
	}
}

void specialKeyboardInput(int key, int x, int y) {
}

void initializeScene()
{
	// glShadeModel( GL_FLAT );
	glShadeModel( GL_SMOOTH );

  	// glEnable(GL_DEPTH_TEST);
  	glEnable(GL_NORMALIZE);

	glDisable(GL_DEPTH_TEST);

	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  	path.addPoint(Vec3(-10, -5, 10));
  	path.addPoint(Vec3(10, -10, 5));
  	path.addPoint(Vec3(10, 10, 0));
  	path.addPoint(Vec3(0, -5, -10));
  	path.addPoint(Vec3(-10, 10, 5));

  	Path::Iterator it = path.begin();
  	it.setSpeed(0.5);
  	emitter.setPath(it);
}

void renderPath() {
    glColor3ub(255, 255, 255);
    glBegin(GL_LINE_STRIP);

    Path::Iterator it = path.begin();
    int curve_sections = 16;
    for(int i = 0; i < path.size() * curve_sections; i++) {
    	const Vec3 p = it.getPosition();
    	glVertex3f(p.x, p.y, p.z);
    	it += 1.0 / (path.size() * curve_sections);
    }
	const Vec3 p = it.getPosition();
	glVertex3f(p.x, p.y, p.z);

	glEnd();
}

void renderControlPoints() {
    glColor3ub(255, 0, 0);
    glBegin(GL_POINTS);
    for(int i = 0; i < path.size(); i++) {
    	const Vec3& p = path[i];
    	glVertex3f(p.x, p.y, p.z);
    }
	glEnd();
}

// GLUT callback to handle rendering the scene
void renderScene(void) {
	static float time_increment = 0.005;

	emitter.update(time_increment);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,   // FOV y
				   1.0f,    // Aspect ratio (width/height)
				   0.1f,    // Near distance
				   1000.0f);// Far distance

	gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, // Eye position
			  0, 0, 0, // Lookat position
  			  camera_up.x, camera_up.y, camera_up.z);// Up vector

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glPushMatrix();
    {
		// calculate arcball rotation
		if (arcball_on && ((last_mpos[0] != cur_mpos[0]) || (last_mpos[1] != cur_mpos[1]))) {
			Vec3 m1 = getArcballVector(last_mpos[0], last_mpos[1]);
			Vec3 m2 = getArcballVector(cur_mpos[0], cur_mpos[1]);
			float angle = 2 * std::acos(std::min(1.0f, Vec3::dotProduct(m1, m2)));
			Vec3 axis = -Vec3::normalize(Vec3::crossProduct(m1, m2) * rotation_matrix);

			Matrix new_rotation = calcRotationMatrix(angle, axis);
			rotation_matrix = rotation_matrix * new_rotation;

			camera_pos = camera_pos * new_rotation;
			camera_right = Vec3::normalize(camera_right * new_rotation);
			camera_up = Vec3::normalize(Vec3::crossProduct(camera_right, -camera_pos));
			
			for(int i = 0; i < 2; i++) {
				last_mpos[i] = cur_mpos[i];
			}

		}

		// glMultMatrixf(rotation_matrix.data);

		if (display_path) {
			renderPath();
		}

	    if (display_control_points) {
	    	renderControlPoints();
		}

		emitter.render(camera_up, camera_right);
	}

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutCreateWindow("Assignment 3 - Eric Roberts");

	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboardInput);
	glutSpecialFunc(specialKeyboardInput);
	glutMouseFunc(mouseAction);
	glutMotionFunc(mouseMotion);

	int tex_handle = loadImage("explosion.png", "textures/");
	emitter.setTextureAtlas(tex_handle, 12, 5);
	emitter.setSize(4, 4);
	// emitter.setColour(Vec3(1, 1, 1), Vec3(1, 1, 1));

	initializeScene();

	glutMainLoop();

	return(0);
}