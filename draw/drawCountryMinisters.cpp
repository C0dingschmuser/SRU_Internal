#include "../Base.h";

std::string GetInitiativeString(uint8_t val)
{
	if (val == 0)
	{
		return "None";
	}
	else if (val == 1)
	{
		return "Low";
	}
	else if (val == 2)
	{
		return "Medium";
	}
	else if (val == 3)
	{
		return "High";
	}
	else //100
	{
		return "Full";
	}
}

void Base::Draw::DrawCountryMinisters(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	DrawSelectedCountryText(cc, "Selected country: %s");

	ImGui::BeginChild("##ministersleft", ImVec2(275, 225));
	{
		for (int i = 0; i < cc->ministers.size(); i++)
		{
			if (ImGui::TreeNode(cc->ministers[i]->name.c_str()))
			{
				if (i == 3)
				{
					//defense

					for (int b = 0; b < Offsets::Ministers::Defense::initiativeOffsets.size(); b++)
					{
						uint8_t* val = (uint8_t*)(cc->base + Offsets::Ministers::Defense::initiativeOffsets[b]);
						int subVal = *val / 25;

						std::string iname = "Land Initiative";

						if (b == 1)
						{
							iname = "Air Initiative";
						}
						else if (b == 2)
						{
							iname = "Sea Initiative";
						}

						ImGui::Text(iname.c_str());

						std::string preview = GetInitiativeString(subVal);
						std::string label = "##init" + std::to_string(b);
						if (ImGui::BeginCombo(label.c_str(), preview.c_str()))
						{
							for (int c = 0; c < 4; c++)
							{
								std::string name = GetInitiativeString(c);
								const bool isSelected = (subVal == c);
								if (ImGui::Selectable(name.c_str(), isSelected))
								{
									*val = c * 25;
								}
							}

							ImGui::EndCombo();
						}
					}
				}

				for (int a = 0; a < cc->ministers[i]->properties.size(); a++)
				{
					static bool ok = false;
					bool color = false;
					if (cc->ministers[i]->properties[a]->status > 0)
					{
						ok = cc->ministers[i]->properties[a]->status - 1;
						ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255)); //red
						color = true;
					}
					else
					{
						ok = !Base::Utils::GetValueBool(cc->ministers[i]->propertiesPtr, cc->ministers[i]->properties[a]->changeVal);
					}

					if (ImGui::Checkbox(cc->ministers[i]->properties[a]->name.c_str(), &ok))
					{
						if (g_shift)
						{
							cc->ministers[i]->properties[a]->status = 0;
						}
						else
						{
							cc->ministers[i]->properties[a]->status = 1 + (int)ok;
						}
						Base::Utils::SetValueBool(cc->ministers[i]->propertiesPtr, cc->ministers[i]->properties[a]->changeVal, !ok);
					}

					if (color)
					{
						ImGui::PopStyleColor();
					}
				}

				ImGui::TreePop();
			}
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##child2", ImVec2(0, 0));
	{
		ImGui::Text("Normal Click:\n Freeze Property (Red Text)");
		ImGui::Text("Shift Click:\n Unfreeze Property");
		ImGui::Dummy(ImVec2(1,1));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255)); //yellow
		ImGui::Text("Unfreezed Properties can be\n changed by the AI");
		ImGui::PopStyleColor();
	}
	ImGui::EndChild();
}