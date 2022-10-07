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

	this->countryId = (uint8_t*)(base + Offsets::unitCountry);
	this->oldCountry = *countryId;

	this->deployedState = (uintptr_t*)(base + Offsets::unitDeployedState);

	uintptr_t defaultBase = (uintptr_t) * (uintptr_t*)(base + Offsets::unitDefaultValues);
	this->defaultStats = Base::SRU_Data::FindUnitDefault(defaultBase, *this->countryId);
	this->defaultStats->AddUserCountry(*this->countryId);

	std::shared_ptr<FloatValue> health(new FloatValue);
	health->valPtr = (float*)(base + Offsets::unitHealth);
	this->health = health;

	std::shared_ptr<FloatValue> maxHealth(new FloatValue);
	maxHealth->valPtr = (float*)(base + Offsets::unitHealthMax);
	this->maxHealth = maxHealth;

	std::shared_ptr<FloatValue> fuel(new FloatValue);
	fuel->valPtr = (float*)(base + Offsets::unitFuel);
	fuel->origVal = 
		*this->maxHealth->valPtr * *this->defaultStats->fuelCapacity->valPtr;
	this->fuel = fuel;

	std::shared_ptr<FloatValue> supply(new FloatValue);
	supply->valPtr = (float*)(base + Offsets::unitSupply);
	supply->origVal = 
		*this->maxHealth->valPtr * *this->defaultStats->supplyCapacity->valPtr;
	this->supply = supply;

	std::shared_ptr<FloatValue> experience(new FloatValue);
	experience->valPtr = (float*)(base + Offsets::unitExperience);
	this->experience = experience;

	std::shared_ptr<FloatValue> morale(new FloatValue);
	morale->valPtr = (float*)(base + Offsets::unitMorale);
	this->morale = morale;
}

void Base::SRU_Data::Unit::SetDesignProperty(UnitDefault::Property p, uint16_t v)
{
	switch (p)
	{
	case UnitDefault::Property::MoveSpeed:
		this->defaultStats->moveSpeed->OverrideVal(v);
		break;
	case UnitDefault::Property::Spotting:
		this->defaultStats->spotting->OverrideVal(v);
		break;
	case UnitDefault::Property::MoveRange:
		this->defaultStats->moveRange->OverrideVal(v);
		break;
	case UnitDefault::Property::SoftGroundAttack:
		this->defaultStats->softGroundAttack->OverrideVal(v);
		break;
	case UnitDefault::Property::HardGroundAttack:
		this->defaultStats->hardGroundAttack->OverrideVal(v);
		break;
	case UnitDefault::Property::CloseGroundAttack:
		this->defaultStats->closeGroundAttack->OverrideVal(v);
		break;
	case UnitDefault::Property::CloseAirAttack:
		this->defaultStats->closeAirAttack->OverrideVal(v);
		break;
	case UnitDefault::Property::MidAirAttack:
		this->defaultStats->midAirAttack->OverrideVal(v);
		break;
	case UnitDefault::Property::HighAirAttack:
		this->defaultStats->highAirAttack->OverrideVal(v);
		break;
	case UnitDefault::Property::GroundRange:
		this->defaultStats->groundRange->OverrideVal(v);
		break;
	case UnitDefault::Property::NavalRange:
		this->defaultStats->navalRange->OverrideVal(v);
		break;
	case UnitDefault::Property::AirRange:
		this->defaultStats->airRange->OverrideVal(v);
		break;
	case UnitDefault::Property::FuelCapacity:
		this->defaultStats->fuelCapacity->OverrideVal(v);
		break;
	case UnitDefault::Property::SupplyCapacity:
		this->defaultStats->supplyCapacity->OverrideVal(v);
		break;
	case UnitDefault::Property::BuildTime:
		this->defaultStats->buildTime->OverrideVal(v);
		break;
	}
}

void Base::SRU_Data::Unit::RestoreDesignProperty(UnitDefault::Property p)
{
	switch (p)
	{
	case UnitDefault::Property::MoveSpeed:
		this->defaultStats->moveSpeed->RestoreVal();
		break;
	case UnitDefault::Property::Spotting:
		this->defaultStats->spotting->RestoreVal();
		break;
	case UnitDefault::Property::MoveRange:
		this->defaultStats->moveRange->RestoreVal();
		break;
	case UnitDefault::Property::SoftGroundAttack:
		this->defaultStats->softGroundAttack->RestoreVal();
		break;
	case UnitDefault::Property::HardGroundAttack:
		this->defaultStats->hardGroundAttack->RestoreVal();
		break;
	case UnitDefault::Property::CloseGroundAttack:
		this->defaultStats->closeGroundAttack->RestoreVal();
		break;
	case UnitDefault::Property::CloseAirAttack:
		this->defaultStats->closeAirAttack->RestoreVal();
		break;
	case UnitDefault::Property::MidAirAttack:
		this->defaultStats->midAirAttack->RestoreVal();
		break;
	case UnitDefault::Property::HighAirAttack:
		this->defaultStats->highAirAttack->RestoreVal();
		break;
	case UnitDefault::Property::GroundRange:
		this->defaultStats->groundRange->RestoreVal();
		break;
	case UnitDefault::Property::NavalRange:
		this->defaultStats->navalRange->RestoreVal();
		break;
	case UnitDefault::Property::AirRange:
		this->defaultStats->airRange->RestoreVal();
		break;
	case UnitDefault::Property::FuelCapacity:
		this->defaultStats->fuelCapacity->RestoreVal();
		break;
	case UnitDefault::Property::SupplyCapacity:
		this->defaultStats->supplyCapacity->RestoreVal();
		break;
	case UnitDefault::Property::BuildTime:
		this->defaultStats->buildTime->RestoreVal();
		break;
	}
}