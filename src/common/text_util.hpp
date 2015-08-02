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
#pragma once

#include "image.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <string>

constexpr SDL_Color COLOR_WHITE = {255, 255, 255, 255};

SDL_Texture* renderPlainText(SDL_Renderer*, TTF_Font*, std::string, SDL_Color color);

class TextBox {
public:
	TextBox();
	~TextBox();

	void DrawTo(SDL_Renderer*);

	void SetText(SDL_Renderer*, TTF_Font*, std::string, SDL_Color color);
	void AddText(SDL_Renderer*, TTF_Font*, std::string, SDL_Color color);
	void ClearText();

	//position
	int SetX(int x) { return posX = x; }
	int SetY(int y) { return posY = y; }
	int SetX() { return posX; }
	int SetY() { return posY; }

protected:
	Image image;
	int posX = 0, posY = 0;
};