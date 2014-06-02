/* Copyright: (c) Kayne Ruse 2014
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
#include "restart.hpp"

#include "channels.hpp"

#include <stdexcept>

//-------------------------
//Public access members
//-------------------------

Restart::Restart(
	ConfigUtility* const argConfig,
	UDPNetworkUtility* const argNetwork,
	int* const argClientIndex,
	int* const argAccountIndex,
	int* const argCharacterIndex,
	std::map<int, CombatData>* argCombatMap,
	std::map<int, CharacterData>* argCharacterMap,
	std::map<int, EnemyData>* argEnemyMap
	):
	config(*argConfig),
	network(*argNetwork),
	clientIndex(*argClientIndex),
	accountIndex(*argAccountIndex),
	characterIndex(*argCharacterIndex),
	combatMap(*argCombatMap),
	characterMap(*argCharacterMap),
	enemyMap(*argEnemyMap)
{
	//setup the utility objects
	image.LoadSurface(config["dir.interface"] + "button_menu.bmp");
	image.SetClipH(image.GetClipH()/3);
	font.LoadSurface(config["dir.fonts"] + "pk_white_8.bmp");

	//pass the utility objects
	backButton.SetImage(&image);
	backButton.SetFont(&font);

	//set the button positions
	backButton.SetX(50);
	backButton.SetY(50 + image.GetClipH() * 0);

	//set the button texts
	backButton.SetText("Back");

	//full reset
	network.Unbind(Channels::SERVER);
	clientIndex = -1;
	accountIndex = -1;
	characterIndex = -1;
	combatMap.clear();
	characterMap.clear();
	enemyMap.clear();

	//auto return
	startTick = std::chrono::steady_clock::now();
}

Restart::~Restart() {
	//
}

//-------------------------
//Frame loop
//-------------------------

void Restart::Update(double delta) {
	if (std::chrono::steady_clock::now() - startTick > std::chrono::duration<int>(10)) {
		QuitEvent();
	}

	while(network.Receive()) {
		//EAT INCOMING PACKETS
	}
}

void Restart::RenderFrame() {
	SDL_FillRect(GetScreen(), 0, 0);
	Render(GetScreen());
	SDL_Flip(GetScreen());
	fps.Calculate();
}

void Restart::Render(SDL_Surface* const screen) {
	backButton.DrawTo(screen);
	font.DrawStringTo("You have been disconnected.", screen, 50, 30);
}

//-------------------------
//Event handlers
//-------------------------

void Restart::QuitEvent() {
	SetNextScene(SceneList::MAINMENU);
}

void Restart::MouseMotion(SDL_MouseMotionEvent const& motion) {
	backButton.MouseMotion(motion);
}

void Restart::MouseButtonDown(SDL_MouseButtonEvent const& button) {
	backButton.MouseButtonDown(button);
}

void Restart::MouseButtonUp(SDL_MouseButtonEvent const& button) {
	if (backButton.MouseButtonUp(button) == Button::State::HOVER) {
		QuitEvent();
	}
}

void Restart::KeyDown(SDL_KeyboardEvent const& key) {
	switch(key.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
			break;
	}
}

void Restart::KeyUp(SDL_KeyboardEvent const& key) {
	//
}