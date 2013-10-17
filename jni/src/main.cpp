#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <map>
#include <memory>
#include <boost/foreach.hpp>

#ifdef PROGRAM_OPTIONS
#include <boost/program_options.hpp>
#endif // PROGRAM_OPTIONS

#include "inputstate.hpp"
#include "constants.hpp"
#include "printlog.hpp"
#include "gamescene.hpp"
#include "menuscene.hpp"
#include "setupscene.hpp"
#include "fadescene.hpp"
#include "victoryscene.hpp"

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

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static std::map<int, InputState::Key> keyMap;

#ifdef PROGRAM_OPTIONS
static boost::program_options::variables_map vm;
#endif // PROGRAM_OPTIONS

void run(std::shared_ptr<bomberman::SceneInterface> scene)
{
	std::vector<InputState> inputState(4);
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
		
		for (auto keyIter = keyMap.begin(); keyIter != keyMap.end(); keyIter++)
		{
			inputState.SetButtonPressed(keyIter->second, false);
			inputState.SetButtonReleased(keyIter->second, false);
		}

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
			auto keyIter = keyMap.find(e.key.keysym.sym);
			if ( keyIter != keyMap.end() )
			{
				inputState.SetButtonPressed(keyIter->second, true);
				inputState.SetButtonState(keyIter->second, true);
			}
		}
		else if (e.type == SDL_KEYUP)
		{
			auto keyIter = keyMap.find(e.key.keysym.sym);
			if ( keyIter != keyMap.end() )
			{
				inputState.SetButtonReleased(keyIter->second, true);
				inputState.SetButtonState(keyIter->second, false);
			}
		}
	}
}

void game()
{
#ifdef PROGRAM_OPTIONS
	// This is the game's options harness. We perform game testing here without having to go through
	// the menus which are tedious
	if (!vm.empty())
	{
		bomberman::PlayerConfigArray players;

		int p1 = vm.count("p1");
		int p2 = vm.count("p2");
		int p3 = vm.count("p3");
		int p4 = vm.count("p4");

		players[0].name = "Athos";
		players[0].spriteName = "test/SaturnBomberman-BlackBomberman.PNG";
		players[0].present = p1 ? true : false;
		players[0].isComputer = p1 ? true : false;
		players[0].aiScript =  p1 ? vm["p1"].as<std::string>() : "";

		players[1].name = "Porthos";
		players[1].spriteName = "test/honey.png";
		players[1].present = p2 ? true : false;
		players[1].isComputer = p2 ? true : false;
		players[1].aiScript = p2 ? vm["p2"].as<std::string>() : "";

		players[2].name = "Aramis";
		players[2].spriteName = "test/manji.png";
		players[2].present = p3 ? true : false;
		players[2].isComputer = p3 ? true : false;
		players[2].aiScript = p3 ? vm["p3"].as<std::string>() : "";

		players[3].name = "D'Artagnan";
		players[3].spriteName = "test/whitebbman.png";
		players[3].present = p4 ? true : false;
		players[3].isComputer = p4 ? true : false;
		players[3].aiScript = p4 ? vm["p4"].as<std::string>() : "";
		
		for (int i=0;i<4;i++)
		{
			if(vm.count("human"))
			{
				int humanplayer = vm["human"].as<int>();
				if(i+1 == humanplayer)
				{
					players[i].present = true;
					players[i].isComputer = false;
				}
			}
		}
		
		std::shared_ptr<bomberman::GameScene> ts(new bomberman::GameScene(players));
		std::shared_ptr<bomberman::FadeScene> cover(new bomberman::FadeScene(ts));
		run(cover);
			
		std::shared_ptr<bomberman::VictoryScene> vs(new bomberman::VictoryScene(ts->GetVictor()));
		std::shared_ptr<bomberman::FadeScene> fs(new bomberman::FadeScene(vs));
		run(fs);
		return;
	}
#endif // PROGRAM_OPTIONS

	// This is where the game's storyboard is encoded
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
#ifdef PROGRAM_OPTIONS
	namespace po = boost::program_options;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Produce this help message")
		("human", po::value<int>(), "Human player number (1-4)")
		("p1", po::value<std::string>(), "Player 1 AI Script")
		("p2", po::value<std::string>(), "Player 2 AI Script")
		("p3", po::value<std::string>(), "Player 3 AI Script")
		("p4", po::value<std::string>(), "Player 4 AI Script");
	
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::stringstream ss;
		ss << desc << std::endl;
		printlog("%s\n", ss.str().c_str());
		return 0;
	}
#endif // PROGRAM_OPTIONS

#ifdef ANDROID
	keyMap[ouya::UP] = InputState::UP;
	keyMap[ouya::DOWN] = InputState::DOWN;
	keyMap[ouya::LEFT] = InputState::LEFT;
	keyMap[ouya::RIGHT] = InputState::RIGHT;
	keyMap[ouya::A] = InputState::A;
	keyMap[ouya::O] = InputState::B;
	keyMap[ouya::U] = InputState::X;
	keyMap[ouya::Y] = InputState::Y;
	keyMap[ouya::L1] = InputState::L1;
	keyMap[ouya::L2] = InputState::L2;
	keyMap[ouya::R1] = InputState::R1;
	keyMap[ouya::R2] = InputState::R2;
	keyMap[ouya::OUYABUTTON] = InputState::START;
#else
	keyMap[SDLK_UP] = InputState::UP;
	keyMap[SDLK_DOWN] = InputState::DOWN;
	keyMap[SDLK_LEFT] = InputState::LEFT;
	keyMap[SDLK_RIGHT] = InputState::RIGHT;
	keyMap[SDLK_a] = InputState::A;
	keyMap[SDLK_s] = InputState::B;
	keyMap[SDLK_z] = InputState::X;
	keyMap[SDLK_x] = InputState::Y;
	keyMap[SDLK_1] = InputState::L1;
	keyMap[SDLK_3] = InputState::L2;
	keyMap[SDLK_2] = InputState::R1;
	keyMap[SDLK_4] = InputState::R2;
	keyMap[SDLK_SPACE] = InputState::START;
#endif

	SDL_Init(SDL_INIT_EVERYTHING);   // Initialize SDL2
	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
	TTF_Init();

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) 
	{
		printlog("Mix_OpenAudio: %s\n", Mix_GetError());
	}
	using namespace bomberman::constants;

	int WIDTH = SCREEN_WIDTH, HEIGHT = SCREEN_HEIGHT;

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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

	SDL_DestroyRenderer(renderer);

	TTF_Quit();

	Mix_CloseAudio();

	Mix_Quit();

	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit(); 
	
	printlog("Success!\n");
	
	return 0;
}
