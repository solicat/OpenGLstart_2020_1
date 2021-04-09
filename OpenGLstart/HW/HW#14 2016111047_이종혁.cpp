#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592
const char* vsSource = "#version 130 \n\
in vec4 aPosition; \n\
in vec4 aColor; \n\
out vec4 vColor; \n\
uniform mat4 urotate; \n\
void main(void) { \n\
gl_Position = urotate * aPosition; \n\
vColor = aColor; \n\
}";
const char* fsSource = "#version 130 \n\
in vec4 vColor; \n\
void main(void) { \n\
gl_FragColor = vColor; \n\
}";
GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;
char buf[1024];
float t = 0.0f;
int mode = 0;

GLfloat vertices[] = {
-0.1, -0.1, -0.1, 1.0, // 0
-0.1, -0.1, +0.1, 1.0, // 1
+0.1, -0.1, -0.1, 1.0, // 2
+0.1, -0.1, +0.1, 1.0, // 3
0.0, +0.1, 0.0, 1.0 //4
};

GLfloat colors[] = { 1.0, 0.0, 0.0, 1.0,
0.0, 1.0, 0.0, 1.0,
0.0, 0.0, 1.0, 1.0,
1.0, 1.0, 0.0, 1.0,
1.0, 0.0, 1.0, 1.0
};

GLushort indices[] = { 
	0, 1, 2,
	1, 2, 3,
	0, 1, 4,
	1, 3, 4,
	2, 3, 4,
	0, 2, 4
};

void mymenu(int id)
{
	switch (id)
	{
	case 0:
		mode = 0;
		break;
	case 1:
		mode = 1;
		break;
	case 2:
		mode = 2;
		break;
	}
}

void myinit(void) {
	glEnable(GL_DEPTH_TEST);

	GLint status;
	printf("2016111047 ¿Ã¡æ«ı\n");
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs); // compile to get .OBJ
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	printf("vs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(vs, sizeof(buf), NULL, buf);
	printf("vs log = [%s]\n", buf);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs); // compile to get .OBJ
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	printf("fs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(fs, sizeof(buf), NULL, buf);
	printf("fs log = [%s]\n", buf);
	// prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog); // link to get .EXE
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	printf("program link status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("link log = [%s]\n", buf);
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("program validate status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("validate log = [%s]\n", buf);
	glUseProgram(prog);


	GLuint loc;
	GLuint vbo[1];
	// using vertex buffer object
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 5 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, 5 * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(GLfloat), 5 * 4 * sizeof(GLfloat), colors);

	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(5 * 4 * sizeof(GLfloat)));
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

void myidle(void)
{
	t += 0.001f;
	if (t > 2*PI)
		t = 0.0f;

	glutPostRedisplay();
}

void mydisplay(void) {
	GLuint loc;
	GLfloat m[16];

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // gray
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mode == 0)
	{

		m[0] = 1.0;		m[4] = 0.0;		m[8] = 0.0;		m[12] = 0.0;
		m[1] = 0.0;		m[5] = cos(t);	m[9] = -sin(t);	m[13] = 0.0;
		m[2] = 0.0;		m[6] = sin(t);	m[10] = cos(t);	m[14] = 0.0;
		m[3] = 0.0;		m[7] = 0.0;		m[11] = 0.0;	m[15] = 1.0;
	}
	else if (mode == 1)
	{
		m[0] = cos(t);	m[4] = 0.0;		m[8] = sin(t);	m[12] = 0.0;
		m[1] = 0.0;		m[5] = 1.0;		m[9] = 0.0;		m[13] = 0.0;
		m[2] = -sin(t);	m[6] = 0.0;		m[10] = cos(t);	m[14] = 0.0;
		m[3] = 0.0;		m[7] = 0.0;		m[11] = 0.0;	m[15] = 1.0;
	}
	else if (mode == 2)
	{
		m[0] = cos(t);	m[4] = -sin(t);	m[8] = 0.0;		m[12] = 0.0;
		m[1] = sin(t);	m[5] = cos(t);	m[9] = 0.0;		m[13] = 0.0;
		m[2] = 0.0;		m[6] = 0.0;		m[10] = 1.0;	m[14] = 0.0;
		m[3] = 0.0;		m[7] = 0.0;		m[11] = 0.0;	m[15] = 1.0;
	}

	loc = glGetUniformLocation(prog, "urotate");
	glUniformMatrix4fv(loc, 1, GL_FALSE, m);

	glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_SHORT, indices);

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2016111047 ¿Ã¡æ«ı");
	glutCreateMenu(mymenu);
	glutAddMenuEntry("x-axis", 0);
	glutAddMenuEntry("y-axis", 1);
	glutAddMenuEntry("z-axis", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(mydisplay);
	glutIdleFunc(myidle);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}