#include "../Base.h"

void Base::Draw::DrawCountryDiplo(Base::SRU_Data::Country* cc, int& treatyMsg)
{
	using namespace Base::SRU_Data;
	bool menuDisabled = false;

	Country t = g_countryList[g_selectedTargetCountry];

	if (t.base == cc->base)
	{
		menuDisabled = true;
	}

	Draw::DrawSelectedCountryText(cc, "Selected country: %s");
	Draw::DrawSelectedCountryText(&g_countryList[g_selectedTargetCountry], "Target country: %s");
	ImGui::Text("Click / Shift-Click to change");
	ImGui::Separator();

	if (menuDisabled)
	{
		ImGui::BeginDisabled();
	}

	ImGui::BeginChild("##countrydiplsliders", ImVec2(225, 225));
	{
		ImGui::Text("Diplomatic Rating");

		ImGui::PushItemWidth(225);

		float* diplRating = (float*)(cc->base + t.oId * 4 + Offsets::countryDiplRatings);
		ImGui::SliderFloat("##countrydiplrating", diplRating, -1.5f, 1.5f, "%.3f");

		ImGui::Text("Civilian Rating");

		float* diplCivRating = (float*)(cc->base + t.oId * 4 + Offsets::countryDiplCivRatings);
		ImGui::SliderFloat("##countrydiplcivrating", diplCivRating, -1.5f, 1.5f, "%.3f");

		ImGui::Text("Casus Belli");

		float* diplBelliRating = (float*)(cc->base + t.oId * 4 + Offsets::countryDiplCasusBelli);
		ImGui::SliderFloat("##countrydiplbellirating", diplBelliRating, -1.5f, 1.5f, "%.3f");

		ImGui::PopItemWidth();
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

		ImGui::Dummy(ImVec2(1, 1));
		ImGui::SameLine(135);
		if (ImGui::Button("Force Treaty", ImVec2(115, 25)))
		{
			treatyMsg = Base::Execute::ExecuteTreaty(currentTreatyId);
		}

		if (treatyMsg != 0)
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
		}
	}
	ImGui::EndChild();

	if (menuDisabled)
	{
		ImGui::EndDisabled();
	}
}