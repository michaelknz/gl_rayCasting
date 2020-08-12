#pragma once
#include <SDL.h>
#include <glew.h>
#include <string>


class Display
{
public:
	Display(int width, int height, const std::string& title);
	~Display();
	void Clear(float r, float g, float b, float a);
	void Swap();
private:
	SDL_Window* window;
	SDL_GLContext context;
};

