#pragma once

#include "player.hpp"
#include "inputstate.hpp"
#include "printlog.hpp"

#include <memory>

#include <luacppinterface.h>

namespace bomberman {
namespace bestiary {

	class Computer;
	typedef std::shared_ptr<Computer> ComputerPtr;

	class Computer : public Player
	{

		class ScriptAPI
		{
			Lua _lua;
		public:
			ScriptAPI(const std::string &script)
			{
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

				global.Set("__coroutine", coroutine);
				global.Set("map", map);
				global.Set("actions", actions);

				actions.Set("initialize", initialize);

				auto result = coroutine.RunScript("actions.initialize()\n" + script);
				printlog("ScriptResult: %s\n", result.c_str());
			}

			InputState Resume(int x, int y, const MapConstPtr &iPresentMap) const
			{
				auto global = _lua.GetGlobalEnvironment();
				LuaCoroutine coroutine = global.Get<LuaCoroutine>("__coroutine");
				LuaTable map = global.Get<LuaTable>("map");
				LuaTable actions = global.Get<LuaTable>("actions");
				coroutine.Resume();

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


				map.Set("getX", getX);
				map.Set("getY", getY);

				actions.Set("moveUp", moveUp);
				actions.Set("moveDown", moveDown);
				actions.Set("moveLeft", moveLeft);
				actions.Set("moveRight", moveRight);

				// make it more convenient in lua
				lua.RunScript(
					"map.getPosition = function()\n"
					"	return map.getX(), map.getY()\n"
					"end\n"
					);

				return input;
			}
		};

		ScriptAPI _script;
		
	public:
		static ComputerPtr Create(const std::string &iName, const std::string &iSpriteName, int iInputStateIdx, SDL_Renderer* iRenderer, bool* alive);
		Computer(ScriptAPI scrip);
		virtual void Evolve(const std::vector<InputState>& iInputs, uint32_t iTimestamp, const MapConstPtr &iPresentMap, const MapPtr &iFutureMap) const;
		virtual void Interact(const std::vector<InputState>& , uint32_t , const EntitySet &);
	};
}
}
