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
uniform vec3 u_trans_vec; \n\
uniform mat4 urx; \n\
uniform mat4 ury; \n\
uniform mat4 urz; \n\
void main(void) { \n\
mat4 transmat = mat4(1.0); \n\
mat4 transmatR = mat4(1.0); \n\
transmatR[3][0] = u_trans_vec[0] * -1.0; \n\
transmatR[3][1] = u_trans_vec[1] * -1.0; \n\
transmatR[3][2] = u_trans_vec[2] * -1.0; \n\
transmat[3][0] = u_trans_vec[0]; \n\
transmat[3][1] = u_trans_vec[1]; \n\
transmat[3][2] = u_trans_vec[2]; \n\
gl_Position = transmat * urz * ury * urx * transmatR * aPosition; \n\
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

GLfloat vertices[] = {
	0.5, 0.8, 0.0, 1.0, // 0
	0.3, 0.3, +0.2, 1.0, // 1
	0.7, 0.3, +0.2, 1.0, // 2
	0.7, 0.3, -0.2, 1.0, // 3
	0.3, 0.3, -0.2, 1.0, // 4
};

GLfloat colors[] = { 
	1.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	1.0, 0.0, 1.0, 1.0,
	1.0, 1.0, 0.0, 1.0
};

GLushort indices[] = {
	0, 1, 2,
	2, 3, 0,
	4, 0, 3,
	1, 0, 4,
	2, 3, 1,
	3, 4, 1
};

void myinit(void) {
	glEnable(GL_DEPTH_TEST);

	GLint status;
	printf("2016111047 ÀÌÁ¾Çõ\n");
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
	if (t > 2 * PI)
		t = 0.0f;

	glutPostRedisplay();
}

void mydisplay(void) {
	GLuint loc;
	GLfloat mx[16], my[16], mz[16];
	GLfloat tx, ty, tz;
	float trans_x, trans_y, trans_z;
	int i;

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // gray
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tx = ty = tz = 30 * PI / 180.0;

	mx[0] = 1.0;	mx[4] = 0.0;	mx[8] = 0.0;	mx[12] = 0.0;
	mx[1] = 0.0;	mx[5] = cos(tx);mx[9] = -sin(tx);mx[13] = 0.0;
	mx[2] = 0.0;	mx[6] = sin(tx);mx[10] = cos(tx);mx[14] = 0.0;
	mx[3] = 0.0;	mx[7] = 0.0;	mx[11] = 0.0;	mx[15] = 1.0;

	my[0] = cos(ty);my[4] = 0.0;	my[8] = sin(ty);my[12] = 0.0;
	my[1] = 0.0;	my[5] = 1.0;	my[9] = 0.0;	my[13] = 0.0;
	my[2] = -sin(ty);my[6] = 0.0;	my[10] = cos(ty);my[14] = 0.0;
	my[3] = 0.0;	my[7] = 0.0;	my[11] = 0.0;	my[15] = 1.0;
	
	mz[0] = cos(tz);mz[4] = -sin(tz);mz[8] = 0.0;	mz[12] = 0.0;
	mz[1] = sin(tz);mz[5] = cos(tz);mz[9] = 0.0;	mz[13] = 0.0;
	mz[2] = 0.0;	mz[6] = 0.0;	mz[10] = 1.0;	mz[14] = 0.0;
	mz[3] = 0.0;	mz[7] = 0.0;	mz[11] = 0.0;	mz[15] = 1.0;	

	loc = glGetUniformLocation(prog, "urx");
	glUniformMatrix4fv(loc, 1, GL_FALSE, mx);
	loc = glGetUniformLocation(prog, "ury");
	glUniformMatrix4fv(loc, 1, GL_FALSE, my);
	loc = glGetUniformLocation(prog, "urz");
	glUniformMatrix4fv(loc, 1, GL_FALSE, mz);

	trans_x = trans_y = trans_z = 0;

	for (i = 0; i < 5; i++)
	{
		trans_x += vertices[0 + i * 4];
		trans_y += vertices[1 + i * 4];
		trans_z += vertices[2 + i * 4];
	}
	trans_x /= 5.0;
	trans_y /= 5.0;
	trans_z /= 5.0;

	float trans_vec[] = { trans_x, trans_y, trans_z };
	
	loc = glGetUniformLocation(prog, "u_trans_vec");
	glUniform3fv(loc, 1, trans_vec);

	glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_SHORT, indices);

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2016111047 ÀÌÁ¾Çõ");
	glutDisplayFunc(mydisplay);
	glutIdleFunc(myidle);
	glutKeyboardFunc(mykeyboard);
	glewInit();
	myinit();
	glutMainLoop();
	return 0;
}