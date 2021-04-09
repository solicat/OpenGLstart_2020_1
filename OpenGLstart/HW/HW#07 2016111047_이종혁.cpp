#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const static char* vsSource= "#version 120 \n\
attribute vec4 vertex; \n\
void main(void) { \n\
	gl_Position= vertex; \n\
}";

const static char* fsSource= "#version 120 \n\
void main(void) { \n\
	gl_FragColor= vec4(1.0, 0.0, 0.0, 1.0); \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

GLfloat vertices[5000][4] = {
	{-0.5, -0.5, 0.0, 1.0},
	{+0.5, -0.5, 0.0, 1.0},
	{-0.5, +0.5, 0.0, 1.0}
};

void myinit(void) {
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs);
	prog = glCreateProgram();

	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glUseProgram(prog);
	
	srand(time(NULL));

	int r;
	GLfloat temp[2];

	do {
		temp[0] = rand() / 32767.0 - 0.5;
		temp[1] = rand() / 32767.0 - 0.5;
		//printf("%lf, %lf\n", temp[0], temp[1]);
	} while ((temp[0] + temp[1] > 0 ));

	for (int i = 3; i < 5000; i++)
	{
		r = rand() % 3;
		vertices[i][0] = (vertices[r][0] + temp[0]) / 2;
		vertices[i][1] = (vertices[r][1] + temp[1]) / 2;
		vertices[i][2] = 0.0;
		vertices[i][3] = 1.0;
		temp[0] = vertices[i][0];
		temp[1] = vertices[i][1];
	}
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

void mydisplay(void) {
	GLuint loc;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	loc = glGetAttribLocation(prog, "vertex"); // provide the vertex attributes
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawArrays(GL_POINTS, 0, 5000);
	glFlush();  // Flush all 
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(0, 0); 
	glutCreateWindow("2016111047 ÀÌÁ¾Çõ");
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}