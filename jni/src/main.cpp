#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <memory>
#include <boost/foreach.hpp>

#include "inputstate.hpp"
#include "constants.hpp"
#include "printlog.hpp"
#include "gamescene.hpp"
#include "menuscene.hpp"
#include "setupscene.hpp"
#include "fadescene.hpp"
#include "victoryscene.hpp"

void PollEvents(std::vector<InputState> &oInputState);

static bool backThroughTime = false;

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

void run(std::shared_ptr<bomberman::SceneInterface> scene)
{
	std::vector<InputState> inputState(4);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	Uint32 time = SDL_GetTicks();
	scene->Init(window, renderer);
	
	while (scene->Running())
	{
	 	PollEvents(inputState);

		Uint32 now = SDL_GetTicks();
		
		if (backThroughTime) {
	 		std::dynamic_pointer_cast<bomberman::GameScene>(scene)->BackThroughTime(renderer, now);
	 	}

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
	SDL_DestroyRenderer(renderer);
}

void PollEvents(std::vector<InputState> &oInputState)
{
	SDL_Event e;

	backThroughTime = false;

	if ( SDL_PollEvent(&e) )
	{
#ifdef ANDROID
		auto &inputState = oInputState[e.jdevice.which];
#else
		auto &inputState = oInputState[0];
#endif
		
		inputState.SetAButtonJustPressed(false);
		inputState.SetBButtonJustPressed(false);
		inputState.SetXButtonJustPressed(false);
		inputState.SetYButtonJustPressed(false);
		inputState.SetStartButtonJustPressed(false);

		inputState.SetAButtonJustReleased(false);
		inputState.SetBButtonJustReleased(false);
		inputState.SetXButtonJustReleased(false);
		inputState.SetYButtonJustReleased(false);
		inputState.SetStartButtonJustReleased(false);

		inputState.SetUpButtonJustPressed(false);
		inputState.SetDownButtonJustPressed(false);
		inputState.SetLeftButtonJustPressed(false);
		inputState.SetRightButtonJustPressed(false);

		inputState.SetUpButtonJustReleased(false);
		inputState.SetDownButtonJustReleased(false);
		inputState.SetLeftButtonJustReleased(false);
		inputState.SetRightButtonJustReleased(false);

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
				inputState.SetUpButtonJustPressed(true);
				inputState.SetUpButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::DOWN)
			{
				inputState.SetDownButtonJustPressed(true);
				inputState.SetDownButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::LEFT)
			{
				inputState.SetLeftButtonJustPressed(true);
				inputState.SetLeftButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::RIGHT)
			{
				inputState.SetRightButtonJustPressed(true);
				inputState.SetRightButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::U)
			{
				inputState.SetAButtonJustPressed(true);
				inputState.SetAButtonState(true);
			}
			else if (e.key.keysym.sym == ouya::OUYABUTTON)
			{
				inputState.SetStartButtonJustPressed(true);
				inputState.SetStartButtonState(true);
			}
#else
			if (e.key.keysym.sym == SDLK_UP)
			{
				inputState.SetUpButtonJustPressed(true);
				inputState.SetUpButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_DOWN)
			{
				inputState.SetDownButtonJustPressed(true);
				inputState.SetDownButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_LEFT)
			{
				inputState.SetLeftButtonJustPressed(true);
				inputState.SetLeftButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_RIGHT)
			{
				inputState.SetRightButtonJustPressed(true);
				inputState.SetRightButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_a)
			{
				inputState.SetAButtonJustPressed(true);
				inputState.SetAButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_s)
			{
				inputState.SetBButtonJustPressed(true);
				inputState.SetBButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_z)
			{
				inputState.SetXButtonJustPressed(true);
				inputState.SetXButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_x)
			{
				inputState.SetYButtonJustPressed(true);
				inputState.SetYButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_SPACE)
			{
				inputState.SetStartButtonJustPressed(true);
				inputState.SetStartButtonState(true);
			}
			else if (e.key.keysym.sym == SDLK_r)
			{
				backThroughTime = true;	
			}
#endif
		}
		else if (e.type == SDL_KEYUP)
		{
#ifdef ANDROID
			if (e.key.keysym.sym == ouya::UP)
			{
				inputState.SetUpButtonJustReleased(true);
				inputState.SetUpButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::DOWN)
			{
				inputState.SetDownButtonJustReleased(true);
				inputState.SetDownButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::LEFT)
			{
				inputState.SetLeftButtonJustReleased(true);
				inputState.SetLeftButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::RIGHT)
			{
				inputState.SetRightButtonJustReleased(true);
				inputState.SetRightButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::U)
			{
				inputState.SetAButtonJustReleased(true);
				inputState.SetAButtonState(false);
			}
			else if (e.key.keysym.sym == ouya::OUYABUTTON)
			{
				inputState.SetStartButtonJustReleased(true);
				inputState.SetStartButtonState(false);
			}
#else
			if (e.key.keysym.sym == SDLK_UP)
			{
				inputState.SetUpButtonJustReleased(true);
				inputState.SetUpButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_DOWN)
			{
				inputState.SetDownButtonJustReleased(true);
				inputState.SetDownButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_LEFT)
			{
				inputState.SetLeftButtonJustReleased(true);
				inputState.SetLeftButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_RIGHT)
			{
				inputState.SetRightButtonJustReleased(true);
				inputState.SetRightButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_a)
			{
				inputState.SetAButtonJustReleased(true);
				inputState.SetAButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_s)
			{
				inputState.SetBButtonJustReleased(true);
				inputState.SetBButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_z)
			{
				inputState.SetXButtonJustReleased(true);
				inputState.SetXButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_x)
			{
				inputState.SetYButtonJustReleased(true);
				inputState.SetYButtonState(false);
			}
			else if (e.key.keysym.sym == SDLK_SPACE)
			{
				inputState.SetStartButtonJustReleased(true);
				inputState.SetStartButtonState(false);
			}
#endif
		}
	}
}

void game()
{
	while (true)
	{
		std::shared_ptr<bomberman::MenuScene> menuScene(new bomberman::MenuScene());
		run(menuScene);

		if (menuScene->GetSelection() == bomberman::MenuScene::NEWGAME)
		{

			std::shared_ptr<bomberman::SetupScene> setupScene(new bomberman::SetupScene());
			run(setupScene);
			
			std::shared_ptr<bomberman::GameScene> ts(new bomberman::GameScene(setupScene->GetConfig()));
			std::shared_ptr<bomberman::FadeScene> cover(new bomberman::FadeScene(ts));
			run(cover);

			std::shared_ptr<bomberman::VictoryScene> vs(new bomberman::VictoryScene(ts->GetVictor()));
			std::shared_ptr<bomberman::FadeScene> fs(new bomberman::FadeScene(vs));
			run(fs);
		}
		else
		{
		}

	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);   // Initialize SDL2
	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	TTF_Init();

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) 
	{
		printlog("Mix_OpenAudio: %s\n", Mix_GetError());
	}
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

	TTF_Quit();

	Mix_CloseAudio();

	Mix_Quit();

	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit(); 
	
	printlog("Success!\n");
	
	return 0;
}
