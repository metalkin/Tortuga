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
#include "waypoint_manager.hpp"

WaypointManager::WaypointManager() {
	//EMPTY
}

WaypointManager::~WaypointManager() {
	UnloadAll();
}

int WaypointManager::Create() {
	//implicitly creates the element
	WaypointData& waypointData = elementMap[counter];

	//no real values set
	waypointData.origin = {0, 0};
	waypointData.bounds = {0, 0, 0, 0};

	return counter++;
}

int WaypointManager::Create(Vector2 origin, BoundingBox bounds) {
	//implicitly creates the element
	WaypointData& waypointData = elementMap[counter];

	waypointData.origin = origin;
	waypointData.bounds = bounds;

	return counter++;
}

void WaypointManager::Unload(int uid) {
	elementMap.erase(uid);
}

void WaypointManager::UnloadAll() {
	elementMap.clear();
}

void WaypointManager::UnloadIf(std::function<bool(std::pair<const int, WaypointData const&>)> fn) {
	std::map<int, WaypointData>::iterator it = elementMap.begin();
	while (it != elementMap.end()) {
		if (fn(*it)) {
			it = elementMap.erase(it);
		}
		else {
			++it;
		}
	}
}

WaypointData* WaypointManager::Get(int uid) {
	std::map<int, WaypointData>::iterator it = elementMap.find(uid);

	if (it == elementMap.end()) {
		return nullptr;
	}

	return &it->second;
}

int WaypointManager::GetLoadedCount() {
	return elementMap.size();
}

std::map<int, WaypointData>* WaypointManager::GetContainer() {
	return &elementMap;
}

//hooks
lua_State* WaypointManager::SetLuaState(lua_State* L) {
	return lua = L;
}

lua_State* WaypointManager::GetLuaState() {
	return lua;
}