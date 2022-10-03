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
					Base::Draw::DrawUnit(cc);

					ImGui::EndTabItem();
				}
				
				if (ImGui::BeginTabItem("Settings"))
				{
					ImGui::Text("Minimum global hex supply");
					ImGui::SliderByte("##hexsupplyslider", &Asm::g_lowestHexSupply, 0, 255);
					ImGui::Checkbox("Let AI create colonies", &g_aiColony);
					if (ImGui::Checkbox("[EXPERIMENTAL] Disable automatic production adjustment", &g_productionAdjustment))
					{
						Base::SRU_Data::Hooks::SetProductionAdjustment(g_productionAdjustment);
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