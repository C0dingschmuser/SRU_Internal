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
	DrawSelectedCountryText(cc, "Selected country: %s");

	float inputWidth = 275;

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
	if (ImGui::InputFloat("Dom. approval", cc->domApproval->valPtr, 0.1f))
	{
		float val = *cc->domApproval->valPtr;
		*cc->domApproval->valPtr = std::clamp(val, 0.0f, 1.0f);
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
	if (ImGui::InputFloat("Mil. approval", cc->milApproval->valPtr, 0.1f))
	{
		float val = *cc->milApproval->valPtr;
		*cc->milApproval->valPtr = std::clamp(val, 0.0f, 1.0f);
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
		*cc->treatyIntegrity->valPtr = std::clamp(val, 0.0f, 3.0f);
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
	if (ImGui::InputFloat("W. market opinion", cc->worldMarketOpinion->valPtr, 0.1f))
	{
		float val = *cc->worldMarketOpinion->valPtr;
		*cc->worldMarketOpinion->valPtr = std::clamp(val, 0.0f, 3.0f);
		cc->worldMarketOpinion->freezeVal = *cc->worldMarketOpinion->valPtr;
	}
	ImGui::PopItemWidth();

	//-----------------------------------------------------------------------------------
	//--- Goods -------------------------------------------------------------------------
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

	ImGui::Text("DEFCON");
	ImGui::PushItemWidth(inputWidth);
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
	if (ImGui::InputFloat("Healthcare", cc->healthcareState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->healthcareState->valPtr;
		*cc->healthcareState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->healthcareState->freezeVal = *cc->healthcareState->valPtr;
	}

	//Education

	if (ImGui::Checkbox("###cb_s_education", &cc->educationState->freeze))
	{
		cc->educationState->freezeVal = *cc->educationState->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Education", cc->educationState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->educationState->valPtr;
		*cc->educationState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->educationState->freezeVal = *cc->educationState->valPtr;
	}

	//Infrastructure

	if (ImGui::Checkbox("###cb_s_infrastructure", &cc->infrastructureState->freeze))
	{
		cc->infrastructureState->freezeVal = *cc->infrastructureState->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Infrastructure", cc->infrastructureState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->infrastructureState->valPtr;
		*cc->infrastructureState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->infrastructureState->freezeVal = *cc->infrastructureState->valPtr;
	}

	//Environment

	if (ImGui::Checkbox("###cb_s_environment", &cc->environmentState->freeze))
	{
		cc->environmentState->freezeVal = *cc->environmentState->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Environment", cc->environmentState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->environmentState->valPtr;
		*cc->environmentState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->environmentState->freezeVal = *cc->environmentState->valPtr;
	}

	//Family

	if (ImGui::Checkbox("###cb_s_family", &cc->familyState->freeze))
	{
		cc->familyState->freezeVal = *cc->familyState->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Family", cc->familyState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->familyState->valPtr;
		*cc->familyState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->familyState->freezeVal = *cc->familyState->valPtr;
	}

	//Law Enforcement

	if (ImGui::Checkbox("###cb_s_lawEnforcement", &cc->lawEnforcementState->freeze))
	{
		cc->lawEnforcementState->freezeVal = *cc->lawEnforcementState->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Law enforcement", cc->lawEnforcementState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->lawEnforcementState->valPtr;
		*cc->lawEnforcementState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->lawEnforcementState->freezeVal = *cc->lawEnforcementState->valPtr;
	}

	//Cultural Subsidies

	if (ImGui::Checkbox("###cb_s_culturalSub", &cc->culturalSubState->freeze))
	{
		cc->culturalSubState->freezeVal = *cc->culturalSubState->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Cultural subsidies", cc->culturalSubState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->culturalSubState->valPtr;
		*cc->culturalSubState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->culturalSubState->freezeVal = *cc->culturalSubState->valPtr;
	}

	//Social Assistance

	if (ImGui::Checkbox("###cb_s_socialAssistance", &cc->socialAssistanceState->freeze))
	{
		cc->socialAssistanceState->freezeVal = *cc->socialAssistanceState->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Social assistance", cc->socialAssistanceState->valPtr, 0.1f, 0.0f, "%.2f"))
	{
		float val = *cc->socialAssistanceState->valPtr;
		*cc->socialAssistanceState->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->socialAssistanceState->freezeVal = *cc->socialAssistanceState->valPtr;
	}
}