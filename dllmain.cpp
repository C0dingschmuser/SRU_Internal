// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "Base.h"

using namespace Base::SRU_Data;
using namespace Base::SRU_Data::Hooks;
using namespace Base::SRU_Data::Asm;

int FindNewCountryOwner(uint8_t oldOwner, bool weak = false);

int unitTimer = 0;

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
            if (Base::Utils::CanReadPtr((uintptr_t*)*address))
            {
                Country c{};
                c.Init(*address);
                c.id = g_countryList.size();

                if (c.real)
                {
                    Base::SRU_Data::g_countryList.push_back(c);
                }
            }
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

    Base::SRU_Data::Asm::g_ownAllocs.clear();

    unitTimer = 0;
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
    //std::cout << g_mouseClicked << " " << g_ingame << g_countryList.size() << std::endl; 

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

                if (g_clickedCountry != *clickedCountryPtr)
                {
                    g_unitSpawnSelectedUnitDesign = -1;
                }

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

                Base::Draw::g_countryColorLoaded = false;
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
                    //std::cout << "exec " << from << " " << to << std::endl;

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

    //AllocConsole();
    //FILE* f;
	//freopen_s(&f, "CONOUT$", "w", stdout);
    //freopen_s(&f, "CONIN$", "r", stdin);

    //Ptr setup

    g_base = (uintptr_t)GetModuleHandle(NULL);
    g_clickedHexPtr = (uintptr_t*)(g_base + Offsets::mouseClickHex);
    g_playSpeedPtr = (uintptr_t*)(g_base + Offsets::gameSpeed);

    g_clickedXPtr = (uint16_t*)(g_base + Offsets::position[0]);
    g_clickedYPtr = (uint16_t*)(g_base + Offsets::position[1]);

    uintptr_t* gameStatePtr = (uintptr_t*)(g_base + Offsets::gameState);
    uintptr_t* clickedCountryPtr = (uintptr_t*)(g_base + Offsets::clickedCountry);
    uintptr_t* selectedUnitsCounterPtr = (uintptr_t*)(g_base + Offsets::selectedUnitsCounter);
    uintptr_t* mouseHoverHex = (uintptr_t*)(g_base + Offsets::mouseHoverHex);

    //Main loop

    Base::Execute::SetupFunctions();
    Base::SRU_Data::Hooks::SetupFunctionHooks();

    int mainTimer = 0;
    int discoTimer = 0;

    bool finish = false;
    while (!finish) {

        mainTimer++;
        if (mainTimer >= 2)
        {
			//only exec every 2nd loop
            mainTimer = 0;
			
            if (GetAsyncKeyState(VK_END) & 1)
            {
                //finish = true;
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
                Base::SRU_Data::CheckSelectedUnits(selectedUnitsCounterPtr);
                ProcessAiSurrenders();

                if (*g_playSpeedPtr > 0)
                {
                    //Only refresh units while game speed > 0
                    unitTimer++;
                    if (unitTimer > g_unitRefreshMaxTime)
                    {
                        unitTimer = 0;
                        Base::SRU_Data::LoadUnits(true);
                    }
                }

                if (g_disco)
                {
                    discoTimer++;
                    if (discoTimer == 4)
                    {
                        static int discoColors[7] = {
                            0xFF0062FF,
                            0xFFFF0000,
                            0xFFFFFF00,
                            0xFF00FF2A,
                            0xFF00FFA6,
                            0xFF9900FF,
                            0xFFFF00E6
                        };

                        discoTimer = 0;

                        for (int i = 0; i < g_countryList.size(); i++)
                        {
                            int r = rand() % 8;

                            while (discoColors[r] == *g_countryList[i].colorPtr)
                            {
                                r = rand() % 8;
                            }

                            *g_countryList[i].colorPtr = discoColors[r];
                        }
                    }
                }
            }
        }
        
        if (g_ingame)
        {
            Base::SRU_Data::HandleFreezes();
        }

        Sleep(g_mainRefreshTime);
    }

    //fclose(f);
    //fclose(stdout);
    //fclose(stdin);
    //FreeConsole();
    
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

