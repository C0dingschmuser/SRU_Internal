#include "../Base.h"

void Base::SRU_Data::LoadDiplTreaties()
{
	g_diplTreatyList.clear();

	using namespace Base::SRU_Data;

	DiplTreaty treaty;

	treaty.name = "Declare War";
	treaty.treatyId = -1;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Ceasefire";
	treaty.treatyId = 3;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Formal Alliance";
	treaty.treatyId = 1;
	g_diplTreatyList.push_back(treaty);

	//treaty.name = "End Alliance";
	//treaty.treatyId = 121;
	//g_diplTreatyList.push_back(treaty);

	treaty.name = "Non Aggression Pact";
	treaty.treatyId = 2;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Peace";
	treaty.treatyId = 4;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Mutual Defense";
	treaty.treatyId = 10;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Full Transit";
	treaty.treatyId = 12;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Land Transit";
	treaty.treatyId = 13;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Air Transit";
	treaty.treatyId = 14;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Sea Transit";
	treaty.treatyId = 15;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Line of Sight";
	treaty.treatyId = 30;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Troup Withdrawal";
	treaty.treatyId = 50;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Combat Supplies";
	treaty.treatyId = 78;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Military Unit Supply";
	treaty.treatyId = 79;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Free Flow of Labor";
	treaty.treatyId = 81;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Free Trade";
	treaty.treatyId = 84;
	g_diplTreatyList.push_back(treaty);

	//Add 87 + 88?

	treaty.name = "Embassy";
	treaty.treatyId = 90;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Crim. Extradition";
	treaty.treatyId = 91;
	g_diplTreatyList.push_back(treaty);

	treaty.name = "Missile Defense";
	treaty.treatyId = 100;
	g_diplTreatyList.push_back(treaty);

	//Add others?
}