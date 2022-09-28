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

	std::shared_ptr<FloatValue> agricultureStock(new FloatValue);
	agricultureStock->valPtr = (float*)(base + Offsets::countryAgricultureStock);
	this->agricultureStock = agricultureStock;
	this->allFloatValues.push_back(agricultureStock);

	std::shared_ptr<FloatValue> rubberStock(new FloatValue);
	rubberStock->valPtr = (float*)(base + Offsets::countryRubberStock);
	this->rubberStock = rubberStock;
	this->allFloatValues.push_back(rubberStock);

	std::shared_ptr<FloatValue> timberStock(new FloatValue);
	timberStock->valPtr = (float*)(base + Offsets::countryTimberStock);
	this->timberStock = timberStock;
	this->allFloatValues.push_back(timberStock);

	std::shared_ptr<FloatValue> petroleumStock(new FloatValue);
	petroleumStock->valPtr = (float*)(base + Offsets::countryPetroleumStock);
	this->petroleumStock = petroleumStock;
	this->allFloatValues.push_back(petroleumStock);

	std::shared_ptr<FloatValue> coalStock(new FloatValue);
	coalStock->valPtr = (float*)(base + Offsets::countryCoalStock);
	this->coalStock = coalStock;
	this->allFloatValues.push_back(coalStock);

	std::shared_ptr<FloatValue> metalStock(new FloatValue);
	metalStock->valPtr = (float*)(base + Offsets::countryMetalStock);
	this->metalStock = metalStock;
	this->allFloatValues.push_back(metalStock);

	std::shared_ptr<FloatValue> uranStock(new FloatValue);
	uranStock->valPtr = (float*)(base + Offsets::countryUranStock);
	this->uranStock = uranStock;
	this->allFloatValues.push_back(uranStock);

	std::shared_ptr<FloatValue> consumerStock(new FloatValue);
	consumerStock->valPtr = (float*)(base + Offsets::countryConsumerStock);
	this->consumerStock = consumerStock;
	this->allFloatValues.push_back(consumerStock);

	std::shared_ptr<FloatValue> industryStock(new FloatValue);
	industryStock->valPtr = (float*)(base + Offsets::countryIndustryStock);
	this->industryStock = industryStock;
	this->allFloatValues.push_back(industryStock);

	std::shared_ptr<FloatValue> militaryStock(new FloatValue);
	militaryStock->valPtr = (float*)(base + Offsets::countryMilitaryStock);
	this->militaryStock = militaryStock;
	this->allFloatValues.push_back(militaryStock);

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
}