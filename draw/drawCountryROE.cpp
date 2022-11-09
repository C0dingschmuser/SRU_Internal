#include "../Base.h";

int GetValue(uintptr_t* addr, int offset)
{
	return ((int)(*addr >> offset) & 3);
}

void Base::Draw::DrawCountryROE(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	DrawSelectedCountryText(cc, "Selected country: %s");

	ImGui::BeginChild("##roeleft", ImVec2(225, 225));
	{
		static std::vector<std::string> speedTypes = {
			"Unchanged",
			"Cautious",
			"Normal",
			"Fast",
			"Fastest",
		};

		static std::vector<std::string> routeTypes = {
			"Unchanged",
			"Cautious",
			"Normal",
			"Direct",
			"Quickest",
		};

		static std::vector<std::string> initiativeTypes = {
			"Unchanged",
			"None",
			"Low",
			"Medium",
			"High",
		};

		static std::vector<std::string> contactOptions = {
			"Unchanged",
			"Avoid",
			"Engage",
			"Pursue",
		};

		static std::vector<std::string> lossTolerances = {
			"Unchanged",
			"None",
			"Low",
			"Medium",
			"High",
		};

		ImGui::Text("Speed");
		if (ImGui::BeginCombo("##speed", speedTypes[cc->roeSpeed].c_str()))
		{
			for (int i = 0; i < speedTypes.size(); i++)
			{
				const bool isSelected = (cc->roeSpeed == i);
				if (ImGui::Selectable(speedTypes[i].c_str(), isSelected))
				{
					if (cc->roeSpeed != i && i > 0)
					{
						//Change value

						uintptr_t* addr = (uintptr_t*)(cc->base + Offsets::mainROE);
						Base::Utils::SetValueMask(addr, Offsets::roeSpeed, i - 1);
					}

					cc->roeSpeed = i;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Route");
		if (ImGui::BeginCombo("##route", routeTypes[cc->roeRoute].c_str()))
		{
			for (int i = 0; i < routeTypes.size(); i++)
			{
				const bool isSelected = (cc->roeRoute == i);
				if (ImGui::Selectable(routeTypes[i].c_str(), isSelected))
				{
					if (cc->roeRoute != i && i > 0)
					{
						uintptr_t* addr = (uintptr_t*)(cc->base + Offsets::mainROE);
						Base::Utils::SetValueMask(addr, Offsets::roeRoute, i - 1);
					}

					cc->roeRoute = i;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Initiative");
		if (ImGui::BeginCombo("##initiative", initiativeTypes[cc->roeInitiative].c_str()))
		{
			for (int i = 0; i < initiativeTypes.size(); i++)
			{
				const bool isSelected = (cc->roeInitiative == i);
				if (ImGui::Selectable(initiativeTypes[i].c_str(), isSelected))
				{
					if (cc->roeInitiative != i && i > 0)
					{
						uintptr_t* addr = (uintptr_t*)(cc->base + Offsets::mainROE);
						Base::Utils::SetValueMask(addr, Offsets::roeInitiative, i - 1);
					}

					cc->roeInitiative = i;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Contact Options");
		if (ImGui::BeginCombo("##contactoptions", contactOptions[cc->roeContactOptions].c_str()))
		{
			for (int i = 0; i < contactOptions.size(); i++)
			{
				const bool isSelected = (cc->roeContactOptions == i);
				if (ImGui::Selectable(contactOptions[i].c_str(), isSelected))
				{
					if (cc->roeContactOptions != i && i > 0)
					{
						uintptr_t* addr = (uintptr_t*)(cc->base + Offsets::mainROE);
						Base::Utils::SetValueMask(addr, Offsets::roeContactOptions, i - 1);
					}

					cc->roeContactOptions = i;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Loss Tolerance");
		if (ImGui::BeginCombo("##losstolerance", lossTolerances[cc->roeLossTolerance].c_str()))
		{
			for (int i = 0; i < lossTolerances.size(); i++)
			{
				const bool isSelected = (cc->roeLossTolerance == i);
				if (ImGui::Selectable(lossTolerances[i].c_str(), isSelected))
				{
					if (cc->roeLossTolerance != i && i > 0)
					{
						uintptr_t* addr = (uintptr_t*)(cc->base + Offsets::mainROE);
						Base::Utils::SetValueMask(addr, Offsets::roeLossTolerance, i - 1);
					}

					cc->roeLossTolerance = i;
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##roechild2");
	{
		static std::vector<std::string> roeBool = {
			"Unchanged",
			"Off",
			"On",
		};

		ImGui::Text("Opportunity Fire");
		if (ImGui::BeginCombo("##opportunityfire", roeBool[cc->roeOpportunityFire].c_str()))
		{
			for (int i = 0; i < roeBool.size(); i++)
			{
				const bool isSelected = (cc->roeOpportunityFire == i);
				if (ImGui::Selectable(roeBool[i].c_str(), isSelected))
				{
					if (cc->roeOpportunityFire != i && i > 0)
					{
						uintptr_t* addr = (uintptr_t*)(cc->base + Offsets::secondROE);
						bool enable = i - 1;

						Base::Utils::SetValueBool(addr, Offsets::roeOpportunityFire, enable);
					}

					cc->roeOpportunityFire = i;
				}
			}
			ImGui::EndCombo();
		}

		static std::vector<std::string> roeApproachs = {
			"Unchanged",
			"Regular",
			"Stealth",
		};

		ImGui::Text("Approach");
		if(ImGui::BeginCombo("##roeapproach", roeApproachs[cc->roeApproach].c_str()))
		{
			for (int i = 0; i < roeApproachs.size(); i++)
			{
				const bool isSelected = (cc->roeApproach == i);
				if (ImGui::Selectable(roeApproachs[i].c_str(), isSelected))
				{
					if (cc->roeApproach != i && i > 0)
					{
						uintptr_t* addr = (uintptr_t*)(cc->base + Offsets::secondROE);
						bool enable = i - 1;

						Base::Utils::SetValueBool(addr, Offsets::roeApproach, enable);
					}

					cc->roeApproach = i;
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::EndChild();
}