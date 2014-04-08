/* Copyright: (c) Kayne Ruse 2013
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
#ifndef PLAYERCHARACTER_HPP_
#define PLAYERCHARACTER_HPP_

#include "vector2.hpp"
#include "sprite_sheet.hpp"

class PlayerCharacter {
public:
	enum class Direction {
		NORTH, SOUTH, EAST, WEST
	};

	PlayerCharacter() = default;
	~PlayerCharacter() = default;

	void Update(double delta);

	void DrawTo(SDL_Surface* const dest, int camX, int camY) { sprite.DrawTo(dest, position.x - camX, position.y - camY); }

	//clunky code results in smooth movement and controls
	void AdjustDirection(Direction);
	void FaceDirection(Direction);
	void ResetDirection();
	void ResetSpeed();

	//accessors and mutators
	Vector2 SetPosition(Vector2 v) { return position = v; }
	Vector2 ShiftPosition(Vector2 v) { return position += v; }
	Vector2 GetPosition() { return position; }

	Vector2 SetMotion(Vector2 v) { return motion = v; }
	Vector2 ShiftMotion(Vector2 v) { return motion += v; }
	Vector2 GetMotion() { return motion; }

	SpriteSheet* GetSprite() { return &sprite; }
private:
	Vector2 position;
	Vector2 motion;
	SpriteSheet sprite;

	//for moving diagonally
	bool diagonal = false;
};

#endif