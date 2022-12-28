#include "../base.h"

void Base::Draw::DrawFacilities(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	DrawSelectedCountryText(cc, "Selected country: %s");

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
}