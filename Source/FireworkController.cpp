#include "global.h"
#include "Particles.h"
#include "FireworkController.h"

FireworkController::FireworkController(): CurrentTime(0.0f), CurrentSecond(0.0f), origin(Vector4(0.0f,0.0f,-40.0f)), direction(Vector4(0.0f,0.0f,1.0f)){
}

FireworkController::~FireworkController(){

}


void FireworkController::UpdateTime(float delta){
	//std::cerr << "delta: " << delta << std::endl;
	//std::cerr << "CurrentTime: " << CurrentTime << std::endl;
	if (delta <= 0.0f || delta > 5.0f) return;
	CurrentTime += delta;
	//std::cerr << CurrentTime << std::endl;
	if (floor(CurrentTime) > CurrentSecond){
		CurrentSecond = floor(CurrentTime);
		std::cerr << "CurrentSecond: " << CurrentSecond << std::endl;
	}

	while (!FireworkQueue.empty() && FireworkQueue.front().TriggerTime < CurrentTime){
		
		FireworkTrailVector.push_back(FireworkQueue.front().ftrail);
		
		FireworkQueue.pop();
		std::cerr << FireworkQueue.size() << std::endl;
	} 
	UpdateTrails(delta);
}

void FireworkController::UpdateTrails(float delta){
	std::vector<FireworkTrail>::iterator it = FireworkTrailVector.begin();
	for (; it != FireworkTrailVector.end(); ){
		particles->Trail(it->loc, it->dir * -1, 30, 255,255,200);
		it->loc += it->dir * it->speed;
		it->life -= delta;
		if (it->life <= 0.0f) {
			
			if (it->fp.type != 0) {
				std::cerr << "Do boom..." << std::endl;
				particles->Explosion(it->loc, it->fp.numparticles, it->fp.type, it->fp.r, it->fp.g, it->fp.b);
			}
			FireworkTrailVector.erase(it);
		}
		else it++;
	}
}

void FireworkController::AddFireworkTrigger(float time, int type, int r, int g, int b){
	FireworkParam fp = {type, r, g, b};
	//Vector4 dircopy = direction;
	//std::cerr << "dir: x:" << direction.x() << " y:" << direction.y() << " z:" << direction.z() << std::endl;
	FireworkTrail ftrail = {origin, direction, 0.4f,3.0f,fp};
	std::cerr << "adding time: " << time << std::endl;
	FireworkTrigger ft = {time, ftrail};
	FireworkQueue.push(ft);
}

void FireworkController::AddFireworkTrigger(float time, float traillength, int type, int r, int g, int b){
	FireworkParam fp = {type, r, g, b};
	//Vector4 dircopy = direction;
	//std::cerr << "dir: x:" << direction.x() << " y:" << direction.y() << " z:" << direction.z() << std::endl;
	FireworkTrail ftrail = {origin, direction, traillength,3.0f,fp};
	std::cerr << "adding time: " << time << std::endl;
	FireworkTrigger ft = {time, ftrail};
	FireworkQueue.push(ft);
}

void FireworkController::AddFireworkTrigger(float time, float traillength, float trailspeed, float numparticles, int type, int r, int g, int b){
	FireworkParam fp = {type, r, g, b, numparticles};
	//Vector4 dircopy = direction;
	//std::cerr << "dir: x:" << direction.x() << " y:" << direction.y() << " z:" << direction.z() << std::endl;
	FireworkTrail ftrail = {origin, direction, traillength,trailspeed,fp};
	std::cerr << "adding time: " << time << std::endl;
	FireworkTrigger ft = {time, ftrail};
	FireworkQueue.push(ft);
}

void FireworkController::AddFireworkTrail(Vector4 loc, Vector4 dir, float time, float speed){
	FireworkParam fp = {0,0,0,0};
	FireworkTrail ftrail = {loc, dir, time, speed, fp};
	FireworkTrailVector.push_back(ftrail);
}

void FireworkController::AddFireworkTrail(Vector4 loc, Vector4 dir, float time, float speed, int type, int r, int g, int b){
	FireworkParam fp = {type, r, g, b};
	FireworkTrail ftrail = {loc, dir, time, speed, fp};
	FireworkTrailVector.push_back(ftrail);
}
