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
#ifndef WAYPOINTMANAGER_HPP_
#define WAYPOINTMANAGER_HPP_

#include "waypoint_data.hpp"
#include "manager_interface.hpp"
#include "singleton.hpp"
#include "vector2.hpp"

#include <functional>
#include <string>

class WaypointManager:
	public Singleton<WaypointManager>,
	public ManagerInterface<WaypointData>
{
public:
	//common public methods
	int Create() override;
	int Load() override;
	int Save(int uid) override;
	void Unload(int uid) override;
	void Delete(int uid) override;

	void UnloadAll() override;
	void UnloadIf(std::function<bool(std::pair<const int, WaypointData>)> fn) override;

	//accessors & mutators
	WaypointData* Get(int uid) override;
	int GetLoadedCount() override;
	int GetTotalCount() override;
	std::map<int, WaypointData>* GetContainer() override;

private:
	friend Singleton<WaypointManager>;

	WaypointManager() = default;
	~WaypointManager() = default;
};

#endif