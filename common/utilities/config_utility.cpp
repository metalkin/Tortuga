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
#include "config_utility.hpp"

#include <cstdlib>
#include <fstream>
#include <stdexcept>

void ConfigUtility::Load(std::string fname) {
	//TODO: recursive rerouting?
	std::ifstream is(fname);

	if (!is.is_open()) {
		throw(std::runtime_error("Failed to open config file"));
	}

	std::string key, val;

	while(true) { //forever
		//eat whitespace
		while(isspace(is.peek())) {
			is.ignore();
		}

		//end of file
		if (is.eof()) {
			break;
		}

		//skip comment lines
		if (is.peek() == '#') {
			while(is.peek() != '\n' && !is.eof()) {
				is.ignore();
			}
			continue;
		}

		//read in the pair
		getline(is, key,'=');
		getline(is, val);

		//trim the strings at the start & end
		while(key.size() && isspace(*key.begin())) key.erase(0, 1);
		while(val.size() && isspace(*val.begin())) val.erase(0, 1);

		while(key.size() && isspace(*(key.end()-1))) key.erase(key.end() - 1);
		while(val.size() && isspace(*(val.end()-1))) val.erase(val.end() - 1);

		//disallow empty/wiped values
		if (key.size() == 0) {
			continue;
		}

		//save the pair
		table[key] = val;
	}

	is.close();
}

std::string& ConfigUtility::String(std::string s) {
	return table[s];
}

int ConfigUtility::Integer(std::string s) {
	std::map<std::string, std::string>::iterator it = table.find(s);
	if (it == table.end()) {
		return 0;
	}
	return atoi(it->second.c_str());
}

double ConfigUtility::Double(std::string s) {
	std::map<std::string, std::string>::iterator it = table.find(s);
	if (it == table.end()) {
		return 0.0;
	}
	return atof(it->second.c_str());
}

bool ConfigUtility::Boolean(std::string s) {
	std::map<std::string, std::string>::iterator it = table.find(s);
	if (it == table.end()) {
		return false;
	}
	return it->second == "true";
}
