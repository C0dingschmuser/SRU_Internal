#include "../Base.h"

using namespace Base::SRU_Data;

void Base::SRU_Data::UnitDefault::RefreshUserCountrys()
{
	bool ok = false;

	this->countryIds.clear();
	for (int i = 0; i < g_countryList.size(); i++)
	{
		if (Base::Execute::HasDesignUnlocked(g_countryList[i].oId, this->base))
		{
			this->countryIds.push_back(g_countryList[i].oId);
		}
	}
}

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

	this->unitClass = *(uint8_t*)(base + Offsets::unitDefaultClass);
	this->unitOrigin = *(uintptr_t*)(base + Offsets::unitDefaultOrigin);

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

	//Load origin countries

	this->originCountryIds.clear();
	for (int i = 0; i < g_countryList.size(); i++)
	{
		int ownData = *(uintptr_t*)(g_countryList[i].base + 0x88);

		if ((this->unitOrigin & ownData) != 0) {

			this->originCountryIds.push_back(g_countryList[i].oId);
		}
	}

	//Load all user countries (unlocked)

	RefreshUserCountrys();
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

bool Base::SRU_Data::UnitDefault::HasOrigin(int countryId)
{
	bool contains = false;

	for (int i = 0; i < this->originCountryIds.size(); i++) {
		if (this->originCountryIds[i] == countryId)
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

bool Base::SRU_Data::Unit::Init(uintptr_t base)
{
	if ((uintptr_t*)base == nullptr)
	{
		return false;
	}

	this->base = base;

	this->countryId = (uint8_t*)(base + Offsets::unitCountry);
	this->oldCountry = *countryId;

	this->xPos = (uint16_t*)(base + Offsets::unitXPos);
	this->yPos = (uint16_t*)(base + Offsets::unitYPos);

	this->deployedState = (uintptr_t*)(base + Offsets::unitDeployedState);

	uintptr_t defaultBase = (uintptr_t) * (uintptr_t*)(base + Offsets::unitDefaultValues);

	this->defaultStats = Base::SRU_Data::FindUnitDefault(defaultBase, *this->countryId);
	//this->defaultStats->AddUserCountry(*this->countryId);

	if (this->defaultStats->customDefault != nullptr)
	{
		//Country modifier active, apply changes

		*(uintptr_t*)(base + Offsets::unitDefaultValues) =
			(uintptr_t)this->defaultStats->customDefault;
	}

	std::shared_ptr<FloatValue> health(new FloatValue);
	health->valPtr = (float*)(base + Offsets::unitHealth);
	this->properties.push_back(health);
	
	std::shared_ptr<FloatValue> maxHealth(new FloatValue);
	maxHealth->valPtr = (float*)(base + Offsets::unitHealthMax);
	this->properties.push_back(maxHealth);

	health->origVal = *maxHealth->valPtr;

	std::shared_ptr<FloatValue> fuel(new FloatValue);
	fuel->valPtr = (float*)(base + Offsets::unitFuel);
	if (this->properties[(int)Property::MaxHalth]->valPtr != nullptr && this->defaultStats->fuelCapacity->valPtr != nullptr)
	{
		fuel->origVal =
			*this->properties[(int)Property::MaxHalth]->valPtr * *this->defaultStats->fuelCapacity->valPtr;
	}

	this->properties.push_back(fuel);

	std::shared_ptr<FloatValue> supply(new FloatValue);
	supply->valPtr = (float*)(base + Offsets::unitSupply);
	if (this->properties[(int)Property::MaxHalth]->valPtr != nullptr && this->defaultStats->supplyCapacity->valPtr != nullptr)
	{
		supply->origVal =
			*this->properties[(int)Property::MaxHalth]->valPtr * *this->defaultStats->supplyCapacity->valPtr;
	}

	this->properties.push_back(supply);

	std::shared_ptr<FloatValue> efficiency(new FloatValue);
	efficiency->valPtr = (float*)(base + Offsets::unitEfficiency);
	efficiency->origVal = 1.5f;
	this->properties.push_back(efficiency);

	std::shared_ptr<FloatValue> experience(new FloatValue);
	experience->valPtr = (float*)(base + Offsets::unitExperience);
	experience->origVal = 5.0f;
	this->properties.push_back(experience);

	std::shared_ptr<FloatValue> morale(new FloatValue);
	morale->valPtr = (float*)(base + Offsets::unitMorale);
	morale->origVal = 1.0f;
	this->properties.push_back(morale);

	return true;
}

void Base::SRU_Data::Unit::SetDesignPropertySimple(Country* c, UnitDefault::Property p, int range)
{
	UnitDefault::HolderValue v;

	float base = 1;

	switch (p)
	{
	case UnitDefault::Property::MoveSpeed:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultMoveSpeed);
		break;
	case UnitDefault::Property::Spotting:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultSpotting);
		break;
	case UnitDefault::Property::MoveRange:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultMoveRange);
		break;
	case UnitDefault::Property::SoftGroundAttack:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultSoftGroundAttack);
		break;
	case UnitDefault::Property::HardGroundAttack:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultHardGroundAttack);
		break;
	case UnitDefault::Property::CloseGroundAttack:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultCloseGroundAttack);
		break;
	case UnitDefault::Property::CloseAirAttack:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultCloseAirAttack);
		break;
	case UnitDefault::Property::MidAirAttack:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultMidAirAttack);
		break;
	case UnitDefault::Property::HighAirAttack:
		base = (float)*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultHighAirAttack);
		break;
	case UnitDefault::Property::GroundRange:
		base = *(float*)(this->defaultStats->base + Offsets::unitDefaultGroundRange);
		break;
	case UnitDefault::Property::NavalRange:
		base = *(float*)(this->defaultStats->base + Offsets::unitDefaultNavalRange);
		break;
	case UnitDefault::Property::AirRange:
		base = *(float*)(this->defaultStats->base + Offsets::unitDefaultAirRange);
		break;
	case UnitDefault::Property::FuelCapacity:
		base = *(float*)(this->defaultStats->base + Offsets::unitDefaultFuelCapacity);
		break;
	case UnitDefault::Property::SupplyCapacity:
		base = *(float*)(this->defaultStats->base + Offsets::unitDefaultSupplyCapacity);
		break;
	case UnitDefault::Property::BuildTime:
		base = *(float*)(this->defaultStats->base + Offsets::unitDefaultBuildTime);
		break;
	}

	switch (range)
	{
	case -3: //Minimum
		base *= 0.1f;
		break;
	case -2: //-50%
		base *= 0.5f;
		break;
	case -1: //-25%
		base *= 0.75f;
		break;
	case 1: //+25%
		base *= 1.25f;
		break;
	case 2: //+50%
		base *= 1.5f;
		break;
	case 3: //Maximum
		base = 15000;
		break;
	}

	if (base < 0)
	{
		base = 0.0001f;
	}

	switch (p)
	{
	default:
		v.ui16 = (uint16_t)base;
		break;
	case UnitDefault::Property::FuelCapacity:
	case UnitDefault::Property::SupplyCapacity:
	case UnitDefault::Property::GroundRange:
	case UnitDefault::Property::NavalRange:
	case UnitDefault::Property::AirRange:
	case UnitDefault::Property::BuildTime:
		v.f = base;
		break;
	}

	SetDesignProperty(c, p, v);
}

void Base::SRU_Data::Unit::SetDesignProperty(Country* c, UnitDefault::Property p, UnitDefault::HolderValue v)
{
	std::shared_ptr<UnitDefault::ChangeHolder> change = 
		this->defaultStats->propertyChanges[(int)p];

	if (change->change > 0)
	{
		//already changed, check if new value
		
		switch (p)
		{
		default:
			if (change->val.ui16 == v.ui16)
			{
				return;
			}
			break;
		case UnitDefault::Property::FuelCapacity:
		case UnitDefault::Property::SupplyCapacity:
		case UnitDefault::Property::GroundRange:
		case UnitDefault::Property::NavalRange:
		case UnitDefault::Property::AirRange:
		case UnitDefault::Property::BuildTime:
			if (Base::Utils::CMPF(change->val.f, v.f))
			{
				return;
			}
			break;
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
	}

	uintptr_t dBase = (uintptr_t)this->defaultStats->customDefault;

	switch (p)
	{
	case UnitDefault::Property::MoveSpeed:
		*(uint16_t*)(dBase + Offsets::unitDefaultMoveSpeed) = v.ui16;
		break;
	case UnitDefault::Property::Spotting:
		*(uint16_t*)(dBase + Offsets::unitDefaultSpotting) = v.ui16;
		break;
	case UnitDefault::Property::MoveRange:
		*(uint16_t*)(dBase + Offsets::unitDefaultMoveRange) = v.ui16;
		break;
	case UnitDefault::Property::SoftGroundAttack:
		*(uint16_t*)(dBase + Offsets::unitDefaultSoftGroundAttack) = v.ui16;
		break;
	case UnitDefault::Property::HardGroundAttack:
		*(uint16_t*)(dBase + Offsets::unitDefaultHardGroundAttack) = v.ui16;
		break;
	case UnitDefault::Property::CloseGroundAttack:
		*(uint16_t*)(dBase + Offsets::unitDefaultCloseGroundAttack) = v.ui16;
		break;
	case UnitDefault::Property::CloseAirAttack:
		*(uint16_t*)(dBase + Offsets::unitDefaultCloseAirAttack) = v.ui16;
		break;
	case UnitDefault::Property::MidAirAttack:
		*(uint16_t*)(dBase + Offsets::unitDefaultMidAirAttack) = v.ui16;
		break;
	case UnitDefault::Property::HighAirAttack:
		*(uint16_t*)(dBase + Offsets::unitDefaultHighAirAttack) = v.ui16;
		break;
	case UnitDefault::Property::GroundRange:
		*(float*)(dBase + Offsets::unitDefaultGroundRange) = v.f;
		break;
	case UnitDefault::Property::NavalRange:
		*(float*)(dBase + Offsets::unitDefaultNavalRange) = v.f;
		break;
	case UnitDefault::Property::AirRange:
		*(float*)(dBase + Offsets::unitDefaultAirRange) = v.f;
		break;
	case UnitDefault::Property::FuelCapacity:
		*(float*)(dBase + Offsets::unitDefaultFuelCapacity) = v.f;
		break;
	case UnitDefault::Property::SupplyCapacity:
		*(float*)(dBase + Offsets::unitDefaultSupplyCapacity) = v.f;
		break;
	case UnitDefault::Property::BuildTime:
		*(float*)(dBase + Offsets::unitDefaultBuildTime) = v.f;
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

		free(this->defaultStats->customDefault);
		this->defaultStats->customDefault = nullptr;
		return;
	}

	switch (p)
	{
	case UnitDefault::Property::MoveSpeed:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultMoveSpeed) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultMoveSpeed);
		break;
	case UnitDefault::Property::Spotting:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultSpotting) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultSpotting);
		break;
	case UnitDefault::Property::MoveRange:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultMoveRange) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultMoveRange);
		break;
	case UnitDefault::Property::SoftGroundAttack:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultSoftGroundAttack) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultSoftGroundAttack);
		break;
	case UnitDefault::Property::HardGroundAttack:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultHardGroundAttack) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultHardGroundAttack);
		break;
	case UnitDefault::Property::CloseGroundAttack:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultCloseGroundAttack) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultCloseGroundAttack);
		break;
	case UnitDefault::Property::CloseAirAttack:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultCloseAirAttack) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultCloseAirAttack);
		break;
	case UnitDefault::Property::MidAirAttack:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultMidAirAttack) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultMidAirAttack);
		break;
	case UnitDefault::Property::HighAirAttack:
		*(uint16_t*)(this->defaultStats->customDefault + Offsets::unitDefaultHighAirAttack) =
			*(uint16_t*)(this->defaultStats->base + Offsets::unitDefaultHighAirAttack);
		break;
	case UnitDefault::Property::GroundRange:
		*(float*)(this->defaultStats->customDefault + Offsets::unitDefaultGroundRange) =
			*(float*)(this->defaultStats->base + Offsets::unitDefaultGroundRange);
		break;
	case UnitDefault::Property::NavalRange:
		*(float*)(this->defaultStats->customDefault + Offsets::unitDefaultNavalRange) =
			*(float*)(this->defaultStats->base + Offsets::unitDefaultNavalRange);
		break;
	case UnitDefault::Property::AirRange:
		*(float*)(this->defaultStats->customDefault + Offsets::unitDefaultAirRange) =
			*(float*)(this->defaultStats->base + Offsets::unitDefaultAirRange);
		break;
	case UnitDefault::Property::FuelCapacity:
		*(float*)(this->defaultStats->customDefault + Offsets::unitDefaultFuelCapacity) =
			*(float*)(this->defaultStats->base + Offsets::unitDefaultFuelCapacity);
		break;
	case UnitDefault::Property::SupplyCapacity:
		*(float*)(this->defaultStats->customDefault + Offsets::unitDefaultSupplyCapacity) =
			*(float*)(this->defaultStats->base + Offsets::unitDefaultSupplyCapacity);
		break;
	case UnitDefault::Property::BuildTime:
		*(float*)(this->defaultStats->customDefault + Offsets::unitDefaultBuildTime) =
			*(float*)(this->defaultStats->base + Offsets::unitDefaultBuildTime);
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