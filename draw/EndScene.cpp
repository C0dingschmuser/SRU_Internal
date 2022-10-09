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
						if (ImGui::BeginTabItem("Unit modifiers"))
						{
							Draw::DrawSelectedCountryText(cc, "Selected country: %s");

							ImGui::BeginChild("##unitmodifierschild", ImVec2(225, 150), true);
							{
								Unit::Property p;
								int stateChange = -1;

								if (ImGui::Checkbox("Invincible", &cc->invincibleUnits))
								{
									p = Unit::Property::Health;
									stateChange = (int)cc->invincibleUnits;
								}

								if (ImGui::Checkbox("Max Supply", &cc->maxSupplyUnits))
								{
									p = Unit::Property::Supply;
									stateChange = (int)cc->maxSupplyUnits;
								}

								if (ImGui::Checkbox("Max Fuel", &cc->maxFuelUnits))
								{
									p = Unit::Property::Fuel;
									stateChange = (int)cc->maxFuelUnits;
								}

								if (ImGui::Checkbox("Max Experience", &cc->maxExperienceUnits))
								{
									p = Unit::Property::Experience;
									stateChange = (int)cc->maxExperienceUnits;
								}

								if (ImGui::Checkbox("Max Morale", &cc->maxMoraleUnits))
								{
									p = Unit::Property::Morale;
									stateChange = (int)cc->maxMoraleUnits;
								}

								if (ImGui::Checkbox("Max Efficiency", &cc->maxEfficiencyUnits))
								{
									p = Unit::Property::Efficiency;
									stateChange = (int)cc->maxEfficiencyUnits;
								}

								if (stateChange == 0)
								{
									//restore
									for (int i = 0; i < cc->allUnits.size(); i++)
									{
										*cc->allUnits[i].properties[(int)p]->valPtr =
											cc->allUnits[i].properties[(int)p]->freezeVal;
									}
								}
								else if (stateChange == 1)
								{
									//save orig
									for (int i = 0; i < cc->allUnits.size(); i++)
									{
										cc->allUnits[i].properties[(int)p]->freezeVal =
											*cc->allUnits[i].properties[(int)p]->valPtr;
									}
								}
							}
							ImGui::EndChild();
							ImGui::SameLine();
							ImGui::BeginChild("##unitmodifierschild2");
							{
								if (ImGui::Checkbox("Lightspeed", &cc->maxMoveSpeedUnitsT))
								{
									if (!cc->maxMoveSpeedUnitsT)
									{
										for (int i = 0; i < cc->allUnits.size(); i++)
										{
											cc->allUnits[i].RestoreDesignProperty(cc, UnitDefault::Property::MoveSpeed);
										}

										*(uintptr_t*)(cc->base + Offsets::countryRailTransport) = (uint8_t)0;
									}
									else
									{
										UnitDefault::HolderValue v;
										v.ui16 = (uint16_t)15000;

										for (int i = 0; i < cc->allUnits.size(); i++)
										{
											cc->allUnits[i].SetDesignProperty(cc, UnitDefault::Property::MoveSpeed, v);
										}

										*(uintptr_t*)(cc->base + Offsets::countryRailTransport) = (uint8_t)2;
									}
								}
							}
							ImGui::EndChild();

							ImGui::Text("Non-highlighted options affect the unit design");
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
					Base::Draw::DrawUnit(cc);

					ImGui::EndTabItem();
				}
				
				if (ImGui::BeginTabItem("Settings"))
				{
					ImGui::BeginChild("##settingscheats", ImVec2(225, 250));
					{
						static bool allunit = false;
						static bool onedaybuild = false;
						static bool breakground = false;
						static bool moreoffers = false;

						{
							uint8_t* ptr = (uint8_t*)(uintptr_t*)(g_base + Offsets::cheatAddr);
							uint8_t val = *ptr;
							uint8_t oldVal = val;

							val = ~val & Offsets::allunit[0] | val & Offsets::allunit[1];
							if (val < oldVal)
							{ //was enabled
								allunit = true;
							}
							else allunit = false;
							val = oldVal;

							val = ~val & Offsets::onedaybuild[0] | val & Offsets::onedaybuild[1];
							if (val < oldVal)
							{
								//was enabled
								onedaybuild = true;
							}
							else onedaybuild = false;
							val = oldVal;

							val = ~val & Offsets::breakground[0] | val & Offsets::breakground[1];
							if (val < oldVal)
							{
								//was enabled
								breakground = true;
							}
							else breakground = false;
							val = oldVal;

							val = ~val & Offsets::moreoffers[0] | val & Offsets::moreoffers[1];
							if (val < oldVal)
							{
								//was enabled
								moreoffers = true;
							}
							else moreoffers = false;
						}

						ImGui::Text("Cheats");
						if (ImGui::Checkbox("All Units", &allunit))
						{
							allunit = !allunit;
							Base::Execute::SetCheat(Offsets::allunit[0]);
						}
						if (ImGui::Checkbox("One day build", &onedaybuild))
						{
							onedaybuild = !onedaybuild;
							Base::Execute::SetCheat(Offsets::onedaybuild[0]);
						}
						if (ImGui::Checkbox("One day facilities", &breakground))
						{
							breakground = !breakground;
							Base::Execute::SetCheat(Offsets::breakground[0]);						 
						}
						if (ImGui::Checkbox("More offers", &moreoffers))
						{
							moreoffers = !moreoffers;
							Base::Execute::SetCheat(Offsets::moreoffers[0]);						 
						}
						
						ImGui::EndChild();
					}
					ImGui::SameLine();
					ImGui::BeginChild("##settings");
					{
						ImGui::Text("Settings");	
						ImGui::Text("Minimum global hex supply");
						ImGui::SliderByte("##hexsupplyslider", &Asm::g_lowestHexSupply, 0, 255);
						ImGui::Checkbox("Let AI create colonies", &g_aiColony);
						if (ImGui::Checkbox("[EXPERIMENTAL]\nNo automatic production adjustment", &g_productionAdjustment))
						{
							Base::SRU_Data::Hooks::SetProductionAdjustment(g_productionAdjustment);
						}
						ImGui::EndChild();
					}

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
			else
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