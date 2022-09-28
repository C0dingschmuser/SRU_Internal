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

	//Treasury

	if (ImGui::Checkbox("###cb_treasury", &cc->treasury->freeze))
	{
		cc->treasury->freezeVal = *cc->treasury->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Treasury", cc->treasury->valPtr, 1000000000, 0.0f, "%.0f"))
	{
		cc->treasury->freezeVal = *cc->treasury->valPtr;
	}

	//Domnestic approval

	if (ImGui::Checkbox("###cb_dom", &cc->domApproval->freeze))
	{
		cc->domApproval->freezeVal = *cc->domApproval->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Dom. approval", cc->domApproval->valPtr, 0.1f))
	{
		float val = *cc->domApproval->valPtr;
		*cc->domApproval->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->domApproval->freezeVal = *cc->domApproval->valPtr;
	}

	//Military Approval

	if (ImGui::Checkbox("###cb_mil", &cc->milApproval->freeze))
	{
		cc->milApproval->freezeVal = *cc->milApproval->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Mil. approval", cc->milApproval->valPtr, 0.1f))
	{
		float val = *cc->milApproval->valPtr;
		*cc->milApproval->valPtr = std::clamp(val, 0.0f, 1.0f);
		cc->milApproval->freezeVal = *cc->milApproval->valPtr;
	}

	//Reserve personnel

	if (ImGui::Checkbox("###cb_reserve", &cc->milReserve->freeze))
	{
		cc->milReserve->freezeVal = *cc->milReserve->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Reserve personnel", cc->milReserve->valPtr, 100000, 0, "%.0f"))
	{
		float val = *cc->milReserve->valPtr;
		*cc->milApproval->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->milReserve->freezeVal = *cc->milReserve->valPtr;
	}

	//Research efficiency

	if (ImGui::Checkbox("###cb_research", &cc->researchEff->freeze))
	{
		cc->researchEff->freezeVal = *cc->researchEff->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Research efficiency", cc->researchEff->valPtr, 0.1f, 0.0f, "%.1f"))
	{
		float val = *cc->researchEff->valPtr;
		*cc->researchEff->valPtr = std::clamp(val, 0.0f, 50000.0f);
		cc->researchEff->freezeVal = *cc->researchEff->valPtr;
	}

	//Treaty integrity

	if (ImGui::Checkbox("###cb_treaty", &cc->treatyIntegrity->freeze))
	{
		cc->treatyIntegrity->freezeVal = *cc->treatyIntegrity->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Treaty integrity", cc->treatyIntegrity->valPtr, 0.1f))
	{
		float val = *cc->treatyIntegrity->valPtr;
		*cc->treatyIntegrity->valPtr = std::clamp(val, 0.0f, 3.0f);
		cc->treatyIntegrity->freezeVal = *cc->treatyIntegrity->valPtr;
	}

	//World market opinion

	if (ImGui::Checkbox("###cb_market", &cc->worldMarketOpinion->freeze))
	{
		cc->worldMarketOpinion->freezeVal = *cc->worldMarketOpinion->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("W. market opinion", cc->worldMarketOpinion->valPtr, 0.1f))
	{
		float val = *cc->worldMarketOpinion->valPtr;
		*cc->worldMarketOpinion->valPtr = std::clamp(val, 0.0f, 3.0f);
		cc->worldMarketOpinion->freezeVal = *cc->worldMarketOpinion->valPtr;
	}

	//-----------------------------------------------------------------------------------
	//--- Goods -------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	ImGui::Separator();
	ImGui::Text("Goods");

	//Agriculture

	if (ImGui::Checkbox("###cb_agriculture", &cc->agricultureStock->freeze))
	{
		cc->agricultureStock->freezeVal = *cc->agricultureStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Agriculture stock", cc->agricultureStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->agricultureStock->valPtr;
		*cc->agricultureStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->agricultureStock->freezeVal = *cc->agricultureStock->valPtr;
	}

	//Rubber

	if (ImGui::Checkbox("###cb_rubber", &cc->rubberStock->freeze))
	{
		cc->rubberStock->freezeVal = *cc->rubberStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Rubber stock", cc->rubberStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->rubberStock->valPtr;
		*cc->rubberStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->rubberStock->freezeVal = *cc->rubberStock->valPtr;
	}

	//Timber

	if (ImGui::Checkbox("###cb_timber", &cc->timberStock->freeze))
	{
		cc->timberStock->freezeVal = *cc->timberStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Timber stock", cc->timberStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->timberStock->valPtr;
		*cc->timberStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->timberStock->freezeVal = *cc->timberStock->valPtr;
	}

	//Petroleum

	if (ImGui::Checkbox("###cb_petroleum", &cc->petroleumStock->freeze))
	{
		cc->petroleumStock->freezeVal = *cc->petroleumStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Petroleum stock", cc->petroleumStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->petroleumStock->valPtr;
		*cc->petroleumStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->petroleumStock->freezeVal = *cc->petroleumStock->valPtr;
	}

	//Coal

	if (ImGui::Checkbox("###cb_coal", &cc->coalStock->freeze))
	{
		cc->coalStock->freezeVal = *cc->coalStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Coal stock", cc->coalStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->coalStock->valPtr;
		*cc->coalStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->coalStock->freezeVal = *cc->coalStock->valPtr;
	}

	//Metal

	if (ImGui::Checkbox("###cb_metal", &cc->metalStock->freeze))
	{
		cc->metalStock->freezeVal = *cc->metalStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Metal stock", cc->metalStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->metalStock->valPtr;
		*cc->metalStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->metalStock->freezeVal = *cc->metalStock->valPtr;
	}

	//Uran

	if (ImGui::Checkbox("###cb_uran", &cc->uranStock->freeze))
	{
		cc->uranStock->freezeVal = *cc->uranStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Uran stock", cc->uranStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->uranStock->valPtr;
		*cc->uranStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->uranStock->freezeVal = *cc->uranStock->valPtr;
	}

	//Consumer

	if (ImGui::Checkbox("###cb_consumer", &cc->consumerStock->freeze))
	{
		cc->consumerStock->freezeVal = *cc->consumerStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Consumer stock", cc->consumerStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->consumerStock->valPtr;
		*cc->consumerStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->consumerStock->freezeVal = *cc->consumerStock->valPtr;
	}

	//Industry

	if (ImGui::Checkbox("###cb_industry", &cc->industryStock->freeze))
	{
		cc->industryStock->freezeVal = *cc->industryStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Industry stock", cc->industryStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->industryStock->valPtr;
		*cc->industryStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->industryStock->freezeVal = *cc->industryStock->valPtr;
	}

	//Military

	if (ImGui::Checkbox("###cb_military", &cc->militaryStock->freeze))
	{
		cc->militaryStock->freezeVal = *cc->militaryStock->valPtr;
	}
	ImGui::SameLine();
	if (ImGui::InputFloat("Military stock", cc->militaryStock->valPtr, 10000.0f, 0.0f, "%.1f"))
	{
		float val = *cc->militaryStock->valPtr;
		*cc->militaryStock->valPtr = std::clamp(val, 0.0f, 999999999.0f);
		cc->militaryStock->freezeVal = *cc->militaryStock->valPtr;
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