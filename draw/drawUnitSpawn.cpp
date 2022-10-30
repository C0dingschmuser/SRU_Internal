#include "../Base.h"

using namespace Base::SRU_Data;

void Base::Draw::DrawUnitSpawn(Base::SRU_Data::Country* cc)
{
	if (ImGui::BeginTabItem("Spawn"))
	{
		g_paintUnitSpawn = true;

		static int showCategory = 0;

		static std::vector<std::vector<std::string>> categories;
		static bool categoriesInitialized = false;

		static std::vector<std::string> landTypes = {
			"All",
			"Infantry",
			"Recon",
			"Tank",
			"Anti-Tank",
			"Artillery",
			"Air Defense",
			"Transport"
		};

		static std::vector<std::string> seaTypes = {
			"All",
			"Submarines",
			"Carriers",
			"Capital Ships",
			"Escort Ships",
			"Patrol Ships",
			"Transport"
		};

		static std::vector<std::string> airTypes = {
			"All",
			"Helicopter",
			"Interceptor",
			"Tactical Bomber",
			"Multi-Role",
			"Strategic Bomber",
			"Patrol",
			"Transport"
		};

		if (!categoriesInitialized)
		{
			categoriesInitialized = true;
			categories.clear();

			categories.push_back(landTypes);
			categories.push_back(seaTypes);
			categories.push_back(airTypes);
		}

		Draw::DrawSelectedCountryText(cc, "Clicked country: %s");
		ImGui::BeginChild("##UnitSpawnData", ImVec2(225, 240));
		{
			static bool spawnClickedCountry = true;
			ImGui::Checkbox("Spawn for clicked Country", &spawnClickedCountry);

			if (spawnClickedCountry)
			{
				//ImGui::BeginDisabled();
				unitSpawnCountry = cc;
			}
			else
			{
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
			}

			static std::string unitDesignTypes[]{
				"All",
				"Land",
				"Sea",
				"Air"
			};

			if (ImGui::BeginCombo("##unitdesigntype", unitDesignTypes[g_unitDesignType].c_str()))
			{
				for (int i = 0; i < 4; i++)
				{
					const bool isSelected = (g_unitDesignType == i);
					if (ImGui::Selectable(unitDesignTypes[i].c_str(), isSelected))
					{
						g_unitDesignType = i;
					}
				}
				ImGui::EndCombo();
			}

			std::string preview = "";

			if (g_unitDesignType)
			{
				preview = categories[g_unitDesignType - 1][showCategory];
			}
			else
			{
				//Disable on all
				ImGui::BeginDisabled();
			}

			if (ImGui::BeginCombo("##unitdesigncategory", preview.c_str()))
			{
				for (int i = 0; i < categories[g_unitDesignType - 1].size(); i++)
				{
					const bool isSelected = (showCategory == i);
					if (ImGui::Selectable(categories[g_unitDesignType - 1][i].c_str(), isSelected))
					{
						if (showCategory != i)
						{
							//cc->selectedUnitDesignId = -1;
						}

						showCategory = i;
					}
				}

				ImGui::EndCombo();
			}

			if (g_unitDesignType == 0)
			{
				//Disable on all
				ImGui::EndDisabled();
			}

			static bool onlyUseOwnDesigns = true;
			bool enter = ImGui::Checkbox("Only researched Designs", &onlyUseOwnDesigns);

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

			ImGui::Text("Amount");
			if (ImGui::InputInt("##spawnunitamount", &g_unitSpawnCount))
			{
				g_unitSpawnCount = std::clamp(g_unitSpawnCount, 1, 2048);
			}

			ImGui::Checkbox("Spawn in reserve", &g_unitSpawnReserve);

			static bool useClickedPos = true;

			if (ImGui::TreeNode("Position"))
			{
				ImGui::Checkbox("Use clicked Position", &useClickedPos);

				if (g_unitSpawnReserve || useClickedPos)
				{
					ImGui::BeginDisabled();
				}

				ImGui::Text("X Pos   Y Pos");

				if (useClickedPos)
				{
					g_unitSpawnXPos = *g_clickedXPtr;
					g_unitSpawnYPos = *g_clickedYPtr;
				}

				ImGui::PushItemWidth(50);
				if (ImGui::InputInt("##inputX", &g_unitSpawnXPos, 0, 0))
				{
					g_unitSpawnXPos = std::clamp(g_unitSpawnXPos, 0, 9999);
				}
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				if (ImGui::InputInt("##inputY", &g_unitSpawnYPos, 0, 0))
				{
					g_unitSpawnYPos = std::clamp(g_unitSpawnYPos, 0, 9999);
				}

				if (g_unitSpawnReserve || useClickedPos)
				{
					ImGui::EndDisabled();
				}

				ImGui::TreePop();
			}
			else
			{
				if (useClickedPos)
				{
					g_unitSpawnXPos = *g_clickedXPtr;
					g_unitSpawnYPos = *g_clickedYPtr;
				}
			}

			/*ImGui::Text("Spread");

			static int spread = 1;

			ImGui::SliderInt("##spawnunitspreadslider", &spread, 1, 20, "%d Hex(es)");
			ImGui::SameLine();*/

			if (ImGui::Button("Spawn", ImVec2(112, 0)))
			{
				int unitDesign = g_defaultUnitList[g_unitSpawnSelectedUnitDesign]->spawnId;
				uintptr_t country = unitSpawnCountry->base;

				Base::Execute::SpawnUnit(unitDesign, g_unitSpawnCount, country, 1, g_unitSpawnReserve, g_unitSpawnXPos, g_unitSpawnYPos);
			}

			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("##Unitspawndata2", ImVec2(0, 240));
			{
				static bool textSearch = false;

				ImGui::Text("Unit Design List (Green = Researched)");
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

						if (g_unitDesignType != 0 && ok)
						{
							ok = false;
							
							int unitType = g_defaultUnitList[i]->unitClass;
							switch (g_unitDesignType)
							{
							case 1: //Land
								if (unitType >= 0 && unitType <= 0x6)
								{
									ok = true;
								}
								break;
							case 2: //Sea 
								if (unitType >= 0xF && unitType <= 0x14)
								{
									ok = true;
								}
								break;
							case 3: //Air
								if (unitType >= 0x7 && unitType <= 0xE)
								{
									ok = true;
								}
								break;
							}
						}

						if (ok && showCategory > 0)
						{
							//Category check

							int unitClass = g_defaultUnitList[i]->unitClass;

							if (g_unitDesignType == 1 || g_unitDesignType == 2)
							{
								//Land + Water

								int start = 0;

								if (g_unitDesignType == 2)
								{
									start = 0xF;
								}

								if (unitClass != start + showCategory - 1)
								{
									ok = false;
								}
							}
							else if (g_unitDesignType == 3)
							{
								//Air

								if (showCategory == 1 && unitClass != 7)
								{
									ok = false;
								}
								else if (showCategory > 1)
								{
									int start = 9;

									if (unitClass != start + showCategory - 1)
									{
										ok = false;
									}
								}
							}
						}

						if (ok)
						{
							bool color = false;
							if (g_defaultUnitList[i]->HasUser(unitSpawnCountry->oId))
							{
								ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
								color = true;
							}

							const bool isSelected = (g_unitSpawnSelectedUnitDesign == i);
							if (ImGui::Selectable(g_defaultUnitList[i]->name.c_str(), isSelected))
							{
								g_unitSpawnSelectedUnitDesign = i;
							}

							if (color)
							{
								ImGui::PopStyleColor();
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

				ImGui::Text("Search (Empty + Enter to reset)");
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

			ImGui::Text("Or toggle paint mode with capslock and hold ctrl");
		}
		ImGui::EndTabItem();
	}	
}