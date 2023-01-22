#include "base.h"

Base::Execute::_DiplFunc Base::Execute::diplFunc = 0;
Base::Execute::_UnlockTechFunc Base::Execute::unlockTechFunc = 0;
Base::Execute::_SpawnUnitFunc Base::Execute::spawnUnitFunc = 0;
Base::Execute::_CreateTransportFuncEvent Base::Execute::createTransportFuncEvent = 0;
Base::Execute::_CreateFactoryFunc Base::Execute::createFactoryFunc = 0;
Base::Execute::_DestroyFactoryFunc Base::Execute::destroyFactoryFunc = 0;
Base::Execute::_TreatyFunc Base::Execute::treatyFunc = 0;
Base::Execute::_FacilityStatusFunc Base::Execute::facilityStatusFunc = 0;

void Base::Execute::SetupFunctions()
{
	using namespace Base::Execute;

	diplFunc = (_DiplFunc)(Base::SRU_Data::g_base + Offsets::diplFunc);
	unlockTechFunc = (_UnlockTechFunc)(Base::SRU_Data::g_base + Offsets::unlockTechFunc);
	spawnUnitFunc = (_SpawnUnitFunc)(Base::SRU_Data::g_base + Offsets::unitFunc);
	createTransportFuncEvent = (_CreateTransportFuncEvent)(Base::SRU_Data::g_base + Offsets::transportFuncEvent);
	createFactoryFunc = (_CreateFactoryFunc)(Base::SRU_Data::g_base + Offsets::createFactoryFunc);
	destroyFactoryFunc = (_DestroyFactoryFunc)(Base::SRU_Data::g_base + Offsets::destroyFactoryFunc);
	treatyFunc = (_TreatyFunc)(Base::SRU_Data::g_base + Offsets::treatyFunc);
	facilityStatusFunc = (_FacilityStatusFunc)(Base::SRU_Data::g_base + Offsets::facilityStatusFunc);
}

void Base::Execute::CreateFacility(__int16 posX, __int16 posY, int countryOId, int facilityId, float constructionState)
{
	using namespace Base::SRU_Data;

	__int16 realX = posX;
	__int16 realY = posY;

	if (realX < 0)
	{
		realX = Base::SRU_Data::g_mapSizeX + posX;
	}

	if (realY < 0) realY = 0;

	DWORD posData = MAKELONG(realX, realY);

	__int16 v9 = (__int16)posData;
	if (v9 >= Base::SRU_Data::g_mapSizeX)
	{
		v9 = (__int16)posData % Base::SRU_Data::g_mapSizeX;
	}
	else if ((posData & 0x8000) != 0)
	{
		v9 = Base::SRU_Data::g_mapSizeX + (__int16)posData % Base::SRU_Data::g_mapSizeX;
	}

	long temp = v9 + Base::SRU_Data::g_mapSizeX * HIWORD(posData);
	long mult = 16 * temp;

	DWORD base1 = *(DWORD*)(Base::SRU_Data::g_base + Offsets::allHexStart);
	DWORD base2 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 0xC) + mult;
	DWORD base3 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 16) + (4 * temp);
	DWORD base4 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 20) + (4 * temp);

#pragma region CheckResource

	//Checks if resource exists for facilities that depend on it.
	//Without this they would pretty much be useless

	int facilResource = -1;

	switch (facilityId)
	{
	case 21010: //Industrial Agriculture
		facilResource = Resource_Agriculture;
		break;
	case 21014: //Timber Mill
		facilResource = Resource_Timber;
		break;
	case 21017: //Gas Field
	case 21018: //Oil Derrick
		facilResource = Resource_Petroleum;
		break;
	case 21025: //Coal Mine
		facilResource = Resource_Coal;
		break;
	case 21028: //Ore Mine
		facilResource = Resource_MetalOre;
		break;
	case 21032: //Uranium Mine
		facilResource = Resource_Uranium;
		break;
	case 21037: //Hydro Sm
	case 21038: //Hydro
		facilResource = Resource_Electricity;
		break;
	case 21065: //Rubber Plantation
		facilResource = Resource_Rubber;
		break;
	}

	uint8_t* resAddr = (uint8_t*)base3 + 2;

	if (facilResource > 3)
	{
		resAddr = (uint8_t*)base3 + 3;
		facilResource -= 4;
	}

	if (facilResource > -1)
	{
		int lvl = Base::Execute::GetMapResource(resAddr, facilResource);

		if (!lvl)
		{
			return;
		}
	}

#pragma endregion

	DWORD* buffer = (DWORD*)calloc(0x20, sizeof(DWORD));
	buffer[0] = posData;
	buffer[1] = base1;
	buffer[2] = base2;
	buffer[3] = base3;
	buffer[4] = base4;
	buffer[5] = temp;

	//Base::SRU_Data::Asm::g_ownAllocs.push_back((uintptr_t)buffer);

	//Dont ask me why 2 times, its called 2 times in the original function
	Base::Execute::createFactoryFunc((int)buffer, 0, 0, countryOId, facilityId, 1.0f, 0, constructionState, 0);
	Base::Execute::createFactoryFunc((int)buffer, 1, 0, countryOId, facilityId, 1.0f, 0, constructionState, 0);
	free(buffer);
}

uintptr_t* Base::Execute::GetTileAddr(uint16_t xPos, uint16_t yPos)
{
	using namespace Base::SRU_Data;

	std::vector<Base::SRU_Data::AddressHolder> facilities;

	__int16 realX = xPos;
	__int16 realY = yPos;
	if (realX < 0)
	{
		realX = Base::SRU_Data::g_mapSizeX + realX;
	}
	if (realY < 0) realY = 0;

	DWORD posData = MAKELONG(realX, realY);

	__int16 v9 = (__int16)posData;
	if (v9 >= Base::SRU_Data::g_mapSizeX)
	{
		v9 = (__int16)posData % Base::SRU_Data::g_mapSizeX;
	}
	else if ((posData & 0x8000) != 0)
	{
		v9 = Base::SRU_Data::g_mapSizeX + (__int16)posData % Base::SRU_Data::g_mapSizeX;
	}

	long temp = v9 + Base::SRU_Data::g_mapSizeX * HIWORD(posData);
	long mult = 16 * temp;

	DWORD base2 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 0xC) + mult;

	uintptr_t* addr = (uintptr_t*)base2;

	return addr;
}

uintptr_t* Base::Execute::GetFacilityRoot(uint16_t xPos, uint16_t yPos)
{
	using namespace Base::SRU_Data;

	std::vector<Base::SRU_Data::AddressHolder> facilities;

	__int16 realX = xPos;
	__int16 realY = yPos;
	if (realX < 0)
	{
		realX = Base::SRU_Data::g_mapSizeX + realX;
	}
	if (realY < 0) realY = 0;

	DWORD posData = MAKELONG(realX, realY);

	__int16 v9 = (__int16)posData;
	if (v9 >= Base::SRU_Data::g_mapSizeX)
	{
		v9 = (__int16)posData % Base::SRU_Data::g_mapSizeX;
	}
	else if ((posData & 0x8000) != 0)
	{
		v9 = Base::SRU_Data::g_mapSizeX + (__int16)posData % Base::SRU_Data::g_mapSizeX;
	}

	long temp = v9 + Base::SRU_Data::g_mapSizeX * HIWORD(posData);
	long mult = 16 * temp;

	DWORD base3 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 20) + (4 * temp);

	uintptr_t* addr = (uintptr_t*)base3;
	
	return addr;
}

std::vector<struct Base::SRU_Data::AddressHolder> Base::Execute::GetFacilities(uintptr_t* root)
{
	using namespace Base::SRU_Data;

	std::vector<Base::SRU_Data::AddressHolder> facilities;

	DWORD val = *root;

	bool ok = true;
	while (ok)
	{
		uint8_t* ptr1 = (uint8_t*)(val + 177);
		DWORD* ptr2 = (DWORD*)(val + 356);
		WORD* ptr3 = (WORD*)(val + 12);
		BYTE* ptr4 = (BYTE*)(val + 179);

		if (!Base::Utils::CanReadPtr(ptr1) || !Base::Utils::CanReadPtr(ptr2) || !Base::Utils::CanReadPtr(ptr3) || !Base::Utils::CanReadPtr(ptr4))
		{
			break;
		}

		uint8_t v1870 = *(uint8_t*)(val + 177);
		DWORD v5012 = *(DWORD*)(val + 356);

		if ((v1870 & 0x20) != 0 && (v1870 & 0x80) == 0 && *(WORD*)(val + 12) && (*(BYTE*)(val + 179) & 0x40) == 0)
		{
			int id = *(uintptr_t*)(val + 0xC);
			int id2 = *(uintptr_t*)(val + 0x58);

			for (int i = 0; i < g_facilityList.size(); i++)
			{
				if (g_facilityList[i]->id == id)
				{
					AddressHolder temp;
					temp.addr = val;
					temp.id = id;
					temp.id2 = id2;
					temp.name = g_facilityList[i]->name;
					facilities.push_back(temp);
					break;
				}
			}
		}
		else
		{
			AddressHolder temp;
			temp.addr = val;
			temp.id = -1;
			facilities.push_back(temp);
		}

		val = v5012;
		if (!v5012) break;
	}

	return facilities;
}

void Base::Execute::DisableAllFacilities(uint16_t xPos, uint16_t yPos, int disable)
{
	uintptr_t* tileAddr = Base::Execute::GetTileAddr(xPos, yPos);
	uintptr_t* root = Base::Execute::GetFacilityRoot(xPos, yPos);
	std::vector<Base::SRU_Data::AddressHolder> facilities = Base::Execute::GetFacilities(root);

	for (int i = 0; i < facilities.size(); i++)
	{
		Base::Execute::DisableFacility(tileAddr, root, facilities, i, disable);
	}
}

void Base::Execute::DisableFacility(uintptr_t* tileAddr, uintptr_t* rootAddr, std::vector<Base::SRU_Data::AddressHolder> facilities, int index, int disable)
{
	if (facilities[index].id == -1) return;

	int* buffer = (int*)calloc(16, sizeof(int));
	buffer[0] = 0xD301B0;
	buffer[1] = 0;
	buffer[2] = (int)tileAddr;
	buffer[3] = 0;
	buffer[4] = (int)rootAddr;

	int num = facilities[index].id2;

	Base::Execute::facilityStatusFunc((int)buffer, 0, 0x000000B8, num, disable);
	free(buffer);
}

void Base::Execute::DestroyAllFacilities(uint16_t xPos, uint16_t yPos)
{
	uintptr_t* root = Base::Execute::GetFacilityRoot(xPos, yPos);
	std::vector<Base::SRU_Data::AddressHolder> facilities = Base::Execute::GetFacilities(root);

	for (int i = 0; i < facilities.size(); i++)
	{
		destroyFactoryFunc(facilities[i].addr, 0, 0);
	}

	*root = 0;
}

void Base::Execute::DestroyFacility(uintptr_t* rootAddr, std::vector<Base::SRU_Data::AddressHolder> facilities, int index)
{
	if (facilities[index].addr == *rootAddr)
	{
		for (int a = 0; a < facilities.size(); a++)
		{
			if (a != index)
			{
				*rootAddr = facilities[a].addr;
				break;
			}
		}
	}

	for (int a = 0; a < facilities.size(); a++)
	{
		if (a != index)
		{
			uintptr_t addr = facilities[a].addr;

			//Fix previous
			if (*(uintptr_t*)(addr + 8) == facilities[index].addr)
			{
				if (index > 0)
				{
					*(uintptr_t*)(addr + 8) = facilities[index - 1].addr;
				}
				else
				{
					*(uintptr_t*)(addr + 8) = 0;
				}
			}

			//Fix next
			if (*(uintptr_t*)(addr + 356) == facilities[index].addr)
			{
				if (index < facilities.size() - 1)
				{
					*(uintptr_t*)(addr + 356) = facilities[index + 1].addr;
					*(uintptr_t*)(addr + 4) = facilities[index + 1].addr;
				}
				else
				{
					*(uintptr_t*)(addr + 356) = 0;
					*(uintptr_t*)(addr + 4) = 0;
				}
			}
		}
	}

	destroyFactoryFunc(facilities[index].addr, 0, 0);
}

void Base::Execute::ExecDipl(DWORD* buffer, char c)
{
	Base::SRU_Data::Asm::g_ownAllocs.push_back((uintptr_t)buffer);
	diplFunc(buffer, c);
}

void Base::Execute::AnnexCountry(int from, int to)
{
	int* surrender = Offsets::CreateSimpleDiplOffer(Base::SRU_Data::g_base, Offsets::surrenderDiplo, 0, from, to, 0, 0, 0);

	Base::Execute::ExecDipl((DWORD*)surrender, '\x01');
}

void Base::Execute::AnnexAllColonies(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	uint16_t current = *(uint16_t*)(cc->base + 0x8);

	for (int i = 0; i < g_countryList.size(); i++)
	{
		uint16_t owner = *(uint16_t*)(g_countryList[i].base + 0x46);

		if (owner > 0)
		{
			if (owner == current)
			{
				AnnexCountry(g_countryList[i].oId, cc->oId);
			}
		}
	}
}

void Base::Execute::RespawnCountry(int from, int to, int type)
{
	int* buffer = Offsets::CreateAdvancedDiplOffer(Base::SRU_Data::g_base, Offsets::respawnCountry, to, 0, 0, 0, type, from, 0, 0, 0, 0);
	ExecDipl((DWORD*)buffer, '\x01');
}

void Base::Execute::RespawnAllColonies(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	uint16_t current = *(uint16_t*)(cc->base + 0x8);

	for (int i = 0; i < g_countryList.size(); i++)
	{
		uint16_t owner = *(uint16_t*)(g_countryList[i].base + 0x46);

		if (owner > 0)
		{
			if (owner == current)
			{
				RespawnCountry(g_countryList[i].oId, cc->oId, 1);
			}
		}
	}
}

void Base::Execute::SetCountryAIStance(Base::SRU_Data::Country* cc, int newAIStance)
{
	uint8_t* stancePtr = (uint8_t*)(cc->base + Offsets::countryAIStance);
	*stancePtr = (uint8_t)newAIStance;

	int v5282 = 0;
	int v1733 = v5282;
	int v1732 = v5282;
	do
	{
		uint8_t v5283;
		if (*stancePtr)
		{
			int v4807 = 100;
			int v4808 = (45 * *stancePtr) - 35;
			if (v4808 <= 100)
				v5283 = v4808;
			else
				v5283 = v4807;
		}
		else
		{
			v5283 = 80;
		}
		uintptr_t v1731 = cc->base;
		v1732 = v5282;
		v1733 = v5283;
		*(uint8_t*)(cc->base + v5282 + 0x444) = v5283;
		*(uint8_t*)(cc->base + v1732 + 0x4128) = v1733;
		v5282 = v1732 + 1;
	} while (v1732 + 1 < 3);
}

void Base::Execute::UnlockTech(int to, int tech, bool lock)
{
	using namespace Base::SRU_Data;

	Country* cc = nullptr;
	for (int i = 0; i < g_countryList.size(); i++)
	{
		if (g_countryList[i].oId == to)
		{
			cc = &g_countryList[i];
			break;
		}
	}

	if (!lock)
	{
		//unlock

		unsigned __int8* buffer = (unsigned __int8*)calloc(16, sizeof(unsigned __int8));
		buffer[4] = to;

		

		//unlocks tech requirements recursive
		unlockTechFunc(buffer, tech);
		free(buffer);
	}
	else
	{
		//lock

		uintptr_t techStart = *(uintptr_t*)(g_base + Offsets::techIdStart);
		int v3 = 88 * tech;

		int main = (v3 + techStart + 0x38);
		//int mini = (techStart + v3);

		int countryIdShiftedR = to >> 5;
		//int countryIdShiftedL = 1 << to;

		*(int*)(main + countryIdShiftedR * 4) &= ~(1 << to);
	}

	if (cc != nullptr)
	{
		cc->RefreshResearch();
	}
}

void Base::Execute::UnlockDesign(int to, int design, bool lock)
{
	using namespace Base::SRU_Data;

	//get unit tech requirements & unlock them (if unlock)

	if (!lock)
	{
		uintptr_t defaultStart = *(uintptr_t*)(g_base + Offsets::allDefaultUnitStart);
		int designShifted = design << 8;
		uintptr_t prereAddr = (defaultStart + designShifted + 0x28);
		int value = *(int*)prereAddr;

		if (LOWORD(value) > 0)
		{
			UnlockTech(to, LOWORD(value), false);
		}
		if (HIWORD(value) > 0)
		{
			UnlockTech(to, HIWORD(value), false);
		}
	}

	//(un)lock unit

	std::shared_ptr<UnitDefault> ud;

	for (int i = 0; i < g_defaultUnitList.size(); i++)
	{
		if (g_defaultUnitList[i]->spawnId == design)
		{
			ud = g_defaultUnitList[i];
			break;
		}
	}

	int base = ud->base;
	int countryIdShifted = to >> 5;
	int unitAddr2 = base + 196;

	int* addr = (int*)(unitAddr2 + countryIdShifted * 4);

	if (!lock)
	{
		//Unlock

		int result = 1 << to;
		int currentVal = *addr;

		int testVal = currentVal | result;

		bool found = false;
		for (int i = 0; i < ud->countryIds.size(); i++)
		{
			if (ud->countryIds[i] == to)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			ud->countryIds.push_back(to);
		}

		if (testVal > currentVal)
		{
			*addr = result | *addr;
		}
	}
	else
	{
		//Lock

		*addr &= ~(1 << to);

		int search = -1;
		for (int i = 0; i < ud->countryIds.size(); i++)
		{
			if (ud->countryIds[i] == to)
			{
				search = i;
				break;
			}
		}

		if (search > -1)
		{
			ud->countryIds.erase(ud->countryIds.begin() + search);
		}
	}
}

void Base::Execute::CreateTransport(int from, int to, int type, int noConstruction)
{
	createTransportFuncEvent(type, 0, from, to, noConstruction, 0);
}

void Base::Execute::CreateTransport(__int16 fromX, __int16 fromY, __int16 toX, __int16 toY, int country, int type, int noConstruction)
{
	int fromPosData = (int)MAKELONG(fromX, fromY);
	int toPosData = (int)MAKELONG(toX, toY);

	CreateTransport(fromPosData, toPosData, type, noConstruction);
}

void Base::Execute::SetRelations(int relationType, uintptr_t country, uintptr_t oCountry, int add)
{

	int offset = Offsets::countryDiplRatings;
	if (relationType == 1)
	{
		offset = Offsets::countryDiplCivRatings;
	}
	else if (relationType == 2)
	{
		offset = Offsets::countryDiplCasusBelli;
	}

	byte oId = *(uintptr_t*)(oCountry + Offsets::countryOtherId);
	float* relationAddr = (float*)(country + oId * 4 + offset);

	if (add)
	{
		*relationAddr += 0.25f;
	}
	else
	{
		*relationAddr -= 0.25f;
	}

	//std::cout << std::hex << relationAddr << " " << relationType << std::endl;
}

void Base::Execute::SetMapResource(uint8_t* byte, int resource, int level)
{
	// Shift the resource level by 2 bits to the left (multiply by 4)
	int resource_level = level << (2 * resource);
	// Clear the existing resource level in the byte
	int mask = ~(0x3 << (2 * resource));
	*byte &= mask;
	// Set the new resource level in the byte
	if (level > 0)
	{
		*byte |= resource_level;
	}
}

int Base::Execute::GetMapResource(uint8_t* byte, int resource)
{
	return (*byte >> (2 * resource)) & 0x3;
}

void Base::Execute::SpawnUnit(int unitDesign, int amount, uintptr_t country, int spread, bool reserve, uint16_t xPos, uint16_t yPos)
{
	//std::cout << std::dec << unitDesign << " " << amount << std::hex << " " << country << " " << std::dec << spread << " " << reserve << " " << xPos << " " << yPos << std::endl;

	int minAm = 512;

	if (amount >= minAm)
	{
		minAm += 4 * amount;
	}

	const uint32_t posData = ((yPos << 16) | ((xPos) & 0xffff));

	if (reserve)
	{
		int* buffer = (int*)malloc(minAm * sizeof(int));
		buffer[0] = 0x20300;
		buffer[1] = 14;
		buffer[2] = 0;
		buffer[3] = 0x70000;
		buffer[4] = 0x20300;
		buffer[5] = 14;

		for (int i = 6; i < minAm; i++)
		{
			buffer[i] = 0;
		}

		int* buffer2 = (int*)malloc(minAm * sizeof(int));

		for (int i = 0; i < minAm; i++)
		{
			buffer2[i] = 0;
		}

		spawnUnitFunc((int)country, (int)buffer, unitDesign, amount, posData, 1, 0, buffer2, 0);
		free(buffer);
		free(buffer2);
	}
	else
	{
		int* buffer2 = (int*)malloc(minAm * sizeof(int));

		for (int i = 0; i < minAm; i++)
		{
			buffer2[i] = 0;
		}

		if (spread > amount)
		{
			spread = amount;
		}

		//force disable
		spread = 1;
		//std::cout << std::hex << country << " " << unitDesign << std::endl;

		if (spread == 1)
		{
			spawnUnitFunc((int)country, (int)xPos, unitDesign, amount, posData, 0, 0, buffer2, 1);
		}
		else
		{
			//Spread units evenly around center point

			
		}

		free(buffer2);
	}
}

void Base::Execute::SetCheat(uint8_t cheat)
{
	using namespace Base::SRU_Data;

	uint8_t* cheatBase = (uint8_t*)(uintptr_t*)(g_base + Offsets::cheatAddr);

	if (cheat == Offsets::allunit[0])
	{
		*cheatBase = ~*cheatBase & Offsets::allunit[0] | *cheatBase & Offsets::allunit[1];
	}
	else if (cheat == Offsets::onedaybuild[0])
	{
		*cheatBase = ~*cheatBase & Offsets::onedaybuild[0] | *cheatBase & Offsets::onedaybuild[1];
	}
	else if (cheat == Offsets::breakground[0])
	{
		*cheatBase = ~*cheatBase & Offsets::breakground[0] | *cheatBase & Offsets::breakground[1];
	}
	else if (cheat == Offsets::moreoffers[0])
	{
		*cheatBase = ~*cheatBase & Offsets::moreoffers[0] | *cheatBase & Offsets::moreoffers[1];
	}
}

int Base::Execute::ExecuteTreaty(int diplTreatyIndex, int set)
{
	using namespace Base::SRU_Data;

	DiplTreaty treaty = g_diplTreatyList[diplTreatyIndex];

	int from = clickedCountry->oId;
	int to = g_countryList[g_selectedTargetCountry].oId;

	if (from == to)
	{
		return -1;
	}

	int diplType = Offsets::defaultDiplo;

	/*if (treaty.treatyId == -1)
	{
		//War

		diplType = Offsets::warDiplo;
	}
	else {
		if (set == 0) {*/
	treatyFunc(clickedCountry->base, 0, treaty.treatyId, set, g_countryList[g_selectedTargetCountry].id, 1, 0, 1, 0);
	treatyFunc(clickedCountry->base, 1, treaty.treatyId, set, g_countryList[g_selectedTargetCountry].id, 1, 0, 1, 0);
	treatyFunc(g_countryList[g_selectedTargetCountry].base, 0, treaty.treatyId, set, clickedCountry->id, 1, 0, 1, 0);
	treatyFunc(g_countryList[g_selectedTargetCountry].base, 1, treaty.treatyId, set, clickedCountry->id, 1, 0, 1, 0);
	return 1;
	//	}
	//}

	long currentDay = *(uintptr_t*)(g_base + Offsets::currentDay);
	int currentTime = *(uintptr_t*)(g_base + Offsets::currentDayTime);
	long expiryDay = currentDay + 7;

	int* diplOffer = 
		Offsets::CreateSimpleDiplOffer(g_base, diplType, treaty.treatyId, from, to, currentDay, currentTime, expiryDay);

	ExecDipl((DWORD*)diplOffer, '\x01');

	return 1;
}

bool Base::Execute::HasTechUnlocked(int countryId, int techId)
{
	using namespace Base::SRU_Data;

	uintptr_t techStart = *(uintptr_t*)(g_base + Offsets::techIdStart);
	uint32_t v3 = 88 * techId;

	uintptr_t main = (v3 + techStart + 0x38);
	//int mini = (techStart + v3);

	uint32_t countryIdShiftedR = countryId >> 5;
	uint32_t countryIdShiftedL = 1 << countryId;

	uintptr_t* addr = (uintptr_t*)(main + countryIdShiftedR * 4);

	if (addr == nullptr)
	{
		return false;
	}

	uint32_t currentVal = *addr;
	uint32_t testVal = currentVal | countryIdShiftedL;

	if (testVal > currentVal)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Base::Execute::HasDesignUnlocked(int countryId, int designId)
{
	return false;
}

bool Base::Execute::HasDesignUnlocked(int countryId, uintptr_t base)
{
	using namespace Base::SRU_Data;

	//std::cout << std::hex << base << std::dec << " " << countryId << std::endl;

	uint32_t countryIdShiftedR = countryId >> 5;
	uint32_t countryIdShiftedL = 1 << countryId;
	
	uintptr_t unitAddr2 = base + 196;

	uintptr_t* addr = (uintptr_t*)(unitAddr2 + countryIdShiftedR * 4);

	if (addr == nullptr) return false;

	uint32_t currentVal = *addr;
	uint32_t testVal = currentVal | countryIdShiftedL;

	if (testVal > currentVal)
	{
		return false;
	}
	else
	{
		return true;
	}
}