#include "../Base.h"

void Base::Draw::DrawUnitSelected(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	if (ImGui::BeginTabItem("Selected"))
	{
		std::string text = "Selected Units: " + std::to_string(g_selectedUnitList.size());
		ImGui::Text(text.c_str());
		ImGui::Separator();
		ImGui::BeginChild("##selectedunitsleft", ImVec2(225, 250));
		{
			bool enabled = true;

			if (!g_selectedUnitList.size())
			{
				enabled = false;
			}

			if (!enabled)
			{
				ImGui::BeginDisabled();
			}

			ImGui::Text("Clone");
			ImGui::Dummy(ImVec2(1, 1));

			static std::string clonePositions[] = {
				"Original",
				"Clicked Hex",
				"Reserve"
			};

			static int clonePosition = 0;

			ImGui::Text("Position");
			if (ImGui::BeginCombo("##unitclone", clonePositions[clonePosition].c_str()))
			{
				for (int i = 0; i < 2; i++)
				{
					const bool isSelected = (clonePosition == i);
					if (ImGui::Selectable(clonePositions[i].c_str(), isSelected))
					{
						clonePosition = i;
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Spawn"))
			{
				uintptr_t country = 0;

				for (int i = 0; i < g_countryList.size(); i++)
				{
					if (g_countryList[i].oId == *g_selectedUnitList[0].countryId)
					{
						country = g_countryList[i].base;
						break;
					}
				}

				for (int i = 0; i < g_selectedUnitList.size(); i++)
				{
					int unitDesign = g_selectedUnitList[i].defaultStats->spawnId;

					bool reserve = false;

					uint16_t x = *g_clickedXPtr;
					uint16_t y = *g_clickedYPtr;

					if (clonePosition == 0)
					{
						x = *g_selectedUnitList[i].xPos;
						y = *g_selectedUnitList[i].yPos;
					}
					else if (clonePosition == 2)
					{
						reserve = true;
					}

					Base::Execute::SpawnUnit(unitDesign, 1, country, 1, reserve, x, y);
				}
			}

			if (!enabled)
			{
				ImGui::EndDisabled();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##selectedunitsright1");
		{

		}
		ImGui::EndChild();

		ImGui::EndTabItem();
	}
}