#include "../Base.h"

void Base::SRU_Data::Country::Init(uintptr_t base)
{
	this->base = base;

	uintptr_t* nameAddr = (uintptr_t*)*(uintptr_t*)(base + Offsets::countryName);

	this->real = false;
	this->name = "Empty";
	if (Base::Utils::CanReadPtr(nameAddr))
	{
		this->real = true;
	}
	else return;

	this->name = std::string((char*)nameAddr);

	byte oId = *(uintptr_t*)(base + Offsets::countryOtherId);
	this->oId = oId;

	this->populationPtr = (uintptr_t*)(base + Offsets::countryPopulation);

	this->alive = 0;
	if (Base::Utils::CanReadPtr(this->populationPtr))
	{
		if (*this->populationPtr > 0)
		{
			this->alive = 1;
		}
	}

	std::shared_ptr<FloatValue> treasury(new FloatValue);
	treasury->valPtr = (float*)(base + Offsets::countryTreasury);
	this->treasury = treasury;
	this->allFloatValues.push_back(treasury);

	std::shared_ptr<FloatValue> domApproval(new FloatValue);
	domApproval->valPtr = (float*)(base + Offsets::countryDomApproval);
	this->domApproval = domApproval;
	this->allFloatValues.push_back(domApproval);

	std::shared_ptr<FloatValue> milApproval(new FloatValue);
	milApproval->valPtr = (float*)(base + Offsets::countryMilApproval);
	this->milApproval = milApproval;
	this->allFloatValues.push_back(milApproval);

	std::shared_ptr<FloatValue> milReserve(new FloatValue);
	milReserve->valPtr = (float*)(base + Offsets::countryMilReserve);
	this->milReserve = milReserve;
	this->allFloatValues.push_back(milReserve);

	std::shared_ptr<FloatValue> researchEff(new FloatValue);
	researchEff->valPtr = (float*)(base + Offsets::countryResearchEff);
	this->researchEff = researchEff;
	this->allFloatValues.push_back(researchEff);

	std::shared_ptr<FloatValue> financeCreditRating(new FloatValue);
	financeCreditRating->valPtr = (float*)(base + Offsets::countryFinanceCreditRating);
	this->financeCreditRating = financeCreditRating;
	this->allFloatValues.push_back(financeCreditRating);

	std::shared_ptr<FloatValue> worldMarketOpinion(new FloatValue);
	worldMarketOpinion->valPtr = (float*)(base + Offsets::countryWorldMarketOpinion);
	this->worldMarketOpinion = worldMarketOpinion;
	this->allFloatValues.push_back(worldMarketOpinion);

	std::shared_ptr<FloatValue> treatyIntegrity(new FloatValue);
	treatyIntegrity->valPtr = (float*)(base + Offsets::countryTreatyIntegrity);
	this->treatyIntegrity = treatyIntegrity;
	this->allFloatValues.push_back(treatyIntegrity);

	//-----------------------------------------------------------------------------------
	//--- Goods -------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	bool copy = true;
	if (g_countryList.size() == 0)
	{
		copy = false;
	}

	for (int i = 0; i < 11; i++)
	{
		std::string name;

		switch (i)
		{
		default:
		case 0:
			name = "Agriculture";
			break;
		case 1:
			name = "Rubber";
			break;
		case 2:
			name = "Timber";
			break;
		case 3:
			name = "Petroleum";
			break;
		case 4:
			name = "Coal";
			break;
		case 5:
			name = "Metal";
			break;
		case 6:
			name = "Uran";
			break;
		case 7:
			name = "Electricity";
			break;
		case 8:
			name = "Consumer";
			break;
		case 9:
			name = "Industry";
			break;
		case 10:
			name = "Military";
			break;
		}
		
		Resource res{};
		res.name = name;
		
		std::shared_ptr<FloatValue> stock(new FloatValue);
		stock->valPtr = (float*)(base + Offsets::countryGoodsStart + (Offsets::countryGoodsDiff * i));
		res.stock = stock;

		std::shared_ptr<FloatValue> production(new FloatValue);
		production->valPtr = (float*)(base + Offsets::countryGoodsStart + (Offsets::countryGoodsDiff * i) + Offsets::goodsProduction);
		res.production = production;

		std::shared_ptr<FloatValue> productionCost(new FloatValue);
		productionCost->valPtr = (float*)(base + Offsets::countryGoodsStart + (Offsets::countryGoodsDiff * i) + Offsets::goodsProductionCost);
		res.productionCost = productionCost;

		if (copy)
		{
			//copy market price obj from first country in list
			res.marketPrice = g_countryList[0].resources[i].marketPrice;
			res.margin = g_countryList[0].resources[i].margin;
		}
		else
		{
			//create new market price + margin obj
			std::shared_ptr<FloatValue> marketPrice(new FloatValue);
			std::shared_ptr<FloatValue> margin(new FloatValue);

			uintptr_t priceBase = *(uintptr_t*)(g_base + Offsets::goodsMarketBase);
			
			marketPrice->valPtr = (float*)(priceBase + Offsets::goodsMarketStart + (Offsets::goodsMarketDiff * i));
			margin->valPtr = (float*)(priceBase + Offsets::goodsMarketMarginStart + (Offsets::goodsMarketDiff * i));

			res.marketPrice = marketPrice;
			res.margin = margin;
		}

		this->resources.push_back(res);
	}

	//-----------------------------------------------------------------------------------
	//--- State -------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	std::shared_ptr<FloatValue> healthcareState(new FloatValue);
	healthcareState->valPtr = (float*)(base + Offsets::countryHealthcareState);
	this->healthcareState = healthcareState;
	this->allFloatValues.push_back(healthcareState);

	std::shared_ptr<FloatValue> educationState(new FloatValue);
	educationState->valPtr = (float*)(base + Offsets::countryEducationState);
	this->educationState = educationState;
	this->allFloatValues.push_back(educationState);

	std::shared_ptr<FloatValue> infrastructureState(new FloatValue);
	infrastructureState->valPtr = (float*)(base + Offsets::countryInfrastructureState);
	this->infrastructureState = infrastructureState;
	this->allFloatValues.push_back(infrastructureState);
	
	std::shared_ptr<FloatValue> environmentState(new FloatValue);
	environmentState->valPtr = (float*)(base + Offsets::countryEnvironmentState);
	this->environmentState = environmentState;
	this->allFloatValues.push_back(environmentState);

	std::shared_ptr<FloatValue> familyState(new FloatValue);
	familyState->valPtr = (float*)(base + Offsets::countryFamilyState);
	this->familyState = familyState;
	this->allFloatValues.push_back(familyState);
	
	std::shared_ptr<FloatValue> lawEnforcementState(new FloatValue);
	lawEnforcementState->valPtr = (float*)(base + Offsets::countryLawEnforcementState);
	this->lawEnforcementState = lawEnforcementState;
	this->allFloatValues.push_back(lawEnforcementState);

	std::shared_ptr<FloatValue> culturalSubState(new FloatValue);
	culturalSubState->valPtr = (float*)(base + Offsets::countryCulturalSubState);
	this->culturalSubState = culturalSubState;
	this->allFloatValues.push_back(culturalSubState);

	std::shared_ptr<FloatValue> socialAssistanceState(new FloatValue);
	socialAssistanceState->valPtr = (float*)(base + Offsets::countrySocialAssistanceState);
	this->socialAssistanceState = socialAssistanceState;
	this->allFloatValues.push_back(socialAssistanceState);

	return;
}

void Base::SRU_Data::Country::HandleUnits()
{
	if (!this->invincibleUnits && !this->maxFuelUnits && !this->maxSupplyUnits && !this->maxMoraleUnits &&
		!this->maxMoveSpeedUnitsT && !this->maxSpottingUnitsT && !this->maxMoveRangeUnitsT &&
		!this->maxGroundAttackRangeUnitsT && !this->maxAirAttackRangeUnitsT && !this->minBuildTimeUnitsT)
	{
		return;
	}

	for (int i = 0; i < this->allUnits.size(); i++)
	{
		if (this->invincibleUnits)
		{
			//*allUnits[i].maxHealth->valPtr =
			//	allUnits[i].maxHealth->origVal * 5;

			*allUnits[i].health->valPtr = 
				*allUnits[i].maxHealth->valPtr;
			//*allUnits[i].supply->valPtr = 5000.0f;
		}
	}
}

void Base::SRU_Data::Country::HandleFreeze()
{
	for (int i = 0; i < this->allFloatValues.size(); i++)
	{
		if (this->allFloatValues[i]->freeze)
		{
			*this->allFloatValues[i]->valPtr = 
				this->allFloatValues[i]->freezeVal;
		}
	}

	for (int i = 0; i < this->resources.size(); i++)
	{
		if (this->id < 2)
		{ //shared between countries
			if (this->resources[i].marketPrice->freeze)
			{
				*this->resources[i].marketPrice->valPtr =
					this->resources[i].marketPrice->freezeVal;
			}

			if (this->resources[i].margin->freeze)
			{
				*this->resources[i].margin->valPtr =
					this->resources[i].margin->freezeVal;
			}
		}

		if (this->resources[i].stock->freeze)
		{
			*this->resources[i].stock->valPtr =
				this->resources[i].stock->freezeVal;
		}

		if (this->resources[i].production->freeze)
		{
			*this->resources[i].production->valPtr =
				this->resources[i].production->freezeVal;
		}

		if (this->resources[i].productionCost->freeze)
		{
			*this->resources[i].productionCost->valPtr =
				this->resources[i].productionCost->freezeVal;
		}
	}

	HandleUnits();
}