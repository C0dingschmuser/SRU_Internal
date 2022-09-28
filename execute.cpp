#include "base.h"

Base::Execute::_DiplFunc Base::Execute::diplFunc = 0;

void Base::Execute::SetupFunctions()
{
	using namespace Base::Execute;

	diplFunc = (_DiplFunc)(Base::SRU_Data::g_base + Offsets::diplFunc);
}

void Base::Execute::AnnexCountry(int from, int to)
{
	int* surrender = Offsets::CreateSimpleDiplOffer(Base::SRU_Data::g_base, Offsets::surrenderDiplo, 0, from, to, 0, 0, 0);

	Base::Execute::diplFunc((void*)surrender, '\x01');
}

void Base::Execute::RespawnCountry(int from, int to, int type)
{
	int* buffer = Offsets::CreateAdvancedDiplOffer(Base::SRU_Data::g_base, Offsets::respawnCountry, to, 0, 0, 0, type, from, 0, 0, 0, 0);
	diplFunc((void*)buffer, '\x01');
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

	std::cout << std::hex << relationAddr << " " << relationType << std::endl;
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

	diplFunc((void*)diplOffer, '\x01');

	return 1;
}