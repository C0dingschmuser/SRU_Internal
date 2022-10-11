#include "../Base.h"

using namespace Base::SRU_Data;

void Base::Draw::DrawUnitSpawn(Base::SRU_Data::Country* cc)
{
	if (ImGui::BeginTabItem("Spawn"))
	{
		Draw::DrawSelectedCountryText(cc, "Clicked country: %s");
		ImGui::BeginChild("##UnitSpawnData", ImVec2(225, 250));
		{
			static bool spawnClickedCountry = true;
			ImGui::Checkbox("Spawn for clicked Country", &spawnClickedCountry);

			if (spawnClickedCountry)
			{
				ImGui::BeginDisabled();
				unitSpawnCountry = cc;
			}

			if (ImGui::BeginCombo("##countrycombo", unitSpawnCountry->name.c_str()))
			{
				for (int i = 0; i < g_countryList.size(); i++)
				{
					const bool isSelected = (unitSpawnCountry->base == g_countryList[i].base);
					if (ImGui::Selectable(Base::SRU_Data::g_countryList[i].name.c_str(), isSelected))
					{
						unitSpawnCountry = &g_countryList[i];
					}
				}
				ImGui::EndCombo();
			}

			if (spawnClickedCountry)
			{
				ImGui::EndDisabled();
			}

			static bool onlyUseOwnDesigns = true;
			bool enter = ImGui::Checkbox("Only show used Designs", &onlyUseOwnDesigns);

			if (g_unitSpawnSelectedUnitDesign == -1 || enter)
			{
				//default-> init

				for (int i = 0; i < g_defaultUnitList.size(); i++)
				{
					if (g_defaultUnitList[i]->HasUser(unitSpawnCountry->oId))
					{
						g_unitSpawnSelectedUnitDesign = i;
						break;
					}
				}

				if (g_unitSpawnSelectedUnitDesign == -1)
				{
					//Backup
					g_unitSpawnSelectedUnitDesign = 0;
				}
			}

			static int unitCount = 1;
			ImGui::Text("Amount");
			if (ImGui::InputInt("##spawnunitamount", &unitCount))
			{
				unitCount = std::clamp(unitCount, 1, 2048);
			}

			static bool spawnReserve = false;
			ImGui::Checkbox("Spawn in reserve", &spawnReserve);

			static bool useClickedPos = true;
			ImGui::Checkbox("Use clicked Position", &useClickedPos);

			if (spawnReserve || useClickedPos)
			{
				ImGui::BeginDisabled();
			}

			ImGui::Text("X Pos            Y Pos");

			static int xPos = 0, yPos = 0;

			if (useClickedPos)
			{
				xPos = Asm::g_xPos;
				yPos = Asm::g_yPos;
			}

			ImGui::PushItemWidth(112);
			if (ImGui::InputInt("##inputX", &xPos, 0, 0))
			{
				xPos = std::clamp(xPos, 0, 9999);
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(112);
			if (ImGui::InputInt("##inputY", &yPos, 0, 0))
			{
				yPos = std::clamp(yPos, 0, 9999);
			}

			if (spawnReserve || useClickedPos)
			{
				ImGui::EndDisabled();
			}

			/*ImGui::Text("Spread");

			static int spread = 1;

			ImGui::SliderInt("##spawnunitspreadslider", &spread, 1, 20, "%d Hex(es)");
			ImGui::SameLine();*/

			if (ImGui::Button("Spawn", ImVec2(112, 0)))
			{
				int unitDesign = g_defaultUnitList[g_unitSpawnSelectedUnitDesign]->spawnId;
				uintptr_t country = unitSpawnCountry->base;

				Base::Execute::SpawnUnit(unitDesign, unitCount, country, 1, spawnReserve, xPos, yPos);
			}

			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("##Unitspawndata2");
			{
				static bool textSearch = false;

				ImGui::Text("Unit Type");
				if (ImGui::BeginListBox("##unitdsignlist", ImVec2(275, 175)))
				{
					for (int i = 0; i < g_defaultUnitList.size(); i++)
					{
						bool ok = !onlyUseOwnDesigns;

						if (!ok)
						{
							//Only use deployed country designs
							if (g_defaultUnitList[i]->HasUser(unitSpawnCountry->oId))
							{
								ok = true;
							}
						}

						if (textSearch && ok)
						{
							if (!g_defaultUnitList[i]->flag)
							{
								ok = false;
							}
						}

						if (ok)
						{
							const bool isSelected = (g_unitSpawnSelectedUnitDesign == i);
							if (ImGui::Selectable(g_defaultUnitList[i]->name.c_str(), isSelected))
							{
								g_unitSpawnSelectedUnitDesign = i;
							}

							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
					}
					ImGui::EndListBox();
				}

				static char* searchVal = (char*)malloc(sizeof(char) * 256);
				static bool searchCleared = false;

				if (!searchCleared)
				{
					for (int i = 0; i < 256; i++)
					{
						memcpy((searchVal + i), "", 1);
					}
					searchCleared = true;
				}

				ImGui::Text("Search");
				ImGui::PushItemWidth(275);
				if (ImGui::InputText("##unitdesignsearch", searchVal, 256, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					textSearch = true;

					//reset
					for (int i = 0; i < g_defaultUnitList.size(); i++)
					{
						g_defaultUnitList[i]->flag = false;
					}

					std::string input = std::string(searchVal);
					std::transform(input.begin(), input.end(), input.begin(), ::tolower);

					if (input.length() == 0)
					{
						textSearch = false;
					}
					else
					{
						for (int i = 0; i < g_defaultUnitList.size(); i++)
						{
							std::string main = g_defaultUnitList[i]->name;
							std::transform(main.begin(), main.end(), main.begin(), ::tolower);

							if (main.find(input) != std::string::npos)
							{
								g_defaultUnitList[i]->flag = true;
							}
						}
					}
				}
				ImGui::EndChild();
			}
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Selected"))
	{

		ImGui::EndTabItem();
	}
	
}