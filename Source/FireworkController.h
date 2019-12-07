#pragma once

#ifndef MaxParticles
#define MaxParticles 60000
#endif

struct FireworkParam {
	int type;
	int r;
	int g;
	int b;
	float numparticles = 1000;
};

struct FireworkTrail {
	Vector4 loc;
	Vector4 dir;
	float life;
	float speed;
	FireworkParam fp;
};

struct FireworkTrigger {
	float TriggerTime;
	FireworkTrail ftrail;
};



class FireworkController
{
public:
	FireworkController();
	~FireworkController();

	void AttachParticles(Particles* _particles) {particles = _particles; }
	void UpdateTime(float delta);
	void UpdateTrails(float delta);
	void AddFireworkTrigger(float time, int type, int r, int g, int b);
	void AddFireworkTrigger(float time, float traillength, int type, int r, int g, int b);
	void AddFireworkTrigger(float time, float traillength, float trailspeed, float numparticles, int type, int r, int g, int b);
	void AddFireworkTrail(Vector4 loc, Vector4 dir, float time, float speed);
	void AddFireworkTrail(Vector4 loc, Vector4 dir, float time, float speed, int type, int r, int g, int b);
	void SetOrigin(Vector4 _origin) {origin = _origin;} 
	void SetDirection(Vector4 _direction) {direction = _direction;}

private:
	Particles* particles;
	std::queue<FireworkTrigger> FireworkQueue;
	std::vector<FireworkTrail> FireworkTrailVector;
	float CurrentTime;
	float CurrentSecond;
	Vector4 origin;
	Vector4 direction;

};
