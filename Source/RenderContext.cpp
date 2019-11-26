#include "global.h"
#include "RenderContext.h"



RenderContext::RenderContext() : screen(0)
{}

bool RenderContext::create(int width, int height)
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	SDL_WM_SetCaption("ShaderLab", 0);

	screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_DOUBLEBUF | SDL_RESIZABLE);
	if (!screen)
	{
		std::cerr << "Failed to set the videomode for SDL" << std::endl;
		return false;
	}
	
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW Failed to initialize" << std::endl;
		return false;
	}

	if (!GLEW_VERSION_2_0)
	{
		std::cerr << "ShaderLab requires OpenGL 2.0 support!" << std::endl;
		return false;
	}

	return true;
}

bool RenderContext::recreate(int width, int height)
{
	SDL_FreeSurface(screen);
	screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_DOUBLEBUF | SDL_RESIZABLE);
	if (!screen)
	{
		std::cerr << "Failed to set the videomode for SDL" << std::endl;
		return false;
	}

	return true;
}

void RenderContext::destroy()
{
	if (screen)
		SDL_FreeSurface(screen);
}
