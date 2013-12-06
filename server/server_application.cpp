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
#include "server_application.hpp"

#include "network_packet.hpp"

#include <stdexcept>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//-------------------------
//Declarations
//-------------------------

int Client::counter = 0;
int Player::counter = 0;

//-------------------------
//Define the ServerApplication
//-------------------------

ServerApplication::ServerApplication() {
	//
}

ServerApplication::~ServerApplication() {
	//
}

void ServerApplication::Init(int argc, char** argv) {
	//TODO: proper command line option parsing

	//load config
	config.Load("rsc\\config.cfg");

	//Init SDL
	if (SDL_Init(0)) {
		throw(runtime_error("Failed to initialize SDL"));
	}
	cout << "initialized SDL" << endl;

	//Init SDL_net
	if (SDLNet_Init()) {
		throw(runtime_error("Failed to init SDL_net"));
	}
	network.Open(config.Int("server.port"), sizeof(NetworkPacket));
	cout << "initialized SDL_net" << endl;

	//Init SQL
	string dbname = (config["server.dbname"].size()) ? config["server.dbname"] : std::string(argv[0]) + ".db"; //fancy and unnecessary
	int ret = sqlite3_open_v2(dbname.c_str(), &database, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_FULLMUTEX, nullptr);
	if (ret != SQLITE_OK || !database) {
		throw(runtime_error("Failed to open the server database"));
	}
	cout << "initialized SQL" << endl;
	cout << "Database filename: " << dbname << endl;

	//TODO: move this into a function?
	//Run setup scripts
	ifstream is("rsc\\scripts\\setup_server.sql");
	if (!is.is_open()) {
		throw(runtime_error("Failed to run database setup script"));
	}
	string script;
	getline(is, script, '\0');
	is.close();
	sqlite3_exec(database, script.c_str(), nullptr, nullptr, nullptr);
}

void ServerApplication::Loop() {
	//debugging

	NetworkPacket packet;

	while(running) {
		//suck in the waiting packets & process them
		try {
			while(network.Receive()) {
				memcpy(&packet, network.GetInData(), sizeof(NetworkPacket));
				packet.meta.srcAddress = network.GetInPacket()->address;
				HandlePacket(packet);
			}
		}
		catch(exception& e) {
			cerr << "Network Error: " << e.what() << endl;
		}

		//give the computer a break
		SDL_Delay(10);
	}
}

void ServerApplication::Quit() {
	//members
	network.Close();

	//APIs
	sqlite3_close_v2(database);
	SDLNet_Quit();
	SDL_Quit();
}

void ServerApplication::HandlePacket(NetworkPacket packet) {
	switch(packet.meta.type) {
		case NetworkPacket::Type::BROADCAST_REQUEST:
			HandleBroadcastRequest(packet);
		break;

		case NetworkPacket::Type::JOIN_REQUEST:
			HandleJoinRequest(packet);
		break;

		case NetworkPacket::Type::DISCONNECT:
			HandleDisconnect(packet);
		break;

		case NetworkPacket::Type::SHUTDOWN:
			HandleShutdown(packet);
		break;

		case NetworkPacket::Type::PLAYER_NEW:
			HandlePlayerNew(packet);
		break;

		case NetworkPacket::Type::PLAYER_DELETE:
			HandlePlayerDelete(packet);
		break;

		case NetworkPacket::Type::PLAYER_UPDATE:
			HandlePlayerUpdate(packet);
		break;

		//handle errors
		default:
			throw(runtime_error("Unknown NetworkPacket::Type encountered"));
		break;
	}
}

void ServerApplication::HandleBroadcastRequest(NetworkPacket packet) {
	//send back the server's name
	packet.meta.type = NetworkPacket::Type::BROADCAST_RESPONSE;
	snprintf(packet.serverInfo.name, PACKET_STRING_SIZE, "%s", config["server.name"].c_str());
	network.Send(&packet.meta.srcAddress, &packet, sizeof(NetworkPacket));
}

void ServerApplication::HandleJoinRequest(NetworkPacket packet) {
	//TODO: prevent duplicate logins from the same address?

	//create the new client, filling it with the correct info
	Client newClient;
	newClient.address = packet.meta.srcAddress;

	//push the new client
	clientMap[Client::counter] = newClient;

	//send the client their info
	packet.meta.type = NetworkPacket::Type::JOIN_RESPONSE;
	packet.clientInfo.index = Client::counter;
	network.Send(&newClient.address, &packet, sizeof(NetworkPacket));

	//finished this routine
	Client::counter++;
	cout << "connect, total: " << clientMap.size() << endl;
}

void ServerApplication::HandleDisconnect(NetworkPacket packet) {
	//disconnect the specified client
	network.Send(&clientMap[packet.clientInfo.index].address, &packet, sizeof(NetworkPacket));
	clientMap.erase(packet.clientInfo.index);

	//TODO: remove players

	cout << "disconnect, total: " << clientMap.size() << endl;
}

void ServerApplication::HandleShutdown(NetworkPacket packet) {
	//end the server
	running = false;

	//disconnect all clients
	packet.meta.type = NetworkPacket::Type::DISCONNECT;
	PumpPacket(packet);

	cout << "shutting down" << endl;
}

void ServerApplication::HandlePlayerNew(NetworkPacket packet) {
	//create the new player object
	Player newPlayer;
	newPlayer.clientIndex = packet.playerInfo.clientIndex;
	newPlayer.handle = packet.playerInfo.handle;
	newPlayer.avatar = packet.playerInfo.avatar;
	newPlayer.position = {0,0};
	newPlayer.motion = {0,0};

	//push this player
	playerMap[Player::counter] = newPlayer;

	//send the client their info
	packet.playerInfo.playerIndex = Player::counter;
	packet.playerInfo.position = newPlayer.position;
	packet.playerInfo.motion = newPlayer.motion;

	//actually send to everyone
	PumpPacket(packet);

	//finish this routine
	Player::counter++;
	cout << "new player, total: " << playerMap.size() << endl;
}

void ServerApplication::HandlePlayerDelete(NetworkPacket packet) {
	if (playerMap.find(packet.playerInfo.playerIndex) == playerMap.end()) {
		throw(std::runtime_error("Cannot delete a non-existant player"));
	}

	playerMap.erase(packet.playerInfo.playerIndex);

	PumpPacket(packet);
}

void ServerApplication::HandlePlayerUpdate(NetworkPacket packet) {
	if (playerMap.find(packet.playerInfo.playerIndex) == playerMap.end()) {
		throw(std::runtime_error("Cannot update a non-existant player"));
	}

	//server is the slave to the clients, but only for now
	playerMap[packet.playerInfo.playerIndex].position = packet.playerInfo.position;
	playerMap[packet.playerInfo.playerIndex].motion = packet.playerInfo.motion;

	PumpPacket(packet);
}

void ServerApplication::PumpPacket(NetworkPacket packet) {
	//send this packet to all clients
	for (auto& it : clientMap) {
		network.Send(&it.second.address, &packet, sizeof(NetworkPacket));
	}
}