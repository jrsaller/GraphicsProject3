// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <cmath>
#include <cstring>
#include <vector>
#include <ctime>
#include <iostream>
#include <GL/freeglut.h>
class Circle{
private:
	double x;
	double y;
	double deltaX;
	double deltaY;
	double radius;
	std::vector<double> color;
public:

	Circle() {}
	Circle(double x, double y, double deltaX, double deltaY, double radius, double red,double green, double blue) {
		this->x = x;
		this->y = y;
		this->deltaX = deltaX;
		this->deltaY = deltaY;
		this->radius = radius;
		this->color.push_back(red);
		this->color.push_back(green);
		this->color.push_back(blue);
	}

	double getX() {
		return this->x;
	}

	double getY() {
		return this->y;
	}

	double getDeltaX() {
		return this->deltaX;
	}

	double getDeltaY() {
		return this->deltaY;
	}

	double getRadius() {
		return this->radius;
	}

	std::vector<double> getColor() {
		return this->color;
	}
	
	void setX(double x) {
		this->x = x;
	}

	void setY(double y) {
		this->y = y;
	}

	void setDeltaX(double deltaX) {
		this->deltaX = deltaX;
	}

	void setDeltaY(double deltaY) {
		this->deltaY = deltaY;
	}

	void setRadius(double radius) {
		this->radius = radius;
	}

	void setColor(std::vector<double> color) {
		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
	}

	double getNextX() { return x + deltaX; }
	double getNextY() { return y + deltaY; }

};



// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;
std::vector <Circle> circles;
const double COLLISION_FRICTION = 1;

struct vectortype
{
	double x;
	double y;
};

void Collide(int p1, int p2, std::vector<Circle> &particles)
{
	vectortype en; // Center of Mass coordinate system, normal component
	vectortype et; // Center of Mass coordinate system, tangential component
	vectortype u[2]; // initial velocities of two particles
	vectortype um[2]; // initial velocities in Center of Mass coordinates
	double umt[2]; // initial velocities in Center of Mass coordinates, tangent component
	double umn[2]; // initial velocities in Center of Mass coordinates, normal component
	vectortype v[2]; // final velocities of two particles
	double m[2];	// mass of two particles
	double M; // mass of two particles together
	vectortype V; // velocity of two particles together
	double size;
	int i;

	double xdif = particles[p1].getNextX() - particles[p2].getNextX();
	double ydif = particles[p1].getNextY() - particles[p2].getNextY();

	// set Center of Mass coordinate system
	size = sqrt(xdif*xdif + ydif * ydif);
	xdif /= size; ydif /= size; // normalize
	en.x = xdif;
	en.y = ydif;
	et.x = ydif;
	et.y = -xdif;

	// set u values
	u[0].x = particles[p1].getDeltaX();
	u[0].y = particles[p1].getDeltaY();
	m[0] = particles[p1].getRadius()*particles[p1].getRadius();
	u[1].x = particles[p2].getDeltaX();
	u[1].y = particles[p2].getDeltaY();
	m[1] = particles[p2].getRadius()*particles[p2].getRadius();

	// set M and V
	M = m[0] + m[1];
	V.x = (u[0].x*m[0] + u[1].x*m[1]) / M;
	V.y = (u[0].y*m[0] + u[1].y*m[1]) / M;

	// set um values
	um[0].x = m[1] / M * (u[0].x - u[1].x);
	um[0].y = m[1] / M * (u[0].y - u[1].y);
	um[1].x = m[0] / M * (u[1].x - u[0].x);
	um[1].y = m[0] / M * (u[1].y - u[0].y);

	// set umt and umn values
	for (i = 0; i < 2; i++)
	{
		umt[i] = um[i].x * et.x + um[i].y * et.y;
		umn[i] = um[i].x * en.x + um[i].y * en.y;
	}

	// set v values
	for (i = 0; i < 2; i++)
	{
		v[i].x = umt[i] * et.x - umn[i] * en.x + V.x;
		v[i].y = umt[i] * et.y - umn[i] * en.y + V.y;
	}

	// reset particle values
	particles[p1].setDeltaX(v[0].x*COLLISION_FRICTION);
	particles[p1].setDeltaY(v[0].y*COLLISION_FRICTION);
	particles[p2].setDeltaX(v[1].x*COLLISION_FRICTION);
	particles[p2].setDeltaY(v[1].y*COLLISION_FRICTION);

} /* Collide */

// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t q = 0; q < circles.size();q++) {
	//apply gravity, then apply collisions,update x and y, draw	
		circles[q].setDeltaY(circles[q].getDeltaY() - (5.0 * (circles[q].getNextY() / screen_y)));//(0.002 * circles[q].getNextY()));
		for (size_t i = 0; i < circles.size();i++) {
			if (i != q) {
				double xdist = pow(circles[q].getNextX() - circles[i].getNextX(), 2);
				double ydist = pow(circles[q].getNextY() - circles[i].getNextY(), 2);
				double totalDist = pow(xdist + ydist, 0.5);
				if (totalDist <= (circles[q].getRadius() + circles[i].getRadius())) {
					Collide(q, i, circles);
					//circles[q].setX(rand() % (int)(screen_x - (2 * circles[q].getRadius())) + circles[q].getRadius());
				}
			}
		}
		std::vector <double> color = circles[q].getColor();
		glColor3d(color[0], color[1], color[2]);			//get the color of the ball
		if (circles[q].getNextX() + circles[q].getRadius() + circles[q].getDeltaX() >= screen_x)
			//dx = -dx;
			circles[q].setDeltaX(-circles[q].getDeltaX());
			
		if (circles[q].getNextX() - circles[q].getRadius() + circles[q].getDeltaX() < 0)
			//dx = -dx;
			circles[q].setDeltaX(-circles[q].getDeltaX());
		if (circles[q].getNextY() + circles[q].getRadius() + circles[q].getDeltaY() >= screen_y)
			//dy = -dy;
			circles[q].setDeltaY(-circles[q].getDeltaY());
		if (circles[q].getNextY() - circles[q].getRadius() + circles[q].getDeltaY() < 0)
			//dy = -dy;
			circles[q].setDeltaY(-circles[q].getDeltaY());
		
		circles[q].setX(circles[q].getNextX());
		circles[q].setY(circles[q].getNextY());
		//x += dx;
		//y += dy;

		for (size_t i = 0; i < circles.size();i++) {
			if (i != q) {
				double xdist = pow(circles[q].getNextX() - circles[i].getNextX(), 2);
				double ydist = pow(circles[q].getNextY() - circles[i].getNextY(), 2);
				double totalDist = pow(xdist + ydist, 0.5);
				if (totalDist <= (circles[q].getRadius() + circles[i].getRadius())) {
					Collide(q, i, circles);
					//circles[q].setX(rand() % (int)(screen_x - (2 * circles[q].getRadius())) + circles[q].getRadius());
				}
			}
		}
		DrawCircle(circles[q].getX(), circles[q].getY(), circles[q].getRadius());
	}

	


	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		for (size_t x = 0; x < circles.size();x++) {
			circles[x].setDeltaX(0);
			circles[x].setDeltaY(0);
		}
		
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{

	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	srand(time(0));
	for (int i = 0; i < 10;i++) {
		double radius = rand() % 20 + 10;
		double x = rand() % (int)(screen_x - (2*radius)) + radius;
		double y = rand() % (int)(screen_y - (2*radius)) + radius;
		double deltaX = rand() % 10 / 1.0 -5 ;
		double deltaY = rand() % 10 / 1.0 -5 ;
		
		double red = (double)(rand() % 100) / 100.0;
		double green = (double)(rand() % 100) / 100.0;
		double blue = (double)(rand() % 100) / 100.0;
		Circle c = Circle(x, y, deltaX, deltaY, radius, red, green, blue);
		
		circles.push_back(c);
	}

	for (size_t x = 0; x < circles.size(); x++) {
		for (size_t y = x+1; y < circles.size();y++) {
			double xdist = pow(circles[x].getX() - circles[y].getX(), 2);
			double ydist = pow(circles[x].getY() - circles[y].getY(), 2);
			double totalDist = pow(xdist + ydist, 0.5);
			while (totalDist < circles[x].getRadius() + circles[y].getRadius()) {
				circles[x].setX(rand() % (int)(screen_x - (2 * circles[x].getRadius())) + circles[x].getRadius());
				circles[x].setY(rand() % (int)(screen_y - (2 * circles[x].getRadius())) + circles[x].getRadius());
				xdist = pow(circles[x].getX() - circles[y].getX(), 2);
				ydist = pow(circles[x].getY() - circles[y].getY(), 2);
				totalDist = pow(xdist + ydist, 0.5);
			}
		}
	}

}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Project 3:Bouncing Balls");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // foreground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
