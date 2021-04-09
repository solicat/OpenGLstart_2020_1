#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

const char* vsSource = "#version 130 \n\
in vec4 aPosition; \n\
in vec4 aColor; \n\
out vec4 vColor; \n\
uniform float uxdist; \n\
uniform float uydist; \n\
void main(void) { \n\
	gl_Position.x = aPosition.x + uxdist; \n\
	gl_Position.y = aPosition.y + uydist; \n\
	gl_Position.zw = aPosition.zw; \n\
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
float xdist = 0.0f;
float ydist = 0.0f;
float step = 0.0001f;
int xdir = 0;
int ydir = 0;

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
	printf("program link status = %s\n", (status == GL_TRUE) ? "true" :	"false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("link log = [%s]\n", buf);

	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("program validate status = %s\n", (status == GL_TRUE) ? "true" :	"false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("validate log = [%s]\n", buf);

	glUseProgram(prog);
}
void mykeyboard(unsigned char key, int x, int y) {
	printf("%c\n", key);
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	case 'w':
		xdir = 0;
		ydir = 1;
		break;
	case 'a':
		xdir = -1;
		ydir = 0;
		break;
	case 's':
		xdir = 0;
		ydir = -1;
		break;
	case 'd':
		xdir = 1;
		ydir = 0;
		break;
	case 'i':
		xdist = 0.0f;
		ydist = 0.0f;
		xdir = 0;
		ydir = 0;
		break;
	}
}

GLfloat vertices[] = {
	0.0, 0.0, 0.0, 1.0,
	0.3, 0.0, 0.0, 1.0,
	0.0, 0.3, 0.0, 1.0
};

GLfloat colors[] = {
	1.0, 0.0, 0.0, 1.0, // red
	0.0, 1.0, 0.0, 1.0, // green
	0.0, 0.0, 1.0, 1.0, // blue
};

void myidle(void) {

	xdist += xdir * step;
	ydist += ydir * step;

	glutPostRedisplay();
}
void mydisplay(void) {
	GLuint loc;
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // gray
	glClear(GL_COLOR_BUFFER_BIT);

	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertices);

	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, colors);

	loc = glGetUniformLocation(prog, "uxdist");
	glUniform1f(loc, xdist);

	loc = glGetUniformLocation(prog, "uydist");
	glUniform1f(loc, ydist);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();
	glutSwapBuffers();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2016111047 ¿Ã¡æ«ı");
	glutDisplayFunc(mydisplay);
	glutIdleFunc(myidle);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}