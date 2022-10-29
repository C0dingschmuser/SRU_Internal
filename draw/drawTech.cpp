#include "../Base.h"

void Base::Draw::DrawCountryTech(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	static int selectedTechnologyId = 0;
	static int showCategory = 0;
	static int showFilter = 0;

	Draw::DrawSelectedCountryText(cc, "Selected country: %s");

	ImGui::BeginChild("##TechOptions", ImVec2(225, 225));
	{
		static std::string filter[]{
			"All",
			"Researched",
			"Unresearched",
		};

		ImGui::Text("Filter");
		if (ImGui::BeginCombo("##techfilter", filter[showFilter].c_str()))
		{
			for (int i = 0; i < 3; i++)
			{
				const bool isSelected = (showFilter == i);
				if (ImGui::Selectable(filter[i].c_str(), isSelected))
				{
					showFilter = i;
				}
			}
			ImGui::EndCombo();
		}
		
		static std::string categories[]{
			"All",
			"Warfare Techs",
			"Transportation Techs",
			"Science Techs",
			"Technology Techs",
			"Medical Techs",
			"Society Techs",
		};

		ImGui::Text("Category");
		if (ImGui::BeginCombo("##techcategory", categories[showCategory].c_str()))
		{
			for (int i = 0; i < 7; i++)
			{
				const bool isSelected = (showCategory == i);
				if (ImGui::Selectable(categories[i].c_str(), isSelected))
				{
					showCategory = i;
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##TechList");
	{
		ImGui::Text("Technologies");
		if (ImGui::BeginListBox("##researchedtechlistbox", ImVec2(275, 175)))
		{
			int size = g_techList.size();

			if (showFilter == 1)
			{
				size = cc->technologies.size();
			}

			for (int i = 0; i < size; i++)
			{
				std::shared_ptr<Tech> tech;
				bool ok = true;

				if (showFilter == 1)
				{
					tech = cc->technologies[i];
				}
				else
				{
					tech = g_techList[i];

					if (showFilter == 2)
					{
						for (int a = 0; a < cc->technologies.size(); a++)
						{
							if (cc->technologies[a]->base == tech->base)
							{
								//only show unresearched

								ok = false;
								break;
							}
						}
					}
				}

				if (ok)
				{
					if (showCategory > 0)
					{
						if (tech->category != showCategory)
						{
							ok = false;
						}
					}
				}

				if (ok)
				{
					const bool isSelected = (selectedTechnologyId == i);
					if (ImGui::Selectable(tech->name.c_str(), isSelected))
					{
						selectedTechnologyId = i;
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndListBox();
		}
	}
	ImGui::EndChild();
}