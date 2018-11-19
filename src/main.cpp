#define _USE_MATH_DEFINES

#include <math.h>
#include <string>
#include <iostream>
#include <iomanip>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include "vec3.hpp"
#include "path.hpp"
#include "stb_image.h"

#define SCREENWIDTH 512
#define SCREENHEIGHT 512

Path path(Path::CATMULROM);
bool display_path = true;
bool display_control_points = true;

void keyboardInput(unsigned char key, int x, int y) {
	if (key == 'd') {
		display_path = !display_path;
		display_control_points = !display_control_points;
	}
}

void specialKeyboardInput(int key, int x, int y) {
}

unsigned int loadImage(std::string file, std::string path) {
	std::string filename = path + file;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);
	
	if (data == NULL) {
		std::cout << "failed to load image " << filename << '\n';
		exit(1);
	}
	
	// create openGL texture
	unsigned int texture;
	glGenTextures(1, &texture);
	
	// bind texture for future operations
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// give image to openGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	GLenum error_enum = glGetError();
	if (GL_NO_ERROR != error_enum) {
		std::cout << "an openGL error occurred while loading " << filename << '\n';
		exit(1);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(data);
	return texture;
}

void initializeScene()
{
	glShadeModel( GL_FLAT );
	// glShadeModel( GL_SMOOTH );

  	glEnable(GL_DEPTH_TEST);
  	glEnable(GL_NORMALIZE);

  	path.addPoint(Vec3(-1, 0, -1));
  	path.addPoint(Vec3(1, 0, -2));
  	path.addPoint(Vec3(1, 1, -3));
  	path.addPoint(Vec3(-1, 1, -2));
}

// GLUT callback to handle rendering the scene
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,   // FOV y
				   1.0f,    // Aspect ratio (width/height)
				   0.1f,    // Near distance
				   1000.0f);// Far distance

	gluLookAt(0, 0, 3, // Eye position
			  0, 0, 0, // Lookat position
  			  0, 1, 0);// Up vector

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    if (display_control_points) {
	    glColor3ub(255, 128, 128);
	    glBegin(GL_LINE_STRIP);
	    for(int i = 0; i < path.size(); i++) {
	    	const Vec3& p = path[i];
	    	glVertex3f(p.x, p.y, p.z);
	    }
		const Vec3& p = path[0];
		glVertex3f(p.x, p.y, p.z);
		glEnd();
	}

	if (display_path) {
	    glColor3ub(128, 128, 255);
	    glBegin(GL_LINE_STRIP);
	    Path::Iterator it = path.begin();
	    int curve_sections = 16;
	    for(int i = 0; i < path.size() * curve_sections; i++) {
	    	const Vec3 p = it.getPoint();
	    	glVertex3f(p.x, p.y, p.z);
	    	it += 1.0 / curve_sections;
	    }
		const Vec3 p = it.getPoint();
		glVertex3f(p.x, p.y, p.z);
		glEnd();
	}

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_SINGLE|GLUT_RGB);

	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREENWIDTH, SCREENHEIGHT );
	glutCreateWindow("Assignment 3 - Eric Roberts");

	glViewport( 0, 0, SCREENWIDTH, SCREENHEIGHT );
	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboardInput);
	glutSpecialFunc(specialKeyboardInput);

	initializeScene();

	glutMainLoop();

	return(0);
}