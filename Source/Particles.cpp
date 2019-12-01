#include "global.h"
#include "Particles.h"
#include <algorithm>



Particles::Particles()
{
	
}

Particles::~Particles(){
	//delete[] ParticlesContainer;
}


void Particles::loadParticles(){
	// The VBO containing the positions and sizes of the particles

	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	ParticlesContainer = new Particle[MaxParticles];

	g_particle_position_size_data = new GLfloat[MaxParticles * 4];
	g_particle_color_data = new GLubyte[MaxParticles * 4];

	for(int i=0; i<MaxParticles; i++){
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
		ParticlesContainer[i].hastrail = 0;
		ParticlesContainer[i].type = 0;
			
	}
}

void Particles::Trail(Vector4 loc, Vector4 dir, int NumParticles, int _r, int _g, int _b){
	unsigned char r = (unsigned char)_r;
	unsigned char g = (unsigned char)_g;
	unsigned char b = (unsigned char)_b;

	for(int i = 0; i < NumParticles; i++){
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life =(rand()%1000)/1000.0f + 0.5f; // This particle will live 5 seconds.
		ParticlesContainer[particleIndex].pos = loc;
		ParticlesContainer[particleIndex].hastrail = 0;
		ParticlesContainer[particleIndex].type = 0;
		//std::cerr << i << " : " << particleIndex << std::endl;
		float spread = 20.0f;
		dir = dir.rotateX((rand()%2000 - 1000.0f)/10000.0f);
		dir = dir.rotateY((rand()%2000 - 1000.0f)/10000.0f);
		
		
		ParticlesContainer[particleIndex].speed = dir*spread * (((rand() % 2000) / 2000.f) + 0.8f);


		ParticlesContainer[particleIndex].r = r;
		ParticlesContainer[particleIndex].g = g;
		ParticlesContainer[particleIndex].b = b;
		ParticlesContainer[particleIndex].a = 0;

		ParticlesContainer[particleIndex].size = (rand()%1000)/7000.0f + 0.3f;
		
	}
}

void Particles::SmokeCloud(Vector4 location, int NumParticles, float cloudsize){
	for(int i = 0; i < NumParticles; i++){
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life =(rand()%1000)/1000.0f + 40.0f; // This particle will live 5 seconds.
		Vector4 randomloc;

		do{
			randomloc = Vector4(
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f,
				(rand()%2000 - 1000.0f)/1000.0f
			);
		} while(randomloc.length() > 1.0f);
		//randomloc = randomloc;

		ParticlesContainer[particleIndex].pos = location + randomloc * 0.5f;
		ParticlesContainer[particleIndex].hastrail = 0;
		ParticlesContainer[particleIndex].type = 0;

		

		ParticlesContainer[particleIndex].speed = randomloc * cloudsize;

		// Very bad way to generate a random color
		ParticlesContainer[particleIndex].r = 0;
		ParticlesContainer[particleIndex].g = 0;
		ParticlesContainer[particleIndex].b = 0;
		ParticlesContainer[particleIndex].a = 1;

		ParticlesContainer[particleIndex].size = 10.0f;
	}
}

void Particles::Explosion(Vector4 location, int NumParticles, int type, int _r, int _g, int _b){
	std::cerr << "Boom" << std::endl;

	unsigned char r = (unsigned char)_r;
	unsigned char g = (unsigned char)_g;
	unsigned char b = (unsigned char)_b;

	for(int i = 0; i < NumParticles; i++){
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life =(rand()%1000)/1000.0f + 4.0f; // This particle will live 5 seconds.
		ParticlesContainer[particleIndex].pos = location;
		ParticlesContainer[particleIndex].hastrail = 0;
		ParticlesContainer[particleIndex].type = 0;
		
		//std::cerr << i << " : " << particleIndex << std::endl;
		float spread = 20.0f;
		//Vector4 maindir = Vector4(0.0f, 0.0f, 30.0f);
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		float radius = 1.0f;

		Vector4 randomdir;
		
		if (type == 1 || type == 3 || type == 4){
			do{
				randomdir = Vector4(
					(rand()%2000 - 1000.0f)/1000.0f,
					(rand()%2000 - 1000.0f)/1000.0f,
					(rand()%2000 - 1000.0f)/1000.0f
				);
			} while(randomdir.length() > radius);
		}
		else if (type == 2){
			do{
				randomdir = Vector4(
					(rand()%2000 - 1000.0f)/1000.0f,
					(rand()%2000 - 1000.0f)/1000.0f,
					0.0f
				);
			} while(randomdir.length() > radius);
			spread = 40.0f;
		}
		if (type == 3){
			ParticlesContainer[particleIndex].hastrail = 5;
			ParticlesContainer[particleIndex].life = (rand() % 1000) / 2000.0f + 0.5f;
			//ParticlesContainer[particleIndex].type = 1;
			spread = 50.0f;		
		}
		else if (type == 4){
			ParticlesContainer[particleIndex].hastrail = 3;
			ParticlesContainer[particleIndex].type = 1;
		}

		
		ParticlesContainer[particleIndex].speed = randomdir*spread;


		// Very bad way to generate a random color
		ParticlesContainer[particleIndex].r = r;
		ParticlesContainer[particleIndex].g = g;
		ParticlesContainer[particleIndex].b = b;
		ParticlesContainer[particleIndex].a = 0;

		ParticlesContainer[particleIndex].size = (rand()%1000)/7000.0f + 0.3f;
		
	}
	SmokeCloud(location, 100, 25.0f);
}

void Particles::Canon(int delta){
	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	//std::cerr << delta << std::endl;
	int newparticles = (int)(delta*1000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);
	//std::cerr << "newp: " << newparticles << std::endl;
	for(int i = 0; i < newparticles; i++){
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life = (rand()%4000)/1000.0f + 1.0f; // This particle will live 5 seconds.
		ParticlesContainer[particleIndex].pos = Vector4(0,0,0.0f,20.0f);
		ParticlesContainer[particleIndex].hastrail = 0;
		ParticlesContainer[particleIndex].type = 0;
		
		//std::cerr << i << " : " << particleIndex << std::endl;
		float spread = 5.0f;
		Vector4 maindir = Vector4(0.0f, 0.0f, 30.0f);
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		Vector4 randomdir = Vector4(
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f
		);
		
		ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;


		// Very bad way to generate a random color
		ParticlesContainer[particleIndex].r = rand() % 256;
		ParticlesContainer[particleIndex].g = rand() % 256;
		ParticlesContainer[particleIndex].b = rand() % 256;
		ParticlesContainer[particleIndex].a = 0;

		ParticlesContainer[particleIndex].size = (rand()%1000)/500.0f + 3.0f;
		
	}

}
void Particles::CreateTrailParticle(Particle& p){
	int particleIndex = FindUnusedParticle();
	ParticlesContainer[particleIndex].life = p.life;
	ParticlesContainer[particleIndex].pos = p.pos;
	ParticlesContainer[particleIndex].hastrail = p.hastrail - 1;

	ParticlesContainer[particleIndex].speed = p.speed * 0.95f;

	ParticlesContainer[particleIndex].r = p.r;
	ParticlesContainer[particleIndex].g = p.g;
	ParticlesContainer[particleIndex].b = p.b;
	ParticlesContainer[particleIndex].a = p.a;

	ParticlesContainer[particleIndex].type = p.type;
	ParticlesContainer[particleIndex].size = p.size;
	p.hastrail = 0;
}

int Particles::Update(float delta, Vector4 CameraPosition){
	
	for(int i = 0; i < MaxParticles; i++){
		Particle& p = ParticlesContainer[i]; // shortcut
		if(p.life > 0.0f && p.a == 0){
			if (p.hastrail > 0) {CreateTrailParticle(p);}
			if (p.type != 0 && p.life <= 1.0f){
				p.r += 10;
				if (p.r > 255) p.r = 255;
				p.g += 10;
				if (p.g > 255) p.g = 255;
				p.b += 10;
				if (p.b > 255) p.b = 255;
				if (p.r == 255 && p.g == 255 && p.b == 255)
					p.type = 0;
			}
		}
	}


	// Simulate all particles
	int ParticlesCount = 0;
	for(int i = 0; i < MaxParticles; i++){

		Particle& p = ParticlesContainer[i]; // shortcut

		if(p.life > 0.0f){

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f){
				

				if (p.a == 0){
					// Simulate simple physics : gravity only, no collisions
					p.speed += Vector4(0.0f,0.0f, -9.81f) * (float)delta * 0.5f;
				}
				else {
					p.speed *= 0.95f;
					p.pos += Vector4(0.0f, -3.0f, 0.0f) * (float)delta;
				}
				p.pos += p.speed * (float)delta;
				p.cameradistance = (p.pos - CameraPosition ).length();
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

				// Fill the GPU buffer
				g_particle_position_size_data[4*ParticlesCount+0] = p.pos.x();
				g_particle_position_size_data[4*ParticlesCount+1] = p.pos.y();
				g_particle_position_size_data[4*ParticlesCount+2] = p.pos.z();
											   
				g_particle_position_size_data[4*ParticlesCount+3] = p.size;
											   
				g_particle_color_data[4*ParticlesCount+0] = p.r;
				g_particle_color_data[4*ParticlesCount+1] = p.g;
				g_particle_color_data[4*ParticlesCount+2] = p.b;
				g_particle_color_data[4*ParticlesCount+3] = p.a;

			}
			else{
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;

		}
	}

	SortParticles();

	return ParticlesCount;
}






int Particles::FindUnusedParticle(){

	for(int i=LastUsedParticle; i<MaxParticles; i++){
		if (ParticlesContainer[i].life <= 0){
			LastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<LastUsedParticle; i++){
		if (ParticlesContainer[i].life <= 0){
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void Particles::SortParticles(){
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}


// void Particles::printparticles(){
// 	for (int i = 0; i < MaxParticles; i++){
// 		ParticlesContainer[i].life = i;
// 		std::cerr << ParticlesContainer[i].life << std::endl;
// 	}
// }