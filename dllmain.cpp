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

    Sleep(500);

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

    //Base::Execute::SetupFunctions();

    Base::SRU_Data::LoadDefaultUnits();
    Base::SRU_Data::LoadUnits();
    Base::SRU_Data::LoadDiplTreaties();
    Base::SRU_Data::LoadGroundTypes();	
    Base::SRU_Data::LoadTechnologies();
    Base::SRU_Data::LoadLeaders();

    Base::SRU_Data::Asm::g_ownAllocs.clear();
    Base::SRU_Data::g_hexNameList.clear();

    g_mapSizeLoaded = false;

    if (g_lastFunctionHookState == 0)
    {
        Base::SRU_Data::Hooks::SetupFunctionHooks(1);
    }

    unitTimer = 0;

    //Get SteamID64
    g_steamId = *(uint64_t*)(g_base + Offsets::steamId);

	//Hash id with salt to keep anonymity
    //std::string uid = Base::Utils::SHA256(std::to_string(g_steamId) + Offsets::salt);
    //std::string url = "bruh.games/internal/sru/update.php?v=1&uid=" + uid;
    
    //std::string response = Base::Utils::StreamToMem(url);
    //if (response.length() == 0)
    //{
    //    Base::Utils::StreamToMem(url, false);
    //}
}

void CheckGameState(uintptr_t* gameStatePtr)
{
    if (Base::Utils::CanReadPtr(gameStatePtr))
    {
        if (*gameStatePtr == 2)
        {
            if (!g_ingame && !g_quitting)
            {
                g_ingame = 1;
                SetupSessionPtr();
                g_ingame = 2;
            }
        }
        else
        {
            g_ingame = 0;
            g_quitting = false;
        }
    }
}

void CheckCurrentCountry(uintptr_t* clickedCountryPtr)
{
    //std::cout << g_mouseClicked << " " << g_ingame << g_countryList.size() << std::endl; 

    if (g_mouseClicked && g_ingame == 2 && g_countryList.size() > 0 && !g_autosaving)
    {
        g_mouseClicked = false;
        g_newClick = true;

        /*__int16 realX = *g_clickedXPtr;
        __int16 realY = *g_clickedYPtr;
        if (realX < 0)
        {
            realX = Base::SRU_Data::g_mapSizeX + realX;
        }
        if (realY < 0) realY = 0;
        DWORD posData = MAKELONG(realX, realY);
        __int16 v9 = (__int16)posData;
        if (v9 >= Base::SRU_Data::g_mapSizeX)
        {
            v9 = (__int16)posData % Base::SRU_Data::g_mapSizeX;
        }
        else if ((posData & 0x8000) != 0)
        {
            v9 = Base::SRU_Data::g_mapSizeX + (__int16)posData % Base::SRU_Data::g_mapSizeX;
        }
        long temp = v9 + Base::SRU_Data::g_mapSizeX * HIWORD(posData);
        long mult = 16 * temp;
        DWORD base1 = *(DWORD*)(Base::SRU_Data::g_base + Offsets::allHexStart);
        DWORD base2 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 0xC) + mult;
        DWORD base6 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 8) + (4 * temp);
        DWORD base5 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 12) + (4 * temp);
        DWORD base4 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 16) + (4 * temp);
        DWORD base3 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 20) + (4 * temp);
        DWORD base7 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 24) + (4 * temp);
        DWORD base8 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 28) + (4 * temp);

        uintptr_t* addr = (uintptr_t*)base3;
        DWORD val = *addr;

        int tempResourceId = g_paintSelectedResource;

        uint8_t* resAddr = (uint8_t*)base4 + 2;

        if (g_paintSelectedResource > 3)
        {
            resAddr = (uint8_t*)base4 + 3;
            tempResourceId -= 4;
        }

        int res = Base::Execute::GetMapResource(resAddr, tempResourceId);

        std::cout << "------------" << std::endl;
        std::cout << std::dec << res << std::endl;
		std::cout << std::hex << base2 << " " << base5 << " " << base4 << " " << base3 << " " << base6 << " " << base7 << " " << base8 << std::endl;

        std::cout << "F:" << std::endl;

        bool ok = true;
        while (ok)
        {
            uint8_t* ptr1 = (uint8_t*)(val + 177);
            DWORD* ptr2 = (DWORD*)(val + 356);
            WORD* ptr3 = (WORD*)(val + 12);
            BYTE* ptr4 = (BYTE*)(val + 179);

            if (!Base::Utils::CanReadPtr(ptr1) || !Base::Utils::CanReadPtr(ptr2) || !Base::Utils::CanReadPtr(ptr3) || !Base::Utils::CanReadPtr(ptr4))
            {
                std::cout << "break1" << std::endl;
                break;
            }

            uint8_t v1870 = *(uint8_t*)(val + 177);
            DWORD v5012 = *(DWORD*)(val + 356);

            if ((v1870 & 0x20) != 0 && (v1870 & 0x80) == 0 && *(WORD*)(val + 12) && (*(BYTE*)(val + 179) & 0x40) == 0)
            {
                uintptr_t id = *(uintptr_t*)(val + 0xC);
                std::cout << std::hex << val << " " << std::dec << id << std::endl;
            }
            else
            {
                std::cout << "break2" << std::endl;
            }

            val = v5012;
            if (!v5012) break;
        }*/

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
                uintptr_t fromBase = surrenderEvents[i].fromBase;
                int to = surrenderEvents[i].to;

                surrenderEvents[i].lastTime = -1;

                if (from != 0)
                {
                    //std::cout << "exec " << from << " " << to << std::endl;

                    Base::Execute::RespawnCountryNew(fromBase, to, 2);
                    //Base::Execute::RespawnCountry(from, to, 2);

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

void PaintMap(uintptr_t mouseHoverHex, uint16_t xPos, uint16_t yPos)
{
    uint8_t* ownerPtr = (uint8_t*)(mouseHoverHex);

    bool ok = false;

    if (!g_paintHexOnlySelected) {
        ok = true;
    }
    else {
        if (clickedCountry->oId == *ownerPtr) {
            ok = true;
        }
    }

    if (ok) {
        if (g_paintMode == Paint_Land)
        {
            //Paint Land

            uint8_t* loyaltyPtr = (uint8_t*)(mouseHoverHex + Offsets::hexLoyalty);
            uint8_t* groundPtr = (uint8_t*)(mouseHoverHex + Offsets::hexGround);

            if (g_paintHexOwner) {
                *ownerPtr = (uint8_t)g_countryList[g_paintSelectedComboCountry].oId;
            }
            if (g_paintHexLoyalty) {
                *loyaltyPtr = (uint8_t)g_countryList[g_paintSelectedComboLoyalty].oId;
            }

            if (g_paintSelectedComboGround > -1)
            {
                uint8_t newGround = (uint8_t)g_groundTypeList[g_paintSelectedComboGround].id;

                if (*groundPtr == 14)
                {
                    //If Ocean override second style attrib
                    uint8_t* groundPtr2 = (uint8_t*)(mouseHoverHex + 0x2);
                    *groundPtr2 = 240;
                }
                else if (newGround == 14)
                {
                    uint8_t* groundPtr2 = (uint8_t*)(mouseHoverHex + 0x2);
                    *groundPtr2 = 243;
                }

                *groundPtr = newGround;
            }
        }
        else if (g_paintMode == Paint_UnitStats)
        {
            //Paint Unit(s) stats

            Country* cc = &g_countryList[g_paintSelectedComboCountry];

            uint8_t newOwner = (uint8_t)cc->oId;

            Country* newcc = nullptr;

            for (int i = 0; i < g_countryList.size(); i++)
            {
                if ((uint8_t)g_countryList[i].oId == newOwner)
                {
                    newcc = &g_countryList[i];
                    break;
                }
            }

            if (newcc == nullptr) return;

            Base::SRU_Data::Unit* tmpUnit = nullptr;

            int size = g_unitList.size();
            for (int i = 0; i < size; i++)
            {
                tmpUnit = &g_unitList[i];

                if (!*tmpUnit->deployedState)
                {
                    //Deployed
                    if (*tmpUnit->xPos == xPos && *tmpUnit->yPos == yPos)
                    {
                        if (*tmpUnit->countryId != newOwner)
                        {
                            //Country change only to units with other country then original

                            *tmpUnit->countryId = newOwner;

                            for (int a = 0; a < cc->allUnits.size(); a++)
                            {
                                if (cc->allUnits[a].base == tmpUnit->base)
                                {
                                    cc->allUnits.erase(cc->allUnits.begin() + a);
                                    break;
                                }
                            }

                            newcc->allUnits.push_back(*tmpUnit);
                        }

                        if ((g_paintUnitTargetCountry && *tmpUnit->countryId == cc->oId) ||
                            !g_paintUnitTargetCountry)
                        {
                            for (int i = 0; i < g_paintUnitModes.size(); i++)
                            {
                                if (g_paintUnitModes[i] > -1 && i != Unit::MaxHalth)
                                {
                                    float newVal =
                                        (g_paintUnitModes[i] / 100.0f) * tmpUnit->properties[i]->origVal;

                                    *tmpUnit->properties[i]->valPtr = newVal;
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (g_paintMode == Paint_Resource) //resource
        {
            //Calculate other base addr

            if (xPos < 0)
            {
                xPos = Base::SRU_Data::g_mapSizeX + xPos;
            }

            if (yPos < 0) yPos = 0;

            DWORD posData = MAKELONG(xPos, yPos);

            __int16 v9 = (__int16)posData;
            if (v9 >= Base::SRU_Data::g_mapSizeX)
            {
                v9 = (__int16)posData % Base::SRU_Data::g_mapSizeX;
            }
            else if ((posData & 0x8000) != 0)
            {
                v9 = Base::SRU_Data::g_mapSizeX + (__int16)posData % Base::SRU_Data::g_mapSizeX;
            }

            long temp = v9 + Base::SRU_Data::g_mapSizeX * HIWORD(posData);
            long mult = 16 * temp;

            //DWORD base1 = *(DWORD*)(Base::SRU_Data::g_base + Offsets::allHexStart);
            //DWORD base2 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 0xC) + mult;
            DWORD base3 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 16) + (4 * temp);
            //DWORD base4 = *(uintptr_t*)((*(uintptr_t*)(Base::SRU_Data::g_base + Offsets::allHexStart)) + 20) + (4 * temp);

            int tempResourceId = g_paintSelectedResource;

            uint8_t* resAddr = (uint8_t*)base3 + 2;

            if (g_paintSelectedResource > 3)
            {
                resAddr = (uint8_t*)base3 + 3;
                tempResourceId -= 4;
            }

            Base::Execute::SetMapResource(resAddr, tempResourceId, g_paintSelectedResourceAmount);
        }
        else if (g_paintMode == Paint_Infrastructure) //infrastructure
        {
            for (int i = 0; i < 4; i++)
            {
                if (*(uint8_t*)(mouseHoverHex + 0x8 + i) != 0)
                {
                    *(uint8_t*)(mouseHoverHex + 0x8 + i) = 0;
                }
            }

            //Check for Bridges since they act like facilities

            uintptr_t* rootAddr = Base::Execute::GetFacilityRoot(xPos, yPos);
            std::vector<Base::SRU_Data::AddressHolder> facilities =
                Base::Execute::GetFacilities(rootAddr);
            
            for (int i = 0; i < facilities.size(); i++)
            {
                if (facilities[i].id == 21902) //Bridge
                {
                    Base::Execute::DestroyFacility(rootAddr, facilities, i);
                }
            }
        }
        else if (g_paintMode == Paint_Unit)
        {
            if (g_unitSpawnSelectedUnitDesign == -1) return;

            //Paint spawn unit(s)

            static uintptr_t lastSpawnTile = 0;
            uintptr_t newSpawnTile = mouseHoverHex;

            if (lastSpawnTile != newSpawnTile)
            {
                lastSpawnTile = newSpawnTile;

                int unitDesign = g_defaultUnitList[g_unitSpawnSelectedUnitDesign]->spawnId;
                uintptr_t country = unitSpawnCountry->base;

                Base::Execute::SpawnUnit(unitDesign, g_unitSpawnCount, country, 1, g_unitSpawnReserve, xPos, yPos);
            }
        }
        else if (g_paintMode == Paint_Facility)
        {
            if (g_facilitySpawnSelectedFacility == -1) return;

            float constructionState = 0;

            if (g_facilitySpawnConstruction)
                constructionState = 1.0f;

            for (int i = 0; i < g_facilitySpawnCount; i++)
            {
                Base::Execute::CreateFacility(xPos, yPos, clickedCountry->oId, g_facilityList[g_facilitySpawnSelectedFacility]->id, constructionState);
            }
        }
        else if (g_paintMode == Paint_FacilityDestroy)
        {
            Base::Execute::DestroyAllFacilities(xPos, yPos);
        }
        else if (g_paintMode == Paint_FacilityDisable)
        {
            Base::Execute::DisableAllFacilities(xPos, yPos, 1);
        }
        else if (g_paintMode == Paint_FacilityEnable)
        {
            Base::Execute::DisableAllFacilities(xPos, yPos, 0);
        }
    }
}

void PaintCoords(int x, int y)
{
    __int16 realX = x;
    __int16 realY = y;

    if (realX < 0)
    {
        realX = g_mapSizeX + x;
    }

    if (realY < 0) realY = 0;

    DWORD posData = MAKELONG(realX, realY);

    __int16 v9 = (__int16)posData;
    if (v9 >= g_mapSizeX)
    {
        v9 = (__int16)posData % g_mapSizeX;
    }
    else if ((posData & 0x8000) != 0)
    {
        v9 = g_mapSizeX + (__int16)posData % g_mapSizeX;
    }

    long temp = v9 + g_mapSizeX * HIWORD(posData);
    long mult = 16 * temp;

    DWORD base = *(uintptr_t*)((*(uintptr_t*)(g_base + Offsets::allHexStart)) + 0xC) + mult;

    PaintMap((uintptr_t)base, realX, realY);
}

void PaintMapBrush(uintptr_t* mouseHoverHex, uint16_t* xPos, uint16_t* yPos)
{
    if (mouseHoverHex == nullptr) return;
    if (*mouseHoverHex == 0) return;


    if (g_paintBrushSize == 1)
    {
        PaintMap(*mouseHoverHex, *xPos, *yPos);
        return;
    }

    //if (g_paintStyle == 0) //Square
    {
        int xMin = *xPos - g_paintBrushSize / 2;
        int xMax = *xPos + g_paintBrushSize / 2;
        int yMin = *yPos - g_paintBrushSize / 2;
        int yMax = *yPos + g_paintBrushSize / 2;

        for (int x = xMin; x < xMax; x++)
        {
            for (int y = yMin; y < yMax; y++)
            {
                PaintCoords(x, y);
            }
        }
    }
}

DWORD WINAPI dllThread(HMODULE hModule) {
    DWORD dwExit = 0;

    MH_Initialize();

    Base::Init(true);

    //AllocConsole();
    //FILE* f;
    //freopen_s(&f, "CONOUT$", "w", stdout);
    //freopen_s(&f, "CONIN$", "r", stdin);

    Base::SRU_Data::LoadSettings();

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
    uint16_t* mouseHoverX = (uint16_t*)(g_base + Offsets::mouseHoverXPos);
    uint16_t* mouseHoverY = (uint16_t*)(g_base + Offsets::mouseHoverYPos);

    //Main loop

    Base::Execute::SetupFunctions();
    Base::SRU_Data::Hooks::SetupFunctionHooks(2);

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

            if (g_ingame == 2 && !g_autosaving)
            {
                if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
                {
                    g_shift = true;
                }
                else g_shift = false;

                bool old = g_unitSelect;

                if ((GetAsyncKeyState(0x58) & 0x0001) != 0) //X Key
                {
                    if (!g_paintEnabled)
                    {
                        g_unitSelect = !g_unitSelect;
                    }
                }

                if (old != g_unitSelect)
                {
                    //change
                    Base::SRU_Data::Hooks::SetUnitSelect(g_unitSelect);
                    Base::SRU_Data::SaveSettings();
                }

                if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
                {
                    if (!g_unitSelect)
                    {
                        g_paintEnabled = true;
                    }
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
                        PaintMapBrush(mouseHoverHex, mouseHoverX, mouseHoverY);
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
        
        if (g_ingame == 2 && !g_autosaving)
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

