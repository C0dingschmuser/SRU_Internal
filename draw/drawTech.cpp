#include "../Base.h"

bool HasTechUnlocked(Base::SRU_Data::Country* cc, int techId)
{
	using namespace Base::SRU_Data;

	if (techId == -1)
	{
		return false;
	}

	bool unlocked = false;

	std::shared_ptr<Tech> tech = g_techList[techId];

	for (int i = 0; i < cc->technologies.size(); i++)
	{
		if (cc->technologies[i]->base == tech->base)
		{
			unlocked = true;
			break;
		}
	}

	return unlocked;
}

bool HasDesignUnlocked(Base::SRU_Data::Country* cc, int designId)
{
	using namespace Base::SRU_Data;

	if (designId == -1)
	{
		return false;
	}

	bool unlocked = false;

	std::shared_ptr<UnitDefault> design = g_defaultUnitList[designId];

	for (int i = 0; i < design->countryIds.size(); i++)
	{
		if (design->countryIds[i] == cc->oId)
		{
			unlocked = true;
			break;
		}
	}

	return unlocked;
}

void Unlock(Base::SRU_Data::Country* cc, int id, bool tech, bool lock)
{
	if (tech)
	{
		Base::Execute::UnlockTech(cc->oId, id, lock);
	}
	else
	{
		Base::Execute::UnlockDesign(cc->oId, id, lock);
	}
}

void UnlockAll(Base::SRU_Data::Country* cc, bool tech, bool lock)
{
	using namespace Base::SRU_Data;

	if (tech)
	{
		for (int i = 0; i < g_techList.size(); i++)
		{
			Unlock(cc, g_techList[i]->id, true, lock);
		}
	}
	else
	{
		for (int i = 0; i < g_defaultUnitList.size(); i++)
		{
			Unlock(cc, g_defaultUnitList[i]->spawnId, false, lock);
		}
	}
}

void Base::Draw::DrawCountryTech(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	static int showCategory = 0;
	static int showFilter = 0;
	static bool textSearch = false;
	static bool checkedUnlock = false;
	static bool unlocked = false;

	static std::shared_ptr<Tech> selectedPtr;

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
					if (showFilter != i)
					{
						cc->selectedTechnologyId = -1;

						/*if (i == 1)
						{
							unlocked = true;
						}
						else
						{
							unlocked = HasTechUnlocked(cc, cc->selectedTechnologyId);
						}*/
					}

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
					if (showCategory != i)
					{
						cc->selectedTechnologyId = -1;

						/*if (i == 1)
						{
							unlocked = true;
						}
						else
						{
							unlocked = HasTechUnlocked(cc, cc->selectedTechnologyId);
						}*/
					}

					showCategory = i;
				}
			}
			ImGui::EndCombo();
		}

		if (!checkedUnlock)
		{
			checkedUnlock = true;

			if (showFilter != 1)
			{
				unlocked = HasTechUnlocked(cc, cc->selectedTechnologyId);
			}
			else unlocked = true;
		}

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();

		if (!unlocked || cc->selectedTechnologyId == -1)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Lock", ImVec2(112, 0)))
		{	
			if (selectedPtr)
			{
				unlocked = false;

				Unlock(cc, selectedPtr->id, true, true);
			}
		}

		if (!unlocked || cc->selectedTechnologyId == -1)
		{
			ImGui::EndDisabled();
		}

		ImGui::SameLine();

		if (unlocked || cc->selectedTechnologyId == -1)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Unlock", ImVec2(112, 0)))
		{
			if (selectedPtr)
			{
				unlocked = true;

				Unlock(cc, selectedPtr->id, true, false);
			}
		}

		if (unlocked || cc->selectedTechnologyId == -1)
		{
			ImGui::EndDisabled();
		}

		if (ImGui::TreeNode("Lock/Unlock All"))
		{
			if (ImGui::Button("Lock All", ImVec2(85, 0)))
			{
				UnlockAll(cc, true, true);
			}
			ImGui::SameLine();
			if (ImGui::Button("Unlock All", ImVec2(85, 0)))
			{
				UnlockAll(cc, true, false);
			}

			ImGui::TreePop();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##TechList");
	{
		ImGui::Text("Technologies (Green = Researched)");
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

				if (textSearch && ok)
				{
					if (!g_techList[i]->flag)
					{
						ok = false;
					}
				}

				if (ok)
				{
					bool color = false;

					if (showFilter != 1)
					{
						if (HasTechUnlocked(cc, i))
						{
							ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
							color = true;
						}
					}
					else
					{ //All researched
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
						color = true;
					}


					const bool isSelected = (cc->selectedTechnologyId == i);
					if (ImGui::Selectable(tech->name.c_str(), isSelected))
					{
						cc->selectedTechnologyId = i;
						selectedPtr = tech;
						checkedUnlock = false; //reset
					}

					if (color)
					{
						ImGui::PopStyleColor();
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
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
		if (ImGui::InputText("##techsearch", searchVal, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			textSearch = true;
			cc->selectedTechnologyId = -1;

			for (int i = 0; i < g_techList.size(); i++)
			{ //reset
				g_techList[i]->flag = false;
			}

			std::string input = std::string(searchVal);
			std::transform(input.begin(), input.end(), input.begin(), ::tolower);

			if (input.length() == 0)
			{
				textSearch = false;
			}
			else
			{
				for (int i = 0; i < g_techList.size(); i++)
				{
					std::string main = g_techList[i]->name;
					std::transform(main.begin(), main.end(), main.begin(), ::tolower);

					if (main.find(input) != std::string::npos)
					{
						g_techList[i]->flag = true;
					}
				}
			}
		}
	}
	ImGui::EndChild();
}

void Base::Draw::DrawCountryDesigns(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	static int showFilter = 0;
	static int showArea = 0;
	static int showCategory = 0;
	static bool textSearch = false;
	static bool checkedUnlock = false;
	static bool unlocked = false;

	static std::shared_ptr<UnitDefault> selectedPtr;

	Draw::DrawSelectedCountryText(cc, "Selected country: %s");

	ImGui::BeginChild("##countrydesigns", ImVec2(225, 235));
	{
		static std::string filter[]{
			"All",
			"Researched",
			"Unresearched",
		};

		ImGui::Text("Filter");
		if (ImGui::BeginCombo("##unitdesignfilter", filter[showFilter].c_str()))
		{
			for (int i = 0; i < 3; i++)
			{
				const bool isSelected = (showFilter == i);
				if (ImGui::Selectable(filter[i].c_str(), isSelected))
				{
					if (showFilter != i)
					{
						cc->selectedUnitDesignId = -1;

						/*if (i == 1)
						{
							unlocked = true;
						}
						else
						{
							unlocked = HasTechUnlocked(cc, cc->selectedUnitDesignId);
						}*/
					}

					showFilter = i;
				}
			}
			ImGui::EndCombo();
		}

		static std::string areas[]{
			"All",
			"Land",
			"Sea",
			"Air"
		};

		ImGui::Text("Area");
		if (ImGui::BeginCombo("##unitdesignarea", areas[showArea].c_str()))
		{
			for (int i = 0; i < 4; i++)
			{
				const bool isSelected = (showArea == i);
				if (ImGui::Selectable(areas[i].c_str(), isSelected))
				{
					if (showArea != i)
					{
						cc->selectedUnitDesignId = -1;
						showCategory = 0;
					}

					showArea = i;
				}
			}

			ImGui::EndCombo();
		}

		static std::vector<std::vector<std::string>> categories;
		static bool categoriesInitialized = false;

		static std::vector<std::string> landTypes = {
			"All",
			"Infantry",
			"Recon",
			"Tank",
			"Anti-Tank",
			"Artillery",
			"Air Defense",
			"Transport"
		};

		static std::vector<std::string> seaTypes = {
			"All",
			"Submarines",
			"Carriers",
			"Capital Ships",
			"Escort Ships",
			"Patrol Ships",
			"Transport"
		};

		static std::vector<std::string> airTypes = {
			"All",
			"Helicopter",
			"Interceptor",
			"Tactical Bomber",
			"Multi-Role",
			"Strategic Bomber",
			"Patrol",
			"Transport"
		};

		if (!categoriesInitialized)
		{
			categoriesInitialized = true;
			categories.clear();

			categories.push_back(landTypes);
			categories.push_back(seaTypes);
			categories.push_back(airTypes);
		}

		ImGui::Text("Category");

		std::string preview = "";

		if (showArea)
		{
			preview = categories[showArea - 1][showCategory];
		}
		else
		{
			//Disable on all
			ImGui::BeginDisabled();
		}

		if (ImGui::BeginCombo("##unitdesigncategory", preview.c_str()))
		{
			for (int i = 0; i < categories[showArea - 1].size(); i++)
			{
				const bool isSelected = (showCategory == i);
				if (ImGui::Selectable(categories[showArea - 1][i].c_str(), isSelected))
				{
					if (showCategory != i)
					{
						cc->selectedUnitDesignId = -1;
					}

					showCategory = i;
				}
			}

			ImGui::EndCombo();
		}

		if (showArea == 0)
		{
			//Disable on all
			ImGui::EndDisabled();
		}

		if (!checkedUnlock)
		{
			checkedUnlock = true;

			if (showFilter != 1)
			{
				unlocked = HasDesignUnlocked(cc, cc->selectedUnitDesignId);
			}
			else unlocked = true;
		}

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();

		if (!unlocked || cc->selectedUnitDesignId == -1)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Lock", ImVec2(112, 0)))
		{
			if (selectedPtr)
			{
				unlocked = false;

				Unlock(cc, selectedPtr->spawnId, false, true);
			}
		}

		if (!unlocked || cc->selectedUnitDesignId == -1)
		{
			ImGui::EndDisabled();
		}

		ImGui::SameLine();

		if (unlocked || cc->selectedUnitDesignId == -1)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Unlock", ImVec2(112, 0)))
		{
			if (selectedPtr)
			{
				unlocked = true;

				Unlock(cc, selectedPtr->spawnId, false, false);
			}
		}

		if (unlocked || cc->selectedUnitDesignId == -1)
		{
			ImGui::EndDisabled();
		}

		if (ImGui::TreeNode("Lock/Unlock All"))
		{
			if (ImGui::Button("Lock All", ImVec2(85, 0)))
			{
				UnlockAll(cc, false, true);
			}
			ImGui::SameLine();
			if (ImGui::Button("Unlock All", ImVec2(85, 0)))
			{
				UnlockAll(cc, false, false);
			}

			ImGui::TreePop();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##CountryDesignList");
	{
		ImGui::Text("Unit Designs (Green = Researched)");
		if (ImGui::BeginListBox("##unitdesignlistbox", ImVec2(275, 175)))
		{
			int size = g_defaultUnitList.size();

			for (int i = 0; i < size; i++)
			{
				bool ok = true;

				if (showFilter == 1) //Researched
				{
					if (!HasDesignUnlocked(cc, i))
					{
						ok = false;
					}
				}
				else if (showFilter == 2) //Unresearched
				{
					if (HasDesignUnlocked(cc, i))
					{
						ok = false;
					}
				}

				if (ok)
				{
					//Area check

					int unitClass = g_defaultUnitList[i]->unitClass;

					switch (showArea)
					{
					case 1: //Land
						if (unitClass > 6)
						{
							ok = false;
						}
						break;
					case 2: //Sea
						if (unitClass < 0xF || unitClass > 0x14)
						{
							ok = false;
						}
						break;
					case 3: //Air
						if (unitClass < 7 || unitClass == 8 || unitClass > 0xE)
						{
							ok = false;
						}
						break;
					}
				}

				if (ok && showCategory > 0)
				{
					//Category check

					int unitClass = g_defaultUnitList[i]->unitClass;

					if (showArea == 1 || showArea == 2)
					{
						//Land + Water

						int start = 0;

						if (showArea == 2)
						{
							start = 0xF;
						}

						if (unitClass != start + showCategory - 1)
						{
							ok = false;
						}
					}
					else if (showArea == 3)
					{
						//Air

						if (showCategory == 1 && unitClass != 7)
						{
							ok = false;
						}
						else if (showCategory > 1)
						{
							int start = 8;

							if (unitClass != start + showCategory - 1)
							{
								ok = false;
							}
						}
					}
				}

				if (textSearch && ok)
				{
					if (!g_defaultUnitList[i]->flag)
					{
						ok = false;
					}
				}

				if (ok)
				{
					bool color = false;

					if (HasDesignUnlocked(cc, i))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
						color = true;
					}

					const bool isSelected = (cc->selectedUnitDesignId == i);
					if (ImGui::Selectable(g_defaultUnitList[i]->name.c_str(), isSelected))
					{
						cc->selectedUnitDesignId = i;
						selectedPtr = g_defaultUnitList[i];
						checkedUnlock = false; //reset
					}

					if (color)
					{
						ImGui::PopStyleColor();
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
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
		if (ImGui::InputText("##designearch", searchVal, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			textSearch = true;
			cc->selectedUnitDesignId = -1;

			for (int i = 0; i < g_defaultUnitList.size(); i++)
			{ //reset
				g_defaultUnitList[i]->flag = false;
			}

			std::string input = std::string(searchVal);
			std::transform(input.begin(), input.end(), input.begin(), ::tolower);

			if (input.length() == 0)
			{
				textSearch = false;
			}
			else
			{
				for (int i = 0; i < g_defaultUnitList.size(); i++)
				{
					std::string main = g_defaultUnitList[i]->name;
					std::transform(main.begin(), main.end(), main.begin(), ::tolower);

					if (main.find(input) != std::string::npos)
					{
						g_defaultUnitList[i]->flag = true;
					}
				}
			}
		}
	}
	ImGui::EndChild();
}