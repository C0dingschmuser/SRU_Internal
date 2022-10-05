#include "base.h"

//Data Vars

EndScene Base::Data::oEndScene = NULL;
Reset Base::Data::oReset = NULL;
WndProc_t Base::Data::oWndProc = NULL;
HWND Base::Data::window = NULL;
bool Base::Data::init = false;
bool Base::Data::ShowMenu = true;
int Base::Data::subMenu = 0;

//Asm Vars

unsigned int Base::SRU_Data::Asm::g_hexReg0;
unsigned int Base::SRU_Data::Asm::g_hexReg1;
unsigned int Base::SRU_Data::Asm::g_hexReg2;
unsigned int Base::SRU_Data::Asm::g_hexReg3;

unsigned int Base::SRU_Data::Asm::g_aiSurrReg0;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg1;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg2;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg3;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg4;
unsigned int Base::SRU_Data::Asm::g_aiSurrReg5;

int Base::SRU_Data::Asm::g_aiSurrSize;
int Base::SRU_Data::Asm::g_aiSurrFrom;
int Base::SRU_Data::Asm::g_aiSurrTo;

int Base::SRU_Data::Asm::g_xPos = 0;
int Base::SRU_Data::Asm::g_yPos = 0;

uintptr_t Base::SRU_Data::Asm::g_aiSurrBase;

//SRU Vars

std::vector<std::shared_ptr<Base::SRU_Data::UnitDefault>> Base::SRU_Data::g_defaultUnitList;
std::vector<Base::SRU_Data::Unit> Base::SRU_Data::g_selectedUnitList;
std::vector<Base::SRU_Data::Country> Base::SRU_Data::g_countryList;
std::vector<Base::SRU_Data::Unit> Base::SRU_Data::g_unitList;
std::vector<Base::SRU_Data::DiplTreaty> Base::SRU_Data::g_diplTreatyList;
std::vector<Base::SRU_Data::GroundType> Base::SRU_Data::g_groundTypeList;

Base::SRU_Data::SurrenderEvent Base::SRU_Data::surrenderEvents[32] = { };

uintptr_t Base::SRU_Data::Hooks::g_selectedJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_hexSupplyJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_aiSurrenderJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_mouseClickedJmpBackAddr = 0;
uintptr_t Base::SRU_Data::Hooks::g_posChangedJmpBackAddr = 0;

uintptr_t Base::SRU_Data::g_nextUnitEntity = 0;

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

int Base::SRU_Data::g_unitSpawnSelectedUnitDesign = -1;

byte Base::SRU_Data::Asm::g_currentHexSupply = 0;
byte Base::SRU_Data::Asm::g_lowestHexSupply = 0x1A;

bool Base::SRU_Data::g_addOk = false;
bool Base::SRU_Data::g_shift = false;
bool Base::SRU_Data::g_uiHexSupplySet = false;
bool Base::SRU_Data::g_mouseClicked = false;
bool Base::SRU_Data::g_paintActive = false;
bool Base::SRU_Data::g_paintEnabled = false;

bool Base::SRU_Data::g_productionAdjustment = false;
bool Base::SRU_Data::g_aiColony = true;

uint8_t Base::SRU_Data::g_currentHexSupply = 0;

Base::SRU_Data::Country* Base::SRU_Data::clickedCountry = nullptr;
Base::SRU_Data::Country* Base::SRU_Data::clickedTargetCountry = nullptr;
Base::SRU_Data::Country* Base::SRU_Data::unitSpawnCountry = nullptr;

uintptr_t* Base::SRU_Data::g_clickedHexPtr = nullptr;

//SRU Game Vars
bool Base::SRU_Data::g_ingame = false;
uintptr_t Base::SRU_Data::g_base = 0;
uintptr_t Base::SRU_Data::g_ownCountryBase = 0;
uintptr_t g_currentCountryBase = 0;
uintptr_t* g_currentCountryNamePtr = 0;

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

bool LoadUnitsIntern(bool refresh, bool dir, uintptr_t start, uintptr_t middle, int istart, int iend)
{
	using namespace Base::SRU_Data;
	uintptr_t main = start;

	std::vector<Base::SRU_Data::Unit> tmpUnits;

	bool ok = true;
	while (ok)
	{
		uintptr_t unitMain = main;
		uintptr_t unitDef = unitMain + Offsets::unitDefaultValues;
		uintptr_t unitId = unitMain + Offsets::unitId;

		bool addOk = true;

		if (refresh)
		{
			for (int i = istart; i < iend; i++)
			{
				if (g_unitList[i].base == main)
				{
					addOk = false;
					break;
				}
			}
		}

		if (addOk)
		{
			if (*(uintptr_t*)unitId > 0)
			{
				if (Base::Utils::CanReadPtr((uintptr_t*)*(uintptr_t*)unitDef))
				{
					Unit newUnit{};
					newUnit.Init(main);

					tmpUnits.push_back(newUnit);
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

		if (main == middle) ok = false;
	}

	for (int i = 0; i < tmpUnits.size(); i++)
	{
		g_unitList.push_back(tmpUnits[i]);
	}

	return true;
}

void Base::SRU_Data::LoadUnits(bool refresh)
{
	if (!refresh)
	{ //Complete reload
		g_unitList.clear();
	}

	uintptr_t main = *(uintptr_t*)(g_base + Offsets::allUnitStart);
	
	if (refresh)
	{
		//main = g_unitList[g_unitList.size() - 1].base;
	}

	auto clockStart = std::chrono::high_resolution_clock::now();

	if (refresh && g_unitList.size() > 96)
	{
		int imiddle = g_unitList.size() / 2;
		uintptr_t middle = g_unitList[imiddle].base;
		uintptr_t end = g_unitList[g_unitList.size() - 1].base;

		std::future<bool> fut1 = std::async(std::launch::async, LoadUnitsIntern, refresh, true, main, middle, 0, imiddle); //forwards
		std::future<bool> fut2 = std::async(std::launch::async, LoadUnitsIntern, refresh, false, end, middle, imiddle, g_unitList.size()); //backwards

		fut1.get();
		fut2.get();
	}
	else
	{
		LoadUnitsIntern(refresh, true, main, 0xFFFFFFFF, 0, g_unitList.size());
	}

	auto clockEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = clockEnd - clockStart;
	std::cout << "Elapsed Time: " << fp_ms.count() << std::endl;

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

	std::cout << "Total default units found: " << std::dec << g_defaultUnitList.size() << std::endl;
	std::cout << "Total units found: " << std::dec << g_unitList.size() << std::endl;	
}

void Base::SRU_Data::LoadDefaultUnits()
{
	g_defaultUnitList.clear();

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

				std::shared_ptr<UnitDefault> newDefaultUnit(new UnitDefault);
				newDefaultUnit->spawnId = count;
				newDefaultUnit->Init(i);

				g_defaultUnitList.push_back(newDefaultUnit);
			}
		}
		count++;
	}

	//auto clockEnd = std::chrono::high_resolution_clock::now();

	std::cout << "Total default units found: " << std::dec << g_defaultUnitList.size() << std::endl;

	//std::chrono::duration<double, std::milli> fp_ms = clockEnd - clockStart;
	//std::cout << "Elapsed Time: " << fp_ms.count() << std::endl;
}

void Base::Init(bool full = false)
{
	Hooks::Init(full);
}

bool Base::Utils::MemCompare(const BYTE* bData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++bData, ++bMask)
	{
		if (*szMask == 'x' && *bData != *bMask)
		{
			return false;
		}
	}
	return (*szMask == NULL);
}

bool Base::Utils::CanReadPtr(void* ptr)
{
	if (ptr == nullptr) return false;

	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(ptr, &mbi, sizeof(mbi));

	return (!(mbi.Protect & PAGE_GUARD) && mbi.Protect != PAGE_NOACCESS && mbi.State == MEM_COMMIT &&
		mbi.Protect & (PAGE_READONLY | PAGE_EXECUTE_READ | PAGE_READWRITE | PAGE_EXECUTE_READWRITE));
}

uintptr_t Base::Utils::PointerChain(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

uintptr_t Base::Utils::PatternScan(HANDLE hProcess, uintptr_t start, uintptr_t size, const char* sig, const char* mask)
{
	BYTE* data = new BYTE[size];
	SIZE_T bytesRead;

	ReadProcessMemory(hProcess, (LPVOID)start, data, size, &bytesRead);

	for (uintptr_t i = 0; i < size; i++)
	{
		if (MemCompare((const BYTE*)(data + i), (const BYTE*)sig, mask))
		{
			return start + i;
		}
	}
	delete[] data;
	return NULL;
}

void Base::Utils::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Base::Utils::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Base::Utils::WPM(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Base::Utils::RPM(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	ReadProcessMemory(hProcess, dst, src, size, nullptr);
}