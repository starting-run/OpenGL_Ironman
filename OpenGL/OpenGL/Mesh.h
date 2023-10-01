#pragma once
#include "Face.h"
#include "Texture.h"
#include "GL/glut.h"

class Mesh
{
public:
	vector<Face*> _faces;
	vector<Vertex*> _vertices;
	vector<Texture*> _textureCoords;
	GLuint _textureIndex; // Multiple textures라면 GLuint ...[x] 배열 형태로 만들어야 한다.
public:
	Mesh();
	Mesh(const char *obj, const char *texture)
	{
		loadObj(obj);
		loadTexture(texture, _textureIndex);

	}
	~Mesh();
public:
	void loadObj(const char *file);
	void loadTexture(const char *file, GLuint &texture_index); //GLuint &index == 텍스쳐 아이디
	void buildList(void);
	void computeNormal(void);
	void moveToCenter(Vec3<double> minBound, Vec3<double> maxBound, double scale);
public:
	void drawPoint(void);
	void drawSurface(bool smoothing = false);
};

