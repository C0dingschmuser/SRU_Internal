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

	int max = (int)UnitDefault::Property::MAX;
	for (int i = 0; i < max; i++)
	{
		std::shared_ptr<ChangeHolder> change(new ChangeHolder);
		change->p = (Property)i;
		propertyChanges.push_back(change);
	}
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

	int id = *(uintptr_t*)(base + Offsets::unitId);
	uint8_t id2 = *(uint8_t*)(defaultStats->base + 0x4);

	//std::cout << std::dec << id << std::hex << " " << (int)id2 << " " << defaultStats->base << " " << defaultStats->name << std::endl;
}

void Base::SRU_Data::Unit::SetDesignProperty(Country* c, UnitDefault::Property p, uint16_t v)
{
	std::shared_ptr<UnitDefault::ChangeHolder> change = 
		this->defaultStats->propertyChanges[(int)p];

	if (change->change > 0)
	{
		//already changed, check if new value
		if (change->val == v)
		{
			return;
		}
	}

	change->change++;
	change->val = v;

	if (this->defaultStats->customDefault == nullptr)
	{
		//Create new fake default unit and override

		uintptr_t* buffer = (uintptr_t*)malloc(sizeof(int) * 256);

		memcpy((void*)buffer, (void*)this->defaultStats->base, sizeof(int) * 256);

		this->defaultStats->customDefault = buffer;

		for (int i = 0; i < c->allUnits.size(); i++)
		{
			if (c->allUnits[i].defaultStats->base == this->defaultStats->base)
			{
				*(int*)(c->allUnits[i].base + Offsets::unitDefaultValues) =
					(int)buffer;
			}
		}

		//*(int*)(base + Offsets::unitDefaultValues) = (int)buffer;
	}

	uintptr_t dBase = (uintptr_t)this->defaultStats->customDefault;

	switch (p)
	{
	case UnitDefault::Property::MoveSpeed:
		*(uint16_t*)(dBase + Offsets::unitDefaultMoveSpeed) = v;
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

void Base::SRU_Data::Unit::RestoreDesignProperty(Country* c, UnitDefault::Property p)
{
	//check if already applied
	//-> if yes, return
	//Check if any other default stat is modified
	//-> if yes, reset only named one to default values
	//-> if no, restore orig pointer and set ptr to null

	std::shared_ptr<UnitDefault::ChangeHolder> change =
		this->defaultStats->propertyChanges[(int)p];

	if (change->change == 0)
	{
		//already reset
		return;
	}

	change->change = 0;

	bool full = true;
	int max = (int)UnitDefault::Property::MAX;

	for (int i = 0; i < max; i++)
	{
		if (this->defaultStats->propertyChanges[i]->change > 0)
		{
			//another variable is still changed -> no full reset
			full = false;
			break;
		}
	}

	if (full)
	{
		for (int i = 0; i < c->allUnits.size(); i++)
		{
			if (c->allUnits[i].defaultStats->base ==
				this->defaultStats->base)
			{
				*(int*)(c->allUnits[i].base + Offsets::unitDefaultValues) =
					(int)this->defaultStats->base;
			}
		}


		this->defaultStats->customDefault = nullptr;
		//*(uintptr_t*)(base + Offsets::unitDefaultValues) = this->defaultStats->base;
		return;
	}

	switch (p)
	{
	case UnitDefault::Property::MoveSpeed:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultMoveSpeed) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultMoveSpeed);
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

bool Base::SRU_Data::IsValidUnit(uintptr_t base)
{
	uintptr_t unitDefault = *(uintptr_t*)(base + Offsets::unitDefaultValues);

	if (Base::Utils::CanReadPtr((uintptr_t*)unitDefault))
	{
		return IsValidDefaultUnit(unitDefault);
	}

	return false;
}

bool Base::SRU_Data::IsValidDefaultUnit(uintptr_t base)
{
	uint8_t unitClass = *(uint8_t*)(base + Offsets::unitDefaultClass);

	if (unitClass != 0x15)
	{
		return true;
	}

	return false;
}