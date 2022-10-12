#include "../Base.h"

void Base::Draw::DrawUnitSelected(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	if (ImGui::BeginTabItem("Selected"))
	{
		std::string text = std::to_string(g_selectedUnitList.size());

		ImGui::Text(text.c_str());
		ImGui::EndTabItem();
	}
}