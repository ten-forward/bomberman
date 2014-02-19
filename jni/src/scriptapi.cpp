#include "scriptapi.hpp"

#include "computer.hpp"
#include "propexplosion.hpp"
#include "bomb.hpp"
#include "propbomb.hpp"
#include "softblock.hpp"
#include "block.hpp"
#include "explosion.hpp"
#include "umpire.hpp"

namespace bomberman {
enum EntityFlags
{
	BOMB = 0x1,
	PLAYER = 0x2,
	BLOCK = 0x4,
	SOFTBLOCK = 0x8,
	PROPBOMB = 0x10,
	EXPLOSION = 0x20,
	PROPEXPLOSION = 0x40,
	OUTSIDE = 0x80,

	PLAYER1 = 0x1002,
	PLAYER2 = 0x2002,
	PLAYER3 = 0x4002,
	PLAYER4 = 0x8002
};

ScriptAPI::ScriptAPI(const std::string &script) :
    _lua(),
    _map(_lua.CreateTable()),
    _actions(_lua.CreateTable())
{
	_lua.LoadStandardLibraries();
	LuaTable global = _lua.GetGlobalEnvironment();

	auto initialize = _lua.CreateYieldingFunction<void()>(
		[]()
		{
			// Just to kick open the script so we 
			// dont have to say if running resume 
			// otherwise start
		}
	);

	auto coroutine = _lua.CreateCoroutine();
				
	global.Set("__waitflush", false);
	global.Set("__coroutine", coroutine);

	global.Set("map", _map);
	global.Set("actions", _actions);

	_actions.Set("initialize", initialize);

	_map.Set<int>("BOMB", BOMB);
	_map.Set<int>("PLAYER", PLAYER);
	_map.Set<int>("BLOCK", BLOCK);
	_map.Set<int>("SOFTBLOCK", SOFTBLOCK);
	_map.Set<int>("PROPBOMB", PROPBOMB);
	_map.Set<int>("EXPLOSION", EXPLOSION);
	_map.Set<int>("PROPEXPLOSION", PROPEXPLOSION);
	_map.Set<int>("OUTSIDE", OUTSIDE);

	_map.Set<int>("PLAYER1", PLAYER1);
	_map.Set<int>("PLAYER2", PLAYER2);
	_map.Set<int>("PLAYER3", PLAYER3);
	_map.Set<int>("PLAYER4", PLAYER4);
				
	std::string a = "actions.initialize()\n" + script;
	auto result = coroutine.RunScript(a);
	printlog("Script result: %s\n", result.c_str());
    
    _map.Set("_x", -1);
    _map.Set("_y", -1);
    _map.Set("_w", -1);
    _map.Set("_h", -1);
    _map.Set<InputState*>("_inputptr", NULL);
    _map.Set<Map*>("_mapptr", NULL);
    
	auto getX = _lua.CreateFunction<int()>([&]() -> int
                  {
                      return _map.Get<int>("_x");
                  });
    
	auto getY = _lua.CreateFunction<int()>([&]() -> int
                   {
                       return _map.Get<int>("_y");
                  });
	
	auto getWidth = _lua.CreateFunction<int()>([&]() -> int
                       {
                           return _map.Get<int>("_w");
                      });
    
	auto getHeight = _lua.CreateFunction<int()>([&]() -> int
                        {
                            return _map.Get<int>("_h");
                       });
    
	auto moveUp = _lua.CreateYieldingFunction<void()>([&]()
                     {
                         InputState* input = _map.Get<InputState*>("_inputptr");
                         input->SetButtonState(InputState::UP, true);
                         input->SetButtonPressed(InputState::UP, true);
                     });
    
	auto moveDown = _lua.CreateYieldingFunction<void()>([&]()
                        {
                            InputState* input = _map.Get<InputState*>("_inputptr");
                            input->SetButtonState(InputState::DOWN, true);
                            input->SetButtonPressed(InputState::DOWN, true);
                        });
    
	auto moveLeft = _lua.CreateYieldingFunction<void()>([&]()
                       {
                           InputState* input = _map.Get<InputState*>("_inputptr");
                           input->SetButtonState(InputState::LEFT, true);
                           input->SetButtonPressed(InputState::LEFT, true);
                       });
    
	auto moveRight = _lua.CreateYieldingFunction<void()>([&]()
                        {
                            InputState* input = _map.Get<InputState*>("_inputptr");
                            input->SetButtonState(InputState::RIGHT, true);
                            input->SetButtonPressed(InputState::RIGHT, true);
                        });
    
	auto placeBomb = _lua.CreateYieldingFunction<void()>([&]()
                        {
                            InputState* input = _map.Get<InputState*>("_inputptr");
                            input->SetButtonState(InputState::A, true);
                            input->SetButtonPressed(InputState::A, true);
                        });
    
    auto getNumOfPlayers = _lua.CreateFunction<int()>([&]
                                                      {
                                                          Map* iPresentMap = _map.Get<Map*>("_mapptr");
                                                          UmpirePtr umpire = std::dynamic_pointer_cast<Umpire>(iPresentMap->GetUmpire());
                                                          return umpire->GetPlayersRemaining();
                                                      });
    
	auto waitUntilFlush = _lua.CreateYieldingFunction<void()>([&]()
                              {
                                  LuaTable g = _lua.GetGlobalEnvironment();
                                  g.Set("__waitflush", true);
                              });
    
	auto lookAt = _lua.CreateFunction<int(int x, int y)>(
                    [&](int x, int y) -> int
                    {
                        Map* iPresentMap = _map.Get<Map*>("_mapptr");
                        int result = 0;
                        if (iPresentMap->CheckPosition(x,y) == Map::BOUNDARY)
                        {
                            return OUTSIDE;
                        }
                        
                        iPresentMap->ForeachEntity(
                                                   [&](const EntityPtr& ntt)
                                                   {
                                                       if (typeid(*ntt) == typeid(bomberman::arsenal::Bomb))
                                                       {
                                                           result |= BOMB;
                                                       }
                                                       else if (typeid(*ntt) == typeid(bomberman::arsenal::PropBomb))
                                                       {
                                                           result |= PROPBOMB;
                                                       }
                                                       else if (typeid(*ntt) == typeid(bomberman::architecture::SoftBlock))
                                                       {
                                                           result |= SOFTBLOCK;
                                                       }
                                                       else if (typeid(*ntt) == typeid(bomberman::architecture::Block))
                                                       {
                                                           result |= BLOCK;
                                                       }
                                                       else if (typeid(*ntt) == typeid(bomberman::arsenal::Explosion))
                                                       {
                                                           result |= EXPLOSION;
                                                       }
                                                       else if (typeid(*ntt) == typeid(bomberman::arsenal::PropExplosion))
                                                       {
                                                           result |= PROPEXPLOSION;
                                                       }
                                                       else if (typeid(*ntt) == typeid(bomberman::bestiary::Player))
                                                       {
                                                           auto player = std::dynamic_pointer_cast<bomberman::bestiary::Player>(ntt);
                                                           result |= PLAYER;
                                                           result |= 1 << (player->GetPlayerIndex() + 12);
                                                       }
                                                       else if (typeid(*ntt) == typeid(bomberman::bestiary::Computer))
                                                       {
                                                           auto player = std::dynamic_pointer_cast<bomberman::bestiary::Computer>(ntt);
                                                           result |= PLAYER;
                                                           result |= 1 << (player->GetPlayerIndex() + 12);
                                                       }
                                                       
                                                   });
                        
                        return result;
                    });
    
    
	_map.Set("getX", getX);
	_map.Set("getY", getY);
    
	_map.Set("getWidth", getWidth);
	_map.Set("getHeight", getHeight);
    
	_map.Set("lookAt", lookAt);
    
	_actions.Set("moveUp", moveUp);
	_actions.Set("moveDown", moveDown);
	_actions.Set("moveLeft", moveLeft);
	_actions.Set("moveRight", moveRight);
	_actions.Set("placeBomb", placeBomb);
	_actions.Set("waitUntilFlush", waitUntilFlush);
    
}

InputState ScriptAPI::Resume(int x, int y, int mx, int my, const MapConstPtr &iPresentMap) const
{
	auto global = _lua.GetGlobalEnvironment();
	LuaCoroutine coroutine = global.Get<LuaCoroutine>("__coroutine");
	LuaTable map = global.Get<LuaTable>("map");
	LuaTable actions = global.Get<LuaTable>("actions");
    
	InputState input;
    
    map.Set("_x", x);
    map.Set("_y", y);
    map.Set("_w", iPresentMap->GetWidth());
    map.Set("_h", iPresentMap->GetHeight());
    map.Set("_inputptr", &input);
    map.Set("_mapptr", iPresentMap.get());
    
	bool canResume = true;
    
	if (global.Get<bool>("__waitflush"))
	{
		canResume = false;
		if (mx % constants::AMOUNT_PER_TILE == 0 && my % constants::AMOUNT_PER_TILE == 0)
		{
			canResume = true;
			global.Set("__waitflush", false);
		}
	}

	if (canResume)
	{
		coroutine.Resume();
    }

	/*printlog("inputa: %d %d\ %d %d\n", 
		input.GetButtonState(InputState::DOWN), 
		input.GetButtonState(InputState::UP),
		mx, my
		);*/
		
	return input;
}
}
