#include "test_systems.hpp"

#include <iostream>

using namespace std;

//-------------------------
//Public access members
//-------------------------

TestSystems::TestSystems(ConfigUtility* cUtil, SurfaceManager* sMgr) {
#ifdef DEBUG
	cout << "entering TestSystems" << endl;
#endif
	configUtil = cUtil;
	surfaceMgr = sMgr;
}

TestSystems::~TestSystems() {
#ifdef DEBUG
	cout << "leaving TestSystems" << endl;
#endif
}

//-------------------------
//Frame loop
//-------------------------

void TestSystems::FrameStart() {
	//
}

void TestSystems::FrameEnd() {
	//
}

void TestSystems::Update() {
	//
}

void TestSystems::Render(SDL_Surface* const screen) {
	//
}

//-------------------------
//Event handlers
//-------------------------

void TestSystems::MouseMotion(SDL_MouseMotionEvent const& motion) {
	//
}

void TestSystems::MouseButtonDown(SDL_MouseButtonEvent const& button) {
	//
}

void TestSystems::MouseButtonUp(SDL_MouseButtonEvent const& button) {
	//
}

void TestSystems::KeyDown(SDL_KeyboardEvent const& key) {
	switch(key.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;
	}
}

void TestSystems::KeyUp(SDL_KeyboardEvent const& key) {
	//
}
