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

	playerCounter = currentPlayer = 0;

	playerMgr.New(playerCounter++, surfaceMgr->Get("elliot"));
	playerMgr.New(playerCounter++, surfaceMgr->Get("elliot"));
	playerMgr.New(playerCounter++, surfaceMgr->Get("coa"));
	playerMgr.New(playerCounter++, surfaceMgr->Get("coa"));
}

TestSystems::~TestSystems() {
	playerMgr.DeleteAll();
#ifdef DEBUG
	cout << "leaving TestSystems" << endl;
#endif
}

//-------------------------
//Frame loop
//-------------------------

void TestSystems::FrameStart() {
	frameRate.Calculate();
}

void TestSystems::FrameEnd() {
	//
}

void TestSystems::Update() {
	delta.Calculate();
	playerMgr.UpdateAll(delta.GetDelta());
}

void TestSystems::Render(SDL_Surface* const screen) {
	playerMgr.DrawAllTo(screen);
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
			SetNextScene(SceneList::MAINMENU);
		break;

		case SDLK_w:
			playerMgr[currentPlayer]->WalkInDirection(Direction::NORTH);
		break;
		case SDLK_s:
			playerMgr[currentPlayer]->WalkInDirection(Direction::SOUTH);
		break;
		case SDLK_a:
			playerMgr[currentPlayer]->WalkInDirection(Direction::WEST);
		break;
		case SDLK_d:
			playerMgr[currentPlayer]->WalkInDirection(Direction::EAST);
		break;

		case SDLK_1:
			SwitchToPlayer(0);
		break;
		case SDLK_2:
			SwitchToPlayer(1);
		break;
		case SDLK_3:
			SwitchToPlayer(2);
		break;
		case SDLK_4:
			SwitchToPlayer(3);
		break;
	}
}

void TestSystems::KeyUp(SDL_KeyboardEvent const& key) {
	switch(key.keysym.sym) {
		case SDLK_w:
			playerMgr[currentPlayer]->WalkInDirection(Direction::SOUTH);
		break;
		case SDLK_s:
			playerMgr[currentPlayer]->WalkInDirection(Direction::NORTH);
		break;
		case SDLK_a:
			playerMgr[currentPlayer]->WalkInDirection(Direction::EAST);
		break;
		case SDLK_d:
			playerMgr[currentPlayer]->WalkInDirection(Direction::WEST);
		break;
	}
}

//-------------------------
//Utilities
//-------------------------

void TestSystems::NewPlayer(int index, std::string avatarName, int x, int y) {
	Player* p = playerMgr.New(index, surfaceMgr->Get(avatarName));
	p->SetPosition(Vector2(x, y));
}

void TestSystems::SwitchToPlayer(int index) {
	//dirty hacks for smooth movement
	playerMgr[currentPlayer]->SetMotion(Vector2(0,0));
	currentPlayer = index;

	Uint8* key = SDL_GetKeyState(nullptr);

	if (key[SDLK_w]) {
		playerMgr[currentPlayer]->WalkInDirection(Direction::NORTH);
	}
	if (key[SDLK_s]) {
		playerMgr[currentPlayer]->WalkInDirection(Direction::SOUTH);
	}
	if (key[SDLK_a]) {
		playerMgr[currentPlayer]->WalkInDirection(Direction::WEST);
	}
	if (key[SDLK_d]) {
		playerMgr[currentPlayer]->WalkInDirection(Direction::EAST);
	}
}
