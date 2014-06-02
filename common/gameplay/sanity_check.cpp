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
#include "account_data.hpp"
#include "character_data.hpp"
#include "client_data.hpp"
#include "combat_data.hpp"
#include "enemy_data.hpp"
#include "room_data.hpp"
#include "statistics.hpp"

/* DOCS: Sanity check, read more
 * Since most/all of the files in this directory are header files, I've created
 * this source file as a "sanity check", to ensure that the above header files
 * are written correctly via make.
 * 
 * Oddly enough, I'm pretty sure this is the first directory compiled in a
 * clean build.
*/