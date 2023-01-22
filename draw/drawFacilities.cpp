#include "../base.h"

void Base::Draw::DrawFacilities(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	g_paintMode = Paint_Land;

	if (ImGui::BeginTabItem("Spawn"))
	{
		DrawSelectedCountryText(cc, "Selected country: %s");

		g_paintMode = Paint_Facility;

		ImGui::BeginChild("##FacilitySpawnData", ImVec2(225, 240));
		{
			static bool useClickedPos = true;

			ImGui::Checkbox("Spawn fully constructed", &g_facilitySpawnConstruction);

			ImGui::Text("Amount\n (Only for supported facilities)");
			ImGui::SliderInt("##facilityamount", &g_facilitySpawnCount, 1, 6);

			if (ImGui::TreeNode("Position"))
			{
				ImGui::Checkbox("Use clicked Position", &useClickedPos);

				if (useClickedPos)
				{
					ImGui::BeginDisabled();
				}

				ImGui::Text("X Pos   Y Pos");

				if (useClickedPos)
				{
					g_facilitySpawnXPos = *g_clickedXPtr;
					g_facilitySpawnYPos = *g_clickedYPtr;
				}

				ImGui::PushItemWidth(50);
				if (ImGui::InputInt("##inputX", &g_facilitySpawnXPos, 0, 0))
				{
					g_facilitySpawnXPos = std::clamp(g_facilitySpawnXPos, 0, 9999);
				}
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				if (ImGui::InputInt("##inputY", &g_facilitySpawnYPos, 0, 0))
				{
					g_facilitySpawnYPos = std::clamp(g_facilitySpawnYPos, 0, 9999);
				}

				if (useClickedPos)
				{
					ImGui::EndDisabled();
				}

				ImGui::TreePop();
			}
			else
			{
				if (useClickedPos)
				{
					g_facilitySpawnXPos = *g_clickedXPtr;
					g_facilitySpawnYPos = *g_clickedYPtr;
				}
			}

			if (g_facilitySpawnSelectedFacility == -1)
			{
				ImGui::BeginDisabled();
			}

			if (ImGui::Button("Spawn", ImVec2(112, 0)))
			{
				float constructionState = 0;

				if (g_facilitySpawnConstruction)
					constructionState = 1.0f;

				for (int i = 0; i < g_facilitySpawnCount; i++)
				{
					Base::Execute::CreateFacility(g_facilitySpawnXPos, g_facilitySpawnYPos, cc->oId, g_facilityList[g_facilitySpawnSelectedFacility]->id, constructionState);
				}
			}

			if (g_facilitySpawnSelectedFacility == -1)
			{
				ImGui::EndDisabled();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##FacilitySpawnList", ImVec2(0, 240));
		{
			static bool textSearch = false;

			ImGui::Text("Facility List");
			if (ImGui::BeginListBox("##facility", ImVec2(275, 175)))
			{

				for (int i = 0; i < g_facilityList.size(); i++)
				{
					bool ok = true;

					if (textSearch)
					{
						if (!g_facilityList[i]->flag)
						{
							ok = false;
						}
					}

					if (ok)
					{
						const bool isSelected = (g_facilitySpawnSelectedFacility == i);
						if (ImGui::Selectable(g_facilityList[i]->name.c_str(), isSelected))
						{
							g_facilitySpawnSelectedFacility = i;
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
			if (ImGui::InputText("##facilitysearch", searchVal, 256, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				textSearch = true;

				//reset
				for (int i = 0; i < g_facilityList.size(); i++)
				{
					g_facilityList[i]->flag = false;
				}

				std::string input = std::string(searchVal);
				std::transform(input.begin(), input.end(), input.begin(), ::tolower);

				if (input.length() == 0)
				{
					textSearch = false;
				}
				else
				{
					for (int i = 0; i < g_facilityList.size(); i++)
					{
						std::string main = g_facilityList[i]->name;
						std::transform(main.begin(), main.end(), main.begin(), ::tolower);

						if (main.find(input) != std::string::npos)
						{
							g_facilityList[i]->flag = true;
						}
					}
				}
			}
		}
		ImGui::EndChild();

		ImGui::Text("Or toggle paint mode with capslock and hold ctrl");

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Manage"))
	{
		DrawSelectedCountryText(cc, "Selected country: %s");

		static std::vector <Base::SRU_Data::AddressHolder> hexFacilityList;
		static std::string posStr;
		static uintptr_t* tileAddr;
		static uintptr_t* rootAddr;
		static int selectedPaintMode = 0;

		g_paintMode = (PaintMode)selectedPaintMode + 6;

		static std::vector<std::string> modeList = {
			"Destroy",
			"Disable",
			"Enable",
		};

		if (g_newClick) {
			g_newClick = false;

			//Refresh Facilities
			
			hexFacilityList.clear();

			posStr = "Facilities on Hex (X: " + std::to_string(*g_clickedXPtr) + ", Y: " + std::to_string(*g_clickedYPtr) + ")";

			tileAddr = Base::Execute::GetTileAddr(*g_clickedXPtr, *g_clickedYPtr);
			rootAddr = Base::Execute::GetFacilityRoot(*g_clickedXPtr, *g_clickedYPtr);
			hexFacilityList = Base::Execute::GetFacilities(rootAddr);
		}

		ImGui::Text(posStr.c_str());

		ImGui::BeginChild("##hexfacilitydisplay", ImVec2(510, 210));
		{
			if (hexFacilityList.size() == 0) {
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
				ImGui::Text("No facilities on hex");
				ImGui::PopStyleColor();
			}
			else {
				for (int i = 0; i < hexFacilityList.size(); i++) {
					
					if (hexFacilityList[i].id == -1)
					{
						continue;
					}

					std::string ident = "##hexFacility" + std::to_string(i);

					float* health = (float*)(hexFacilityList[i].addr + 0x74);

					ImGui::BeginChild(ident.c_str(), ImVec2(160, 77), true);
					{
						ImGui::Text(hexFacilityList[i].name.c_str());
						std::string identBtn = "Destroy##" + std::to_string(i);
						if (ImGui::Button(identBtn.c_str(), ImVec2(65, 20))) {
							g_newClick = true;
							
							Base::Execute::DestroyFacility(rootAddr, hexFacilityList, i);
						}
						
						ImGui::SameLine();
						
						identBtn = "On##" + std::to_string(i);
						if (ImGui::Button(identBtn.c_str(), ImVec2(30, 20)))
						{
							Base::Execute::DisableFacility(tileAddr, rootAddr, hexFacilityList, i, 0);
						}

						ImGui::SameLine();

						identBtn = "Off##" + std::to_string(i);
						if (ImGui::Button(identBtn.c_str(), ImVec2(30, 20)))
						{
							Base::Execute::DisableFacility(tileAddr, rootAddr, hexFacilityList, i, 1);
						}

						ImGui::PushItemWidth(141);

						std::string identSlider = "##Health" + std::to_string(i);
						std::string data = Base::Utils::FloatToPercent(*health, 1, false);
						ImGui::SliderFloat(identSlider.c_str(), health, 0.0f, 1.0f, data.c_str());
						
						ImGui::PopItemWidth();
					}
					ImGui::EndChild();

					if (i != 2 && i != 5) {
						ImGui::SameLine();
					}
				}
			}
		}
		ImGui::EndChild();

		ImGui::Text("Paint Mode (applies to all on hex under mouse)");
		ImGui::SameLine();

		ImGui::PushItemWidth(80);
		if (ImGui::BeginCombo("##paintmodecombo", modeList[selectedPaintMode].c_str()))
		{
			for (int i = 0; i < modeList.size(); i++)
			{
				const bool isSelected = (selectedPaintMode == i);
				if (ImGui::Selectable(modeList[i].c_str(), isSelected))
				{
					selectedPaintMode = i;
					g_paintMode = (PaintMode)(selectedPaintMode + 6);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		ImGui::EndTabItem();
	}
}
