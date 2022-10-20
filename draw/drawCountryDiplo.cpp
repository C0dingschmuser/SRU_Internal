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

		std::string str = Base::Utils::FloatToPercent(*diplRating, 1.0f);

		ImGui::SliderFloat("##countrydiplrating", diplRating, -1.5f, 1.5f, str.c_str());//"%.3f");

		ImGui::Text("Civilian Rating");

		str = Base::Utils::FloatToPercent(*diplCivRating, 1.0f);

		ImGui::SliderFloat("##countrydiplcivrating", diplCivRating, -1.5f, 1.5f, str.c_str());//"%.3f");

		ImGui::Text("Casus Belli");

		str = Base::Utils::FloatToPercent(*diplBelliRating, 1.0f);

		ImGui::SliderFloat("##countrydiplbellirating", diplBelliRating, -1.5f, 1.5f, str.c_str());//"%.3f");

		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(1, 80));
		ImGui::Text("Relation: Selected -> Target");
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