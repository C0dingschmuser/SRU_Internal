#include "../Base.h"

using namespace Base::SRU_Data;


void Base::Draw::DrawMap(Base::SRU_Data::Country* cc)
{
	Country* target = &g_countryList[g_selectedTargetCountry];

	g_paintUnitSpawn = false;
	g_paintFacilitySpawn = false;
	g_paintFacilityDestroy = false;

	uintptr_t hexBase = *g_clickedHexPtr;
	uint8_t hexOwner = *(uintptr_t*)(hexBase);
	uint8_t hexLoyalty = *(uintptr_t*)(hexBase + Offsets::hexLoyalty);
	uint8_t* hexSupply = (uint8_t*)(hexBase + Offsets::hexSupply);
	uint8_t* hexGround = (uint8_t*)(hexBase + Offsets::hexGround);

	ImGui::BeginTabBar("##maptabs");
	{
		if (ImGui::BeginTabItem("Hex Data"))
		{
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

			ImGui::BeginChild("##MapHexData", ImVec2(225, 240));
			{
				ImGui::Text("Hex Owner:");

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

				ImGui::Text("Set Hex Name\n(Empty + Enter to reset)");

				static uint16_t* hexNameId = (uint16_t*)(hexBase + 0xC);
				static char* newNameStr = (char*)malloc(sizeof(char) * 128);
				static bool nameCleared = false;

				if (g_newClick)
				{
					//Refresh hex name

					g_newClick = false;

					hexBase = *g_clickedHexPtr;
					hexNameId = (uint16_t*)(hexBase + 0xC);

					if (*hexNameId == 65123) {
						//custom -> load name

						for (int i = 0; i < g_hexNameList.size(); i++)
						{
							if (g_hexNameList[i]->base == hexBase)
							{

								strcpy(newNameStr, g_hexNameList[i]->newName.data());
								break;
							}
						}
					}
					else {
						nameCleared = false;
					}
				}

				if (!nameCleared)
				{

					for (int i = 0; i < 128; i++)
					{
						memcpy((newNameStr + i), "", 1);
					}
					nameCleared = true;
				}

				ImGui::PushItemWidth(225);
				if (ImGui::InputText("##newhexname", newNameStr, 128, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (strlen(newNameStr) > 0)
					{
						//Set custom

						std::shared_ptr<Hex> newHexName = nullptr;
						bool found = false;

						for (int i = 0; i < g_hexNameList.size(); i++)
						{
							if (g_hexNameList[i]->base == hexBase)
							{
								newHexName = g_hexNameList[i];
								found = true;
								break;
							}
						}

						if (newHexName == nullptr)
						{
							newHexName = std::make_shared<Hex>();
							newHexName->base = hexBase;
							newHexName->defaultName = *hexNameId;
						}

						newHexName->newName = std::string(newNameStr);

						if (!found)
						{
							g_hexNameList.push_back(newHexName);
						}

						*hexNameId = 65123;
					}
					else {
						//Empty = reset

						int index = -1;

						for (int i = 0; i < g_hexNameList.size(); i++)
						{
							if (g_hexNameList[i]->base == hexBase)
							{
								*hexNameId = g_hexNameList[i]->defaultName;
								index = i;
								break;
							}
						}

						if (index > -1)
						{
							//remove
							g_hexNameList.erase(g_hexNameList.begin() + index);
						}
					}
				}

				ImGui::Text("Set Hex Population (0-250M)");

				uintptr_t* hexPop = (uintptr_t*)(g_base + Offsets::mouseClickHexPopulation);

				static char* newPopValStr = (char*)malloc(sizeof(char) * 256);
				static bool searchCleared = false;

				if (!searchCleared)
				{
					for (int i = 0; i < 256; i++)
					{
						memcpy((newPopValStr + i), "", 1);
					}
					searchCleared = true;
				}

				if (*hexPop) {

					ImGui::PushItemWidth(225);
					if (ImGui::InputText("##newhexpop", newPopValStr, 256, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						int newPopVal = atoi(newPopValStr);

						if (newPopVal <= 0) {
							newPopVal = 0;
						}
						else if (newPopVal > 250000000) {
							newPopVal = 250000000;
						}

						unsigned short newPop = Base::Utils::GetShortPopulationValue(newPopVal);
						*(unsigned short*)*hexPop = newPop;
					}
					ImGui::PopItemWidth();
				}

				ImGui::Text("Hex Supply");

				std::string str = Base::Utils::FloatToPercent((float)*hexSupply, 255.0f);

				ImGui::PushItemWidth(225);
				ImGui::SliderByte("##hexsupplyslider", hexSupply, 0, 255, str.c_str());


				ImGui::Text("Hex Ground");

				ImGui::PushItemWidth(225);
				ImGui::SliderByte("##hexgroundslider", hexGround, 0, 254);
			}
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("##hexdatabuttons");
			{
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

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Hex Painting"))
		{
			DrawSelectedCountryText(cc, "Selected country: %s");
			Draw::DrawSelectedCountryText(target, "Target country: %s");
			ImGui::Text("Shift-Click to change");
			ImGui::Separator();
			
			ImGui::BeginChild("##MapHexPainting", ImVec2(225, 210));
			{
				ImGui::Text("Painting");
				ImGui::Text("- Capslock to enable");
				ImGui::Text("- Hold ctrl to paint");

				if (g_mapSizeX > 0)
				{
					ImGui::Text("Brush size");
					ImGui::SliderInt("##paintbrushsuze", &g_paintBrushSize, 1, 15);
				}

				ImGui::Checkbox("Only paint on territory of\nselected country", &g_paintHexOnlySelected);
			}
			ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild("##MapHexPainting2", ImVec2(275, 210));
			{
				ImGui::BeginTabBar("##paintingtabbar");

				if (ImGui::BeginTabItem("Land"))
				{
					g_paintMode = 0;
					static bool customTargetCountry = false;

					ImGui::Checkbox("Paint target country", &g_paintHexOwner);
					ImGui::Checkbox("Paint target loyalty", &g_paintHexLoyalty);
					ImGui::Separator();

					ImGui::Checkbox("Custom target country", &customTargetCountry);

					if (!customTargetCountry)
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

					if (!customTargetCountry)
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

				if (ImGui::BeginTabItem("Resource"))
				{
					g_paintMode = 2;

					static std::vector<std::string> resourceList = {
						"Agriculture",
						"Rubber",
						"Timber",
						"Petroleum",
						"Coal",
						"Metal Ore",
						"Uranium",
						"Electricity"
					};

					static std::vector<std::string> amountList = {
						"None",
						"Low",
						"Medium",
						"High"
					};

					std::string resourceText = resourceList[g_paintSelectedResource];

					ImGui::Text("Type");
					if (ImGui::BeginCombo("##resourcecombo", resourceText.c_str()))
					{
						for (int i = 0; i < resourceList.size(); i++)
						{
							const bool isSelected = (g_paintSelectedResource == i);
							if (ImGui::Selectable(resourceList[i].c_str(), isSelected))
							{
								g_paintSelectedResource = i;
							}
						}
						ImGui::EndCombo();
					}

					ImGui::Text("Amount");
					ImGui::SliderInt("##resourceamount", &g_paintSelectedResourceAmount, 0, 3, amountList[g_paintSelectedResourceAmount].c_str());

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Infrastructure"))
				{
					g_paintMode = 3;

					ImGui::Text("Building:\n  Ingame road/rail builder is unlocked.\n  You can build anywhere.");
					ImGui::Text("Destroying:\n  Enter Paint Mode to reset all\n  infrastructure under mouse cursor.");

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Unit"))
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


			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
}
