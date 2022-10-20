#include "../Base.h"

using namespace Base::SRU_Data;


void Base::Draw::DrawMap(Base::SRU_Data::Country* cc)
{
	Country* target = &g_countryList[g_selectedTargetCountry];

	g_paintUnitSpawn = false;

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

		std::string str = Base::Utils::FloatToPercent((float)*hexSupply, 255.0f);

		ImGui::PushItemWidth(225);
		ImGui::SliderByte("##hexsupplyslider", hexSupply, 0, 255, str.c_str());


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

		if (g_mapSizeX > 0)
		{
			ImGui::Text("Brush size");
			ImGui::SliderInt("##paintbrushsuze", &g_paintBrushSize, 1, 10);
		}

		ImGui::BeginTabBar("##paintingtabbar");

		if (ImGui::BeginTabItem("Land"))
		{
			g_paintMode = 0;
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
			ImGui::EndTabItem();
		}

		if(ImGui::BeginTabItem("Unit"))
		{
			ImGui::Text("Changes unit stats under cursor");
			g_paintMode = 1;

			static bool customUnitCountry = false;

			ImGui::Checkbox("Only Apply to target Country Units", &g_paintUnitTargetCountry);
			ImGui::Checkbox("Manual Unit Country", &customUnitCountry);

			if (!customUnitCountry)
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

			if (!customUnitCountry)
			{
				ImGui::EndDisabled();
			}

			std::string sliderLabel = "";
			std::string val = "";
			for (int i = 0; i < Unit::Property::MAX; i++)
			{
				if (i == 1) i++; //skip max health

				switch (i)
				{
				default:
				case 0:
					sliderLabel = "Health";
					break;
				case 2:
					sliderLabel = "Fuel";
					break;
				case 3:
					sliderLabel = "Supply";
					break;
				case 4:
					sliderLabel = "Efficiency";
					break;
				case 5:
					sliderLabel = "Experience";
					break;
				case 6:
					sliderLabel = "Morale";
					break;
				}

				ImGui::Text(sliderLabel.c_str());

				if (g_paintUnitModes[i] == -1)
				{
					val = "Don't change";
				}
				else
				{
					val = std::to_string(g_paintUnitModes[i]) + "%%";
				}

				sliderLabel = "##paint" + sliderLabel;
				ImGui::SliderInt(sliderLabel.c_str(), &g_paintUnitModes[i], -1, 100, val.c_str());
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::EndChild();
}
