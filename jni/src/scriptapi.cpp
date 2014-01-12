#include "scriptapi.hpp"

#include "computer.hpp"
#include "propexplosion.hpp"
#include "bomb.hpp"
#include "propbomb.hpp"
#include "softblock.hpp"
#include "block.hpp"
#include "explosion.hpp"

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

ScriptAPI::ScriptAPI(const std::string &script)
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
				
	LuaTable map = _lua.CreateTable();
	LuaTable actions = _lua.CreateTable();
				
	global.Set("__waitflush", false);
	global.Set("__coroutine", coroutine);

	global.Set("map", map);
	global.Set("actions", actions);

	actions.Set("initialize", initialize);

	map.Set<int>("BOMB", BOMB);
	map.Set<int>("PLAYER", PLAYER);
	map.Set<int>("BLOCK", BLOCK);
	map.Set<int>("SOFTBLOCK", SOFTBLOCK);
	map.Set<int>("PROPBOMB", PROPBOMB);
	map.Set<int>("EXPLOSION", EXPLOSION);
	map.Set<int>("PROPEXPLOSION", PROPEXPLOSION);
	map.Set<int>("OUTSIDE", OUTSIDE);

	map.Set<int>("PLAYER1", PLAYER1);
	map.Set<int>("PLAYER2", PLAYER2);
	map.Set<int>("PLAYER3", PLAYER3);
	map.Set<int>("PLAYER4", PLAYER4);
				
	std::string a = "actions.initialize()\n" + script;
	auto result = coroutine.RunScript(a);
	printlog("Script result: %s\n", result.c_str());
}

InputState ScriptAPI::Resume(int x, int y, int mx, int my, const MapConstPtr &iPresentMap) const
{
	auto global = _lua.GetGlobalEnvironment();
	LuaCoroutine coroutine = global.Get<LuaCoroutine>("__coroutine");
	LuaTable map = global.Get<LuaTable>("map");
	LuaTable actions = global.Get<LuaTable>("actions");

	InputState input;
				
	Lua lua = _lua;
	auto getX = lua.CreateFunction<int()>([&]() -> int
	{
		return x;
	});

	auto getY = lua.CreateFunction<int()>([&]() -> int
	{
		return y;
	});
	
	auto getWidth = lua.CreateFunction<int()>([&]() -> int
	{
		return iPresentMap->GetWidth();
	});

	auto getHeight = lua.CreateFunction<int()>([&]() -> int
	{
		return iPresentMap->GetHeight();
	});

	auto moveUp = lua.CreateYieldingFunction<void()>([&]()
	{
		input.SetButtonState(InputState::UP, true);
		input.SetButtonPressed(InputState::UP, true);
	});
				
	auto moveDown = lua.CreateYieldingFunction<void()>([&]()
	{
		input.SetButtonState(InputState::DOWN, true);
		input.SetButtonPressed(InputState::DOWN, true);
	});
				
	auto moveLeft = lua.CreateYieldingFunction<void()>([&]()
	{
		input.SetButtonState(InputState::LEFT, true);
		input.SetButtonPressed(InputState::LEFT, true);
	});
				
	auto moveRight = lua.CreateYieldingFunction<void()>([&]()
	{
		input.SetButtonState(InputState::RIGHT, true);
		input.SetButtonPressed(InputState::RIGHT, true);
	});
				
	auto placeBomb = lua.CreateYieldingFunction<void()>([&]()
	{
		input.SetButtonState(InputState::A, true);
		input.SetButtonPressed(InputState::A, true);
	});

	auto waitUntilFlush = lua.CreateYieldingFunction<void()>([&]()
	{
		global.Set("__waitflush", true);
	});

	auto lookAt = lua.CreateFunction<int(int x, int y)>(
	[&](int x, int y) -> int
	{
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


	map.Set("getX", getX);
	map.Set("getY", getY);

	map.Set("getWidth", getWidth);
	map.Set("getHeight", getHeight);

	map.Set("lookAt", lookAt);

	actions.Set("moveUp", moveUp);
	actions.Set("moveDown", moveDown);
	actions.Set("moveLeft", moveLeft);
	actions.Set("moveRight", moveRight);
	actions.Set("placeBomb", placeBomb);
	actions.Set("waitUntilFlush", waitUntilFlush);

	bool canResume = true;

	if (global.Get<bool>("__waitflush"))
	{
		canResume = false;
		if (mx == 0 && my == 0)
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
