#ifndef TESTSYSTEMS_HPP_
#define TESTSYSTEMS_HPP_

#include "base_scene.hpp"

#include "config_utility.hpp"
#include "surface_manager.hpp"

class TestSystems : public BaseScene {
public:
	//Public access members
	TestSystems(ConfigUtility*, SurfaceManager*);
	virtual ~TestSystems();

protected:
	//Frame loop
	virtual void FrameStart();
	virtual void FrameEnd();
	virtual void Update();
	virtual void Render(SDL_Surface* const);

	//Event handlers
	virtual void MouseMotion(SDL_MouseMotionEvent const&);
	virtual void MouseButtonDown(SDL_MouseButtonEvent const&);
	virtual void MouseButtonUp(SDL_MouseButtonEvent const&);
	virtual void KeyDown(SDL_KeyboardEvent const&);
	virtual void KeyUp(SDL_KeyboardEvent const&);

	//members
	ConfigUtility* configUtil;
	SurfaceManager* surfaceMgr;
};

#endif
