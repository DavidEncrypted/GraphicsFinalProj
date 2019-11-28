#pragma once
#include "TextureManager.h"

#include "Particles.h"

#ifndef MaxParticles
#define MaxParticles 20000
#endif

struct Skybox
{
	GLuint id, vao, vbo;
};

struct Billboard
{
	GLuint textureid, vao, vbo;
};

struct Material
{
	Vector4 diffuse, specular;
	float specularlevel, glossiness;
	GLuint texdiffuse, texspecular, texbump;
};


struct Mesh
{
	GLuint vbo_vertex, vbo_tbn, vbo_texcoord, vbo_faces;
	GLsizei numfaces;
	Material * material;
};


struct Light
{
	Vector4 position, target;
	Vector4 intensity;
	Vector4 params;
};


class RenderData
{
public:
	RenderData();

	bool loadSkybox(const std::vector<std::string> & facefilenames);

	bool loadBillboard(const std::string & texturename);

	bool loadModel(const std::string & modelfile);

	bool addLight(const std::string & description);

	bool setCamera(const std::string & description);

	Skybox & getSkybox() {return sb;}

	Billboard & getBillboard() {return bb;}

	Particles & getParticles() {return particles;}

	size_t getNumMeshes() const;
	Mesh & getMesh(size_t i);

	size_t getNumLights() const;
	Light & getLight(size_t i);

	const Vector4 & getCameraPosition();
	const Vector4 & getCameraRotation();

	void updateCamera(bool forward, bool backward, bool left, bool right, int turnright, int turndown, int deltatime);
	//void updateParticles(float delta, Vector4 CameraPosition){particles.Update(delta,CameraPosition);}


private:
	TextureManager texturemanager;
	
	Particles particles;
	Skybox sb;
	Billboard bb;

	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<Light> lights;

	Vector4 cameraposition;
	Vector4 camerarotation;

	


	unsigned char readByte(std::ifstream & file);
	int readInt(std::ifstream & file);
	float readFloat(std::ifstream & file);
	std::string readString(std::ifstream & file);

	Vector4 readColor(std::ifstream & file);
	Vector4 readPoint2(std::ifstream & file);
	Vector4 readPoint3(std::ifstream & file);
	
	void readPoint2(std::ifstream & file, float * point2);
	void readPoint3(std::ifstream & file, float * point3);
	void readIndex3(std::ifstream & file, unsigned int * index3);

	
};
