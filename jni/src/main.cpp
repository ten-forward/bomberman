#include <SDL.h>
#include <SDL_image.h>

#include <memory>
#include "scene_interface.hpp"
#include "inputstate.hpp"
#include "constants.hpp"
#include "printlog.hpp"
#include "testscene.hpp"

void PollEvents(std::vector<InputState> &oInputState);

#ifdef ANDROID
#include <jni.h>

extern "C" void Java_net_astrobunny_aldebaran_BombermanSurface_onOuyaControllerKey(
                                                               JNIEnv* env, jclass jcls,
                                                               jint player, jint keyCode, jint action)
{
    printlog("onOuyaControllerKey player=%d, keyCode=%d, action=%d", player, keyCode, action);
    SDL_Event event;
    memset(&event, sizeof(event), 0);
    event.jdevice.which = player;
    event.key.keysym.sym = keyCode;
 	event.key.type = action ? SDL_KEYUP : SDL_KEYDOWN;
   	SDL_PushEvent(&event);
}
#endif

SDL_Window* window = NULL;

void run(std::shared_ptr<SceneInterface> scene)
{
	std::vector<InputState> inputState(4);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	Uint32 time = SDL_GetTicks();
	scene->Init(window, renderer);
	
	while (scene->Running())
	{
	 	PollEvents(inputState);
		
		Uint32 now = SDL_GetTicks();
		
		scene->Update(inputState, now);
		if (now - time > 12)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			scene->Render(renderer);
			SDL_RenderPresent(renderer);
			time = SDL_GetTicks();
		}
	}
}

void PollEvents(std::vector<InputState> &oInputState)
{
	SDL_Event e;
	if ( SDL_PollEvent(&e) )
	{
#ifdef ANDROID
		auto &inputState = oInputState[e.jdevice.which];
#else
		auto &inputState = oInputState[0];
#endif
		
		if (e.type == SDL_QUIT)
		{
			exit(0);
		}
		else if (e.type == SDL_FINGERUP)
		{	
			inputState.SetFingered(false);
		}
		else if (e.type == SDL_FINGERDOWN)
		{
			inputState.SetFingered(true);
			inputState.SetFingerX(e.tfinger.x);
			inputState.SetFingerY(e.tfinger.y);
		}
		else if (e.type == SDL_KEYDOWN)
		{
#ifdef ANDROID
			if (e.key.keysym.sym == ouya::UP)
			{
				inputState.SetUpButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::DOWN)
			{
				inputState.SetDownButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::LEFT)
			{
				inputState.SetLeftButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::RIGHT)
			{
				inputState.SetRightButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::U)
			{
				inputState.SetAButtonState(true);
			}
#else
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
			else if (e.key.keysym.sym == SDLK_a)
			{
				inputState.SetAButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_s)
			{
				inputState.SetBButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_z)
			{
				inputState.SetXButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_x)
			{
				inputState.SetYButtonState(true);
			}
#endif
		}
		else if (e.type == SDL_KEYUP)
		{
#ifdef ANDROID
			if (e.key.keysym.sym == ouya::UP)
			{
				inputState.SetUpButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::DOWN)
			{
				inputState.SetDownButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::LEFT)
			{
				inputState.SetLeftButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::RIGHT)
			{
				inputState.SetRightButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::U)
			{
				inputState.SetAButtonState(false);
			}
#else
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
			else if (e.key.keysym.sym == SDLK_a)
			{
				inputState.SetAButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_s)
			{
				inputState.SetBButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_z)
			{
				inputState.SetXButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_x)
			{
				inputState.SetYButtonState(false);
			}
#endif
		}
	}
}

void game()
{
	std::array<bomberman::PlayerConfig, 4> players;

	std::shared_ptr<bomberman::TestScene> ts(new bomberman::TestScene(players));
	
	run(ts);
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);   // Initialize SDL2
	//TTF_Init();

	using namespace bomberman::constants;

	SDL_DisplayMode mode;
	int WIDTH = SCREEN_WIDTH, HEIGHT = SCREEN_HEIGHT;

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
		printlog("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	
	printlog("Beginning game with window=%p\n", window);
	try
	{
		game();
	}
	catch(std::exception e)
	{
		printlog("Caught a std::exception! %s\n", e.what());
	}
	catch(...)
	{
		printlog("Caught an unknown exception!\n");
	}

	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit(); 
	
	printlog("Success!\n");
	
	return 0;
}
