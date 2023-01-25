#include "../Base.h"

void Base::Draw::DrawSettings()
{
	using namespace Base::SRU_Data;

	ImGui::BeginChild("##settingsleft", ImVec2(225, 250));
	{
		static bool allunit = false;
		static bool onedaybuild = false;
		static bool breakground = false;
		static bool moreoffers = false;

		{
			uint8_t* ptr = (uint8_t*)(uintptr_t*)(g_base + Offsets::cheatAddr);
			uint8_t val = *ptr;
			uint8_t oldVal = val;

			val = ~val & Offsets::allunit[0] | val & Offsets::allunit[1];
			if (val < oldVal)
			{ //was enabled
				allunit = true;
			}
			else allunit = false;
			val = oldVal;

			val = ~val & Offsets::onedaybuild[0] | val & Offsets::onedaybuild[1];
			if (val < oldVal)
			{
				//was enabled
				onedaybuild = true;
			}
			else onedaybuild = false;
			val = oldVal;

			val = ~val & Offsets::breakground[0] | val & Offsets::breakground[1];
			if (val < oldVal)
			{
				//was enabled
				breakground = true;
			}
			else breakground = false;
			val = oldVal;

			val = ~val & Offsets::moreoffers[0] | val & Offsets::moreoffers[1];
			if (val < oldVal)
			{
				//was enabled
				moreoffers = true;
			}
			else moreoffers = false;
		}

		ImGui::Text("Cheats");
		if (ImGui::Checkbox("All Units", &allunit))
		{
			allunit = !allunit;
			Base::Execute::SetCheat(Offsets::allunit[0]);
		}
		if (ImGui::Checkbox("One day build", &onedaybuild))
		{
			onedaybuild = !onedaybuild;
			Base::Execute::SetCheat(Offsets::onedaybuild[0]);
		}
		if (ImGui::Checkbox("One day facilities", &breakground))
		{
			breakground = !breakground;
			Base::Execute::SetCheat(Offsets::breakground[0]);
		}
		if (ImGui::Checkbox("More offers", &moreoffers))
		{
			moreoffers = !moreoffers;
			Base::Execute::SetCheat(Offsets::moreoffers[0]);
		}

		ImGui::Separator();
		
		ImGui::Text("Painting Brush size");
		ImGui::SliderInt("##paintbrushsuze", &g_paintBrushSize, 1, 15);
		ImGui::Checkbox("Only paint on territory of\nselected country", &g_paintHexOnlySelected);

		ImGui::EndChild();
	}
	ImGui::SameLine();
	ImGui::BeginChild("##settings");
	{
		ImGui::Text("Settings");
		ImGui::Text("Minimum global hex supply");

		std::string str = Base::Utils::FloatToPercent((float)Asm::g_lowestHexSupply, 255.0f);

		ImGui::SliderByte("##hexsupplyslider", &Asm::g_lowestHexSupply, 0, 255, str.c_str());
		ImGui::Checkbox("Let AI create colonies", &g_aiColony);
		if (ImGui::Checkbox("[EXPERIMENTAL]\nNo automatic production adjustment", &g_productionAdjustment))
		{
			Base::SRU_Data::Hooks::SetProductionAdjustment(g_productionAdjustment);
		}
		if (ImGui::Checkbox("Disco Mode\n(Epilepsy warning)", &g_disco))
		{
			if (g_disco)
			{
				//save orig

				for (int i = 0; i < g_countryList.size(); i++)
				{
					Country* tmpcc = &g_countryList[i];

					unsigned long originalColor = *(uintptr_t*)(tmpcc->base + Offsets::countryColor);

					if (originalColor > 0)
					{
						tmpcc->hasOwnColor = true;
						tmpcc->originalColor = originalColor;
					}
					else
					{
						tmpcc->hasOwnColor = false;
						tmpcc->originalColor = 0;
					}
				}
			}
			else
			{
				//restore

				for (int i = 0; i < g_countryList.size(); i++)
				{
					Country* tmpcc = &g_countryList[i];

					if (tmpcc->hasOwnColor)
					{
						*(uintptr_t*)(tmpcc->base + Offsets::countryColor) = tmpcc->originalColor;
					}
					else
					{
						*(uintptr_t*)(tmpcc->base + Offsets::countryColor) = 0;
					}
				}
			}
		}
		ImGui::EndChild();
	}
}