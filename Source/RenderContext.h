#pragma once



class RenderContext
{
public:
	RenderContext();
	bool create(int width, int height);
	bool recreate(int width, int height);
	void destroy();

private:
	SDL_Surface* screen;
};
