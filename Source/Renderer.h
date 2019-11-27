#pragma once
#include "RenderContext.h"
#include "ShaderProgram.h"
#include "RenderData.h"
#include "Skybox.h"


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

	// Matrices
	Matrix4 matprojection;
	Matrix4 matmodelview;
	Matrix4 rotmodelview;

	// Time value
	float time;

	void drawSkybox();
	void drawScreenQuad();
	void drawUserModel();
	void drawUserModelDepth();

	bool createMainFBO();
	bool resizeMainFBO();
	void destroyMainFBO();
};
