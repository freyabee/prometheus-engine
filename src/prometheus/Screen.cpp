#include "Screen.h"
namespace prometheus
{
	Screen::Screen(int w, int h)
	{
		windowWidth = w;
		windowHeight = h;


		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw std::exception();
		}

		window = SDL_CreateWindow("Test Window",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);


		SDL_GLContext glContext = SDL_GL_CreateContext(window);

		if (!SDL_GL_CreateContext(window))
		{
			throw std::exception();
		}
	}

	Screen::~Screen()
	{
	}
	void Screen::DestroyScreen()
	{
		//std::cout << "Destroy Entered" << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void Screen::SwapWindow()
	{
		
		SDL_GL_SwapWindow(window);
		//std::cout << "Swapped" << std::endl;
	}

	void Screen::ClearWindow()
	{

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	}

	glm::ivec2 Screen::GetWindowDimensions()
	{
		return glm::ivec2(windowWidth, windowHeight);
	}

}
