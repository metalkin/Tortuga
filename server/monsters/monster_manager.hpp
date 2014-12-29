/* Copyright: (c) Kayne Ruse 2013-2015
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source
 * distribution.
*/
#ifndef MONSTERMANAGER_HPP_
#define MONSTERMANAGER_HPP_

#include "manager_interface.hpp"
#include "monster_data.hpp"
#include "singleton.hpp"

#ifdef __unix__
 #include "lua.hpp"
 #include "sqlite3.h"
#else
 #include "lua/lua.hpp"
 #include "sqlite3/sqlite3.h"
#endif

#include <functional>
#include <string>

class MonsterManager:
	public Singleton<MonsterManager>,
	public ManagerInterface<MonsterData, std::string>
{
public:
	//common public methods
	int Create(std::string) override;
	int Load(std::string) override;
	int Save(int uid) override;
	void Unload(int uid) override;
	void Delete(int uid) override;

	void UnloadAll() override;
	void UnloadIf(std::function<bool(std::pair<const int, MonsterData>)> fn) override;

	//accessors & mutators
	MonsterData* Get(int uid) override;
	int GetLoadedCount() override;
	int GetTotalCount() override;
	std::map<int, MonsterData>* GetContainer() override;

	//hooks
	sqlite3* SetDatabase(sqlite3* db);
	sqlite3* GetDatabase();
	lua_State* SetLuaState(lua_State* L);
	lua_State* GetLuaState();

private:
	friend Singleton<MonsterManager>;

	MonsterManager() = default;
	~MonsterManager() = default;

	sqlite3* database = nullptr;
	lua_State* lua = nullptr;
};

#endif