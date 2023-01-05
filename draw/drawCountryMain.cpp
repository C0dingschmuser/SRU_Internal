#include "../Base.h"

void Base::Draw::DrawSelectedCountryText(Base::SRU_Data::Country* cc, const char *text)
{
	std::string countryText = std::string(text);

	if (cc->base == Base::SRU_Data::g_ownCountryBase)
	{ //own country
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));

		countryText += " (You)";
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
	}
	ImGui::Text(countryText.c_str(), cc->name.c_str());
	ImGui::PopStyleColor();
}

void Base::Draw::DrawCountry(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	DrawSelectedCountryText(cc, "Selected country: %s");

	float inputWidth = 300;

	//Name

	static char* buffer = (char*)calloc(1024, sizeof(char));

	if (!g_countryColorLoaded)
	{
		//set to true down below
		strcpy(buffer, cc->name.data());
	}

	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::InputText("Name", buffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		std::string test = std::string(buffer);

		if (test.length() > 0)
		{
			//apply
			cc->ChangeName(test);
		}
		else
		{
			//reset
			strcpy(buffer, cc->name.data());
		}
	}
	ImGui::PopItemWidth();

	//Treasury

	if (ImGui::Checkbox("###cb_treasury", &cc->treasury->freeze))
	{
		cc->treasury->freezeVal = *cc->treasury->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("Treasury", cc->treasury->valPtr, 1000000000, 0.0f, "%.0f"))
	{
		cc->treasury->freezeVal = *cc->treasury->valPtr;
	}
	ImGui::PopItemWidth();

	//Domnestic approval

	if (ImGui::Checkbox("###cb_dom", &cc->domApproval->freeze))
	{
		cc->domApproval->freezeVal = *cc->domApproval->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	
	std::string data = Base::Utils::FloatToPercent3(*cc->domApproval->valPtr, 1, true);
	if (ImGui::SliderFloat("Domenstic Approval", cc->domApproval->valPtr, -3, 3, data.c_str()))
	{
		cc->domApproval->freezeVal = *cc->domApproval->valPtr;
	}
	ImGui::PopItemWidth();

	//Military Approval

	if (ImGui::Checkbox("###cb_mil", &cc->milApproval->freeze))
	{
		cc->milApproval->freezeVal = *cc->milApproval->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);

	data = Base::Utils::FloatToPercent3(*cc->milApproval->valPtr, 1, true);
	if (ImGui::SliderFloat("Military Approval", cc->milApproval->valPtr, -3, 3, data.c_str()))
	{
		cc->milApproval->freezeVal = *cc->milApproval->valPtr;
	}
	ImGui::PopItemWidth();

	//Reserve personnel

	if (ImGui::Checkbox("###cb_reserve", &cc->milReserve->freeze))
	{
		cc->milReserve->freezeVal = *cc->milReserve->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("Reserve personnel", cc->milReserve->valPtr, 100000, 0, "%.0f"))
	{
		float val = *cc->milReserve->valPtr;
		*cc->milApproval->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->milReserve->freezeVal = *cc->milReserve->valPtr;
	}
	ImGui::PopItemWidth();

	//Research efficiency

	if (ImGui::Checkbox("###cb_research", &cc->researchEff->freeze))
	{
		cc->researchEff->freezeVal = *cc->researchEff->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("Research efficiency", cc->researchEff->valPtr, 0.1f, 0.0f, "%.1f"))
	{
		float val = *cc->researchEff->valPtr;
		*cc->researchEff->valPtr = std::clamp(val, 0.0f, 50000.0f);
		cc->researchEff->freezeVal = *cc->researchEff->valPtr;
	}
	ImGui::PopItemWidth();

	//GDPc

	if (ImGui::Checkbox("###cb_gdp", &cc->financeGDPc->freeze))
	{
		cc->financeGDPc->freezeVal = *cc->financeGDPc->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("GDP", cc->financeGDPc->valPtr, 0, 0, "%.1f"))
	{
		float val = *cc->financeGDPc->valPtr;
		*cc->financeGDPc->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->financeGDPc->freezeVal = *cc->financeGDPc->valPtr;
	}
	ImGui::PopItemWidth();

	//Inflation

	if (ImGui::Checkbox("###cb_inflation", &cc->financeInflation->freeze))
	{
		cc->financeInflation->freezeVal = *cc->financeInflation->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	
	data = Base::Utils::FloatToPercent3(*cc->financeInflation->valPtr, 1, true);
	if (ImGui::SliderFloat("Inflation", cc->financeInflation->valPtr, -3, 3, data.c_str()))
	{
		cc->financeInflation->freezeVal = *cc->financeInflation->valPtr;
	}
	ImGui::PopItemWidth();

	//Treaty integrity

	if (ImGui::Checkbox("###cb_treaty", &cc->treatyIntegrity->freeze))
	{
		cc->treatyIntegrity->freezeVal = *cc->treatyIntegrity->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("Treaty integrity", cc->treatyIntegrity->valPtr, 0.1f))
	{
		float val = *cc->treatyIntegrity->valPtr;
		*cc->treatyIntegrity->valPtr = std::clamp(val, -3.0f, 3.0f);
		cc->treatyIntegrity->freezeVal = *cc->treatyIntegrity->valPtr;
	}
	ImGui::PopItemWidth();

	//World market opinion

	if (ImGui::Checkbox("###cb_market", &cc->worldMarketOpinion->freeze))
	{
		cc->worldMarketOpinion->freezeVal = *cc->worldMarketOpinion->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("World market opinion", cc->worldMarketOpinion->valPtr, 0.1f))
	{
		float val = *cc->worldMarketOpinion->valPtr;
		*cc->worldMarketOpinion->valPtr = std::clamp(val, -3.0f, 3.0f);
		cc->worldMarketOpinion->freezeVal = *cc->worldMarketOpinion->valPtr;
	}
	ImGui::PopItemWidth();

	//Unemployment

	if (ImGui::Checkbox("###cb_unemployment", &cc->unemployment->freeze))
	{
		cc->unemployment->freezeVal = *cc->unemployment->valPtr;
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);

	data = Base::Utils::FloatToPercent3(*cc->unemployment->valPtr, 1, true);
	if (ImGui::SliderFloat("Unemployment", cc->unemployment->valPtr, -3, 3, data.c_str()))
	{
		cc->unemployment->freezeVal = *cc->unemployment->valPtr;
	}
	ImGui::PopItemWidth();

	//-----------------------------------------------------------------------------------
	//--- Government --------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	static std::vector<std::string> govTypes = {
		"Democracy",
		"Monarchy",
		"Communism",
		"Dictatorship",
		"Theocracy",
		"Colony",
		"Read Error"
	};

	int govType = *cc->govPtr - 1;

	ImGui::Text("Government Type");
	
	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::BeginCombo("##govtypecombo", govTypes[govType].c_str()))
	{
		for (int i = 0; i < govTypes.size() - 1; i++)
		{
			bool isSelected = (govType == i);
			if (ImGui::Selectable(govTypes[i].c_str(), isSelected))
			{
				*cc->govPtr = i + 1;
				govType = i + 1;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	//-----------------------------------------------------------------------------------
	//--- Color -------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	static float color[3] = { 0, 0, 0 };

	if (!g_countryColorLoaded)
	{
		unsigned long inputColor = *cc->colorPtr;
		Base::Utils::RGB rgb = Base::Utils::ColorConverter(inputColor);

		color[0] = rgb.r;
		color[1] = rgb.g;
		color[2] = rgb.b;
	}

	g_countryColorLoaded = true;

	ImGui::Text("Country Color (click square)");

	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::ColorEdit3("##countrycoloredit", (float*)&color))
	{
		unsigned long final = Base::Utils::ColorConverter(color[0], color[1], color[2], 1.0);

		*cc->colorPtr = final;
	}
	ImGui::PopItemWidth();

	//-----------------------------------------------------------------------------------
	//--- Flag --------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	static std::string flagOptions[3]{
		"Unchanged",
		"Flag from Country",
		"Flag from Id"
	};

	static Country* flagCountry = cc;

	ImGui::Text("Flag");
	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::BeginCombo("##countryflagcombo", flagOptions[cc->flagOption].c_str()))
	{
		for (int i = 0; i < 3; i++)
		{
			bool isSelected = (cc->flagOption == i);
			if (ImGui::Selectable(flagOptions[i].c_str(), isSelected))
			{
				cc->flagOption = i;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	if (cc->flagOption == 2 || cc->flagOption == 0)
	{
		ImGui::BeginDisabled();
	}

	if (!g_countryColorLoaded)
	{
		flagCountry = cc;

		if (*cc->flagIdPtr != cc->originalFlagId)
		{
			for (int i = 0; i < g_countryList.size(); i++)
			{
				if (g_countryList[i].base != cc->base)
				{
					if (*g_countryList[i].flagIdPtr == *cc->flagIdPtr)
					{
						flagCountry = &g_countryList[i];
						break;
					}
				}
			}
		}
	}

	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::BeginCombo("##countrycombo", flagCountry->name.c_str()))
	{
		for (int i = 0; i < g_countryList.size(); i++)
		{
			const bool isSelected = (flagCountry->base == g_countryList[i].base);
			if (ImGui::Selectable(Base::SRU_Data::g_countryList[i].name.c_str(), isSelected))
			{
				flagCountry = &g_countryList[i];
				*cc->flagIdPtr = flagCountry->originalFlagId;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	if (cc->flagOption == 2)
	{
		ImGui::EndDisabled();
	}
	else if (cc->flagOption == 1)
	{
		ImGui::BeginDisabled();
	}

	ImGui::PushItemWidth(inputWidth + 27);
	ImGui::InputUInt16("##countryflagid", cc->flagIdPtr, 1U, 0);
	ImGui::PopItemWidth();

	if (cc->flagOption == 1 || cc->flagOption == 0)
	{
		ImGui::EndDisabled();
	}

	//-----------------------------------------------------------------------------------
	//--- Leader ------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	static std::string leaderOptions[4]{
		"Unchanged",
		"Leader from Name List",
		"Leader from Id",
	};

	static Country* leaderCountry = cc;
	static std::shared_ptr<Leader> leader = cc->currentLeader;

	ImGui::Text("Leader");
	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::BeginCombo("##countryleadercombo", leaderOptions[cc->leaderOption].c_str()))
	{
		for (int i = 0; i < 3; i++)
		{
			bool isSelected = (cc->leaderOption == i);
			if (ImGui::Selectable(leaderOptions[i].c_str(), isSelected))
			{
				cc->leaderOption = i;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	if (cc->leaderOption != 1) { //Name list
		ImGui::BeginDisabled();
	}

	if (!g_countryColorLoaded)
	{
		leaderCountry = cc;
		leader = cc->currentLeader;

		if (*cc->leaderIdPtr != cc->originalLeaderId)
		{
			for (int i = 0; i < g_countryList.size(); i++)
			{
				if (g_countryList[i].base != cc->base)
				{
					if (*g_countryList[i].leaderIdPtr == *cc->leaderIdPtr)
					{
						leaderCountry = &g_countryList[i];
						break;
					}
				}
			}
		}
	}

	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::BeginCombo("##leaderlistcombo", leader->name.c_str()))
	{
		for (int i = 0; i < g_leaderList.size(); i++) {
			const bool isSelected = (leader->id == g_leaderList[i]->id);
			if (ImGui::Selectable(g_leaderList[i]->name.c_str(), isSelected))
			{
				leader = g_leaderList[i];
				*cc->leaderIdPtr = leader->id;
				cc->currentLeader = leader;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	if (cc->leaderOption != 1) { //Name list
		ImGui::EndDisabled();
	}

	if (cc->leaderOption != 2) { //Id
		ImGui::BeginDisabled(); 
	}
	
	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::InputUInt16("##countryleaderid", cc->leaderIdPtr, 1U, 0)) {
		cc->RefreshLeader();
		leader = cc->currentLeader;
	}
	ImGui::PopItemWidth();

	if (cc->leaderOption != 2) { //Id
		ImGui::EndDisabled();
	}

	//-----------------------------------------------------------------------------------
	//--- DEFCON ------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	ImGui::Separator();
	
	std::string defconText = "Auto";

	switch (cc->defconState)
	{
	case 0:
		defconText = "Peace";
		break;
	case 1:
		defconText = "Guarded";
		break;
	case 2:
		defconText = "Elevated";
		break;
	case 3:
		defconText = "High";
		break;
	case 4:
		defconText = "War";
		break;
		
	}

	static std::vector<std::string> stances = {
		"Normal",
		"Passive",
		"Defensive",
		"Aggressive",
		"Unpredictable"
	};

	int stance = (int)*(uint8_t*)(cc->base + Offsets::countryAIStance);

	ImGui::Text("AI Stance");
	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::BeginCombo("##aistancecombo", stances[stance].c_str()))
	{
		for (int i = 0; i < stances.size(); i++)
		{
			bool isSelected = (stance == i);
			if (ImGui::Selectable(stances[i].c_str(), isSelected))
			{
				stance = i;
				Base::Execute::SetCountryAIStance(cc, stance);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Text("DEFCON");
	ImGui::PushItemWidth(inputWidth + 27);
	if (ImGui::SliderInt("###defconslider", &cc->defconState, -1, 4, defconText.c_str()))
	{
		if (cc->defconState > -1)
		{
			*cc->defconPtrs[0] = cc->defconState;
			*cc->defconPtrs[1] = cc->defconState;
		}
	}
	ImGui::PopItemWidth();
	ImGui::Separator();;

	//-----------------------------------------------------------------------------------
	//--- Goods -------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	ImGui::Text("Goods");

	for (int i = 0; i < cc->resources.size(); i++)
	{
		std::string name = cc->resources[i].name;
		if (ImGui::TreeNode(name.c_str()))
		{
			//Stock
			if (ImGui::Checkbox(std::string("###cb" + name + "stock").c_str(), &cc->resources[i].stock->freeze))
			{
				cc->resources[i].stock->freezeVal = *cc->resources[i].stock->valPtr;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			if (ImGui::InputFloat("Stock", cc->resources[i].stock->valPtr, 10000.0f, 0.0f, "%.1f"))
			{
				float val = *cc->resources[i].stock->valPtr;
				*cc->resources[i].stock->valPtr = std::clamp(val, 0.0f, FLT_MAX);
				cc->resources[i].stock->freezeVal = *cc->resources[i].stock->valPtr;
			}
			ImGui::PopItemWidth();

			//Production
			if (ImGui::Checkbox(std::string("###cb" + name + "production").c_str(), &cc->resources[i].production->freeze))
			{
				cc->resources[i].production->freezeVal = *cc->resources[i].production->valPtr;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			if (ImGui::InputFloat("Production", cc->resources[i].production->valPtr, 100000.0f, 0.0f, "%.1f"))
			{
				float val = *cc->resources[i].production->valPtr;
				*cc->resources[i].production->valPtr = std::clamp(val, 0.0f, FLT_MAX);
				cc->resources[i].production->freezeVal = *cc->resources[i].production->valPtr;
			}
			ImGui::PopItemWidth();

			//Production cost
			if (ImGui::Checkbox(std::string("###cb" + name + "productioncost").c_str(), &cc->resources[i].productionCost->freeze))
			{
				cc->resources[i].productionCost->freezeVal = *cc->resources[i].productionCost->valPtr;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			if (ImGui::InputFloat("Production cost", cc->resources[i].productionCost->valPtr, 10000.0f, 0.0f, "%.1f"))
			{
				float val = *cc->resources[i].productionCost->valPtr;
				*cc->resources[i].productionCost->valPtr = std::clamp(val, 0.0f, FLT_MAX);
				cc->resources[i].productionCost->freezeVal = *cc->resources[i].productionCost->valPtr;
			}
			ImGui::PopItemWidth();

			//Market price
			if (ImGui::Checkbox(std::string("###cb" + name + "marketprice").c_str(), &cc->resources[i].marketPrice->freeze))
			{
				cc->resources[i].marketPrice->freezeVal = *cc->resources[i].marketPrice->valPtr;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			if (ImGui::InputFloat("Market price", cc->resources[i].marketPrice->valPtr, 10000.0f, 0.0f, "%.1f"))
			{
				float val = *cc->resources[i].marketPrice->valPtr;
				*cc->resources[i].marketPrice->valPtr = std::clamp(val, 0.0f, FLT_MAX);
				cc->resources[i].marketPrice->freezeVal = *cc->resources[i].marketPrice->valPtr;
			}
			ImGui::PopItemWidth();

			//Margin
			if (ImGui::Checkbox(std::string("###cb" + name + "margin").c_str(), &cc->resources[i].margin->freeze))
			{
				cc->resources[i].margin->freezeVal = *cc->resources[i].margin->valPtr;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(inputWidth);
			if (ImGui::InputFloat("Margin", cc->resources[i].margin->valPtr, 10000.0f, 0.0f, "%.1f"))
			{
				float val = *cc->resources[i].margin->valPtr;
				*cc->resources[i].margin->valPtr = std::clamp(val, 0.0f, FLT_MAX);
				cc->resources[i].margin->freezeVal = *cc->resources[i].margin->valPtr;
			}
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}
	}

	//-----------------------------------------------------------------------------------
	//--- State -------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	ImGui::Separator();
	ImGui::Text("Social spending states");

	//Healthcare

	if (ImGui::Checkbox("###cb_s_healthcare", &cc->healthcareState->freeze))
	{
		cc->healthcareState->freezeVal = *cc->healthcareState->valPtr;
	}
	ImGui::SameLine();

	data = Base::Utils::FloatToPercent3(*cc->healthcareState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Healthcare", cc->healthcareState->valPtr, -3, 3, data.c_str()))
	{
		cc->healthcareState->freezeVal = *cc->healthcareState->valPtr;
	}
	ImGui::PopItemWidth();

	//Education

	if (ImGui::Checkbox("###cb_s_education", &cc->educationState->freeze))
	{
		cc->educationState->freezeVal = *cc->educationState->valPtr;
	}
	ImGui::SameLine();

	data = Base::Utils::FloatToPercent3(*cc->educationState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Education", cc->educationState->valPtr, -3, 3, data.c_str()))
	{
		cc->educationState->freezeVal = *cc->educationState->valPtr;
	}
	ImGui::PopItemWidth();

	//Infrastructure

	if (ImGui::Checkbox("###cb_s_infrastructure", &cc->infrastructureState->freeze))
	{
		cc->infrastructureState->freezeVal = *cc->infrastructureState->valPtr;
	}
	ImGui::SameLine();

	data = Base::Utils::FloatToPercent3(*cc->infrastructureState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Infrastructure", cc->infrastructureState->valPtr, -3, 3, data.c_str()))
	{
		cc->infrastructureState->freezeVal = *cc->infrastructureState->valPtr;
	}
	ImGui::PopItemWidth();

	//Environment

	if (ImGui::Checkbox("###cb_s_environment", &cc->environmentState->freeze))
	{
		cc->environmentState->freezeVal = *cc->environmentState->valPtr;
	}
	ImGui::SameLine();

	data = Base::Utils::FloatToPercent3(*cc->environmentState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Environment", cc->environmentState->valPtr, -3, 3, data.c_str()))
	{
		cc->environmentState->freezeVal = *cc->environmentState->valPtr;
	}
	ImGui::PopItemWidth();

	//Family

	if (ImGui::Checkbox("###cb_s_family", &cc->familyState->freeze))
	{
		cc->familyState->freezeVal = *cc->familyState->valPtr;
	}
	ImGui::SameLine();

	data = Base::Utils::FloatToPercent3(*cc->familyState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Family", cc->familyState->valPtr, -3, 3, data.c_str()))
	{
		cc->familyState->freezeVal = *cc->familyState->valPtr;
	}
	ImGui::PopItemWidth();

	//Law Enforcement

	if (ImGui::Checkbox("###cb_s_lawEnforcement", &cc->lawEnforcementState->freeze))
	{
		cc->lawEnforcementState->freezeVal = *cc->lawEnforcementState->valPtr;
	}
	ImGui::SameLine();

	data = Base::Utils::FloatToPercent3(*cc->lawEnforcementState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Law enforcement", cc->lawEnforcementState->valPtr, -3, 3, data.c_str()))
	{
		cc->lawEnforcementState->freezeVal = *cc->lawEnforcementState->valPtr;
	}
	ImGui::PopItemWidth();

	//Cultural Subsidies

	if (ImGui::Checkbox("###cb_s_culturalSub", &cc->culturalSubState->freeze))
	{
		cc->culturalSubState->freezeVal = *cc->culturalSubState->valPtr;
	}
	ImGui::SameLine();

	data = Base::Utils::FloatToPercent3(*cc->culturalSubState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Cultural subsidies", cc->culturalSubState->valPtr, -3, 3, data.c_str()))
	{
		cc->culturalSubState->freezeVal = *cc->culturalSubState->valPtr;
	}
	ImGui::PopItemWidth();

	//Social Assistance

	if (ImGui::Checkbox("###cb_s_socialAssistance", &cc->socialAssistanceState->freeze))
	{
		cc->socialAssistanceState->freezeVal = *cc->socialAssistanceState->valPtr;
	}
	ImGui::SameLine();
	
	data = Base::Utils::FloatToPercent3(*cc->socialAssistanceState->valPtr, 1, true);
	
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::SliderFloat("Social assistance", cc->socialAssistanceState->valPtr, -3, 3, data.c_str()))
	{
		cc->socialAssistanceState->freezeVal = *cc->socialAssistanceState->valPtr;
	}
	ImGui::PopItemWidth();
}