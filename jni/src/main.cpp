#include <SDL.h>
#include <SDL_image.h>

#include <boost/tr1/memory.hpp>
#include "scene_interface.hpp"
#include "inputstate.hpp"

#include "printlog.hpp"
#include "android_boost_fix.hpp"

#include "testscene.hpp"

SDL_Window* window = NULL;

void run(std::tr1::shared_ptr<SceneInterface> scene)
{
	InputState inputState;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	Uint32 time = SDL_GetTicks();
	scene->Init(window, renderer);
	while (scene->Running())
	{
		SDL_Event e;
		if ( SDL_PollEvent(&e) )
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_UP)
				{
					inputState.SetUpButtonState(true);
				}
				else if (e.key.keysym.sym == SDLK_DOWN)
				{
					inputState.SetDownButtonState(true);
				}
				else if (e.key.keysym.sym == SDLK_LEFT)
				{
					inputState.SetLeftButtonState(true);
				}
				else if (e.key.keysym.sym == SDLK_RIGHT)
				{
					inputState.SetRightButtonState(true);
				}
			}
			else if (e.type == SDL_KEYUP)
			{
				if (e.key.keysym.sym == SDLK_UP)
				{
					inputState.SetUpButtonState(false);
				}
				else if (e.key.keysym.sym == SDLK_DOWN)
				{
					inputState.SetDownButtonState(false);
				}
				else if (e.key.keysym.sym == SDLK_LEFT)
				{
					inputState.SetLeftButtonState(false);
				}
				else if (e.key.keysym.sym == SDLK_RIGHT)
				{
					inputState.SetRightButtonState(false);
				}
			}
		}
		
		scene->Update(inputState);
		Uint32 now = SDL_GetTicks();
		if (now - time > 15)
		{
			SDL_RenderClear(renderer);
			scene->Render(renderer);
			SDL_RenderPresent(renderer);
			time = SDL_GetTicks();
		}
	}
}

void game()
{
	std::tr1::shared_ptr<TestScene> ts(new TestScene());
	
	run(ts);
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);   // Initialize SDL2
	TTF_Init();

	SDL_DisplayMode mode;
	int WIDTH = 1024, HEIGHT = 600;
	//if (SDL_GetCurrentDisplayMode(0, &mode)==0)
	//{
	//	/* I read that android ignores these so you can just as well set
	//	them to 0 */
	//	WIDTH=mode.w;
	//	HEIGHT=mode.h;
	//}
	printlog("Window size: %d x %d!\n", WIDTH, HEIGHT);

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"Aldebaran",                 
		SDL_WINDOWPOS_UNDEFINED,           
		SDL_WINDOWPOS_UNDEFINED,           
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL 
	);		

	// Check that the window was successfully made
	if(window == NULL)
	{   
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	game();

	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit(); 
	
	printlog("Success!\n");
	
	return 0;
}
