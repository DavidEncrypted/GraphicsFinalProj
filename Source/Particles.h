#pragma once

#ifndef MaxParticles
#define MaxParticles 60000
#endif

struct Particle{
	Vector4 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
	int type;
	int hastrail;
	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};



class Particles
{
public:
	Particles();
	~Particles();
	void loadParticles();
	int FindUnusedParticle();
	void SortParticles();

	void SmokeCloud(Vector4 location, int NumParticles, float cloudsize);
	void Trail(Vector4 loc, Vector4 dir, int NumParticles, int _r, int _g, int _b);
	void Explosion(Vector4 location, int NumParticles, int type, int _r, int _g, int _b);
	void Canon(int delta);
	int Update(float delta, Vector4 CameraPosition);
	void CreateTrailParticle(Particle& p);

	GLuint getPositionVBO(){return particles_position_buffer;}
	GLuint getColorVBO(){return particles_color_buffer;}

	GLfloat* getParticlePositionPtr() {return g_particle_position_size_data;}
	GLubyte* getParticleColorPtr() {return g_particle_color_data;}


private:
	// Particles
	// VBOs
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	//GPU buffers
	GLfloat* g_particle_position_size_data;
	GLubyte* g_particle_color_data;

	Particle* ParticlesContainer;
	int LastUsedParticle = 0;
	
};
