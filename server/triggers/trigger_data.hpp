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
#ifndef TRIGGERDATA_HPP_
#define TRIGGERDATA_HPP_

#include "bounding_box.hpp"
#include "vector2.hpp"

#include "lua.hpp"

#include <string>

//TODO: (0) state-system for preventing double triggering
class TriggerData {
public:
	TriggerData() = default;
	~TriggerData() = default;

	std::string SetHandle(std::string);
	std::string GetHandle() const;

	Vector2 SetOrigin(Vector2 v);
	Vector2 GetOrigin();

	BoundingBox SetBoundingBox(BoundingBox b);
	BoundingBox GetBoundingBox();

	int SetScriptReference(int i);
	int GetScriptReference();

private:
	std::string handle;
	Vector2 origin;
	BoundingBox bounds;
	int scriptRef = LUA_NOREF;
};

#endif