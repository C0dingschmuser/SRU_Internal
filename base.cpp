#include "base.h"

//Data Vars

EndScene Base::Data::oEndScene = NULL;
DrawIndexedPrimitive Base::Data::oDrawIndexedPrimitive = NULL;
DrawIndexedPrimitiveUp Base::Data::oDrawIndexedPrimitiveUp = NULL;
Reset Base::Data::oReset = NULL;
WndProc_t Base::Data::oWndProc = NULL;
HWND Base::Data::window = NULL;
bool Base::Data::init = false;
bool Base::Data::ShowMenu = true;
int Base::Data::subMenu = 0;

//Asm Vars

unsigned int Base::SRU_Data::Asm::g_selectedReg0;
unsigned int Base::SRU_Data::Asm::g_selectedReg1;
unsigned int Base::SRU_Data::Asm::g_selectedReg2;
unsigned int Base::SRU_Data::Asm::g_selectedReg3;
unsigned int Base::SRU_Data::Asm::g_selectedReg4;
unsigned int Base::SRU_Data::Asm::g_selectedReg5;
unsigned int Base::SRU_Data::Asm::g_selectedReg6;
unsigned int Base::SRU_Data::Asm::g_selectedReg7;

unsigned int Base::SRU_Data::Asm::g_hexReg0;
unsigned int Base::SRU_Data::Asm::g_hexReg1;
unsigned int Base::SRU_Data::Asm::g_hexReg2;
unsigned int Base::SRU_Data::Asm::g_hexReg3;
unsigned int Base::SRU_Data::Asm::g_hexReg4;
unsigned int Base::SRU_Data::Asm::g_hexReg5;
unsigned int Base::SRU_Data::Asm::g_hexReg6;
unsigned int Base::SRU_Data::Asm::g_hexReg7;

unsigned int Base::SRU_Data::Asm::g_aiSurrReg0;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg1;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg2;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg3;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg4;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg5;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg6;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg7;

int Base::SRU_Data::Asm::g_aiSurrSize;
int Base::SRU_Data::Asm::g_aiSurrFrom;
int Base::SRU_Data::Asm::g_aiSurrTo;

unsigned int Base::SRU_Data::Asm::g_defconReg0;
unsigned int Base::SRU_Data::Asm::g_defconReg1;
unsigned int Base::SRU_Data::Asm::g_defconReg2;
unsigned int Base::SRU_Data::Asm::g_defconReg3;
unsigned int Base::SRU_Data::Asm::g_defconReg4;
unsigned int Base::SRU_Data::Asm::g_defconReg5;
unsigned int Base::SRU_Data::Asm::g_defconReg6;
unsigned int Base::SRU_Data::Asm::g_defconReg01;
unsigned int Base::SRU_Data::Asm::g_defconReg11;
unsigned int Base::SRU_Data::Asm::g_defconReg21;
unsigned int Base::SRU_Data::Asm::g_defconReg31;
unsigned int Base::SRU_Data::Asm::g_defconReg41;
unsigned int Base::SRU_Data::Asm::g_defconReg51;
unsigned int Base::SRU_Data::Asm::g_defconReg61;
int Base::SRU_Data::Asm::g_defconReg02;
int Base::SRU_Data::Asm::g_defconNew;
unsigned int Base::SRU_Data::Asm::g_defconReg12;
unsigned int Base::SRU_Data::Asm::g_defconReg22;
unsigned int Base::SRU_Data::Asm::g_defconReg32;
unsigned int Base::SRU_Data::Asm::g_defconReg42;
unsigned int Base::SRU_Data::Asm::g_defconReg52;
unsigned int Base::SRU_Data::Asm::g_defconReg62;
unsigned int Base::SRU_Data::Asm::g_defconReg03;
unsigned int Base::SRU_Data::Asm::g_defconReg13;
unsigned int Base::SRU_Data::Asm::g_defconReg23;
unsigned int Base::SRU_Data::Asm::g_defconReg33;
unsigned int Base::SRU_Data::Asm::g_defconReg43;
unsigned int Base::SRU_Data::Asm::g_defconReg53;
unsigned int Base::SRU_Data::Asm::g_defconReg63;

unsigned int Base::SRU_Data::Asm::g_diplFreeReg0;
unsigned int Base::SRU_Data::Asm::g_diplFreeReg1;
unsigned int Base::SRU_Data::Asm::g_diplFreeReg2;
unsigned int Base::SRU_Data::Asm::g_diplFreeReg3;
unsigned int Base::SRU_Data::Asm::g_diplFreeReg4;
unsigned int Base::SRU_Data::Asm::g_diplFreeReg5;
unsigned int Base::SRU_Data::Asm::g_diplFreeReg6;

unsigned int Base::SRU_Data::Asm::g_mapSizeReg0;
unsigned int Base::SRU_Data::Asm::g_mapSizeReg1;
unsigned int Base::SRU_Data::Asm::g_mapSizeReg2;
unsigned int Base::SRU_Data::Asm::g_mapSizeReg3;
unsigned int Base::SRU_Data::Asm::g_mapSizeReg4;
unsigned int Base::SRU_Data::Asm::g_mapSizeReg5;
unsigned int Base::SRU_Data::Asm::g_mapSizeReg6;

unsigned int Base::SRU_Data::Asm::g_buildCheckReg0;
unsigned int Base::SRU_Data::Asm::g_buildCheckReg1;
unsigned int Base::SRU_Data::Asm::g_buildCheckReg2;
unsigned int Base::SRU_Data::Asm::g_buildCheckReg3;
unsigned int Base::SRU_Data::Asm::g_buildCheckReg4;
unsigned int Base::SRU_Data::Asm::g_buildCheckReg5;
unsigned int Base::SRU_Data::Asm::g_buildCheckReg6;
unsigned int Base::SRU_Data::Asm::g_buildCheckReg7;

unsigned int Base::SRU_Data::Asm::g_hexNameReg0;
unsigned int Base::SRU_Data::Asm::g_hexNameReg1;
unsigned int Base::SRU_Data::Asm::g_hexNameReg2;
unsigned int Base::SRU_Data::Asm::g_hexNameReg3;
unsigned int Base::SRU_Data::Asm::g_hexNameReg4;
unsigned int Base::SRU_Data::Asm::g_hexNameReg5;
unsigned int Base::SRU_Data::Asm::g_hexNameReg6;
unsigned int Base::SRU_Data::Asm::g_hexNameReg7;

unsigned int Base::SRU_Data::Asm::g_sphereNameReg0;
unsigned int Base::SRU_Data::Asm::g_sphereNameReg1;
unsigned int Base::SRU_Data::Asm::g_sphereNameReg2;
unsigned int Base::SRU_Data::Asm::g_sphereNameReg3;
unsigned int Base::SRU_Data::Asm::g_sphereNameReg4;
unsigned int Base::SRU_Data::Asm::g_sphereNameReg5;
unsigned int Base::SRU_Data::Asm::g_sphereNameReg6;
unsigned int Base::SRU_Data::Asm::g_sphereNameReg7;

unsigned int Base::SRU_Data::Asm::g_openSettingsReg0;
unsigned int Base::SRU_Data::Asm::g_openSettingsReg1;
unsigned int Base::SRU_Data::Asm::g_openSettingsReg2;
unsigned int Base::SRU_Data::Asm::g_openSettingsReg3;
unsigned int Base::SRU_Data::Asm::g_openSettingsReg4;
unsigned int Base::SRU_Data::Asm::g_openSettingsReg5;
unsigned int Base::SRU_Data::Asm::g_openSettingsReg6;
unsigned int Base::SRU_Data::Asm::g_openSettingsReg7;

unsigned int Base::SRU_Data::Asm::g_closeSettingsReg0;
unsigned int Base::SRU_Data::Asm::g_closeSettingsReg1;
unsigned int Base::SRU_Data::Asm::g_closeSettingsReg2;
unsigned int Base::SRU_Data::Asm::g_closeSettingsReg3;
unsigned int Base::SRU_Data::Asm::g_closeSettingsReg4;
unsigned int Base::SRU_Data::Asm::g_closeSettingsReg5;
unsigned int Base::SRU_Data::Asm::g_closeSettingsReg6;
unsigned int Base::SRU_Data::Asm::g_closeSettingsReg7;

unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg0;
unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg1;
unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg2;
unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg3;
unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg4;
unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg5;
unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg6;
unsigned int Base::SRU_Data::Asm::g_autosaveBeginReg7;

unsigned int Base::SRU_Data::Asm::g_autosaveEndReg0;
unsigned int Base::SRU_Data::Asm::g_autosaveEndReg1;
unsigned int Base::SRU_Data::Asm::g_autosaveEndReg2;
unsigned int Base::SRU_Data::Asm::g_autosaveEndReg3;
unsigned int Base::SRU_Data::Asm::g_autosaveEndReg4;
unsigned int Base::SRU_Data::Asm::g_autosaveEndReg5;
unsigned int Base::SRU_Data::Asm::g_autosaveEndReg6;
unsigned int Base::SRU_Data::Asm::g_autosaveEndReg7;

unsigned int Base::SRU_Data::Asm::g_quitGameReg0;
unsigned int Base::SRU_Data::Asm::g_quitGameReg1;
unsigned int Base::SRU_Data::Asm::g_quitGameReg2;
unsigned int Base::SRU_Data::Asm::g_quitGameReg3;
unsigned int Base::SRU_Data::Asm::g_quitGameReg4;
unsigned int Base::SRU_Data::Asm::g_quitGameReg5;
unsigned int Base::SRU_Data::Asm::g_quitGameReg6;
unsigned int Base::SRU_Data::Asm::g_quitGameReg7;

std::vector<uintptr_t> Base::SRU_Data::Asm::g_ownAllocs;

uintptr_t Base::SRU_Data::Asm::g_aiSurrBase;

//SRU Vars

std::vector<std::shared_ptr<Base::SRU_Data::UnitDefault>> Base::SRU_Data::g_defaultUnitList;
std::vector<std::shared_ptr<Base::SRU_Data::Facility>> Base::SRU_Data::g_facilityList;
std::vector<std::shared_ptr<Base::SRU_Data::Leader>> Base::SRU_Data::g_leaderList;
std::vector<std::shared_ptr<Base::SRU_Data::Hex>> Base::SRU_Data::g_hexNameList;
std::vector<Base::SRU_Data::Unit> Base::SRU_Data::g_selectedUnitList;
std::vector<Base::SRU_Data::Country> Base::SRU_Data::g_countryList;
std::vector<std::shared_ptr<Base::SRU_Data::Tech>> Base::SRU_Data::g_techList;
std::vector<Base::SRU_Data::Unit> Base::SRU_Data::g_unitList;
std::vector<Base::SRU_Data::DiplTreaty> Base::SRU_Data::g_diplTreatyList;
std::vector<Base::SRU_Data::GroundType> Base::SRU_Data::g_groundTypeList;

Base::SRU_Data::SurrenderEvent Base::SRU_Data::surrenderEvents[32] = { };

std::vector<Base::SRU_Data::HookHolder> Base::SRU_Data::Hooks::g_hookList;

int Base::SRU_Data::Hooks::g_lastFunctionHookState = 0;

uintptr_t Base::SRU_Data::Hooks::g_selectedJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_hexSupplyJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_aiSurrenderJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_mouseClickedJmpBackAddr = 0;

uintptr_t Base::SRU_Data::Hooks::g_defconJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_defconJmpBackAddr2 = 0;
uintptr_t Base::SRU_Data::Hooks::g_defconJmpBackAddr3 = 0;
uintptr_t Base::SRU_Data::Hooks::g_defconJmpBackAddr4 = 0;

uintptr_t Base::SRU_Data::Hooks::g_diplFreeJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_diplFreeJmpBackAddrDefault = 0;

uintptr_t Base::SRU_Data::Hooks::g_mapSizeJumpBackAddr = 0;

uintptr_t Base::SRU_Data::Hooks::g_buildCheckJumpBackAddr = 0;

uintptr_t Base::SRU_Data::Hooks::g_hexNameBigJumpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_hexNameBigJumpBackAddrNone = 0;
uintptr_t Base::SRU_Data::Hooks::g_hexNameBigJumpBackAddrData = 0;

uintptr_t Base::SRU_Data::Hooks::g_sphereNameJumpBackAddr = 0;

uintptr_t Base::SRU_Data::Hooks::g_openSettingsJumpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_closeSettingsJumpBackAddr = 0;

uintptr_t Base::SRU_Data::Hooks::g_autosaveBeginJumpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_autosaveEndJumpBackAddr = 0;

uintptr_t Base::SRU_Data::Hooks::g_quitGameJumpBackAddr = 0;

uintptr_t Base::SRU_Data::g_nextUnitEntity = 0;

uint64_t Base::SRU_Data::g_steamId = 0;

int Base::SRU_Data::g_unitEntityCountSelected = 0;
int Base::SRU_Data::g_clickedCountry = 0;
int Base::SRU_Data::g_clickedCountryRaw = 0;
int Base::SRU_Data::g_selectedTargetCountry = 1;
int Base::SRU_Data::g_lastClickedCountry = 0;
int Base::SRU_Data::g_lastClickedTargetCountry = 0;
int Base::SRU_Data::g_ownCountryId = -1;
int Base::SRU_Data::g_ownOtherCountryId = -1;
int Base::SRU_Data::g_surrenderEventCount = 0;

int Base::SRU_Data::g_paintSelectedComboCountry = 0;
int Base::SRU_Data::g_paintSelectedComboLoyalty = 0;
int Base::SRU_Data::g_paintSelectedComboGround = -1;

int Base::SRU_Data::g_paintSelectedResource = 0;
int Base::SRU_Data::g_paintSelectedResourceAmount = 1;

int Base::SRU_Data::g_unitSpawnSelectedUnitDesign = -1;
int Base::SRU_Data::g_facilitySpawnSelectedFacility = -1;

int Base::SRU_Data::g_unitRefreshMaxTime = 500;
int Base::SRU_Data::g_mainRefreshTime = 25;

int Base::SRU_Data::g_mapSizeX = 0;

int Base::SRU_Data::g_paintMode = Base::SRU_Data::Paint_Land;
int Base::SRU_Data::g_paintBrushSize = 1;
int Base::SRU_Data::g_paintStyle = 0;
bool Base::SRU_Data::g_paintUnitTargetCountry = false;
std::vector<int> Base::SRU_Data::g_paintUnitModes;

int Base::SRU_Data::g_unitDesignType = 0;
int Base::SRU_Data::g_unitSpawnCount = 1;
int Base::SRU_Data::g_unitSpawnXPos = 0;
int Base::SRU_Data::g_unitSpawnYPos = 0;
bool Base::SRU_Data::g_unitSpawnReserve = false;

int Base::SRU_Data::g_facilitySpawnXPos = 0;
int Base::SRU_Data::g_facilitySpawnYPos = 0;
int Base::SRU_Data::g_facilitySpawnCount = 1;
bool Base::SRU_Data::g_facilitySpawnConstruction = true;

byte Base::SRU_Data::Asm::g_currentHexSupply = 0;
byte Base::SRU_Data::Asm::g_lowestHexSupply = 0x0;

bool Base::SRU_Data::g_newClick = false;
bool Base::SRU_Data::g_addOk = false;
bool Base::SRU_Data::g_shift = false;
bool Base::SRU_Data::g_mapSizeLoaded = false;
bool Base::SRU_Data::g_uiHexSupplySet = false;
bool Base::SRU_Data::g_mouseClicked = false;
bool Base::SRU_Data::g_paintActive = false;
bool Base::SRU_Data::g_paintEnabled = false;
bool Base::SRU_Data::g_paintHexOwner = true;
bool Base::SRU_Data::g_paintHexLoyalty = true;
bool Base::SRU_Data::g_paintHexOnlySelected = false;

bool Base::SRU_Data::g_disco = false;
bool Base::SRU_Data::g_hexSupply = false;
bool Base::SRU_Data::g_productionAdjustment = false;
bool Base::SRU_Data::g_aiColony = false;
bool Base::SRU_Data::g_fastRoad = false;
bool Base::SRU_Data::g_unitSelect = false;

bool Base::Draw::g_countryColorLoaded = false;

uint8_t Base::SRU_Data::g_currentHexSupply = 0;

Base::SRU_Data::Country* Base::SRU_Data::clickedCountry = nullptr;
Base::SRU_Data::Country* Base::SRU_Data::clickedTargetCountry = nullptr;
Base::SRU_Data::Country* Base::SRU_Data::unitSpawnCountry = nullptr;

uintptr_t* Base::SRU_Data::g_clickedHexPtr = nullptr;
uintptr_t* Base::SRU_Data::g_playSpeedPtr = nullptr;

uint16_t* Base::SRU_Data::g_clickedXPtr = nullptr;
uint16_t* Base::SRU_Data::g_clickedYPtr = nullptr;

//SRU Game Vars
int Base::SRU_Data::g_ingame = 0;
bool Base::SRU_Data::g_autosaving = false;
bool Base::SRU_Data::g_quitting = false;
uintptr_t Base::SRU_Data::g_base = 0;
uintptr_t Base::SRU_Data::g_ownCountryBase = 0;
uintptr_t g_currentCountryBase = 0;
uintptr_t* g_currentCountryNamePtr = 0;

std::string Base::SRU_Data::g_sphereNames[2] = { };

unsigned int ntthreads = std::thread::hardware_concurrency();

//Functions

std::shared_ptr<Base::SRU_Data::UnitDefault> Base::SRU_Data::FindUnitDefault(uintptr_t base, int countryId)
{
	for (int i = 0; i < g_defaultUnitList.size(); i++)
	{
		if (g_defaultUnitList[i]->base == base)
		{
			return g_defaultUnitList[i];
		}
	}

	//Not found -> add

	std::shared_ptr<UnitDefault> newDefaultUnit(new UnitDefault);
	newDefaultUnit->Init(base);

	g_defaultUnitList.push_back(newDefaultUnit);

	return newDefaultUnit;
	//return g_defaultUnitList[0];
}

uint32_t Base::SRU_Data::ResolveUnitCountry(uint32_t country, int dir)
{
	std::string finalStr;
	if (dir == 0)
	{
		//0x??XY???? -> XY
		std::string countryHexStr = Base::Utils::n2hexstr(country);
		finalStr = std::string(1, countryHexStr[2]) + countryHexStr[3];
	}
	else
	{
		//XY -> 0x00XY0000
		std::string countryHexStr = Base::Utils::n2hexstr(country);
		if (countryHexStr.length() < 2)
		{
			countryHexStr = "0" + countryHexStr;
		}
		finalStr = countryHexStr + "0000";
	}
	return std::stoul(finalStr, nullptr, 16);
}

void Base::SRU_Data::HandleFreezes()
{
	for (int i = 0; i < g_countryList.size(); i++)
	{
		g_countryList[i].HandleFreeze();
	}
}

void Base::SRU_Data::CheckSelectedUnits(uintptr_t* selectedUnitsCounter)
{
	if (Base::Utils::CanReadPtr(selectedUnitsCounter))
	{
		if (*selectedUnitsCounter == 0 && g_unitEntityCountSelected > 0)
		{
			g_selectedUnitList.clear();
			g_unitEntityCountSelected = 0;
		}
	}
}

std::vector<uintptr_t> LoadUnitsIntern(bool refresh, bool dir, uintptr_t start, uintptr_t end, int istart, int iend)
{
	//std::cout << "beginintern" << std::endl;

	using namespace Base::SRU_Data;
	uintptr_t main = start;

	std::vector<uintptr_t> tmpUnits;

	bool lastadd = false;

	bool ok = true;
	while (ok)
	{
		uintptr_t unitMain = main;
		uintptr_t unitDef = unitMain + Offsets::unitDefaultValues;
		uintptr_t unitId = unitMain + Offsets::unitId;
		uintptr_t* unitCountry = (uintptr_t*)(unitMain + Offsets::unitCountry);

		bool addOk = true;

		if (refresh)
		{
			bool inCountryList = false;

			if (unitCountry != nullptr)
			{
				Country* tmpCountry = nullptr;
				int csize = g_countryList.size();
				for (int i = 0; i < csize; i++)
				{
					tmpCountry = &g_countryList[i];
					if (tmpCountry->oId == *unitCountry)
					{
						break;
					}
				}

				if (tmpCountry != nullptr)
				{
					int size = tmpCountry->allUnits.size();
					for (int i = 0; i < size; i++)
					{
						if (tmpCountry->allUnits[i].base == unitMain)
						{
							inCountryList = true;
							break;
						}
					}
				}
			}

			if (!inCountryList)
			{
				//Not in country list -> do partial check -> do full check

				for (int i = istart; i < iend; i++)
				{
					if (g_unitList[i].base == main)
					{
						addOk = false;
						break;
					}
				}

				if (addOk)
				{
					int end = g_unitList.size();
					for (int i = 0; i < end; i++)
					{
						if (g_unitList[i].base == main)
						{
							addOk = false;
							break;
						}
					}
				}
			}
			else addOk = false;
		}

		if (addOk)
		{
			if (*(uintptr_t*)unitId > 0)
			{
				if (IsValidUnit(main))
				{
					//Unit newUnit{};
					//newUnit.Init(main);

					tmpUnits.push_back(main);
				}
			}
		}

		uintptr_t* next = nullptr;
		if (dir)
		{
			next = (uintptr_t*)(main + Offsets::unitNext);
		}
		else
		{
			next = (uintptr_t*)(main + Offsets::unitPrev);
		}

		if (next != nullptr && ok)
		{
			//if (Base::Utils::CanReadPtr(next))
			{
				if (*next > 0)
				{
					main = *next;
				}
				else ok = false;
			}
			//else ok = false;
		}
		else ok = false;

		if (main == end) ok = false;
	}

	//std::cout << "endloadintern" << std::endl;

	return tmpUnits;
}

void Base::SRU_Data::LoadUnits(bool refresh)
{
	//std::cout << "begin load units: " << refresh << std::endl;

	if (g_ingame != 2 || g_autosaving)
	{
		return;
	}

	if (!refresh)
	{ //Complete reload
		g_unitList.clear();
	}

	uintptr_t main = 0;

	bool mainOk = Base::Utils::CanReadPtr((uintptr_t*)(g_base + Offsets::allUnitStart));

	if (mainOk)
	{
		main = *(uintptr_t*)(g_base + Offsets::allUnitStart);
	}

	auto clockStart = std::chrono::high_resolution_clock::now();

	if (refresh && g_unitList.size() > 196)
	{
		std::vector<std::future<std::vector<uintptr_t>>> futures;

		int threads = 6;

		if (ntthreads >= 16)
		{
			threads = 8;
		}

		if (ntthreads <= 6)
		{
			threads = 3;
		}

		for (int i = 0; i < threads; i++)
		{
			int istart = (g_unitList.size() / threads) * i;
			int iend = (g_unitList.size() / threads) * (i + 1);

			uintptr_t tmpStart = g_unitList[istart].base;
			uintptr_t tmpEnd = 0xFFFFFFFF;
			
			if (i == threads - 1)
			{
				iend = g_unitList.size();
			}
			else
			{
				tmpEnd = g_unitList[iend].base;
			}

			futures.push_back(std::async(std::launch::async, LoadUnitsIntern, refresh, true, tmpStart, tmpEnd, istart, iend));
		}

		//Combine into one vector and check for duplicates
		int size = g_unitList.size();
		std::vector<uintptr_t> possibleNewUnits;
		for (int i = 0; i < futures.size(); i++)
		{
			std::vector<uintptr_t> tmp = futures[i].get();
			for (int j = 0; j < tmp.size(); j++)
			{
				bool found = false;
				for (int a = 0; a < possibleNewUnits.size(); a++)
				{
					if (possibleNewUnits[a] == tmp[j])
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					possibleNewUnits.push_back(tmp[j]);
				}
			}
		}

		//Final check for duplicates
		for (int i = 0; i < possibleNewUnits.size(); i++)
		{
			bool found = false;
			for (int a = 0; a < size; a++)
			{
				if (g_unitList[a].base == possibleNewUnits[i])
				{
					found = true;
					break;
				}
			}

			if (!found && *g_playSpeedPtr > 0)
			{
				bool uOk = false;

				Unit newUnit{};
				uOk = newUnit.Init(possibleNewUnits[i]);

				if (uOk)
				{
					g_unitList.push_back(newUnit);
				}
			}
		}
	}
	else
	{
		if (mainOk)
		{
			std::vector<uintptr_t> possibleNewUnits = LoadUnitsIntern(refresh, true, main, 0xFFFFFFFF, 0, g_unitList.size());

			//Final check for duplicates
			for (int i = 0; i < possibleNewUnits.size(); i++)
			{
				bool found = false;
				int size = g_unitList.size();
				for (int a = 0; a < size; a++)
				{
					if (g_unitList[a].base == possibleNewUnits[i])
					{
						found = true;
						break;
					}
				}

				bool firstScan = false;
				if (!refresh)
				{
					firstScan = true;
				}

				if (!found && (*g_playSpeedPtr > 0 || firstScan))
				{
					bool uOk = false;

					Unit newUnit{};
					uOk = newUnit.Init(possibleNewUnits[i]);

					if (uOk)
					{
						g_unitList.push_back(newUnit);

						Country* tmpCountry = nullptr;

						for (int n = 0; n < g_countryList.size(); n++)
						{
							tmpCountry = &g_countryList[n];

							if (newUnit.oldCountry == tmpCountry->oId)
							{
								tmpCountry->allUnits.push_back(newUnit);
								break;
							}
						}
					}
				}
			}
		}
	}

#pragma region benchmark

	auto clockEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = clockEnd - clockStart;

	static int benchmarked = 0;
	static double benchmarkResults[3] = { 0, 0, 0 };
	double elapsedTime = fp_ms.count();

	//std::cout << "Elapsed U Time: " << fp_ms.count() << "ms" << std::endl;

	if (refresh && g_unitList.size() > 1000 && benchmarked < 3)
	{
		if (benchmarked < 3)
		{
			benchmarkResults[benchmarked] = elapsedTime;
			benchmarked++;
		}
		if(benchmarked == 3)
		{
			double avg = benchmarkResults[0] + benchmarkResults[1] + benchmarkResults[2];
			avg /= 3;

			//now calc avg per 100 units

			double factor = 100.0 / g_unitList.size();
			avg *= factor;

			avg *= 1000;
			//Simulate 100k units

			if (avg + 1500 > 2500)
			{
				//adjust refresh rate so there are at least 1.5s of breathing room
				g_unitRefreshMaxTime = g_unitRefreshMaxTime + (((avg + 1500) - 2500) / g_unitRefreshMaxTime);
			}
		}
	}

#pragma endregion benchmark

	clockStart = std::chrono::high_resolution_clock::now();

	if (!refresh)
	{
		Country* tmpCountry = nullptr;
		int size = g_unitList.size();

		for (int n = 0; n < g_countryList.size(); n++)
		{
			tmpCountry = &g_countryList[n];
			tmpCountry->allUnits.clear();

			for (int i = 0; i < size; i++)
			{
				if (g_unitList[i].oldCountry == tmpCountry->oId)
				{
					tmpCountry->allUnits.push_back(g_unitList[i]);
				}
			}

			//std::cout << "Country " << tmpCountry->name << " has " << tmpCountry->allUnits.size() << " units" << std::endl;
		}
	}

	clockEnd = std::chrono::high_resolution_clock::now();
	fp_ms = clockEnd - clockStart;

	//std::cout << "Elapsed C Time: " << fp_ms.count() << "ms" << std::endl;

	if (g_defaultUnitList.size() > 2)
	{
		//Sort by name (insertion sort)
		for (int i = 1; i < g_defaultUnitList.size(); i++)
		{
			std::shared_ptr<UnitDefault> tmp = g_defaultUnitList[i];
			int n = i - 1;

			while (n >= 0 && g_defaultUnitList[n]->name > tmp->name)
			{
				g_defaultUnitList[n + 1] = g_defaultUnitList[n];
				n--;
			}
			g_defaultUnitList[n + 1] = tmp;
		}
	}

	//std::cout << "End load units" << std::endl;
	//std::cout << "Total default units found: " << std::dec << g_defaultUnitList.size() << std::endl;
	//std::cout << "Total units found: " << std::dec << g_unitList.size() << std::endl;	
}

void Base::SRU_Data::LoadDefaultUnits()
{
	g_defaultUnitList.clear();
	g_facilityList.clear();
	g_paintUnitModes.clear();

	for (int i = 0; i < Unit::Property::MAX; i++)
	{
		g_paintUnitModes.push_back(-1);
	}

	uintptr_t start = *(uintptr_t*)(g_base + Offsets::allDefaultUnitStart);
	uintptr_t main = start;

	MEMORY_BASIC_INFORMATION mBI, mBINext;
	
	VirtualQuery((LPVOID)start, &mBI, sizeof(MEMORY_BASIC_INFORMATION));

	DWORD firstOldProtect = NULL;
	VirtualProtect((LPVOID)start, mBI.RegionSize, PAGE_EXECUTE_READWRITE, &firstOldProtect);

	uintptr_t regionBase = (uintptr_t)mBI.BaseAddress;
	uintptr_t regionMax = (uintptr_t)mBI.BaseAddress + mBI.RegionSize;

	int count = 0;
	for (uintptr_t i = start; i < regionMax; i += Offsets::unitDefaultNext)
	{
		uintptr_t* check1 = (uintptr_t*)i;
		uintptr_t* check2 = (uintptr_t*)(i + Offsets::unitDefaultCheck1);
		uintptr_t* check3 = (uintptr_t*)(i + Offsets::unitDefaultCheck2);

		if (Base::Utils::CanReadPtr(check1) &&
			Base::Utils::CanReadPtr(check2) &&
			Base::Utils::CanReadPtr(check3))
		{
			if (*check1 > 0 && *check2 == 0 && *check3 > 0)
			{
				if (IsValidDefaultUnit(i))
				{
					std::shared_ptr<UnitDefault> newDefaultUnit(new UnitDefault);
					newDefaultUnit->spawnId = count;
					newDefaultUnit->Init(i);

					//std::cout << std::hex << i << " " << newDefaultUnit->name << " " << newDefaultUnit->spawnId << std::endl;

					g_defaultUnitList.push_back(newDefaultUnit);
				}
			}
		}

		if (Base::Utils::CanReadPtr(check1))
		{
			char* namePtr = (char*)*(uintptr_t*)i;
			if (Base::Utils::CanReadPtr(namePtr))
			{
				int classVal = *(uint8_t*)(i + Offsets::unitDefaultClass);

				if (classVal == 21)
				{
					//Check for duplicates
					
					std::string fName = std::string(namePtr);

					bool found = false;

					for (int i = 0; i < g_facilityList.size(); i++)
					{
						if (g_facilityList[i]->name == fName)
						{
							found = true;

							g_facilityList[i]->variantIDs.push_back(count);
							break;
						}
					}

					//add

					if (!found)
					{
						std::shared_ptr<Facility> newFacility(new Facility);

						newFacility->id = count;
						newFacility->name = fName;

						g_facilityList.push_back(newFacility);
					}
				}
			}
		}

		count++;
	}

	//Sort facilities by name
	for (int i = 1; i < g_facilityList.size(); i++)
	{
		std::shared_ptr<Facility> tmp = g_facilityList[i];
		int n = i - 1;

		while (n >= 0 && g_facilityList[n]->name > tmp->name)
		{
			g_facilityList[n + 1] = g_facilityList[n];
			n--;
		}
		g_facilityList[n + 1] = tmp;
	}

	//auto clockEnd = std::chrono::high_resolution_clock::now();

	//std::cout << "Total default units found: " << std::dec << g_defaultUnitList.size() << std::endl;

	//std::chrono::duration<double, std::milli> fp_ms = clockEnd - clockStart;
	//std::cout << "Elapsed Time: " << fp_ms.count() << std::endl;
}

void Base::SRU_Data::LoadTechnologies()
{
	g_techList.clear();

	int maxTech = *(int*)(g_base + Offsets::techIdMax);

	uintptr_t techStart = *(uintptr_t*)(g_base + Offsets::techIdStart);

	for (int i = 0; i < maxTech; i++)
	{
		int v3 = 88 * i;

		//int main = (v3 + techStart + 0x38);
		int mini = (techStart + v3);

		if (*(uint8_t*)mini)
		{
			//valid

			char* namePtr = (char*)*(uintptr_t*)(mini + 0x4);

			if (namePtr != nullptr)
			{
				std::shared_ptr<Tech> tech(new Tech);

				tech->name = std::string(namePtr);
				tech->id = i;
				tech->base = (uintptr_t)mini;
				tech->category = *(uint8_t*)(mini);

				g_techList.push_back(tech);
			}
		}
	}

	//sort by name
	for (int i = 1; i < g_techList.size(); i++)
	{
		std::shared_ptr<Tech> tmp = g_techList[i];
		int n = i - 1;

		while (n >= 0 && g_techList[n]->name > tmp->name)
		{
			g_techList[n + 1] = g_techList[n];
			n--;
		}
		g_techList[n + 1] = tmp;
	}

	for (int i = 0; i < g_countryList.size(); i++)
	{
		g_countryList[i].RefreshResearch();
	}
}

void Base::SRU_Data::LoadLeaders()
{
	g_leaderList.clear();

	uintptr_t leaderStart = *(uintptr_t*)(g_base + Offsets::allLeaderStart);
	int leaderMax = *(int*)(g_base + Offsets::leaderMax);

	for (int i = 1; i < leaderMax; i++) {
		int offset = 196 * i;

		uintptr_t base = leaderStart + offset;

		if (*(uintptr_t*)base) {

			char* namePtr = (char*)*(uintptr_t*)(base + 0x8);
			char* surnamePtr = (char*)*(uintptr_t*)(base + 0xC);

			if (namePtr != nullptr && surnamePtr != nullptr) {
				
				std::shared_ptr<Leader> newLeader(new Leader);

				newLeader->id = *(uintptr_t*)base;
				newLeader->name = std::string(namePtr) + " " + std::string(surnamePtr);

				g_leaderList.push_back(newLeader);
			}
		}
	}

	//Sort leaders by name
	for (int i = 1; i < g_leaderList.size(); i++)
	{
		std::shared_ptr<Leader> tmp = g_leaderList[i];
		int n = i - 1;

		while (n >= 0 && g_leaderList[n]->name > tmp->name)
		{
			g_leaderList[n + 1] = g_leaderList[n];
			n--;
		}
		g_leaderList[n + 1] = tmp;
	}

	//Assign leaders to countries

	for (int i = 0; i < g_countryList.size(); i++) {
		g_countryList[i].RefreshLeader();
	}
}

void Base::SRU_Data::LoadSettings()
{
	bool openChangelog = false;

	if (std::filesystem::exists("sru_internal_config.ini"))
	{
		int configVersion = 0;

		std::ifstream file("sru_internal_config.ini");

		for (std::string line; getline(file, line); )
		{
			Base::Utils::GetSettingsInt(line, "version", configVersion);
			Base::Utils::GetSettingsBool(line, "fastroad", g_fastRoad);
			//Base::Utils::GetSettingsBool(line, "unitselect", g_unitSelect);
			//Base::Utils::GetSettingsBool(line, "hexsupply", g_hexSupply);
			g_hexSupply = true;
			Base::Utils::GetSettingsUInt8(line, "hsupplyval", Asm::g_lowestHexSupply);
			Base::Utils::GetSettingsBool(line, "aicolony", g_aiColony);
			Base::Utils::GetSettingsBool(line, "prodadjustment", g_productionAdjustment);
		}

		if (g_version > configVersion)
		{
			openChangelog = true;
		}
	} else openChangelog = true;

	if (openChangelog)
	{
		Offsets::Changelog();
		SaveSettings();
	}
}

void Base::SRU_Data::SaveSettings()
{
	std::ofstream file("sru_internal_config.ini", std::ios::trunc);

	if (file.is_open())
	{
		file << Base::Utils::SetSettingsInt("version", g_version);
		file << Base::Utils::SetSettingsBool("fastroad", g_fastRoad);
		//file << Base::Utils::SetSettingsBool("unitselect", g_unitSelect);
		file << Base::Utils::SetSettingsUInt8("hsupplyval", Asm::g_lowestHexSupply);
		file << Base::Utils::SetSettingsBool("aicolony", g_aiColony);
		file << Base::Utils::SetSettingsBool("prodadjustment", g_productionAdjustment);
		file << Base::Utils::SetSettingsBool("modenabled", true);
		file.close();
	}
}

void Base::Init(bool full = false)
{
	Hooks::Init(full);
}