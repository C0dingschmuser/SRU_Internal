#include "../Base.h"

using namespace Base::SRU_Data;

long __stdcall Base::Hooks::hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	using namespace Base::SRU_Data;

    if (!Base::Data::init)
    {
        D3DDEVICE_CREATION_PARAMETERS params;
        pDevice->GetCreationParameters(&params);

        ImGui::CreateContext();
        ImGui_ImplWin32_Init(params.hFocusWindow);
        ImGui_ImplDX9_Init(pDevice);

        Base::Data::init = true;
    }

    if (!Base::Data::init) return Base::Data::oEndScene(pDevice);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	if (Data::ShowMenu)
	{
		if (g_paintEnabled && !g_paintActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
		}
		
		if (g_paintActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		}

		ImGui::Begin("SRU Internal", 0, ImGuiWindowFlags_NoResize);

		if (g_paintActive || g_paintEnabled)
		{
			ImGui::PopStyleColor();
		}

		ImGui::SetWindowSize(ImVec2(525, 350));

		if (g_ingame && clickedCountry)
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("##tabs", tab_bar_flags))
			{
				Country* cc = clickedCountry;

				if (ImGui::BeginTabItem("Country"))
				{
					if (cc->base > 0 && cc->real)
					{
						ImGui::BeginTabBar("##countrytabs", tab_bar_flags);
						if (ImGui::BeginTabItem("Main"))
						{
							Draw::DrawCountry(cc);
							ImGui::EndTabItem();
						}

						static int treatyMsg = 0;
						if (ImGui::BeginTabItem("Diplomacy"))
						{
							Draw::DrawCountryDiplo(cc, treatyMsg);
							ImGui::EndTabItem();
						}
						else
						{
							treatyMsg = 0;
						}
						if (ImGui::BeginTabItem("Units"))
						{
							Draw::DrawSelectedCountryText(cc, "Selected country: %s");
							ImGui::EndTabItem();
						}

						ImGui::EndTabBar();
					}
					else
					{
						ImGui::Text("No country selected");
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Map"))
				{
					Base::Draw::DrawMap(cc);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Units"))
				{
					ImGui::BeginTabBar("##unittabs");
					{
						if (ImGui::BeginTabItem("Spawn"))
						{
							ImGui::BeginChild("##UnitSpawnData", ImVec2(225, 275));
							{
								Draw::DrawSelectedCountryText(cc, "Clicked country: %s");

								static bool spawnClickedCountry = true;
								ImGui::Checkbox("Spawn for clicked Country", &spawnClickedCountry);

								if (spawnClickedCountry)
								{
									ImGui::BeginDisabled();

									g_unitSpawnSelectedCountry = g_clickedCountryRaw;
								}

								if (ImGui::BeginCombo("##countrycombo", g_countryList[g_unitSpawnSelectedCountry].name.c_str()))
								{
									for (int i = 0; i < g_countryList.size(); i++)
									{
										const bool isSelected = (g_unitSpawnSelectedCountry == i);
										if (ImGui::Selectable(Base::SRU_Data::g_countryList[i].name.c_str(), isSelected))
										{
											g_unitSpawnSelectedCountry = i;
										}
									}
									ImGui::EndCombo();
								}

								if (spawnClickedCountry)
								{
									ImGui::EndDisabled();
								}
								
								static bool onlyUseOwnDesigns = true;
								bool enter = ImGui::Checkbox("Only show deployed Designs", &onlyUseOwnDesigns);

								if (g_unitSpawnSelectedUnitDesign == -1 || enter)
								{
									//default-> init

									for (int i = 0; i < g_defaultUnitList.size(); i++)
									{
										if (g_defaultUnitList[i]->HasUser(g_countryList[g_unitSpawnSelectedCountry].oId))
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

								ImGui::Text("X Pos");
								ImGui::SameLine();
								ImGui::Text("Y Pos");
								
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

								if (ImGui::Button("Spawn"))
								{
									int unitDesign = g_defaultUnitList[g_unitSpawnSelectedUnitDesign]->spawnId;
									uintptr_t country = g_countryList[g_unitSpawnSelectedCountry].base;

									Base::Execute::SpawnUnit(unitDesign, unitCount, country, spawnReserve, xPos, yPos);
								}

								ImGui::EndChild();
								ImGui::SameLine();
								ImGui::BeginChild("##Unitspawndata2");
								{
									static bool textSearch = false;

									ImGui::Text("Unit Type");
									if (ImGui::BeginListBox("##unitdsignlist", ImVec2(275, 200)))
									{
										for (int i = 0; i < g_defaultUnitList.size(); i++)
										{
											bool ok = !onlyUseOwnDesigns;

											if (!ok)
											{
												//Only use deployed country designs
												if (g_defaultUnitList[i]->HasUser(g_countryList[g_unitSpawnSelectedCountry].oId))
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
					ImGui::EndTabBar();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Selected Units"))
				{
					std::string text = "Selected Units: " + std::to_string(g_unitEntityCountSelected);

					ImGui::Text(text.c_str());

					for (int i = 0; i < g_selectedUnitList.size(); i++)
					{
						//ImGui::Text(std::to_string((int)*g_selectedUnitList[i].country).c_str());
					}

					ImGui::EndTabItem();
				}
				
				if (ImGui::BeginTabItem("Settings"))
				{
					
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		else
		{
			if (!g_ingame && !clickedCountry)
			{
				ImGui::Text("Waiting for active Game...");
			}
			else if (clickedCountry)
			{
				ImGui::Text("Click on Map to initialize...");
			}
		}
		ImGui::End();
	}

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	
    return Base::Data::oEndScene(pDevice);
}

long __stdcall Base::Hooks::hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    long result = Base::Data::oReset(pDevice, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}