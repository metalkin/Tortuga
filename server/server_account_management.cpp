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
#include "server_application.hpp"

#include "sqlite3/sqlite3.h"

#include <stdexcept>

//-------------------------
//Define the queries
//-------------------------

static const char* CREATE_USER_ACCOUNT = "INSERT INTO UserAccounts (username) VALUES (?);";
static const char* LOAD_USER_ACCOUNT = "SELECT * FROM UserAccounts WHERE username = ?;";
static const char* SAVE_USER_ACCOUNT = "INSERT OR REPLACE INTO UserAccounts VALUES (?, ?, ?, ?);";
static const char* DELETE_USER_ACCOUNT = "DELETE FROM UserAccounts WHERE uid = ?;";

//-------------------------
//Define the methods
//-------------------------

int ServerApplication::CreateUserAccount(std::string username, int clientIndex, int characterIndex) {
	//create this user account, failing if it exists, leave this account in memory
	sqlite3_stmt* statement = nullptr;

	//prep
	if (sqlite3_prepare_v2(database, CREATE_USER_ACCOUNT, -1, &statement, nullptr) != SQLITE_OK) {
		throw( std::runtime_error(std::string() + "Failed to prepare an SQL statement: " + sqlite3_errmsg(database)) );
	}

	//parameter
	if (sqlite3_bind_text(statement, 1, username.c_str(), username.size() + 1, SQLITE_STATIC) != SQLITE_OK) {
		throw( std::runtime_error(std::string() + "Failed to replace a prepared statement's parameter: " + sqlite3_errmsg(database)) );
	}

	//execute
	if (sqlite3_step(statement) != SQLITE_DONE) {
		//if this fails, than this account exists
		sqlite3_finalize(statement);
		return -1;
	}

	sqlite3_finalize(statement);

	//load this account into memory
	return LoadUserAccount(username, clientIndex, characterIndex);
}

int ServerApplication::LoadUserAccount(std::string username, int clientIndex, int characterIndex) {
	//load this user account, failing if it is in memory, creating it if it doesn't exist
	sqlite3_stmt* statement = nullptr;

	//prep
	if (sqlite3_prepare_v2(database, LOAD_USER_ACCOUNT, -1, &statement, nullptr) != SQLITE_OK) {
		throw( std::runtime_error(std::string() + "Failed to prepare an SQL statement: " + sqlite3_errmsg(database)) );
	}

	//parameter
	if (sqlite3_bind_text(statement, 1, username.c_str(), username.size() + 1, SQLITE_STATIC) != SQLITE_OK) {
		throw( std::runtime_error(std::string() + "Failed to replace a prepared statement's parameter: " + sqlite3_errmsg(database)) );
	}

	//execute
	int ret = sqlite3_step(statement);

	//process the result
	if (ret == SQLITE_ROW) {
		//get the index
		int uid = sqlite3_column_int(statement, 0);

		//check to see if this account is already loaded
		if (accountMap.find(uid) != accountMap.end()) {
			sqlite3_finalize(statement);
			return -1;
		}

		//extract the data into memory
		AccountData& newAccount = accountMap[uid];
		newAccount.username = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		newAccount.blackListed = sqlite3_column_int(statement, 2);
		newAccount.whiteListed = sqlite3_column_int(statement, 3);
		newAccount.clientIndex = clientIndex;
		newAccount.characterIndex = characterIndex;

		//finish the routine
		sqlite3_finalize(statement);
		return uid;
	}

	sqlite3_finalize(statement);

	if (ret == SQLITE_DONE) {
		//create the non-existant account instead
		return CreateUserAccount(username, clientIndex, characterIndex);
	}

	throw(std::runtime_error(std::string() + "Unknown SQL error in LoadUserAccount: " + sqlite3_errmsg(database) ));
}

int ServerApplication::SaveUserAccount(int uid) {
	//save this user account from memory, replacing it if it exists in the database
	//DOCS: To use this method, change the in-memory copy, and then call this function using that object's UID.

	//this method fails if this account is not loaded
	if (accountMap.find(uid) == accountMap.end()) {
		return -1;
	}

	AccountData& account = accountMap[uid];
	sqlite3_stmt* statement = nullptr;

	//prep
	if (sqlite3_prepare_v2(database, SAVE_USER_ACCOUNT, -1, &statement, nullptr) != SQLITE_OK) {
		throw( std::runtime_error(std::string() + "Failed to prepare an SQL statement: " + sqlite3_errmsg(database)) );
	}

	//parameters
	bool ret = false;
	ret |= sqlite3_bind_int(statement, 1, uid) != SQLITE_OK;
	ret |= sqlite3_bind_text(statement, 2, account.username.c_str(), account.username.size() + 1, SQLITE_STATIC) != SQLITE_OK;
	ret |= sqlite3_bind_int(statement, 3, account.blackListed) != SQLITE_OK;
	ret |= sqlite3_bind_int(statement, 4, account.whiteListed) != SQLITE_OK;

	//check for binding errors
	if (ret) {
		throw( std::runtime_error(std::string() + "Failed to replace a prepared statement's parameter: " + sqlite3_errmsg(database)) );
	}

	//execute
	if (sqlite3_step(statement) != SQLITE_DONE) {
		//if this fails, than something went horribly wrong
		sqlite3_finalize(statement);
		throw( std::runtime_error(std::string() + "Unknown SQL error when saving an account: " + sqlite3_errmsg(database)) );
	}

	sqlite3_finalize(statement);

	//successful execution
	return 0;
}

void ServerApplication::UnloadUserAccount(int uid) {
	//save this user account, and then unload it
	SaveUserAccount(uid);
	accountMap.erase(uid);
	//TODO: unload this account's characters?
}

void ServerApplication::DeleteUserAccount(int uid) {
	//delete a user account from the database, and remove it from memory
	sqlite3_stmt* statement = nullptr;

	//prep
	if (sqlite3_prepare_v2(database, DELETE_USER_ACCOUNT, -1, &statement, nullptr) != SQLITE_OK) {
		throw( std::runtime_error(std::string() + "Failed to prepare an SQL statement: " + sqlite3_errmsg(database)) );
	}

	//parameter
	if (sqlite3_bind_int(statement, 1, uid) != SQLITE_OK) {
		throw( std::runtime_error(std::string() + "Failed to replace a prepared statement's parameter: " + sqlite3_errmsg(database)) );
	}

	//execute
	if (sqlite3_step(statement) != SQLITE_DONE) {
		//if this fails, than something went horribly wrong
		sqlite3_finalize(statement);
		throw( std::runtime_error(std::string() + "Unknown SQL error when deleting an account: " + sqlite3_errmsg(database)) );
	}

	//finish the routine
	sqlite3_finalize(statement);
	accountMap.erase(uid);
}
