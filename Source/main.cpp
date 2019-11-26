#include "global.h"

#include "InputHandler.h"
#include "RenderData.h"
#include "Renderer.h"


int main(int argc, char * argv[])
{
	bool done = false;
	InputHandler inputhandler;
	RenderData renderdata;
	Renderer renderer(renderdata);
	std::string setupfile;
	unsigned int time, newtime;
	int deltatime;

	if (argc < 2)
	{
		std::cerr << "This program requires a setup file as a parameter!" << std::endl;
		return 0;
	}
	else
	{
		setupfile = std::string(argv[1]);
	}

	if (!renderer.initRenderer(800, 600, setupfile))
		return false;
	time = SDL_GetTicks();

	std::cout << "Controls:" << std::endl;
	std::cout << " Left click and drag the mouse to rotate the view" << std::endl;
	std::cout << std::endl;
	std::cout << " Arrow-Up    or W - move camera forward" << std::endl;
	std::cout << " Arrow-Down  or A - move camera left" << std::endl;
	std::cout << " Arrow-Left  or S - move camera backward" << std::endl;
	std::cout << " Arrow-Right or D - move camera right" << std::endl;
	std::cout << std::endl;
	std::cout << " Q - Toggle Anti-alisasing" << std::endl;
	std::cout << std::endl;
	std::cout << " Escape - Exit" << std::endl;

	while (!done)
	{
		newtime = SDL_GetTicks();
		deltatime = newtime - time;
		time = newtime;

		if (inputhandler.keyIsPressed(SDLK_ESCAPE))
		{
			done = true;
			break;
		}
		if (inputhandler.keyIsPressed(SDLK_q))
		{
			if (!renderer.toggleAA())
				break;
		}
		int camturnright, camturndown;
		inputhandler.mouseDragGetRelMove(camturnright, camturndown);
		renderdata.updateCamera(inputhandler.keyIsDown(SDLK_w) || inputhandler.keyIsDown(SDLK_UP),
		                        inputhandler.keyIsDown(SDLK_s) || inputhandler.keyIsDown(SDLK_DOWN),
		                        inputhandler.keyIsDown(SDLK_a) || inputhandler.keyIsDown(SDLK_LEFT),
		                        inputhandler.keyIsDown(SDLK_d) || inputhandler.keyIsDown(SDLK_RIGHT),
		                        camturnright, camturndown, deltatime);

		renderer.render();
		SDL_GL_SwapBuffers();

		inputhandler.keyResetPress();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				inputhandler.keyDown(event.key.keysym.sym);
			}
			else if (event.type == SDL_KEYUP)
			{
				inputhandler.keyUp(event.key.keysym.sym);
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				inputhandler.mouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
					inputhandler.mouseButtonDown();
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
					inputhandler.mouseButtonUp();
			}
			else if (event.type == SDL_VIDEORESIZE)
			{
				if (!renderer.resize(event.resize.w, event.resize.h))
					break;
			}
		}
	}

	return 0;
}
