#pragma once


class InputHandler
{
public:
	InputHandler();


	void keyDown(int key);
	void keyUp(int key);

	void mouseButtonDown();
	void mouseButtonUp();
	void mouseMove(int mousex, int mousey, int mousexrel, int mouseyrel);
	

	bool keyIsPressed(int key);
	bool keyIsDown(int key);

	void mouseDragGetRelMove(int & x, int & y);


	void keyResetPress();

private:
	bool keydown[SDLK_LAST];
	bool keypress[SDLK_LAST];

	int mousex, mousey;
	
	bool mousedrag;
	int mousedrag_xrel, mousedrag_yrel;
};
