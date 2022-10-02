// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "Base.h"

using namespace Base::SRU_Data;
using namespace Base::SRU_Data::Hooks;
using namespace Base::SRU_Data::Asm;

int FindNewCountryOwner(uint8_t oldOwner, bool weak = false);

/*void __declspec(naked) CheckAISurrender()
{
    __asm {
        mov esi, [edx + 0x0000A2B4]
        mov [g_aiSurrBase], eax
        mov [g_aiSurrReg0], eax
        mov [g_aiSurrReg1], ecx
        mov [g_aiSurrReg2], edx
        mov [g_aiSurrReg3], ebp
        mov ecx, [ebp + 0x8]
        mov edx, [ebp + 0x10]
        mov eax, [ebp + 0x1C]
        mov [g_aiSurrSize], ecx
        mov [g_aiSurrFrom], edx
        mov [g_aiSurrTo], eax
    }

    DebugLog();
    if (g_aiSurrSize == Offsets::surrenderDiplo)
    {
        CheckSurrender();
    }

    __asm {
        mov eax, g_aiSurrReg0
        mov ecx, g_aiSurrReg1
        mov edx, g_aiSurrReg2
        mov ebp, g_aiSurrReg3
        jmp [g_aiSurrenderJmpBackAddr]
    }
}*/


void SetupSessionPtr(uintptr_t base = NULL)
{
    if (base == NULL)
    {
        base = (uintptr_t)GetModuleHandle(NULL);
    }

	//Load Countries

    g_ownCountryBase = *(uintptr_t*)(base + Offsets::ownCountry);

	//reset so re-initialized for new country
    Base::SRU_Data::g_unitSpawnSelectedUnitDesign = -1;

    Base::SRU_Data::g_countryList.clear();

    bool end = false;
    uintptr_t start = base + Offsets::allCountryStart;
    while (!end)
    {
        uintptr_t* address = (uintptr_t*)start;

        if (*address > 0)
        {
            Country c{};
            c.Init(*address);
            c.id = g_countryList.size();
            Base::SRU_Data::g_countryList.push_back(c);
        }
        else
        {
            end = true;
        }

        start += 4;
    }

    std::sort(g_countryList.begin(), g_countryList.end());

    Country own;
    int pos = 0;
    for (int i = 0; i < g_countryList.size(); i++)
    {
        if (g_countryList[i].base == g_ownCountryBase)
        {
            own = g_countryList[i];
            pos = i;
            break;
        }
    }

    g_ownCountryId = own.id;
    g_ownOtherCountryId = own.oId;
    
    bool ok = false;
    while (!ok && g_countryList.size() > 1)
    {
        int num = std::rand() % g_countryList.size();

        if (num != pos)
        {
            ok = true;

            g_selectedTargetCountry = num;
            Base::SRU_Data::clickedTargetCountry = &g_countryList[num];
        }
    }

    //Put own country at beginning of list
    g_countryList.erase(g_countryList.begin() + pos);
    g_countryList.insert(g_countryList.begin(), own);

    Base::Execute::SetupFunctions();

    Base::SRU_Data::LoadDefaultUnits();
    Base::SRU_Data::LoadUnits();
    Base::SRU_Data::LoadDiplTreaties();
    Base::SRU_Data::LoadGroundTypes();	

    //Base::Execute::SpawnUnit(2304, 256, g_ownCountryBase, false, 875, 151);
}

void CheckGameState(uintptr_t* gameStatePtr)
{
    bool ok = false;
    if (Base::Utils::CanReadPtr(gameStatePtr))
    {
        if (*gameStatePtr == 2)
        {
            if (!g_ingame)
            {
                SetupSessionPtr();
            }
            ok = true;
        }
    }

    g_ingame = ok;
}

void CheckCurrentCountry(uintptr_t* clickedCountryPtr)
{
    if (g_mouseClicked && g_ingame && g_countryList.size() > 0)
    {
        g_mouseClicked = false;
        //if (g_lastClickedCountry != *clickedCountryPtr)
        {
            g_lastClickedCountry = *clickedCountryPtr;

            if (!g_shift)
            {
                //Set selected country

                //if (*clickedCountryPtr == g_lastClickedTargetCountry) return;

                g_clickedCountry = *clickedCountryPtr;

                for (int i = 0; i < g_countryList.size(); i++)
                {
                    if (g_clickedCountry == g_countryList[i].id)
                    {
                        g_clickedCountryRaw = i;
                        clickedCountry = &g_countryList[i];
                        break;
                    }
                }

                if (!clickedCountry)
                {
                    g_clickedCountryRaw = 0;
                    clickedCountry = &g_countryList[0];
                }
            }
            else
            {
                //Set target country

                int newTarget = *clickedCountryPtr;

                //if (newTarget == currentClickedCountry) return;

                g_lastClickedTargetCountry = newTarget;

                for (int i = 0; i < g_countryList.size(); i++)
                {
                    if (newTarget == g_countryList[i].id)
                    {
                        g_selectedTargetCountry = i;
                        Base::SRU_Data::clickedTargetCountry = &g_countryList[i];
                        break;
                    }
                }

                if (!Base::SRU_Data::clickedTargetCountry)
                {
                    int num = std::rand() % g_countryList.size();
                    Base::SRU_Data::clickedTargetCountry = &g_countryList[num];
                }
            }
        }
    }
}

static int lastProcessTime = 0;

void ProcessAiSurrenders()
{
    /*
    --- Alternative method ---
    for (int i = 0; i < g_countryList.size(); i++)
    {
        Country* c = &g_countryList[i];
        if (c->alive == 1)
        {
            int newAlive = 0;

            if (Base::Utils::CanReadPtr(c->populationPtr))
            {
                if (*c->populationPtr > 0)
                {
                    newAlive = 1;
                }
            }

            if (newAlive == 0)
            {
                SurrenderEvent* s = &surrenderEvents[g_surrenderEventCount];
                s->lastTime = *(uintptr_t*)(g_base + Offsets::currentDayTime) + 500;
                s->from = c->oId;
                s->to = FindNewCountryOwner(c->oId, false);

                std::cout << "surrender: " << s->from << " " << s->to << std::endl;

                g_surrenderEventCount++;
                if (g_surrenderEventCount >= 32)
                {
                    g_surrenderEventCount = 0;
                }

                c->alive = newAlive;
            }
        }
    }*/

    int currentTime = *(uintptr_t*)(g_base + Offsets::currentDayTime);

    if (currentTime < lastProcessTime + 50) return;

    for (int i = 0; i < 32; i++)
    {
        if (surrenderEvents[i].lastTime > -1)
        {
            if (currentTime > surrenderEvents[i].lastTime + 50)
            {
                int from = surrenderEvents[i].from;
                int to = surrenderEvents[i].to;

                surrenderEvents[i].lastTime = -1;

                if (from != 0)
                {
                    std::cout << "exec " << from << " " << to << std::endl;

                    Base::Execute::RespawnCountry(from, to, 2);

                    lastProcessTime = currentTime;
                    return;
                }
            }
        }
    }
}

int FindNewCountryOwner(uint8_t oldOwner, bool weak)
{
    //Could propably be improved by checking for loyalty
    //changes before and after event and comparing instead of getting largest non-loyal region

    MEMORY_BASIC_INFORMATION mBI;

    uintptr_t startAddr = *(uintptr_t*)(g_base + Offsets::mouseHoverHex);

    VirtualQuery((LPVOID)startAddr, &mBI, sizeof(MEMORY_BASIC_INFORMATION));

    DWORD firstOldProtect = NULL;
    VirtualProtect((LPVOID)startAddr, mBI.RegionSize, PAGE_EXECUTE_READWRITE, &firstOldProtect);

    uintptr_t regionBase = (uintptr_t)mBI.BaseAddress;
    uintptr_t regionMax = (uintptr_t)mBI.BaseAddress + mBI.RegionSize;

    std::vector<std::vector<unsigned int>> newOwners;

    for (uintptr_t i = regionBase; i < regionMax; i += 0x10)
    {
        uint8_t* loyalty = (uint8_t*)(uintptr_t*)(i + 0x1);
        uint8_t* owner = (uint8_t*)(uintptr_t*)(i);
        uint8_t* terr = (uint8_t*)(uintptr_t*)(i + 0x2);
        uint8_t* terr2 = (uint8_t*)(uintptr_t*)(i + 0x7);

        if (*loyalty == oldOwner && *owner != oldOwner && 
            ((!weak && *terr > 0 && *terr2 > 0) || weak))
        {
            //int l = *loyalty;
            //uintptr_t addr = *(uintptr_t*)(g_base + Offsets::allCountryStart + 4 * l);
            //int alive = *(uintptr_t*)(addr + 0xA8A0);

            //if (alive == 0)
            {
                bool found = false;
                for (int n = 0; n < newOwners.size(); n++)
                {
                    if (newOwners[n][0] == *owner)
                    {
                        newOwners[n][1]++;
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    std::vector<unsigned int> data;
                    data.push_back(*owner);
                    data.push_back(1);

                    newOwners.push_back(data);
                }
            }

        }
    }

    VirtualProtect((LPVOID)startAddr, mBI.RegionSize, firstOldProtect, NULL);

    unsigned int largestPos = 0;
    unsigned largestCount = 0;

    for (int i = 0; i < newOwners.size(); i++)
    {
        if (newOwners[i][1] > largestCount)
        {
            largestPos = i;
            largestCount = newOwners[i][1];
        }
    }

    if (newOwners.size() > 0)
    {
        return newOwners[largestPos][0];
    }
    else return 0;
}

void PaintMap(uintptr_t* mouseHoverHex)
{
    if (mouseHoverHex == nullptr) return;
    if (*mouseHoverHex == 0) return;
	
    uint8_t* ownerPtr = (uint8_t*)(uintptr_t*)(*mouseHoverHex);
    uint8_t* loyaltyPtr = (uint8_t*)(uintptr_t*)(*mouseHoverHex + Offsets::hexLoyalty);
    uint8_t* groundPtr = (uint8_t*)(uintptr_t*)(*mouseHoverHex + Offsets::hexGround);

    *ownerPtr = (uint8_t)g_countryList[g_paintSelectedComboCountry].oId;
    *loyaltyPtr = (uint8_t)g_countryList[g_paintSelectedComboLoyalty].oId;

    if (g_paintSelectedComboGround > -1)
    {
        *groundPtr = (uint8_t)g_groundTypeList[g_paintSelectedComboGround].id;
    }
}

DWORD WINAPI dllThread(HMODULE hModule) {
    DWORD dwExit = 0;

    Base::Init(true);

    AllocConsole();
    FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    uintptr_t procMin = (uintptr_t)si.lpMinimumApplicationAddress;
    uintptr_t procMax = (uintptr_t)si.lpMaximumApplicationAddress;

    MEMORY_BASIC_INFORMATION mBI, mBINext;

    //Ptr setup

    g_base = (uintptr_t)GetModuleHandle(NULL);
    g_clickedHexPtr = (uintptr_t*)(g_base + Offsets::mouseClickHex);

    uintptr_t* gameStatePtr = (uintptr_t*)(g_base + Offsets::gameState);
    uintptr_t* clickedCountryPtr = (uintptr_t*)(g_base + Offsets::clickedCountry);
    uintptr_t* selectedUnitsCounterPtr = (uintptr_t*)(g_base + Offsets::selectedUnitsCounter);
    uintptr_t* mouseHoverHex = (uintptr_t*)(g_base + Offsets::mouseHoverHex);

    //Main loop

    Base::Execute::SetupFunctions();
    Base::SRU_Data::Hooks::SetupFunctionHooks();
    
    bool finish = false;
    while (!finish) {
        if (GetAsyncKeyState(VK_END) & 1) {
            finish = true;
        }

        CheckGameState(gameStatePtr);

        if (g_ingame)
        {
            if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
            {
                g_shift = true;
            }
            else g_shift = false;

            if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
            {
                g_paintEnabled = true;
            }
            else g_paintEnabled = false;

            if (g_paintEnabled)
            {
                if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
                {
                    g_paintActive = true;
                }
                else g_paintActive = false;

                if (g_paintActive)
                {
                    PaintMap(mouseHoverHex);
                }
            }

            CheckCurrentCountry(clickedCountryPtr);
            Base::SRU_Data::HandleFreezes();
            Base::SRU_Data::CheckSelectedUnits(selectedUnitsCounterPtr);
            ProcessAiSurrenders();
        }

        Sleep(25);
    }

    /*
    SuspendThreads(true);

    procMin = 0x15000000;
	
    if (procMax > 0x50000000) {
        procMax = 0x50000000;
    }

    int start = 0;
    int unitsFound = 0;

    std::vector<Unit> unitList;

    auto clockStart = std::chrono::high_resolution_clock::now();

    DWORD firstOldProtect = NULL;

    while (procMin < procMax) {
        VirtualQuery((LPVOID)procMin, &mBI, sizeof(MEMORY_BASIC_INFORMATION));

        if (!(mBI.Protect & PAGE_GUARD) && mBI.Protect != PAGE_NOACCESS && mBI.State == MEM_COMMIT &&
            mBI.Protect & (PAGE_READONLY | PAGE_EXECUTE_READ | PAGE_READWRITE | PAGE_EXECUTE_READWRITE)) {
			
            VirtualProtect((LPVOID)procMin, mBI.RegionSize, PAGE_EXECUTE_READWRITE, &firstOldProtect);  // Set page to read/write/execute

            uintptr_t regionBase = (uintptr_t)mBI.BaseAddress;
            uintptr_t regionMax = (uintptr_t)mBI.BaseAddress + mBI.RegionSize;

            int incr = 0x04;
            bool unitFound = false;

            for (uintptr_t n = regionBase; n < regionMax; n += incr) {
                uintptr_t* addressPtr = (uintptr_t*)n;
                uint32_t val = 0;
				
                if (n > regionMax) {
                    break;
                }

                bool no = false;

                if (addressPtr != nullptr) {
					
                    //std::cout << "trying read: " << std::hex << addressPtr << std::endl;

                    val = *addressPtr;
					
                    if (val) {
                        if (val == 0x16BC580) {

                            uintptr_t* testValidPtr = (uintptr_t*)(n + 0x14);
                            //std::cout << "trying read country: " << std::hex << addressPtr << " " << testValidPtr << " ";
                            if (testValidPtr != nullptr) {
                                //std::cout << "ok1 ";
                                if (*testValidPtr < 0x80000000 && *testValidPtr > 0) {
                                    unitsFound++;

                                    //std::cout << " ok2";

                                    Unit unit{};
                                    unit.base = n;
                                    unit.country = testValidPtr;
                                    unit.countryID = resolveUnitCountry(*unit.country);
                                    unitList.push_back(unit);
                                }
                            }
                            //std::cout << std::endl;

                            if (!unitFound) {
                                //Erste Einheit im Block gefunden, von jetzt an in 0x1C8 steps
                                unitFound = true;
                                incr = 0x1C8;
                            }
                        }
                        else {
                            no = true;
                        }
                    }
                }
                else {
                    no = true;
                }

                if (no) {
                    if (unitFound) {
                        n -= 0x1C8;
                        incr = 0x04;
                        unitFound = false;
                    }
                }
            }

            VirtualProtect((LPVOID)procMin, mBI.RegionSize, firstOldProtect, NULL);
        }
        procMin += mBI.RegionSize;
    }

    SuspendThreads(false);

	auto clockEnd = std::chrono::high_resolution_clock::now();

	std::cout << "Total units found: " << std::dec << unitList.size() << std::endl;
	
    std::chrono::duration<double, std::milli> fp_ms = clockEnd - clockStart;

    std::cout << "Elapsed Time: " << fp_ms.count() << std::endl;

    bool finish = false;
    bool malen = false;

    int malValue = 0xF38585;

    /*while (!finish) {
        if (GetAsyncKeyState(VK_END) & 1) {
            finish = true;
        }
		
        if (GetAsyncKeyState(VK_CONTROL) & 1) {
            if (!malen) {
                malen = true;
            }
            else {
                malen = false;
            }

            std::cout << "Malen: " << malen << std::endl;
        }

		uintptr_t* hoverKachel = (uintptr_t*)(base + 0x1033C88);

        //std::cout << std::hex << hoverKachel << std::endl;

        if (malen) {
            if (CanReadPtr(hoverKachel)) {
                uintptr_t* hoverKachelValue = (uintptr_t*)*hoverKachel;
				if (hoverKachelValue != nullptr) {
                    if (*hoverKachelValue != malValue) {
                        std::cout << std::hex << *hoverKachel << " " << *hoverKachelValue << std::endl;

                        *hoverKachelValue = malValue;
                    }
                }
            }
        }

        Sleep(5);
    }*/
	

    /*Sleep(2500);

    for (int i = 0; i < unitList.size(); i++) {
        if (unitList[i].countryID == 133) {
            uintptr_t* countryValue = (uintptr_t*)unitList[i].country;
			
            if (CanReadPtr(countryValue)) {
                if (*countryValue < 0x80000000 && *countryValue > 0) {
                    *countryValue = resolveUnitCountry(28, 1);
                }
                //std::cout << std::hex << countryValue << " " <<  *countryValue << std::endl;
            }
        }
    }*/

    fclose(f);
    fclose(stdout);
    fclose(stdin);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return dwExit;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)dllThread, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

