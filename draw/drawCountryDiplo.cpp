#include "../Base.h"

void Base::Draw::DrawCountryDiplo(Base::SRU_Data::Country* cc, int& treatyMsg)
{
	using namespace Base::SRU_Data;
	bool menuDisabled = false;

	Country* t = &g_countryList[g_selectedTargetCountry];

	if (t->base == cc->base)
	{
		menuDisabled = true;
	}

	Draw::DrawSelectedCountryText(cc, "Selected country: %s");
	Draw::DrawSelectedCountryText(&g_countryList[g_selectedTargetCountry], "Target country: %s");
	ImGui::Text("Click / Shift-Click to change");
	ImGui::Separator();

	static float tmp1 = 0, tmp2 = 0, tmp3 = 0;

	float* diplRating = nullptr;
	float* diplCivRating = nullptr;
	float* diplBelliRating = nullptr;

	if (menuDisabled)
	{
		ImGui::BeginDisabled();

		diplRating = &tmp1;
		diplCivRating = &tmp2;
		diplBelliRating = &tmp3;
	}
	else
	{
		diplRating = (float*)(cc->base + t->oId * 4 + Offsets::countryDiplRatings);
		diplCivRating = (float*)(cc->base + t->oId * 4 + Offsets::countryDiplCivRatings);
		diplBelliRating = (float*)(cc->base + t->oId * 4 + Offsets::countryDiplCasusBelli);
	}

	ImGui::BeginChild("##countrydiplsliders", ImVec2(225, 225));
	{
		ImGui::Text("Diplomatic Rating");

		ImGui::PushItemWidth(225);

		std::string str = Base::Utils::FloatToPercent2(*diplRating, 1.0f);

		ImGui::SliderFloat("##countrydiplrating", diplRating, -1.5f, 1.5f, str.c_str());//"%.3f");

		ImGui::Text("Civilian Rating");

		str = Base::Utils::FloatToPercent2(*diplCivRating, 1.0f);

		ImGui::SliderFloat("##countrydiplcivrating", diplCivRating, -1.5f, 1.5f, str.c_str());//"%.3f");

		ImGui::Text("Casus Belli");

		str = Base::Utils::FloatToPercent2(*diplBelliRating, 1.0f);

		ImGui::SliderFloat("##countrydiplbellirating", diplBelliRating, -1.5f, 1.5f, str.c_str());//"%.3f");

		ImGui::PopItemWidth();

		ImGui::Text("Relation: Selected -> Target");

		uintptr_t hexBase = *g_clickedHexPtr;
		uint8_t hexOwner = *(uintptr_t*)(hexBase);
		uint8_t hexLoyalty = *(uintptr_t*)(hexBase + Offsets::hexLoyalty);

		Country* loyalCountry = nullptr;
		for (int i = 0; i < g_countryList.size(); i++)
		{
			if (g_countryList[i].oId == hexLoyalty)
			{
				loyalCountry = &g_countryList[i];
				break;
			}
		}

		Country* ownerCountry = nullptr;
		for (int i = 0; i < g_countryList.size(); i++)
		{
			if (g_countryList[i].oId == hexOwner)
			{
				ownerCountry = &g_countryList[i];
				break;
			}
		}

		//if (ImGui::TreeNode("Advanced Options"))
		{
			if (ImGui::Button("Annex Colonies of Selected"))
			{
				Base::Execute::AnnexAllColonies(cc);
			}

			if (ImGui::Button("Liberate Colonies of Selected"))
			{
				Base::Execute::RespawnAllColonies(cc);
			}
			
			//ImGui::TreePop();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##countrydiploptions");
	{
		static int currentTreatyId = 0;

		ImGui::Text("Treaties");
		if (ImGui::BeginListBox("##countrydipltreaties", ImVec2(250, 165)))
		{
			for (int i = 0; i < Base::SRU_Data::g_diplTreatyList.size(); i++)
			{
				const bool isSelected = (currentTreatyId == i);
				if (ImGui::Selectable(Base::SRU_Data::g_diplTreatyList[i].name.c_str(), isSelected))
				{
					currentTreatyId = i;
				}
			}

			ImGui::EndListBox();
		}

		if (g_diplTreatyList[currentTreatyId].treatyId == -1) {
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Cancel Treaty", ImVec2(115, 25))) {
			treatyMsg = Base::Execute::ExecuteTreaty(currentTreatyId, 0);
		}
		
		if (g_diplTreatyList[currentTreatyId].treatyId == -1) {
			ImGui::EndDisabled();
		}

		ImGui::SameLine(135);
		if (ImGui::Button("Force Treaty", ImVec2(115, 25)))
		{
			treatyMsg = Base::Execute::ExecuteTreaty(currentTreatyId);
		}

		/*if (treatyMsg != 0)
		{
			ImGui::Dummy(ImVec2(1, 1));
			ImGui::SameLine(175);
			if (treatyMsg == 1)
			{
				ImGui::Text("Done!");
			}
			else
			{
				ImGui::Text("Error");
			}
		}*/
	}
	ImGui::EndChild();

	if (menuDisabled)
	{
		ImGui::EndDisabled();
	}
}