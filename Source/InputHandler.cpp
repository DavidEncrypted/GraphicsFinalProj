#include "global.h"
#include "InputHandler.h"



InputHandler::InputHandler() : mousedrag(false), mousedrag_xrel(0), mousedrag_yrel(0)
{
	int i;
	for (i = 0; i < SDLK_LAST; i++)
	{
		keydown[i] = false;
		keypress[i] = false;
	}
}

void InputHandler::keyDown(int key)
{
	keydown[key] = true;
	keypress[key] = true;
}
void InputHandler::keyUp(int key)
{
	keydown[key] = false;
}

void InputHandler::mouseButtonDown()
{
	mousedrag = true;
}
void InputHandler::mouseButtonUp()
{
	mousedrag = false;
}
void InputHandler::mouseMove(int mousex, int mousey, int mousexrel, int mouseyrel)
{
	this->mousex = mousex;
	this->mousey = mousey;

	if (mousedrag)
	{
		mousedrag_xrel += mousexrel;
		mousedrag_yrel += mouseyrel;
	}
}


bool InputHandler::keyIsPressed(int key)
{
	if (keypress[key])
	{
		keypress[key] = false;
		return true;
	}
	return false;
}
bool InputHandler::keyIsDown(int key)
{
	return keydown[key];
}

void InputHandler::mouseDragGetRelMove(int & x, int & y)
{
	x = mousedrag_xrel;
	y = mousedrag_yrel;
	mousedrag_xrel = mousedrag_yrel = 0;
}


void InputHandler::keyResetPress()
{
	int i;
	for (i = 0; i < SDLK_LAST; i++)
	{
		keypress[i] = false;
	}
}
