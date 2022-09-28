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
		if (g_paintActive)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		}

		ImGui::Begin("SRU Internal", 0, ImGuiWindowFlags_NoResize);

		if (g_paintActive)
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
						ImGui::Text("Painting - Hold ctrl to paint");

						static bool paintTargetCountry = true;

						ImGui::Checkbox("Paint Target Country", &paintTargetCountry);

						if (paintTargetCountry)
						{
							ImGui::BeginDisabled();

							g_paintSelectedComboCountry = g_selectedTargetCountry;
						}

						if(ImGui::BeginCombo("##countrycombo", g_countryList[g_paintSelectedComboCountry].name.c_str()))
						{
							for (int i = 0; i < g_countryList.size(); i++)
							{
								const bool isSelected = (g_paintSelectedComboCountry == i);
								if (ImGui::Selectable(Base::SRU_Data::g_countryList[i].name.c_str(), isSelected))
								{
									g_paintSelectedComboCountry = i;
								}
							}
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
						}

						if (!customLoyalty)
						{
							ImGui::EndDisabled();
						}
					}
					ImGui::EndChild();


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
				ImGui::EndTabBar();
			}
			//ImGui::InputInt("countryptr", (int*) & Base::SRU_Data::g_ownCountryBase);
		}
		else
		{
			ImGui::Text("Waiting for active Game...");
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