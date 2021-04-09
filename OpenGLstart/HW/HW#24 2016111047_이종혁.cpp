#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define my_PI 3.141592

const char* vsSource = "#version 140 \n\
in vec4 aPosition; \n\
in vec4 aNormal; \n\
out vec4 vPosition; \n\
out vec4 vNormal; \n\
uniform mat4 uscale; \n\
uniform mat4 utranslate; \n\
uniform mat4 urotate; \n\
uniform mat4 uView; \n\
void main(void) { \n\
	vPosition = uView * urotate * uscale * utranslate * aPosition; \n\
	mat4 mNormal = transpose(inverse(uView * urotate * uscale * utranslate)); \n\
	vNormal	= mNormal * aNormal; \n\
	gl_Position	= vPosition; \n\
}";

const char* fsSource = "#version 140 \n\
in vec4 vPosition; \n\
in vec4 vNormal; \n\
uniform vec4 light_position; \n\
uniform vec4 light_ambient; \n\
uniform vec4 light_diffuse; \n\
uniform vec4 light_specular; \n\
uniform vec4 light_att; \n\
uniform vec4 material_ambient; \n\
uniform vec4 material_diffuse; \n\
uniform vec4 material_specular; \n\
uniform float material_shineness; \n\
void main(void) { \n\
	vec3 N = normalize(vNormal.xyz); \n\
	vec3 L = normalize(light_position.xyz - vPosition.xyz); \n\
	vec3 V = normalize(vec3(0.0, 0.0, 0.0) - vPosition.xyz); \n\
	vec3 H = normalize(L + V); \n\
	vec4 ambient = light_ambient * material_ambient; \n\
	float d = length(light_position.xyz - vPosition.xyz); \n\
	float denom = light_att.x + light_att.y * d + light_att.z * d * d; \n\
	vec4 diffuse = max(dot(L, N), 0.0) * light_diffuse * material_diffuse / denom; \n\
	vec4 specular = pow(max(dot(N, H), 0.0), material_shineness) * light_specular * material_specular / denom; \n\
	vec4 vColor = ambient + diffuse + specular; \n\
	gl_FragColor = vColor; \n\
}";

GLuint vs = 0;
GLuint fs = 0;
GLuint prog = 0;

float eye[] = { 0.0, 0.0, 0.5, 1.0 };
float at[] = { 0.0, 0.0, 0.0, 1.0 };
float up[] = { 0.0, 1.0, 0.0, 0.0 };

char buf[1024];
int DRAW_MODE = 0;
float s = 0.0, t = 0.0;

int num_vertices, num_faces;

GLfloat* vertices;
GLfloat* colors;
GLfloat* normals;
GLushort* indices;
GLfloat min_x, max_x, min_y, max_y, min_z, max_z;

void setLightAndMaterial()
{
	GLfloat light_pos[4] = { 0.5, 1.0, -2.0, 1.0 }; 
	GLfloat light_amb[4] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_dif[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_spe[4] = { 1.0, 1.0, 1.0, 1.0 }; 
	GLfloat light_att[4] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat mat_amb[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_dif[4] = { 1.0, 1.0, 1.0, 1.0 }; 
	GLfloat mat_spe[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shi = 1;
	GLuint loc;

	// light
	loc = glGetUniformLocation(prog, "light_position"); glUniform4fv(loc, 1, light_pos);
	loc = glGetUniformLocation(prog, "light_ambient"); glUniform4fv(loc, 1, light_amb);
	loc = glGetUniformLocation(prog, "light_diffuse"); glUniform4fv(loc, 1, light_dif);
	loc = glGetUniformLocation(prog, "light_specular"); glUniform4fv(loc, 1, light_spe);
	loc = glGetUniformLocation(prog, "light_att"); glUniform4fv(loc, 1, light_att);
	// material
	loc = glGetUniformLocation(prog, "material_ambient"); glUniform4fv(loc, 1, mat_amb);
	loc = glGetUniformLocation(prog, "material_diffuse"); glUniform4fv(loc, 1, mat_dif);
	loc = glGetUniformLocation(prog, "material_specular"); glUniform4fv(loc, 1, mat_spe);
	loc = glGetUniformLocation(prog, "material_shineness"); glUniform1f(loc, mat_shi);
}

void myinit(void) {
	GLint status;
	FILE* f;
	int i;

	printf("2016111047 ¿Ã¡æ«ı\n");
	f = fopen("teapot.obj", "r");
	fscanf(f, " %d %d", &num_vertices, &num_faces);
	vertices = (GLfloat*)malloc(sizeof(GLfloat) * 4 * num_vertices);
	for (i = 0; i < num_vertices * 4; i++)
	{
		if (i % 4 == 3)
			vertices[i] = 1.0;
		else
			fscanf(f, " %f", &vertices[i]);
	}

	colors = (GLfloat*)malloc(sizeof(GLfloat) * 4 * num_vertices);
	for (i = 0; i < num_vertices * 4; i += 4)
	{
		colors[i] = 1.0;
		colors[i + 1] = 0.0;
		colors[i + 2] = 1.0;
		colors[i + 3] = 1.0;
	}

	normals = (GLfloat*)malloc(sizeof(GLfloat) * 4 * num_vertices);
	for (i = 0; i < num_vertices * 4; i++)
	{
		if (i % 4 == 3)
			normals[i] = 0.0;
		else
			fscanf(f, " %f", &normals[i]);
	}

	indices = (GLushort*)malloc(sizeof(GLushort) * 3 * num_faces);
	for (i = 0; i < num_faces * 3; i++)
		fscanf(f, " %d", &indices[i]);

	fclose(f);

	min_x = max_x = vertices[0];
	min_y = max_y = vertices[1];
	min_z = max_z = vertices[2];
	for (i = 4; i < num_vertices * 4; i += 4)
	{
		if (min_x > vertices[i])
			min_x = vertices[i];
		if (max_x < vertices[i])
			max_x = vertices[i];
		if (min_y > vertices[i + 1])
			min_y = vertices[i + 1];
		if (max_y < vertices[i + 1])
			max_y = vertices[i + 1];
		if (min_z > vertices[i + 2])
			min_z = vertices[i + 2];
		if (max_z < vertices[i + 2])
			max_z = vertices[i + 2];
	}

	printf("Center: (%f, %f, %f)\n", (min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	printf("Box length in x-direction: %f\n", max_x - min_x);
	printf("Box length in y-direction: %f\n", max_y - min_y);
	printf("Box length in z-direction: %f\n", max_z - min_z);

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
	glBufferData(GL_ARRAY_BUFFER, 3 * num_vertices * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * 4 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, num_vertices * 4 * sizeof(GLfloat), num_vertices * 4 * sizeof(GLfloat), colors);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * num_vertices * 4 * sizeof(GLfloat), num_vertices * 4 * sizeof(GLfloat), normals);

	loc = glGetAttribLocation(prog, "aPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	loc = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(num_vertices * 4 * sizeof(GLfloat)));

	loc = glGetAttribLocation(prog, "aNormal");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(2 * num_vertices * 4 * sizeof(GLfloat)));

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glEnable(GL_DEPTH_TEST);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void mykeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESCAPE
		exit(0);
		break;
	}
}

void myidle(void) {
	t += 0.0003f;
	s += 0.0003f;
	// redisplay 
//	glutPostRedisplay();
}

GLfloat m[16], m_view[16], m_ortho[16];

void vec_minus(float* v3, float* v1, float* v2)
{   // v3 = v1 - v2
	for (int i = 0; i < 4; i++)
		v3[i] = v1[i] - v2[i];
}
float vec_dot_prod(float* v1, float* v2)
{   // return the dot product of v1 and v2
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float vec_len(float* v)
{
	// return the length of v
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void vec_normalize(float* nv, float* v)
{   // nv = normalized v
	float len;
	len = vec_len(v);
	nv[0] = v[0] / len;
	nv[1] = v[1] / len;
	nv[2] = v[2] / len;
	nv[3] = v[3] / len;
}

void vec_cross_prod(float* v3, float* v1, float* v2)
{
	// v3 = v1 x v2
	v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
	v3[1] = -v1[0] * v2[2] + v1[2] * v2[0];
	v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void vec_assign(float* v2, float* v1)
{
	for (int i = 0; i < 4; i++)
		v2[i] = v1[i];
}

void LookAt(float* m_view, float* eye, float* at, float* up)
{// construct m_view by using eye, at, up
// Fill this function. You may use any functions defined in this program to compute m_view.
	float p[4], n[4], v[4], u[4];
	int i;

	vec_assign(p, eye);

	vec_minus(n, at, eye);
	vec_normalize(n, n);

	for (i = 0; i < 4; i++)
		v[i] = -vec_dot_prod(up, n) * n[i] + up[i];
	vec_normalize(v, v);

	vec_cross_prod(u, n, v);
	vec_normalize(u, u);

	m_view[0] = u[0];	m_view[4] = u[1];	 m_view[8] = u[2];	m_view[12] = -vec_dot_prod(p, u);
	m_view[1] = v[0];	m_view[5] = v[1];	 m_view[9] = v[2];	m_view[13] = -vec_dot_prod(p, v);
	m_view[2] = n[0];	m_view[6] = n[1];	 m_view[10] = n[2]; m_view[14] = -vec_dot_prod(p, n);
	m_view[3] = 0.0;	m_view[7] = 0.0;	 m_view[11] = 0.0;	m_view[15] = 1.0;

}

void myOrtho(float* m_ortho, float x_min, float x_max, float y_min, float y_max, float z_min, float z_max)
{
	m_ortho[0] = 2 / (x_max - x_min);	m_ortho[4] = 0.0;					m_ortho[8] = 0.0;					m_ortho[12] = -((x_max + x_min) / (x_max - x_min));
	m_ortho[1] = 0.0;					m_ortho[5] = 2 / (y_max - y_min);	m_ortho[9] = 0.0;					m_ortho[13] = -((y_max + y_min) / (y_max - y_min));
	m_ortho[2] = 0.0;					m_ortho[6] = 0.0;					m_ortho[10] = 2 / (z_max - z_min);	m_ortho[14] = -((z_max + z_min) / (z_max - z_min));
	m_ortho[3] = 0.0;					m_ortho[7] = 0.0;					m_ortho[11] = 0.0;					m_ortho[15] = 1.0;
}

void myFrust(float* m_persp, float x_min, float x_max, float y_min, float y_max, float z_min, float z_max)
{
	m_persp[0] = (2 * z_min) / (x_max - x_min);		m_persp[4] = 0.0;								m_persp[8] = -((x_max + x_min) / (x_max - x_min));	m_persp[12] = 0.0;
	m_persp[1] = 0.0;								m_persp[5] = (2 * z_min) / (y_max - y_min);		m_persp[9] = -((y_max + y_min) / (y_max - y_min));	m_persp[13] = 0.0;
	m_persp[2] = 0.0;								m_persp[6] = 0.0;								m_persp[10] = (z_max + z_min) / (z_max - z_min);	m_persp[14] = -((2 * z_max * z_min) / (z_max - z_min));
	m_persp[3] = 0.0;								m_persp[7] = 0.0;								m_persp[11] = 1.0;									m_persp[15] = 0.0;
}

void mat_translate(float* m, float* t_v)
{
	m[0] = 1.0;      m[4] = 0.0;     m[8] = 0.0;      m[12] = t_v[0];
	m[1] = 0.0;      m[5] = 1.0;     m[9] = 0.0;      m[13] = t_v[1];
	m[2] = 0.0;      m[6] = 0.0;     m[10] = 1.0;     m[14] = t_v[2];
	m[3] = 0.0;      m[7] = 0.0;     m[11] = 0.0;     m[15] = 1.0;
}
void mat_scale(float* m, float* s_v)
{
	m[0] = s_v[0];   m[4] = 0.0;     m[8] = 0.0;      m[12] = 0.0;
	m[1] = 0.0;      m[5] = s_v[1];  m[9] = 0.0;      m[13] = 0.0;
	m[2] = 0.0;      m[6] = 0.0;     m[10] = s_v[2];  m[14] = 0.0;
	m[3] = 0.0;      m[7] = 0.0;     m[11] = 0.0;     m[15] = 1.0;
}
void mat_rotate_y(float* m, float t)
{
	m[0] = cos(t);   m[4] = 0.0;     m[8] = sin(t);   m[12] = 0.0;
	m[1] = 0.0;      m[5] = 1.0;     m[9] = 0.0;      m[13] = 0.0;
	m[2] = -sin(t);  m[6] = 0.0;     m[10] = cos(t);  m[14] = 0.0;
	m[3] = 0.0;      m[7] = 0.0;     m[11] = 0.0;     m[15] = 1.0;
}
void mydisplay(void) {
	GLuint loc;
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // gray
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat translate[16];
	translate[0] = 1.0;		translate[4] = 0.0;		translate[8] = 0.0;		translate[12] = -(max_x + min_x) / 2;
	translate[1] = 0.0;		translate[5] = 1.0;		translate[9] = 0.0;		translate[13] = -(max_y + min_y) / 2;
	translate[2] = 0.0;		translate[6] = 0.0;		translate[10] = 1.0;	translate[14] = -(max_z + min_z) / 2;
	translate[3] = 0.0;		translate[7] = 0.0;		translate[11] = 0.0;	translate[15] = 1.0;

	loc = glGetUniformLocation(prog, "utranslate");
	glUniformMatrix4fv(loc, 1, GL_FALSE, translate);

	GLfloat scale[16];
	scale[0] = 2.0 / (max_x - min_x);		scale[4] = 0.0;							scale[8] = 0.0;							scale[12] = 0.0;
	scale[1] = 0.0;							scale[5] = 1.0 / (max_y - min_y);		scale[9] = 0.0;							scale[13] = 0.0;
	scale[2] = 0.0;							scale[6] = 0.0;							scale[10] = 2.0 / (max_z - min_z);		scale[14] = 0.0;
	scale[3] = 0.0;							scale[7] = 0.0;							scale[11] = 0.0;						scale[15] = 1.0;

	loc = glGetUniformLocation(prog, "uscale");
	glUniformMatrix4fv(loc, 1, GL_FALSE, scale);

	GLfloat rotate[16] = { 1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0 };

	loc = glGetUniformLocation(prog, "urotate");
	glUniformMatrix4fv(loc, 1, GL_FALSE, rotate);

	LookAt(m, eye, at, up);
	loc = glGetUniformLocation(prog, "uView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, m);

	setLightAndMaterial();

	glDrawElements(GL_TRIANGLES, num_faces * 3, GL_UNSIGNED_SHORT, indices);
	glFlush();

	glutSwapBuffers();
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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
