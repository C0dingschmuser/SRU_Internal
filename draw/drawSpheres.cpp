#include "../Base.h"

void Base::Draw::DrawSpheres(Base::SRU_Data::Country* cc)
{
	using namespace Base::SRU_Data;

	ImGui::Text("Override 'Red' Sphere name (or leave empty for default)");
	ImGui::InputText("##spherenamered", &g_sphereNames[0]);
	ImGui::Text("Override 'Blue' Sphere name (or leave empty for default)");
	ImGui::InputText("##spherenameblue", &g_sphereNames[1]);
}