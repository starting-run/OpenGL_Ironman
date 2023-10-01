#include "skybox.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL\glut.h"
#include "GL\glaux.h"

#define TEX_SIZE 20
#define GL_CLAMP_TO_EDGE 0x812F

const char *texFiles[6] = {
	"skybox\\front.bmp",
	"skybox\\back.bmp",
	"skybox\\left.bmp",
	"skybox\\right.bmp",
	"skybox\\top.bmp",
	"skybox\\bottom.bmp" };

Skybox::Skybox(void) {
	for (int i = 0; i < 6; i++)
		textures[i] = _loadTexture(texFiles[i]);
}
Skybox::~Skybox(void)
{

}

void Skybox::draw(bool blend) {
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glDisable(GL_DEPTH_TEST);
	//DECAL로 적용시 라이팅이 텍스쳐에 영향을 주지 않게되므로 퐁쉐이딩을 볼 수가 없음.
	if (blend)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	else
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glColor3f(0, 0, 0);
	//front
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glEnd();

	//back
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glEnd();

	//up
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, TEX_SIZE, TEX_SIZE);
	glEnd();

	//down
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 0.0); glVertex3f(TEX_SIZE, -TEX_SIZE, TEX_SIZE);
	glTexCoord2f(1.0, 1.0); glVertex3f(TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glTexCoord2f(0.0, 1.0); glVertex3f(-TEX_SIZE, -TEX_SIZE, -TEX_SIZE);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

Skybox::uint Skybox::_loadTexture(pcStr filename) {
	AUX_RGBImageRec* img = auxDIBImageLoadA(filename);
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	free(img->data);
	free(img);
	return texId;
}