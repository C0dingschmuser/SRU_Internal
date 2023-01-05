#include "../base.h"

void Base::Draw::DrawFacilities(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	g_paintUnitSpawn = false;

	if (ImGui::BeginTabItem("Spawn"))
	{
		DrawSelectedCountryText(cc, "Selected country: %s");

		g_paintFacilityDestroy = false;
		g_paintFacilitySpawn = true;

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

	if (ImGui::BeginTabItem("Destroy"))
	{
		g_paintFacilitySpawn = false;
		g_paintFacilityDestroy = true;

		DrawSelectedCountryText(cc, "Selected country: %s");
		
		struct tempFacility {
			uintptr_t addr;
			int id;
			std::string name;
		};

		static std::vector<tempFacility> hexFacilityList;
		static std::string posStr;
		static uintptr_t* rootAddr;

		if (g_newClick) {
			g_newClick = false;

			//Refresh Facilities
			
			hexFacilityList.clear();

			posStr = "Facilities on Hex (X: " + std::to_string(*g_clickedXPtr) + ", Y: " + std::to_string(*g_clickedYPtr) + ")";

			__int16 realX = *g_clickedXPtr;
			__int16 realY = *g_clickedYPtr;
			if (realX < 0)
			{
				realX = Base::SRU_Data::g_mapSizeX + realX;
			}
			if (realY < 0) realY = 0;
			DWORD posData = MAKELONG(realX, realY);
			__int16 v9 = (__int16)posData;
			if (v9 >= Base::SRU_Data::g_mapSizeX)
			{
				v9 = (__int16)posData % Base::SRU_Data::g_mapSizeX;
			}
			else if ((posData & 0x8000) != 0)
			{
				v9 = Base::SRU_Data::g_mapSizeX + (__int16)posData % Base::SRU_Data::g_mapSizeX;
			}
			
			long temp = v9 + Base::SRU_Data::g_mapSizeX * HIWORD(posData);
			long mult = 16 * temp;
			
			DWORD base3 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 20) + (4 * temp);

			uintptr_t* addr = (uintptr_t*)base3;
			rootAddr = addr;
			DWORD val = *addr;

			bool ok = true;
			while (ok)
			{
				uint8_t* ptr1 = (uint8_t*)(val + 177);
				DWORD* ptr2 = (DWORD*)(val + 356);
				WORD* ptr3 = (WORD*)(val + 12);
				BYTE* ptr4 = (BYTE*)(val + 179);

				if (!Base::Utils::CanReadPtr(ptr1) || !Base::Utils::CanReadPtr(ptr2) || !Base::Utils::CanReadPtr(ptr3) || !Base::Utils::CanReadPtr(ptr4))
				{
					break;
				}

				uint8_t v1870 = *(uint8_t*)(val + 177);
				DWORD v5012 = *(DWORD*)(val + 356);

				if ((v1870 & 0x20) != 0 && (v1870 & 0x80) == 0 && *(WORD*)(val + 12) && (*(BYTE*)(val + 179) & 0x40) == 0)
				{
					int id = *(uintptr_t*)(val + 0xC);
					
					for (int i = 0; i < g_facilityList.size(); i++) {
						if (g_facilityList[i]->id == id) {
							tempFacility temp;
							temp.addr = val;
							temp.id = id;
							temp.name = g_facilityList[i]->name;
							hexFacilityList.push_back(temp);
							break;
						}
					}
				}
				else break;

				val = v5012;
				if (!v5012) break;
			}
		}

		ImGui::Text(posStr.c_str());

		ImGui::BeginChild("##hexfacilitydisplay", ImVec2(500, 200));
		{
			if (hexFacilityList.size() == 0) {
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
				ImGui::Text("No facilities on hex");
				ImGui::PopStyleColor();
			}
			else {
				for (int i = 0; i < hexFacilityList.size(); i++) {
					std::string ident = "##hexFacility" + std::to_string(i);

					ImGui::BeginChild(ident.c_str(), ImVec2(160, 55), true);
					{
						ImGui::Text(hexFacilityList[i].name.c_str());
						std::string identBtn = "Destroy##" + std::to_string(i);
						if (ImGui::Button(identBtn.c_str(), ImVec2(75, 20))) {
							g_newClick = true;
							
							//Fix root
							if (hexFacilityList[i].addr == *rootAddr) {
								for (int a = 0; a < hexFacilityList.size(); a++) {
									if (a != i) {
										*rootAddr = hexFacilityList[a].addr;
										break;
									}
								}
							}
			
							for (int a = 0; a < hexFacilityList.size(); a++) {
								if (a != i) {
									uintptr_t addr = hexFacilityList[a].addr;
									
									//Fix previous
									if (*(uintptr_t*)(addr + 8) == hexFacilityList[i].addr) {
										if (i > 0) {
											*(uintptr_t*)(addr + 8) = hexFacilityList[i - 1].addr;
										}
										else {
											*(uintptr_t*)(addr + 8) = 0;
										}
									}

									//Fix next
									if (*(uintptr_t*)(addr + 356) == hexFacilityList[i].addr) {
										if (i < hexFacilityList.size() - 1) {
											*(uintptr_t*)(addr + 356) = hexFacilityList[i + 1].addr;
											*(uintptr_t*)(addr + 4) = hexFacilityList[i + 1].addr;
										}
										else {
											*(uintptr_t*)(addr + 356) = 0;
											*(uintptr_t*)(addr + 4) = 0;
										}
									}
								}
							}

							Base::Execute::destroyFactoryFunc(hexFacilityList[i].addr, 0, 0);
						}
					}
					ImGui::EndChild();

					if (i != 2 && i != 5) {
						ImGui::SameLine();
					}
				}
			}
		}
		ImGui::EndChild();

		ImGui::Text("Or toggle paint mode with capslock and hold ctrl to destroy all on Hex");

		ImGui::EndTabItem();
	}
}
