#include "GL\glew.h"
#include "Mesh.h"
#include "skybox.h"
#include "GL\glut.h"
#include "GL\glaux.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mesh::Mesh()
{

}
Mesh::~Mesh()
{

}

void Mesh::loadTexture(const char *file, GLuint &texture_index)
{
	glGenTextures(1, &texture_index);
	FILE *fp;
	fopen_s(&fp, file, "rb");
	if (!fp) {
		printf("ERROR: NB %s.\nfail to bind %d\n", file, texture_index);
	}
	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);
	// Binding texture
	glBindTexture(GL_TEXTURE_2D, texture_index);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
}


void Mesh::loadObj(const char *file)
{
	FILE *fp;
	int index[3],tex[3], empty[3], id;
	char buffer[100] = { 0 };
	Vec3<double> pos;
	Vec3<double> _minBound, _maxBound;
	_minBound.Set(DBL_MAX);
	_maxBound.Set(DBL_MIN);
	
	id = 0;
	fopen_s(&fp, file, "r");
	while (fscanf(fp, "%s %lf %lf %lf", &buffer, &pos[0], &pos[1], &pos[2]) != EOF){
		if (buffer[0] == 'v' && buffer[1] == NULL) {
			if (_minBound[0] > pos[0]) _minBound[0] = pos[0];
			if (_minBound[1] > pos[1]) _minBound[1] = pos[1];
			if (_minBound[2] > pos[2]) _minBound[2] = pos[2];

			if (_maxBound[0] < pos[0]) _maxBound[0] = pos[0];
			if (_maxBound[1] < pos[1]) _maxBound[1] = pos[1];
			if (_maxBound[2] < pos[2]) _maxBound[2] = pos[2];
			_vertices.push_back(new Vertex(id++, pos));
		}
	}
	printf("num. of vertices : %d\n", _vertices.size());
	id = 0;
	fseek(fp, 0, SEEK_SET);
	while (fscanf(fp, "%s %lf %lf", &buffer, &pos[0], &pos[1]) != EOF) {
		if (!strcmp(buffer, "vt")) {
			_textureCoords.push_back(new Texture(pos[0], 1.0 - pos[1], 0.0));
		}
	}

	id = 0;
	fseek(fp, 0, SEEK_SET);
	while (fscanf(fp, "%s %d/%d/%d %d/%d/%d %d/%d/%d", &buffer, &index[0], &tex[0], &empty[0], &index[1], &tex[1], &empty[1], &index[2], &tex[2], &empty[2]) != EOF) {

		if (buffer[0] == 'f' && buffer[1] == NULL) {
			_faces.push_back(new Face(id++, _vertices[index[0] - 1], _vertices[index[1] - 1], _vertices[index[2] - 1], tex[0]-1, tex[1]-1, tex[2]-1));
		}
	} 

	printf("num. of faces : %d\n", _faces.size());
	fclose(fp);
	moveToCenter(_minBound, _maxBound, 3.0f);
	buildList();
	computeNormal();
}

void Mesh::buildList(void)
{
	for (auto f : _faces) {
		for (auto v : f->_vertices) {
			v->_nbFaces.push_back(f);
		}
	}
}

void Mesh::computeNormal(void)
{
	//face normal
	for (auto f : _faces) {
		auto v01 = f->_vertices[1]->_pos - f->_vertices[0]->_pos;
		auto v02 = f->_vertices[2]->_pos - f->_vertices[0]->_pos;
		f->_normal = v01.Cross(v02);
		f->_normal.Normalize();
	}

	//vertex normal
	for (auto v : _vertices) {
		v->_normal.Clear();
		for (auto f : v->_nbFaces) {
			v->_normal += f->_normal;
		}
		v->_normal /= (double)v->_nbFaces.size();
	}
}

void Mesh::drawSurface(bool smoothing)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIndex);
	glEnable(GL_LIGHTING);
	if (smoothing) {
		glEnable(GL_SMOOTH);
	}
	else {
		glEnable(GL_FLAT);
	}
	for (auto f : _faces) {
		glBegin(GL_POLYGON);
		if (smoothing) {
			for (auto v : f->_vertices) {
				glNormal3f(v->_normal.x(), v->_normal.y(), v->_normal.z());
				glVertex3f(v->x(), v->y(), v->z());
			}
		}
		else {
			glNormal3f(f->_normal.x(), f->_normal.y(), f->_normal.z());
			for (int i = 0; i < 3; i++) {
				auto t = _textureCoords[f->_texelPos[i]];
				auto v = f->_vertices[i];
				glTexCoord2f((GLfloat)t->x(), (GLfloat)t->y());
				glVertex3f(v->x(), v->y(), v->z());
			}

		}
		glEnd();
	}
	glPointSize(1.0f);
	glDisable(GL_LIGHTING);
	glDisable(GL_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Mesh::drawPoint(void)
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glPointSize(2.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);

	for (auto v : _vertices) {
		auto pos = v->_pos;
		glVertex3f(pos.x(), pos.y(), pos.z());
	}
	glEnd();
	glPointSize(1.0f);
	glDisable(GL_LIGHTING);
	glPopMatrix();
}

void Mesh::moveToCenter(Vec3<double> minBound, Vec3<double> maxBound, double scale)
{
	double longestLength = fmaxf(fmaxf(fabs(maxBound.x() - minBound.x()), fabs(maxBound.y() - minBound.y())), fabs(maxBound.z() - minBound.z()));
	auto center = (maxBound + minBound) / 2.0f;
	Vec3<double> origin(0.0, 0.0, 0.0);

	for (auto v : _vertices) {
		auto pos = v->_pos;
		auto vecVertexFromCenter = pos - center;
		vecVertexFromCenter /= longestLength; //<=1
		vecVertexFromCenter *= scale; // <=1*scale
		pos = origin;
		vecVertexFromCenter *= scale;
		pos += vecVertexFromCenter;
		v->_pos = pos;

	}
}