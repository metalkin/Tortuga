/* Copyright: (c) Kayne Ruse 2013, 2014
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
#ifndef INWORLD_HPP_
#define INWORLD_HPP_

//maps
#include "region_pager_base.hpp"

//networking
#include "udp_network_utility.hpp"
#include "serial_packet.hpp"

//graphics
#include "image.hpp"
#include "raster_font.hpp"
#include "button.hpp"
#include "tile_sheet.hpp"

//common
#include "frame_rate.hpp"

#include "base_character.hpp"
#include "local_character.hpp"

//client
#include "base_scene.hpp"

//STL
#include <map>

#include <chrono>

class InWorld : public BaseScene {
public:
	//Public access members
	InWorld(int* const argClientIndex, int* const argAccountIndex);
	~InWorld();

protected:
	//Frame loop
	void FrameStart();
	void Update();
	void FrameEnd();
	void RenderFrame();
	void Render(SDL_Surface* const);

	//Event handlers
	void QuitEvent();
	void MouseMotion(SDL_MouseMotionEvent const&);
	void MouseButtonDown(SDL_MouseButtonEvent const&);
	void MouseButtonUp(SDL_MouseButtonEvent const&);
	void KeyDown(SDL_KeyboardEvent const&);
	void KeyUp(SDL_KeyboardEvent const&);

	//Network handlers
	void HandlePacket(SerialPacket* const);
	void HandlePing(ServerPacket* const);
	void HandlePong(ServerPacket* const);
	void HandleDisconnect(ClientPacket* const);
	void HandleCharacterNew(CharacterPacket* const);
	void HandleCharacterDelete(CharacterPacket* const);
	void HandleCharacterUpdate(CharacterPacket* const);
	void HandleCharacterRejection(TextPacket* const);
	void HandleRegionContent(RegionPacket* const);

	//Server control
	void RequestSynchronize();
	void SendPlayerUpdate();
	void RequestDisconnect();
	void RequestShutDown();
	void RequestRegion(int roomIndex, int x, int y);

	//utilities
	void UpdateMap();

	//shared parameters
	UDPNetworkUtility& network = UDPNetworkUtility::GetSingleton();
	int& clientIndex;
	int& accountIndex;
	int characterIndex = -1;
	std::map<int, BaseCharacter> characterMap;

	//graphics
	Image buttonImage;
	RasterFont font;
	TileSheet tileSheet;

	//map
	RegionPagerBase regionPager;

	//UI
	Button disconnectButton;
	Button shutDownButton;

	//the camera structure
	struct {
		int x = 0, y = 0;
		int width = 0, height = 0;
		int marginX = 0, marginY = 0;
	} camera;
	FrameRate fps;

	//game
	BaseCharacter* localCharacter = nullptr;

	//connections
	//TODO: This needs it's own utility, for both InWorld and InCombat
	typedef std::chrono::steady_clock Clock;
	Clock::time_point lastBeat = Clock::now();
	int attemptedBeats = 0;
};

#endif
