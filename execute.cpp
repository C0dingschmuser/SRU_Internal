#include "base.h"

Base::Execute::_DiplFunc Base::Execute::diplFunc = 0;
Base::Execute::_UnlockTechFunc Base::Execute::unlockTechFunc = 0;
Base::Execute::_SpawnUnitFunc Base::Execute::spawnUnitFunc = 0;

void Base::Execute::SetupFunctions()
{
	using namespace Base::Execute;

	diplFunc = (_DiplFunc)(Base::SRU_Data::g_base + Offsets::diplFunc);
	unlockTechFunc = (_UnlockTechFunc)(Base::SRU_Data::g_base + Offsets::unlockTechFunc);
	spawnUnitFunc = (_SpawnUnitFunc)(Base::SRU_Data::g_base + Offsets::unitFunc);
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

void Base::Execute::RespawnCountry(int from, int to, int type)
{
	int* buffer = Offsets::CreateAdvancedDiplOffer(Base::SRU_Data::g_base, Offsets::respawnCountry, to, 0, 0, 0, type, from, 0, 0, 0, 0);
	ExecDipl((DWORD*)buffer, '\x01');
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

		*(int*)(main + countryIdShiftedR * 4) = 0;
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

		*addr = 0;

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

int Base::Execute::ExecuteTreaty(int diplTreatyIndex)
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

	if (treaty.treatyId == -1)
	{
		//War

		diplType = Offsets::warDiplo;
	}

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

	uint32_t currentVal = *(uintptr_t*)(main + countryIdShiftedR * 4);
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