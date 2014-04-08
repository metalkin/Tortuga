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
#include "region_pager.hpp"

#include "utility.hpp"

RegionPagerBase::RegionPagerBase(int argWidth, int argHeight, int argDepth):
	regionWidth(argWidth),
	regionHeight(argHeight),
	regionDepth(argDepth)
{
	//EMPTY
}

RegionPagerBase::~RegionPagerBase() {
	//EMPTY
}

Region::type_t RegionPagerBase::SetTile(int x, int y, int z, Region::type_t v) {
	Region* ptr = GetRegion(x, y);
	return ptr->SetTile(x - ptr->GetX(), y - ptr->GetY(), z, v);
}

Region::type_t RegionPagerBase::GetTile(int x, int y, int z) {
	Region* ptr = GetRegion(x, y);
	return ptr->GetTile(x - ptr->GetX(), y - ptr->GetY(), z);
}

Region* RegionPagerBase::GetRegion(int x, int y) {
	//snap the coords
	x = snapToBase(regionWidth, x);
	y = snapToBase(regionHeight, y);

	//get the region by various means

	//TODO: revert this try/catch point
	Region* ptr = nullptr;
	ptr = FindRegion(x, y);
	if (ptr) return ptr;
	ptr = LoadRegion(x, y);
	if (ptr) return ptr;
	return CreateRegion(x, y);
}

Region* RegionPagerBase::FindRegion(int x, int y) {
	//find the region
	for (std::list<Region*>::iterator it = regionList.begin(); it != regionList.end(); it++) {
		if ((*it)->GetX() == x && (*it)->GetY() == y) {
			return *it;
		}
	}
	return nullptr;
}

Region* RegionPagerBase::PushRegion(Region* ptr) {
	regionList.push_front(ptr);
	return regionList.front();
}