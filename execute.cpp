#include "base.h"

Base::Execute::_DiplFunc Base::Execute::diplFunc = 0;
Base::Execute::_SpawnUnitFunc Base::Execute::spawnUnitFunc = 0;

void Base::Execute::SetupFunctions()
{
	using namespace Base::Execute;

	diplFunc = (_DiplFunc)(Base::SRU_Data::g_base + Offsets::diplFunc);
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
	int minAm = 512;

	if (amount >= minAm)
	{
		minAm += 4 * amount;
	}

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

		spawnUnitFunc((int)country, (int)buffer, unitDesign, amount, 0x590070, 1, 0, buffer2, 0);
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
			const uint32_t posData = ((yPos << 16) | ((xPos) & 0xffff));

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