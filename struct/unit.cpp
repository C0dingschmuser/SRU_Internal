#include "../Base.h"

using namespace Base::SRU_Data;

void Base::SRU_Data::UnitDefault::Init(uintptr_t base)
{
	this->base = base;

	if (!base)
	{
		return;
	}

	if (this->spawnId == -1)
	{
		uintptr_t start = *(uintptr_t*)(g_base + Offsets::allDefaultUnitStart);
		this->spawnId = (base - start) / Offsets::unitDefaultNext;
	}

	char* namePtr = (char*)*(uintptr_t*)base;
	this->name = std::string(namePtr);

	std::shared_ptr<IntValue> moveSpeed(new IntValue);
	moveSpeed->valPtr = (uintptr_t*)(base + Offsets::unitDefaultMoveSpeed);
	this->moveSpeed = moveSpeed;

	std::shared_ptr<IntValue> spotting(new IntValue);
	spotting->valPtr = (uintptr_t*)(base + Offsets::unitDefaultSpotting);
	this->spotting = spotting;

	std::shared_ptr<IntValue> moveRange(new IntValue);
	moveRange->valPtr = (uintptr_t*)(base + Offsets::unitDefaultMoveRange);
	this->moveRange = moveRange;

	std::shared_ptr<IntValue> softGroundAttack(new IntValue);
	softGroundAttack->valPtr = (uintptr_t*)(base + Offsets::unitDefaultSoftGroundAttack);
	this->softGroundAttack = softGroundAttack;

	std::shared_ptr<IntValue> hardGroundAttack(new IntValue);
	hardGroundAttack->valPtr = (uintptr_t*)(base + Offsets::unitDefaultHardGroundAttack);
	this->hardGroundAttack = hardGroundAttack;

	std::shared_ptr<IntValue> closeGroundAttack(new IntValue);
	closeGroundAttack->valPtr = (uintptr_t*)(base + Offsets::unitDefaultCloseGroundAttack);
	this->closeGroundAttack = closeGroundAttack;

	std::shared_ptr<IntValue> closeAirAttack(new IntValue);
	closeAirAttack->valPtr = (uintptr_t*)(base + Offsets::unitDefaultCloseAirAttack);
	this->closeAirAttack = closeAirAttack;

	std::shared_ptr<IntValue> midAirAttack(new IntValue);
	midAirAttack->valPtr = (uintptr_t*)(base + Offsets::unitDefaultMidAirAttack);
	this->midAirAttack = midAirAttack;

	std::shared_ptr<IntValue> highAirAttack(new IntValue);
	highAirAttack->valPtr = (uintptr_t*)(base + Offsets::unitDefaultHighAirAttack);
	this->highAirAttack = highAirAttack;

	std::shared_ptr<FloatValue> groundRange(new FloatValue);
	groundRange->valPtr = (float*)(base + Offsets::unitDefaultGroundRange);
	this->groundRange = groundRange;

	std::shared_ptr<FloatValue> navalRange(new FloatValue);
	navalRange->valPtr = (float*)(base + Offsets::unitDefaultNavalRange);
	this->navalRange = navalRange;

	std::shared_ptr<FloatValue> airRange(new FloatValue);
	airRange->valPtr = (float*)(base + Offsets::unitDefaultAirRange);
	this->airRange = airRange;

	std::shared_ptr<FloatValue> fuelCapacity(new FloatValue);
	fuelCapacity->valPtr = (float*)(base + Offsets::unitDefaultFuelCapacity);
	this->fuelCapacity = fuelCapacity;

	std::shared_ptr<FloatValue> supplyCapacity(new FloatValue);
	supplyCapacity->valPtr = (float*)(base + Offsets::unitDefaultSupplyCapacity);
	this->supplyCapacity = supplyCapacity;

	std::shared_ptr<FloatValue> buildTime(new FloatValue);
	buildTime->valPtr = (float*)(base + Offsets::unitDefaultBuildTime);
	this->buildTime = buildTime;
}

bool Base::SRU_Data::UnitDefault::HasUser(int countryId)
{
	bool contains = false;

	for (int i = 0; i < this->countryIds.size(); i++)
	{
		if (this->countryIds[i] == countryId)
		{
			contains = true;
			break;
		}
	}

	return contains;
}

void Base::SRU_Data::UnitDefault::AddUserCountry(int countryId)
{
	bool contains = Base::SRU_Data::UnitDefault::HasUser(countryId);

	if (!contains)
	{
		this->countryIds.push_back(countryId);
	}
}

void Base::SRU_Data::Unit::Init(uintptr_t base)
{
	this->base = base;
	this->currentHex = (uintptr_t*)(base + Offsets::unitCurrentHex);

	uint8_t* country = (uint8_t*)(base + Offsets::unitCountry);
	this->countryID = *country;

	uintptr_t defaultBase = (uintptr_t) * (uintptr_t*)(base + Offsets::unitDefaultValues);
	this->defaultStats = Base::SRU_Data::FindUnitDefault(defaultBase, this->countryID);
	this->defaultStats->AddUserCountry(this->countryID);
}