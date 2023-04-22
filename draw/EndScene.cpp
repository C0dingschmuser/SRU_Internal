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
		static std::string id = "###titletext";
		static std::string titleText = "";
		std::string brushSize = "Brush size: " + std::to_string(g_paintBrushSize);
		
		if (g_autosaving)
		{
			titleText = "SRU Internal | Autosaving...";
		}
		else
		{
			titleText = "SRU Internal";
		}

		if (g_paintEnabled && !g_paintActive)
		{
			//Yellow

			ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(255, 255, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(255, 255, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, IM_COL32(255, 255, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

			titleText = "SRU Internal | " + brushSize;
		}
		
		if (g_paintActive)
		{
			//Green

			ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(0, 255, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(0, 255, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, IM_COL32(0, 255, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

			titleText = "SRU Internal | " + brushSize + " | Painting";
		}

		titleText += id;

		ImGui::Begin(titleText.c_str(), 0, ImGuiWindowFlags_NoResize);

		if (g_paintActive || g_paintEnabled)
		{
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		if (g_ingame == 2)
		{
			ImGui::SetWindowSize(ImVec2(525, 387));
		}
		else
		{
			ImGui::SetWindowSize(ImVec2(350, 50));
		}

		if (g_ingame == 2 && clickedCountry)
		{
			bool disabled = g_autosaving;

			if (disabled)
			{
				ImGui::BeginDisabled();
			}

			ImGui::BeginChild("##mainwindow", ImVec2(525, 327), false);

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

						if (ImGui::BeginTabItem("Ministers"))
						{
							Draw::DrawCountryMinisters(cc);
							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("ROE"))
						{
							Draw::DrawCountryROE(cc);
							ImGui::EndTabItem();
						}

						static bool techWasOpen = false;
						if (ImGui::BeginTabItem("Technologies"))
						{
							if (!techWasOpen)
							{
								techWasOpen = true;

								for (int i = 0; i < g_countryList.size(); i++)
								{
									g_countryList[i].RefreshResearch();
								}
							}

							Draw::DrawCountryTech(cc);
							ImGui::EndTabItem();
						}
						else
						{
							techWasOpen = false;
						}

						static bool designWasOpen = false;
						if (ImGui::BeginTabItem("Unit Designs"))
						{
							if (!designWasOpen)
							{
								designWasOpen = true;

								for (int i = 0; i < g_defaultUnitList.size(); i++)
								{
									g_defaultUnitList[i]->RefreshUserCountrys();
								}
							}

							Draw::DrawCountryDesigns(cc);
							ImGui::EndTabItem();
						}
						else
						{
							designWasOpen = false;
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

				if (ImGui::BeginTabItem("Facilities"))
				{
					ImGui::BeginTabBar("##facilitytabs");
					{
						Base::Draw::DrawFacilities(cc);
					}
					ImGui::EndTabBar();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Units"))
				{
					ImGui::BeginTabBar("##unittabs");
					{
						Base::Draw::DrawUnitSpawn(cc);
						Base::Draw::DrawUnitModifiers(cc);
						Base::Draw::DrawUnitSelected(cc);
					}
					ImGui::EndTabBar();

					ImGui::EndTabItem();
				}
				
				if (ImGui::BeginTabItem("Spheres"))
				{
					Draw::DrawSpheres(cc);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Settings"))
				{
					Draw::DrawSettings();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::EndChild();

			if (disabled)
			{
				ImGui::EndDisabled();
			}

			ImGui::Separator();
			ImGui::Text("Need help? Found a bug? Have an idea or wish? Join my");
			ImGui::SameLine();
			if (ImGui::Button("Discord", ImVec2(100, 17)))
			{
				ShellExecuteA(NULL, "open", "https://discord.gg/dAvpvRGS8d", NULL, NULL, SW_SHOWNORMAL);
			}
		}
		else
		{
			std::string text = "Waiting for active Game...";
			
			if (g_ingame == 1)
			{
				text = "Loading...";
			}
			else if (g_ingame == 2)
			{
				text = "Click on Map to initialize...";
			}
			else if (g_ingame == 3)
			{
				text = "Disabled in Settings";
			}

			ImGui::Text(text.c_str());
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