#include "../Base.h"

using namespace Base::SRU_Data;


void Base::Draw::DrawMap(Base::SRU_Data::Country* cc)
{
	Country* target = &g_countryList[g_selectedTargetCountry];

	Draw::DrawSelectedCountryText(target, "Target country: %s");
	ImGui::Text("Shift-Click to change");
	ImGui::Separator();

	uintptr_t hexBase = *g_clickedHexPtr;
	uint8_t hexOwner = *(uintptr_t*)(hexBase);
	uint8_t hexLoyalty = *(uintptr_t*)(hexBase + Offsets::hexLoyalty);
	uint8_t* hexSupply = (uint8_t*)(hexBase + Offsets::hexSupply);
	uint8_t* hexGround = (uint8_t*)(hexBase + Offsets::hexGround);

	ImGui::BeginChild("##MapHexData", ImVec2(225, 225));
	{
		ImGui::Text("Hex Owner:");

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

		Draw::DrawSelectedCountryText(ownerCountry, "%s");

		ImGui::Text("Hex Loyalty:");
		if (loyalCountry)
		{
			Draw::DrawSelectedCountryText(loyalCountry, "%s");
		}
		else
		{
			ImGui::Text("None");
		}

		ImGui::Text("Hex Supply");

		ImGui::PushItemWidth(225);
		ImGui::SliderByte("##hexsupplyslider", hexSupply, 0, 254);


		ImGui::Text("Hex Ground");

		ImGui::PushItemWidth(225);
		ImGui::SliderByte("##hexgroundslider", hexGround, 0, 254);

		ImGui::Separator();

		bool disabled = true;

		if (loyalCountry)
		{
			if (loyalCountry->base != cc->base)
			{
				disabled = false;
			}
		}

		Country t = g_countryList[g_selectedTargetCountry];

		if (disabled)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Create Colony"))
		{
			Base::Execute::RespawnCountry(loyalCountry->oId, t.oId, 2);
		}
		ImGui::SameLine();
		if (ImGui::Button("Liberate Country"))
		{
			Base::Execute::RespawnCountry(loyalCountry->oId, t.oId, 1);
		}

		if (disabled)
		{
			ImGui::EndDisabled();
		}

		disabled = true;
		if (t.base != ownerCountry->base)
		{
			disabled = false;
		}

		if (disabled)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Annex Country"))
		{
			Base::Execute::AnnexCountry(ownerCountry->oId, t.oId);
		}

		if (disabled)
		{
			ImGui::EndDisabled();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##MapPainting");
	{
		ImGui::Text("Painting");
		ImGui::Text("- Capslock to enable");
		ImGui::Text("- Hold ctrl to paint");

		static bool paintTargetCountry = true;

		ImGui::Checkbox("Paint target country", &paintTargetCountry);

		if (paintTargetCountry)
		{
			ImGui::BeginDisabled();

			g_paintSelectedComboCountry = g_selectedTargetCountry;
		}

		if (ImGui::BeginCombo("##countrycombo", g_countryList[g_paintSelectedComboCountry].name.c_str()))
		{
			for (int i = 0; i < g_countryList.size(); i++)
			{
				const bool isSelected = (g_paintSelectedComboCountry == i);
				if (ImGui::Selectable(Base::SRU_Data::g_countryList[i].name.c_str(), isSelected))
				{
					g_paintSelectedComboCountry = i;
				}
			}
			ImGui::EndCombo();
		}

		if (paintTargetCountry)
		{
			ImGui::EndDisabled();
		}

		static bool customLoyalty = false;

		ImGui::Checkbox("Custom Loyalty", &customLoyalty);

		if (!customLoyalty)
		{
			ImGui::BeginDisabled();

			for (int i = 0; i < g_countryList.size(); i++)
			{
				if (g_countryList[i].oId == hexLoyalty)
				{
					g_paintSelectedComboLoyalty = i;
					break;
				}
			}
		}

		if (ImGui::BeginCombo("##loyaltycombo", g_countryList[g_paintSelectedComboLoyalty].name.c_str()))
		{
			for (int i = 0; i < g_countryList.size(); i++)
			{
				const bool isSelected = (g_paintSelectedComboLoyalty == i);
				if (ImGui::Selectable(Base::SRU_Data::g_countryList[i].name.c_str(), isSelected))
				{
					g_paintSelectedComboLoyalty = i;
				}
			}
			ImGui::EndCombo();
		}

		if (!customLoyalty)
		{
			ImGui::EndDisabled();
		}

		static bool customGround = false;
		std::string groundText = "Unchanged";

		ImGui::Checkbox("Custom Ground", &customGround);

		if (!customGround)
		{
			ImGui::BeginDisabled();
			g_paintSelectedComboGround = -1;
		}
		else
		{
			if (g_paintSelectedComboGround == -1)
			{
				g_paintSelectedComboGround = 0;
			}

			groundText = g_groundTypeList[g_paintSelectedComboGround].name;
		}

		if (ImGui::BeginCombo("##groundcombo", groundText.c_str()))
		{
			for (int i = 0; i < g_groundTypeList.size(); i++)
			{
				const bool isSelected = (g_paintSelectedComboGround == i);
				if (ImGui::Selectable(g_groundTypeList[i].name.c_str(), isSelected))
				{
					g_paintSelectedComboGround = i;
				}
			}
			ImGui::EndCombo();
		}

		if (!customGround)
		{
			ImGui::EndDisabled();
		}
	}
	ImGui::EndChild();
}
