/* Copyright: (c) Kayne Ruse 2013-2016
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
#include "splash_screen.hpp"

#include "config_utility.hpp"

//-------------------------
//Public access members
//-------------------------

SplashScreen::SplashScreen(SDL_Window* w) {
	//fit the screen to the logo
	//NOTE: not using this window trick
	window = w;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	logo.Load(GetRenderer(), ConfigUtility::GetSingleton()["dir.logos"] + "krstudios.png");

	startTick = std::chrono::steady_clock::now();
}

SplashScreen::~SplashScreen() {
	//
}

//-------------------------
//Frame loop
//-------------------------

void SplashScreen::FrameStart() {
	//TODO: (0) config flag to change the delay
	if (std::chrono::steady_clock::now() - startTick > std::chrono::duration<int>(3)) {
		SetSceneSignal(SceneSignal::MAINMENU);
	}
}

void SplashScreen::RenderFrame(SDL_Renderer* renderer) {
	//TODO: version information
	int w = 0, h = 0;
	SDL_RenderGetLogicalSize(renderer, &w, &h);
	logo.DrawTo(renderer, (w - logo.GetClipW()) / 2, (h - logo.GetClipH()) / 2);
}
