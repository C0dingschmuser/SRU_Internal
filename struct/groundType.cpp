#include "../Base.h"

using namespace Base::SRU_Data;

void Base::SRU_Data::LoadGroundTypes()
{
	g_groundTypeList.clear();

	GroundType grnd;

	grnd.id = 0;
	grnd.name = "Plains";
	g_groundTypeList.push_back(grnd);

	grnd.id = 2;
	grnd.name = "Light Forest";
	g_groundTypeList.push_back(grnd);

	grnd.id = 3;
	grnd.name = "Dense Forest";
	g_groundTypeList.push_back(grnd);

	grnd.id = 14;
	grnd.name = "Ocean";
	g_groundTypeList.push_back(grnd);

	grnd.id = 15;
	grnd.name = "Impassible";
	g_groundTypeList.push_back(grnd);
	
	grnd.id = 24;
	grnd.name = "Mountain";
	g_groundTypeList.push_back(grnd);
}