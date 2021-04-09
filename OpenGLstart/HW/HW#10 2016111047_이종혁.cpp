#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const static char* vsSource= "#version 130 \n\
in vec4 vertex; \n\
in vec4 acolor; \n\
out vec4 fcolor; \n\
void main(void) { \n\
	gl_Position= vertex; \n\
	fcolor = acolor; \n\
}";

const static char* fsSource= "#version 130 \n\
in vec4 fcolor; \n\
void main(void) { \n\
	gl_FragColor= fcolor; \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

GLfloat vertices[] = {
	-0.5, -0.5, 0.0, 1.0,
	+0.5, -0.5, 0.0, 1.0,
	-0.5, +0.5, 0.0, 1.0,
	+0.5, +0.5, 0.0, 1.0,
	0.0, +0.7, 0.0, 1.0
};

GLfloat colors[] = {
	1.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	1.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 1.0, 1.0
};

char buf[1024];

void myinit(void) {
	GLint status;

	printf("2016111047 ¿Ã¡æ«ı\n");

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSource, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	printf("vs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(vs, sizeof(buf), NULL, buf);
	printf("vs log = [%s]\n", buf);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSource, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	printf("fs compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(fs, sizeof(buf), NULL, buf);
	printf("fs log = [%s]\n", buf);

	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	printf("program link status = %s\n", (status == GL_TRUE) ? "true" : "false"); glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("link log = [%s]\n", buf);
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("program validate status = %s\n",
		(status == GL_TRUE) ? "true" : "false"); glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("validate log = [%s]\n", buf);
	glUseProgram(prog);
}

int DRAW_MODE = 0;

void mymenu(int id)
{
	switch (id)
	{
	case 0:
		DRAW_MODE = 0;
		break;
	case 1:
		DRAW_MODE = 1;
		break;
	case 2:
		DRAW_MODE = 2;
		break;
	}
	glutPostRedisplay();
}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) 
	{
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

	loc = glGetAttribLocation(prog, "acolor"); 
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, colors);

	if (DRAW_MODE == 0)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
	else if (DRAW_MODE == 1)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	else
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

	glFlush();  // Flush all 
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(0, 0); 
	glutCreateWindow("2016111047 ¿Ã¡æ«ı");

	glutCreateMenu(mymenu);
	glutAddMenuEntry("Triangle", 0);
	glutAddMenuEntry("Rectangle", 1);
	glutAddMenuEntry("Pentagon", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}