#pragma once
#include "RenderContext.h"
#include "ShaderProgram.h"
#include "RenderData.h"
#include "Skybox.h"
#ifndef MaxParticles
#define MaxParticles 20000
#endif
class Renderer
{
public:
	Renderer(RenderData & renderdata);
	bool initRenderer(int rendersizex, int rendersizey, const std::string & setupfile);
	~Renderer();

	bool resize(int rendersizex, int rendersizey);
	
	void render();

	bool toggleAA();

private:
	RenderContext rendercontext;
	RenderData & renderdata;
	int rendersizex, rendersizey;

	// Frame Buffer Object Data
	GLuint fboms, rbcolor, rbdepth;
	GLuint fbo, rbcolortexture;

	// AA settings
	bool useAA, useCSAA;
	int AAcolorsamples, AAcoveragesamples;

	// Screenquad vbo
	GLuint vboquad;
	
	// Shaders
	ShaderProgram usershader;
	ShaderProgram tonemapshader;
	ShaderProgram skyboxshader;
	ShaderProgram billboardshader;

	// Matrices
	Matrix4 matprojection;
	Matrix4 matmodelview;
	Matrix4 rotmodelview;

	// Time value
	float time;
	float prevtime;
	float delta;

	void drawSkybox();
	void drawBillboard(int ParticlesCount);
	void drawScreenQuad();
	void drawUserModel();
	void drawUserModelDepth();

	bool createMainFBO();
	bool resizeMainFBO();
	void destroyMainFBO();
};
