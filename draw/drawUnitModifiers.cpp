#include "../base.h";

std::string GetRangeString(int range)
{
	std::string s;

	switch (range)
	{
	case -3:
		s = "Minimum";
		break;
	case -2:
		s = "-50%";
		break;
	case -1:
		s = "-25%";
		break;
	case 0:
		s = "Default";
		break;
	case 1:
		s = "+25%";
		break;
	case 2:
		s = "+50%";
		break;
	case 3:
		s = "Maximum";
		break;
	}

	return s;
}

void Base::Draw::DrawUnitModifiers(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	if (ImGui::BeginTabItem("Modifiers"))
	{
		Draw::DrawSelectedCountryText(cc, "Selected country: %s");

		ImGui::BeginChild("##unitmodifierschild", ImVec2(225, 150), true);
		{
			Unit::Property p;
			int stateChange = -1;

			if (ImGui::Checkbox("Invincible", &cc->invincibleUnits))
			{
				p = Unit::Property::Health;
				stateChange = (int)cc->invincibleUnits;
			}

			if (ImGui::Checkbox("Max Supply", &cc->maxSupplyUnits))
			{
				p = Unit::Property::Supply;
				stateChange = (int)cc->maxSupplyUnits;
			}

			if (ImGui::Checkbox("Max Fuel", &cc->maxFuelUnits))
			{
				p = Unit::Property::Fuel;
				stateChange = (int)cc->maxFuelUnits;
			}

			if (ImGui::Checkbox("Max Experience", &cc->maxExperienceUnits))
			{
				p = Unit::Property::Experience;
				stateChange = (int)cc->maxExperienceUnits;
			}

			if (ImGui::Checkbox("Max Morale", &cc->maxMoraleUnits))
			{
				p = Unit::Property::Morale;
				stateChange = (int)cc->maxMoraleUnits;
			}

			if (ImGui::Checkbox("Max Efficiency", &cc->maxEfficiencyUnits))
			{
				p = Unit::Property::Efficiency;
				stateChange = (int)cc->maxEfficiencyUnits;
			}

			if (stateChange == 0)
			{
				//restore
				for (int i = 0; i < cc->allUnits.size(); i++)
				{
					*cc->allUnits[i].properties[(int)p]->valPtr =
						cc->allUnits[i].properties[(int)p]->freezeVal;
				}
			}
			else if (stateChange == 1)
			{
				//save orig
				for (int i = 0; i < cc->allUnits.size(); i++)
				{
					cc->allUnits[i].properties[(int)p]->freezeVal =
						*cc->allUnits[i].properties[(int)p]->valPtr;
				}
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##unitmodifierschild2");
		{
			int max = UnitDefault::Property::MAX - 1; //exclude build time
			for (int i = 0; i < max; i++)
			{
				std::string label = "";
				UnitDefault::Property property = (UnitDefault::Property)i;

				switch (property)
				{
				default:
				case UnitDefault::Property::MoveSpeed:
					label = "Move speed";
					break;
				case UnitDefault::Property::Spotting:
					label = "Spotting";
					break;
				case UnitDefault::Property::MoveRange:
					label = "Move range";
					break;
				case UnitDefault::Property::SoftGroundAttack:
					label = "Soft ground attack";
					break;
				case UnitDefault::Property::HardGroundAttack:
					label = "Hard ground attack";
					break;
				case UnitDefault::Property::CloseGroundAttack:
					label = "Close ground attack";
					break;
				case UnitDefault::Property::CloseAirAttack:
					label = "Close air attack";
					break;
				case UnitDefault::Property::MidAirAttack:
					label = "Mid air attack";
					break;
				case UnitDefault::Property::GroundRange:
					label = "Ground range";
					break;
				case UnitDefault::Property::NavalRange:
					label = "Naval range";
					break;
				case UnitDefault::Property::AirRange:
					label = "Air range";
					break;
				case UnitDefault::Property::FuelCapacity:
					label = "Fuel capcaity";
					break;
				case UnitDefault::Property::SupplyCapacity:
					label = "Supply capacity";
					break;
				case UnitDefault::Property::BuildTime:
					label = "Build time";
					break;
				}

				ImGui::Text(label.c_str());
				label = "##" + label + "i";
				if (ImGui::SliderInt(label.c_str(), &cc->unitDefaultModifiers[i].range, -3, 3, 
					GetRangeString(cc->unitDefaultModifiers[i].range).c_str()))
				{
					if (cc->unitDefaultModifiers[i].range == 0)
					{ //restore
						for (int a = 0; a < cc->allUnits.size(); a++)
						{
							cc->allUnits[a].RestoreDesignProperty(cc, property);
						}

						if (property == UnitDefault::MoveSpeed)
						{
							*(uintptr_t*)(cc->base + Offsets::countryRailTransport) = (uint8_t)0;
						}
					}
					else
					{ //apply
						for (int a = 0; a < cc->allUnits.size(); a++)
						{
							cc->allUnits[a].SetDesignPropertySimple(cc, property, cc->unitDefaultModifiers[i].range);
						}

						if (property == UnitDefault::MoveSpeed)
						{
							*(uintptr_t*)(cc->base + Offsets::countryRailTransport) = (uint8_t)2;
						}
					}
				}
			}
		}
		
		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}