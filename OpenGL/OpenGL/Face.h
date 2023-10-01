#ifndef __FACE_H__
#define __FACE_H__
#pragma once
#include "Vertex.h"

class Face
{
public:
	int _index;
	int _texelPos[3];
	Vec3<double> _normal;
	vector<Vertex*> _vertices;

public:
	Face();
	Face(int index, Vertex *v0, Vertex *v1, Vertex *v2)
	{
		_index = index;
		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
	}
	Face(int index, Vertex *v0, Vertex *v1, Vertex *v2, int tu, int tv, int tw)
	{
		_index = index;
		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		_texelPos[0] = tu;
		_texelPos[1] = tv;
		_texelPos[2] = tw;
	}
	~Face();
};

#endif