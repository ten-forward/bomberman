#include <SDL.h>
#include <SDL_image.h>
#include <android/log.h>

#include <boost/tr1/memory.hpp>
#include "scene_interface.hpp"
#include "inputstate.hpp"

#include "android_boost_fix.hpp"

#include "testscene.hpp"

void run(std::tr1::shared_ptr<SceneInterface> scene)
{
	InputState inputState;
	Uint32 time = SDL_GetTicks();
	while (scene->Running())
	{
		SDL_Event e;
		if ( SDL_PollEvent(&e) )
		{
			scene->Update();
		}
		
		Uint32 now = SDL_GetTicks();
		if (now - time > 15)
		{
			scene->Render();
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

	SDL_DisplayMode mode;
	int WIDTH,HEIGHT;
	if (SDL_GetCurrentDisplayMode(0, &mode)==0)
	{
		/* I read that android ignores these so you can just as well set
		them to 0 */
		WIDTH=mode.w;
		HEIGHT=mode.h;
	}
	__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Window size: %d x %d!\n", WIDTH, HEIGHT);

	// Create an application window with the following settings:
	std::tr1::shared_ptr<SDL_Window> window
	(
		SDL_CreateWindow(
		"Aldebaran",                 
		SDL_WINDOWPOS_UNDEFINED,           
		SDL_WINDOWPOS_UNDEFINED,           
		WIDTH,
		HEIGHT,
		SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL 
		),
		SDL_DestroyWindow
	);

	// Check that the window was successfully made
	if(window == NULL)
	{   
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}


	// Clean up
	SDL_Quit(); 
	
	__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Success!\n");
	
	return 0;
}
