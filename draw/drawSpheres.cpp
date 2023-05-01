#include "../Base.h"

void Base::Draw::DrawSpheres(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	static uint8_t* blueSphereLead = (uint8_t*)(g_base + Offsets::blueSphereLeader);
	static uint8_t* redSphereLead = (uint8_t*)(g_base + Offsets::redSphereLeader);

	int selectedBlue = -1;
	int selectedRed = -1;

	for (int i = 0; i < g_countryList.size(); i++)
	{
		if (g_countryList[i].oId == *blueSphereLead)
		{
			selectedBlue = i;
		}
		else if (g_countryList[i].oId == *redSphereLead)
		{
			selectedRed = i;
		}

		if (selectedBlue > -1 && selectedRed > -1)
		{
			break;
		}
	}

	std::string selectedBlueLead = "None";
	std::string selectedRedLead = "None";

	if (selectedBlue > -1)
	{
		selectedBlueLead = g_countryList[selectedBlue].name;
	}

	if (selectedRed > -1)
	{
		selectedRedLead = g_countryList[selectedRed].name;
	}

	ImGui::Text("Blue Sphere Leader");
	if (ImGui::BeginCombo("##bluespherelead", selectedBlueLead.c_str()))
	{
		for (int i = 0; i < g_countryList.size(); i++)
		{
			bool isSelected = selectedBlue == i;
			if (ImGui::Selectable(g_countryList[i].name.c_str(), isSelected))
			{
				selectedBlue = i;
				*blueSphereLead = g_countryList[i].oId;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Red Sphere Leader");
	if (ImGui::BeginCombo("##redspherelead", selectedRedLead.c_str()))
	{
		for (int i = 0; i < g_countryList.size(); i++)
		{
			bool isSelected = selectedRed == i;
			if (ImGui::Selectable(g_countryList[i].name.c_str(), isSelected))
			{
				selectedRed = i;
				*redSphereLead = g_countryList[i].oId;
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Override 'Blue' Sphere name (or leave empty for default)");
	ImGui::InputText("##spherenameblue", &g_sphereNames[1]);
	ImGui::Text("Override 'Red' Sphere name (or leave empty for default)");
	ImGui::InputText("##spherenamered", &g_sphereNames[0]);
}