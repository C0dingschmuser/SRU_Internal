#include "base.h"

using namespace Base::SRU_Data;
using namespace Base::SRU_Data::Hooks;
using namespace Base::SRU_Data::Asm;

void CreateUnit(uintptr_t base);
void AddSurrenderEvent(int from, int to);

void HandleSelectedUnits()
{
    g_addOk = true;
    for (int i = 0; i < g_selectedUnitList.size(); i++)
    {
        if (g_selectedUnitList[i].base == g_nextUnitEntity)
        {
            g_addOk = false;
            break;
        }
    }
    if (g_addOk)
    {
        uintptr_t base = g_nextUnitEntity;

        CreateUnit(base);

        g_unitEntityCountSelected++;
    }
}

void __declspec(naked) GetSelectedUnits()
{
    __asm {
        xor eax, eax
        mov[g_nextUnitEntity], ebp
        mov[g_selectedReg0], eax
        mov[g_selectedReg1], ecx
        mov[g_selectedReg2], edx
        mov[g_selectedReg3], ebp
        mov[g_selectedReg4], edi
        mov[g_selectedReg5], esi
        mov[g_selectedReg6], ebx
        mov[g_selectedReg7], esp
    }

    HandleSelectedUnits();

    __asm {
		mov eax, g_selectedReg0
		mov ecx, g_selectedReg1
		mov edx, g_selectedReg2
		mov ebp, g_selectedReg3
		mov edi, g_selectedReg4
		mov esi, g_selectedReg5
		mov ebx, g_selectedReg6
        mov esp, g_selectedReg7
        cmp byte ptr[ebp + 0x16], 00
        jmp[g_selectedJmpBackAddr]
    }
}

void HandleGameQuit()
{
    g_quitting = true;
    g_ingame = 0;
}

void __declspec(naked) GameQuit()
{
    __asm {
        mov[g_quitGameReg0], eax
		mov[g_quitGameReg1], ecx
		mov[g_quitGameReg2], edx
		mov[g_quitGameReg3], ebp
		mov[g_quitGameReg4], edi
		mov[g_quitGameReg5], esi
		mov[g_quitGameReg6], ebx
		mov[g_quitGameReg7], esp
    }

    HandleGameQuit();

    __asm {
        mov eax, g_quitGameReg0
        mov ecx, g_quitGameReg1
        mov edx, g_quitGameReg2
        mov ebp, g_quitGameReg3
        mov edi, g_quitGameReg4
        mov esi, g_quitGameReg5
        mov ebx, g_quitGameReg6
        mov esp, g_quitGameReg7
        mov ebp, DWORD PTR ds : 0xdc35f4
        jmp [g_quitGameJumpBackAddr]
    }
}

void HandleAutosaveEnd()
{
    g_autosaving = false;

    if (g_ingame != 3)
    {
        SetupFunctionHooks(1);
    }
}

void __declspec(naked) AutosaveEnd()
{
    __asm {
        push 0x00000100
        mov[g_autosaveEndReg0], eax
        mov[g_autosaveEndReg1], ecx
        mov[g_autosaveEndReg2], edx
        mov[g_autosaveEndReg3], ebp
        mov[g_autosaveEndReg4], edi
        mov[g_autosaveEndReg5], esi
        mov[g_autosaveEndReg6], ebx
        mov[g_autosaveEndReg7], esp
    }

    HandleAutosaveEnd();

    __asm {
        mov eax, g_autosaveEndReg0
        mov ecx, g_autosaveEndReg1
        mov edx, g_autosaveEndReg2
        mov ebp, g_autosaveEndReg3
        mov edi, g_autosaveEndReg4
        mov esi, g_autosaveEndReg5
        mov ebx, g_autosaveEndReg6
        mov esp, g_autosaveEndReg7
        jmp[g_autosaveEndJumpBackAddr]
    }
}

void HandleAutosaveBegin()
{
    g_autosaving = true;

    if (g_ingame == 2)
    {
        SetupFunctionHooks(0);
    }
}

void __declspec(naked) AutosaveBegin()
{
    __asm {
        mov[g_autosaveBeginReg0], eax
        mov[g_autosaveBeginReg1], ecx
        mov[g_autosaveBeginReg2], edx
        mov[g_autosaveBeginReg3], ebp
        mov[g_autosaveBeginReg4], edi
        mov[g_autosaveBeginReg5], esi
        mov[g_autosaveBeginReg6], ebx
        mov[g_autosaveBeginReg7], esp
    }

    HandleAutosaveBegin();

    __asm {
        mov eax, g_autosaveBeginReg0
        mov ecx, g_autosaveBeginReg1
        mov edx, g_autosaveBeginReg2
        mov ebp, g_autosaveBeginReg3
        mov edi, g_autosaveBeginReg4
        mov esi, g_autosaveBeginReg5
        mov ebx, g_autosaveBeginReg6
        mov esp, g_autosaveBeginReg7
        sub esp, 0x00000084
        jmp [g_autosaveBeginJumpBackAddr]
    }
}

void HandleOpenSettings()
{
    if (g_ingame == 2)
    {
        if (g_ingame != 3)
        {
            g_ingame = 3;
            SetupFunctionHooks(0);
        }
    }
}

void __declspec(naked) OpenSettings()
{
    __asm {
        mov edi, [esp + 0x00000100]
        mov[g_openSettingsReg0], eax
        mov[g_openSettingsReg1], ecx
        mov[g_openSettingsReg2], edx
        mov[g_openSettingsReg3], ebp
        mov[g_openSettingsReg4], edi
        mov[g_openSettingsReg5], esi
        mov[g_openSettingsReg6], ebx
        mov[g_openSettingsReg7], esp
    }

    HandleOpenSettings();

    __asm {
        mov eax, g_openSettingsReg0
        mov ecx, g_openSettingsReg1
        mov edx, g_openSettingsReg2
        mov ebp, g_openSettingsReg3
        mov edi, g_openSettingsReg4
        mov esi, g_openSettingsReg5
        mov ebx, g_openSettingsReg6
        mov esp, g_openSettingsReg7
        jmp [g_openSettingsJumpBackAddr]
    }
}

void HandleCloseSettings()
{
    if (g_ingame == 3)
    {
        SetupFunctionHooks(1);
		g_ingame = 2;
	}
}

void __declspec(naked) CloseSettings()
{
    __asm {
        mov[g_closeSettingsReg0], eax
        mov[g_closeSettingsReg1], ecx
        mov[g_closeSettingsReg2], edx
        mov[g_closeSettingsReg3], ebp
        mov[g_closeSettingsReg4], edi
        mov[g_closeSettingsReg5], esi
        mov[g_closeSettingsReg6], ebx
        mov[g_closeSettingsReg7], esp
    }

    HandleCloseSettings();

    __asm {
        mov eax, g_closeSettingsReg0
        mov ecx, g_closeSettingsReg1
        mov edx, g_closeSettingsReg2
        mov ebp, g_closeSettingsReg3
        mov edi, g_closeSettingsReg4
        mov esi, g_closeSettingsReg5
        mov ebx, g_closeSettingsReg6
        mov esp, g_closeSettingsReg7
        mov eax, [edi + 0x00009D08]
        jmp [g_closeSettingsJumpBackAddr]
    }
}

void CheckSphereNameIntern()
{
    int* sphereTypeAddr = (int*)(g_sphereNameReg7 + 0x18);
    
    if (sphereTypeAddr == nullptr) return;

    int sphereType = *sphereTypeAddr;
    //char* namePtr = (char*)g_sphereNameReg0;
    
    //if (namePtr != nullptr)
    {
        if (sphereType <= 1 && g_sphereNames[0].length() > 0)
        {
            g_sphereNameReg0 = (unsigned int)g_sphereNames[0].c_str();
        }
        else if (sphereType >= 2 && g_sphereNames[1].length() > 0)
        {
            g_sphereNameReg0 = (unsigned int)g_sphereNames[1].c_str();
        }
    }
}

void __declspec(naked) CheckSphereName()
{
    __asm {
        mov[g_sphereNameReg0], eax            //char* pointer to sphere name
        mov[g_sphereNameReg1], ecx
        mov[g_sphereNameReg2], edx
        mov[g_sphereNameReg3], ebp
        mov[g_sphereNameReg4], edi
        mov[g_sphereNameReg5], esi
        mov[g_sphereNameReg6], ebx
        mov[g_sphereNameReg7], esp
    }

    CheckSphereNameIntern();

    __asm {
        mov eax, g_sphereNameReg0
        mov ecx, g_sphereNameReg1
        mov edx, g_sphereNameReg2
        mov ebp, g_sphereNameReg3
        mov edi, g_sphereNameReg4
        mov esi, g_sphereNameReg5
        mov ebx, g_sphereNameReg6
        mov esp, g_sphereNameReg7
        mov[esp], ebp
		mov[esp + 0x4], eax
		jmp[g_sphereNameJumpBackAddr]
    }
}

void CheckHexNameBigIntern()
{
    if (g_hexNameReg2 == 65123) {
        //trigger for custom

        std::shared_ptr<Hex> ptr = nullptr;

        for (int i = 0; i < g_hexNameList.size(); i++) {
            if (g_hexNameList[i]->base == g_hexNameReg0) {
                ptr = g_hexNameList[i];
                break;
            }
        }

        if (ptr != nullptr) {
			g_hexNameReg1 = (uintptr_t)ptr->newName.c_str();
            g_hexNameBigJumpBackAddr = g_hexNameBigJumpBackAddrData;
        }
        else {
            g_hexNameBigJumpBackAddr = g_hexNameBigJumpBackAddrNone;
        }
    }
    else {
        g_hexNameBigJumpBackAddr = g_hexNameBigJumpBackAddrNone;
    }
}

void __declspec(naked) CheckHexNameBig()
{
    __asm {
        mov eax, [esp + 0x1FEC]        //load hex base addr
        movzx edx, word ptr[eax + 0xC] //load town string id
        mov[g_hexNameReg0], eax
        mov[g_hexNameReg1], ecx
        mov[g_hexNameReg2], edx
        mov[g_hexNameReg3], ebp
        mov[g_hexNameReg4], edi
        mov[g_hexNameReg5], esi
        mov[g_hexNameReg6], ebx
        mov[g_hexNameReg7], esp
    }

    CheckHexNameBigIntern();

    __asm {
        mov eax, g_hexNameReg0
        mov ecx, g_hexNameReg1
        mov edx, g_hexNameReg2
        mov ebp, g_hexNameReg3
        mov edi, g_hexNameReg4
        mov esi, g_hexNameReg5
        mov ebx, g_hexNameReg6
		mov esp, g_hexNameReg7
        jmp [g_hexNameBigJumpBackAddr]
    }
}

void CreateUnit(uintptr_t base)
{
    Unit newUnit{};
    newUnit.Init(base);
 
    g_selectedUnitList.push_back(newUnit);
}

void CheckHexSupply()
{
    Asm::g_currentHexSupply = Asm::g_hexReg0 & 0xFF;

    if (g_hexSupply)
    {
        if (Asm::g_currentHexSupply < g_lowestHexSupply ||
            Asm::g_hexReg0 == 0xFFFFFFFF)
        {
            Asm::g_currentHexSupply = g_lowestHexSupply;
        }
    }

    Asm::g_hexReg0 = (Asm::g_hexReg0 & 0xFFFFFF00) | Asm::g_currentHexSupply;
}

void __declspec(naked) SetHexSupply()
{
    __asm {
        lea eax, [ebp - 01]
        mov[g_hexReg0], eax
        mov[g_hexReg1], ecx
        mov[g_hexReg2], edx
        mov[g_hexReg3], ebx
        mov[g_hexReg4], edi
        mov[g_hexReg5], esi
        mov[g_hexReg6], ebp
        mov[g_hexReg7], esp
    }

    CheckHexSupply();

    __asm {
        mov eax, g_hexReg0
        mov ecx, g_hexReg1
        mov edx, g_hexReg2
        mov ebx, g_hexReg3
		mov edi, g_hexReg4
		mov esi, g_hexReg5
		mov ebp, g_hexReg6
		mov esp, g_hexReg7
        mov[edx + 04], al
        jmp[g_hexSupplyJmpBackAddr]
    }
}

void __declspec(naked) HandleAISurrender()
{
    __asm {
        sub esp, 0x00000094
        mov[g_aiSurrFrom], ebx
        mov[g_aiSurrTo], edx
        mov[g_aiSurrReg0], eax
        mov[g_aiSurrReg1], ecx
        mov[g_aiSurrReg2], edx
        mov[g_aiSurrReg3], ebp
        mov[g_aiSurrReg4], edi
        mov[g_aiSurrReg5], esi
        mov[g_aiSurrReg6], ebx
        mov[g_aiSurrReg7], esp
    }

    AddSurrenderEvent(Asm::g_aiSurrFrom, Asm::g_aiSurrTo);

    __asm {
        mov eax, g_aiSurrReg0
        mov ecx, g_aiSurrReg1
        mov edx, g_aiSurrReg2
        mov ebp, g_aiSurrReg3
        mov edi, g_aiSurrReg4
        mov esi, g_aiSurrReg5
		mov ebx, g_aiSurrReg6
		mov esp, g_aiSurrReg7
        jmp [g_aiSurrenderJmpBackAddr]
    }
}

void __declspec(naked) HandleMouseClick()
{
    __asm {
        imul edx, ebp
        add edx, eax
    }

    g_mouseClicked = true;

    __asm {
        jmp [g_mouseClickedJmpBackAddr]
    }
}

void check()
{
    using namespace Base::SRU_Data;
	//do not free if own pointer

    int found = -1;

    for (int i = 0; i < Asm::g_ownAllocs.size(); i++)
    {
        if (Asm::g_ownAllocs[i] == (uintptr_t)g_diplFreeReg3)
        {
            found = i;
            break;
        }
    }

    if (found)
    {
        g_diplFreeJmpBackAddr = g_diplFreeJmpBackAddrDefault + 38;
    }
    else
    {
        g_diplFreeJmpBackAddr = g_diplFreeJmpBackAddrDefault;
    }
}

void __declspec(naked) DiplFree()
{
    __asm {
        mov ebp, ecx
		mov edx, [ebp+0x3C]
        mov[g_diplFreeReg0], eax
        mov[g_diplFreeReg1], ecx
        mov[g_diplFreeReg2], edx
        mov[g_diplFreeReg3], ebp
        mov[g_diplFreeReg4], edi
        mov[g_diplFreeReg5], esi
        mov[g_diplFreeReg6], ebx
    }

    check();

    __asm {
        mov eax, g_diplFreeReg0
        mov ecx, g_diplFreeReg1
        mov edx, g_diplFreeReg2
        mov ebp, g_diplFreeReg3
        mov edi, g_diplFreeReg4
        mov esi, g_diplFreeReg5
        mov ebx, g_diplFreeReg6
        jmp [g_diplFreeJmpBackAddr]
    }
}

void HandleMapSize()
{
    if (!g_mapSizeLoaded)
    {
        if (g_mapSizeReg6 > 0)
        {
            g_mapSizeLoaded = true;
            g_mapSizeX = g_mapSizeReg6;
        }
    }
}

void __declspec(naked) GetMapSize()
{
    __asm {
        mov [esp], eax
        mov ebx, [ecx + 0x5C]
        mov[g_mapSizeReg0], eax
        mov[g_mapSizeReg1], ecx
        mov[g_mapSizeReg2], edx
        mov[g_mapSizeReg3], ebp
        mov[g_mapSizeReg4], edi
        mov[g_mapSizeReg5], esi
        mov[g_mapSizeReg6], ebx
    }

    HandleMapSize();

    __asm {
        mov eax, g_mapSizeReg0
        mov ecx, g_mapSizeReg1
        mov edx, g_mapSizeReg2
        mov ebp, g_mapSizeReg3
        mov edi, g_mapSizeReg4
        mov esi, g_mapSizeReg5
        mov ebx, g_mapSizeReg6
		jmp [g_mapSizeJumpBackAddr]
    }
}

void CheckBuildOwner()
{
    if (g_buildCheckReg6 == g_ownOtherCountryId && g_fastRoad)
    {
        int type = *(int*)(g_buildCheckReg7 + 0x90);

        if (type == 21903 || type == 21904 || type == 21902) //road or rail or bridge
        {
            int base = g_buildCheckReg5;

            int from = *(int*)base;

            __int16 toX = (__int16)*(int*)(base + 0xA4);
            __int16 toY = (__int16)*(int*)(base + 0xA8);

            int to = (int)MAKELONG(toX, toY);

            Base::Execute::CreateTransport(from, to, type, 1);
        }
    }
}

void __declspec(naked) BuildCheckOwner()
{
    __asm {
		movzx ecx, byte ptr [edx]
        mov[g_buildCheckReg0], eax
        mov[g_buildCheckReg1], ecx
        mov[g_buildCheckReg2], edx
        mov[g_buildCheckReg3], ebp
        mov[g_buildCheckReg4], edi
        mov[g_buildCheckReg5], esi
        mov[g_buildCheckReg6], ebx
        mov[g_buildCheckReg7], esp
    }

    CheckBuildOwner();

    __asm {
        mov eax, g_buildCheckReg0
        mov ecx, g_buildCheckReg1
        mov edx, g_buildCheckReg2
        mov ebp, g_buildCheckReg3
        mov edi, g_buildCheckReg4
        mov esi, g_buildCheckReg5
        mov ebx, g_buildCheckReg6
		jmp [g_buildCheckJumpBackAddr]
    }
}

int FindOldCountryOwner2(uint8_t newOwner, bool weak)
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
        uint8_t* loyalty = (uint8_t*)(uintptr_t*)(i + Offsets::hexLoyalty);
        uint8_t* owner = (uint8_t*)(uintptr_t*)(i);
        uint8_t* terr = (uint8_t*)(uintptr_t*)(i + Offsets::hexTerr);
        uint8_t* terr2 = (uint8_t*)(uintptr_t*)(i + Offsets::hexTerr2);

        if (*loyalty != newOwner && *owner == newOwner &&
            ((!weak && *terr > 0 && *terr2 > 0) || weak))
        {
            int l = *loyalty;
            uintptr_t addr = *(uintptr_t*)(g_base + Offsets::allCountryStart + 4 * l);

            if (Base::Utils::CanReadPtr((void*)addr))
            {
                uintptr_t* pop = (uintptr_t*)(addr + Offsets::countryPopulation);

                if (Base::Utils::CanReadPtr(pop))
                {
                    int alive = 0;

                    if ((int)*pop > 0)
                    {
                        alive = 1;
                    }

                    if (alive == 0)
                    {
                        bool found = false;
                        for (int n = 0; n < newOwners.size(); n++)
                        {
                            if (newOwners[n][0] == *loyalty)
                            {
                                newOwners[n][1]++;
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            std::vector<unsigned int> data;
                            data.push_back(*loyalty);
                            data.push_back(1);

                            newOwners.push_back(data);
                        }
                    }
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

    if (newOwners.size())
    {
        return newOwners[largestPos][0];
    }
    else return 0;
}

void AddSurrenderEvent(int from, int to)
{
    //std::cout << from << " " << to << " " << g_aiColony << std::endl;

    if (!g_ingame || from > 255 || to > 255 || !g_aiColony) return;

    int last = g_surrenderEventCount - 1;
    if (last < 0)
    {
        last = 31;
    }

    if (to == g_ownOtherCountryId) return;

    if (from == 0)
    {
		//Causes crashing (prob. takes too long)
        //from = FindOldCountryOwner2(to, false);
    }

    if (from == 0) return;

    bool exists = false;
    for (int i = 0; i < 32; i++)
    {
        if (surrenderEvents[i].lastTime > -1)
        {
            if (surrenderEvents[i].from == from)
            {
                exists = true;
                break;
            }
        }
    }

    if (exists) return;
    //std::cout << std::dec << " " << from << " " << to << std::endl;

    uintptr_t base = 0;

    for (int i = 0; i < g_countryList.size(); i++)
    {
        if (g_countryList[i].oId == from)
        {
            base = g_countryList[i].base;
        }
    }

    if (base == 0) return;

    SurrenderEvent* s = &surrenderEvents[g_surrenderEventCount];
    s->lastTime = *(uintptr_t*)(g_base + Offsets::currentDayTime) + 500;
    s->fromBase = base;
    s->from = from;
    s->to = to;

    g_surrenderEventCount++;
    if (g_surrenderEventCount >= 32)
    {
        g_surrenderEventCount = 0;
    }
}

#pragma region DEFCON

void HandleDefcon(int call)
{
    Country* tmp = nullptr;
    unsigned int cmp = g_defconReg6;

    if (call == 1)
    {
        cmp = g_defconReg31;
    }
    else if (call == 2)
    {
        cmp = g_defconReg62;
    }
    else if (call == 3)
    {
        cmp = g_defconReg13;
    }

    for (int i = 0; i < g_countryList.size(); i++)
    {
        if (g_countryList[i].base == cmp)
        {
            tmp = &g_countryList[i];
            break;
        }
    }

    if (tmp)
    {
        if (tmp->defconState > -1)
        {
            if (call == 0)
            {
                g_defconReg0++;
            }
            else if(call == 1)
            {
                g_defconReg21 = tmp->defconState;
            }
            else if (call == 2)
            {
                g_defconReg02--;
            }
            else if (call == 3)
            {
                g_defconNew = tmp->defconState;
            }
        }
    }
}

void __declspec(naked) HandleDefconRaw()
{
    __asm {
        mov edi, edx
		and edi, 0x20
        mov[g_defconReg0], eax
        mov[g_defconReg1], ecx
        mov[g_defconReg2], edx
        mov[g_defconReg3], ebp
        mov[g_defconReg4], edi
        mov[g_defconReg5], esi
        mov[g_defconReg6], ebx //country
    }

    HandleDefcon(0);

    __asm {
        mov eax, g_defconReg0
        mov ecx, g_defconReg1
        mov edx, g_defconReg2
        mov ebp, g_defconReg3
        mov edi, g_defconReg4
        mov esi, g_defconReg5
        mov ebx, g_defconReg6
        dec eax
		jmp [g_defconJmpBackAddr]
    }
}

void __declspec(naked) HandleDefconRaw2()
{
    __asm {
        mov [g_defconReg01], eax
        mov [g_defconReg11], ecx
        mov [g_defconReg21], edx
        mov [g_defconReg31], ebp
        mov [g_defconReg41], edi
        mov [g_defconReg51], esi
        mov [g_defconReg61], ebx
    }

    HandleDefcon(1);

    __asm {
        mov eax, g_defconReg01
        mov ecx, g_defconReg11
        mov edx, g_defconReg21
        mov ebp, g_defconReg31
        mov edi, g_defconReg41
        mov esi, g_defconReg51
        mov ebx, g_defconReg61
		mov [esi + 0xD8], edx
		jmp [g_defconJmpBackAddr2]
    }
}

void __declspec(naked) HandleDefconRaw3()
{
    __asm {
        mov eax, [esp + 0x144]
        mov [g_defconReg02], eax
        mov [g_defconReg12], ecx
        mov [g_defconReg22], edx
        mov [g_defconReg32], ebp
        mov [g_defconReg42], edi
        mov [g_defconReg52], esi
        mov [g_defconReg62], ebx
    }

    HandleDefcon(2);

    __asm {
        mov eax, g_defconReg02
        mov ecx, g_defconReg12
        mov edx, g_defconReg22
        mov ebp, g_defconReg32
        mov edi, g_defconReg42
        mov esi, g_defconReg52
        mov ebx, g_defconReg62
        inc eax
		jmp [g_defconJmpBackAddr3]
    }
}

void __declspec(naked) HandleDefconRaw4()
{
    __asm {
        mov eax, [esp + 0x5E18]
        mov [g_defconNew], eax
        mov [g_defconReg03], eax
        mov [g_defconReg13], ecx
        mov [g_defconReg23], edx
        mov [g_defconReg33], ebp
        mov [g_defconReg43], edi
        mov [g_defconReg53], esi
        mov [g_defconReg63], ebx
    }

    HandleDefcon(3);

    __asm {
        mov eax, g_defconNew
        mov [esp + 0x5E18], eax
        mov eax, g_defconReg03
        mov ecx, g_defconReg13
        mov edx, g_defconReg23
        mov ebp, g_defconReg33
        mov edi, g_defconReg43
        mov esi, g_defconReg53
        mov ebx, g_defconReg63
        mov eax, [esp + 0x5E18]
		jmp [g_defconJmpBackAddr4]
    }
}

#pragma endregion DEFCON

void Base::SRU_Data::Hooks::SetupFunctionHooks(int enabled)
{
    if (g_lastFunctionHookState == enabled)
    {
        //No changes needed
        return;
    }

    g_lastFunctionHookState = enabled;

    if (enabled)
    {
        g_hookList.clear(); 
    }

    //Hook selected units

    int hookLength = 6;
    uintptr_t hookAddress = g_base + Offsets::selectedUnitsHook;
    Base::SRU_Data::Hooks::g_selectedJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, GetSelectedUnits, hookLength, enabled);

    //Hook hex supply

    hookAddress = g_base + Offsets::hexSupplyNop;

    if (enabled)
    {
        HookHolder holder;
        holder.addr = hookAddress;

        std::vector<BYTE> origData = Base::Hooks::GetOrigBytes((BYTE*)hookAddress, 2);
        holder.origBytes = origData;

        Hooks::g_hookList.push_back(holder);

        Base::Utils::Nop((BYTE*)hookAddress, 2);
    }
    else
    {
        HookHolder holder;

        for (int i = 0; i < Base::SRU_Data::Hooks::g_hookList.size(); i++)
        {
            if (Base::SRU_Data::Hooks::g_hookList[i].addr == (uintptr_t)hookAddress)
            {
                holder = Base::SRU_Data::Hooks::g_hookList[i];
                break;
            }
        }

        DWORD curProtection;
        VirtualProtect((void*)hookAddress, 2, PAGE_EXECUTE_READWRITE, &curProtection);

        for (int i = 0; i < holder.origBytes.size(); i++)
        {
            *((BYTE*)hookAddress + i) = holder.origBytes[i];
        }

        DWORD temp;
        VirtualProtect((void*)hookAddress, 2, curProtection, &temp);
    }

    hookAddress = g_base + Offsets::hexSupplyHook;
    Base::SRU_Data::Hooks::g_hexSupplyJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, SetHexSupply, hookLength, enabled);

    //Hook ai surrender
    hookAddress = g_base + Offsets::aiSurrenderHook;
    Base::SRU_Data::Hooks::g_aiSurrenderJmpBackAddr = hookAddress + hookLength;
    //Base::Hooks::FunctionHook((void*)hookAddress, HandleAISurrender, hookLength, enabled);

    //Hook mouse click
    hookLength = 5;
    hookAddress = g_base + Offsets::mouseClickHook;
    Base::SRU_Data::Hooks::g_mouseClickedJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleMouseClick, hookLength, enabled);

    //Hook defcon

    hookLength = 6;
    hookAddress = g_base + Offsets::defconHook;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw, hookLength, enabled);

    hookLength = 6;
    hookAddress = g_base + Offsets::defconHook2;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr2 = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw2, hookLength, enabled);

    hookLength = 8;
    hookAddress = g_base + Offsets::defconHook3;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr3 = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw3, hookLength, enabled);

    hookLength = 7;
    hookAddress = g_base + Offsets::defconHook4;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr4 = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw4, hookLength, enabled);

    hookLength = 5;
    hookAddress = g_base + Offsets::diplFreeHook;
    Base::SRU_Data::Hooks::g_diplFreeJmpBackAddr = hookAddress + hookLength;
    Base::SRU_Data::Hooks::g_diplFreeJmpBackAddrDefault = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, DiplFree, hookLength, enabled);

    hookLength = 6;
    hookAddress = g_base + Offsets::mapSizeHook;
    Base::SRU_Data::Hooks::g_mapSizeJumpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, GetMapSize, hookLength, enabled);

    hookLength = 5;
    hookAddress = g_base + Offsets::buildCheckHook;
    Base::SRU_Data::Hooks::g_buildCheckJumpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, BuildCheckOwner, hookLength, enabled);

    if (enabled)
    {
        SetFastRoad(g_fastRoad);
        SetProductionAdjustment(g_productionAdjustment);
        SetUnitSelect(g_unitSelect);
    }
    else
    {
        SetFastRoad(false);
        SetProductionAdjustment(false);
        SetUnitSelect(false);
    }

    //hex name hook

    hookLength = 11;
	hookAddress = g_base + Offsets::hexNameHookBig;
    Base::SRU_Data::Hooks::g_hexNameBigJumpBackAddrNone = hookAddress + hookLength; //jump here if no custom name
    Base::SRU_Data::Hooks::g_hexNameBigJumpBackAddrData = g_base + 0xC8E25; //jump here if custom name
	Base::Hooks::FunctionHook((void*)hookAddress, CheckHexNameBig, hookLength, enabled);

    //sphere name hook

    hookLength = 7;
    hookAddress = g_base + Offsets::sphereNameHook;
    Base::SRU_Data::Hooks::g_sphereNameJumpBackAddr = hookAddress + hookLength;
	Base::Hooks::FunctionHook((void*)hookAddress, CheckSphereName, hookLength, enabled);

    if (enabled == 2)
    {
        //first setup

        //open settings hook

        hookLength = 7;
        hookAddress = g_base + Offsets::settingsOpenHook;
        Base::SRU_Data::Hooks::g_openSettingsJumpBackAddr = hookAddress + hookLength;
        Base::Hooks::FunctionHook((void*)hookAddress, OpenSettings, hookLength, 2);

        //close settings hook

        hookLength = 6;
        hookAddress = g_base + Offsets::settingsQuitHook;
        Base::SRU_Data::Hooks::g_closeSettingsJumpBackAddr = hookAddress + hookLength;
        Base::Hooks::FunctionHook((void*)hookAddress, CloseSettings, hookLength, 2);

        //begin autosave hook

        hookLength = 6;
        hookAddress = g_base + Offsets::autosaveEnter;
        Base::SRU_Data::Hooks::g_autosaveBeginJumpBackAddr = hookAddress + hookLength;
        Base::Hooks::FunctionHook((void*)hookAddress, AutosaveBegin, hookLength, 2);

        //end autosave hook

        hookLength = 5;
        hookAddress = g_base + Offsets::autosaveExit;
        Base::SRU_Data::Hooks::g_autosaveEndJumpBackAddr = hookAddress + hookLength;
        Base::Hooks::FunctionHook((void*)hookAddress, AutosaveEnd, hookLength, 2);

        //quit game hook

        hookLength = 6;
        hookAddress = g_base + Offsets::gameQuit;
        Base::SRU_Data::Hooks::g_quitGameJumpBackAddr = hookAddress + hookLength;
        Base::Hooks::FunctionHook((void*)hookAddress, GameQuit, hookLength, 2);
    }
}

void PatchCustom(uintptr_t addr, std::vector<unsigned char> buffer, unsigned char oId)
{
    DWORD curProtection;
    VirtualProtect((void*)addr, buffer.size(), PAGE_EXECUTE_READWRITE, &curProtection);

    for (int i = 0; i < buffer.size(); i++)
    {
        unsigned char newVal = buffer[i];

        if (newVal == 0xFF)
        {
            newVal = oId;
        }

        *(BYTE*)(addr + i) = newVal;
    }

    DWORD temp;
    VirtualProtect((void*)addr, buffer.size(), curProtection, &temp);
}

void Base::SRU_Data::Hooks::SetUnitSelect(bool enabled)
{
    std::vector<unsigned char> buffer1;
    std::vector<unsigned char> buffer2;
    std::vector<unsigned char> buffer3;

    if (enabled)
    {
        std::copy(Offsets::selectUnit1_New.begin(), Offsets::selectUnit1_New.end(), std::back_inserter(buffer1));
        std::copy(Offsets::selectUnit2_New.begin(), Offsets::selectUnit2_New.end(), std::back_inserter(buffer2));
        std::copy(Offsets::selectUnit3_New.begin(), Offsets::selectUnit3_New.end(), std::back_inserter(buffer3));
    }
    else
    {
        std::copy(Offsets::selectUnit1_Default.begin(), Offsets::selectUnit1_Default.end(), std::back_inserter(buffer1));
        std::copy(Offsets::selectUnit2_Default.begin(), Offsets::selectUnit2_Default.end(), std::back_inserter(buffer2));
        std::copy(Offsets::selectUnit3_Default.begin(), Offsets::selectUnit3_Default.end(), std::back_inserter(buffer3));
    }

    PatchCustom(g_base + Offsets::selectUnitHook1, buffer1, g_ownOtherCountryId);
    PatchCustom(g_base + Offsets::selectUnitHook2, buffer2, g_ownOtherCountryId);
    PatchCustom(g_base + Offsets::selectUnitHook3, buffer3, g_ownOtherCountryId);
}

void Base::SRU_Data::Hooks::SetProductionAdjustment(bool enabled)
{
	uintptr_t addr = g_base + Offsets::productionAdjustmentHook;

    DWORD curProtection;
    VirtualProtect((void*)addr, 7, PAGE_EXECUTE_READWRITE, &curProtection);

    std::vector<unsigned char> buffer;

    std::copy(Offsets::productionAdjustmentDefault.begin(),
        Offsets::productionAdjustmentDefault.end(), std::back_inserter(buffer));

    if (enabled)
    {
        buffer.clear();
        std::copy(Offsets::productionAdjustmentNew.begin(),
            Offsets::productionAdjustmentNew.end(), std::back_inserter(buffer));
    }

    for (int i = 0; i < buffer.size(); i++)
    {
        *(BYTE*)(addr + i) = buffer[i];
    }

    DWORD temp;
    VirtualProtect((void*)addr, 7, curProtection, &temp);
}

void Base::SRU_Data::Hooks::SetFastRoad(bool enabled)
{
	static std::vector<unsigned char> defaultBuildTransport = { 0x0F, 0x85, 0x1B, 0x03, 0x00, 0x00 };
    static std::vector<unsigned char> defaultBuildTransportSub = { 0x3B, 0xD1 };
    static std::vector<unsigned char> defaultBuildTransportColony = { 0x74, 0x0F };

    uintptr_t addr = g_base + Offsets::buildTransportHook;

    if (enabled)
    {
        Utils::Nop((BYTE*)addr, 6);
    }
    else
    {
        DWORD curProtection;
        VirtualProtect((void*)addr, 6, PAGE_EXECUTE_READWRITE, &curProtection);

        for (int i = 0; i < defaultBuildTransport.size(); i++)
        {
            *(BYTE*)(addr + i) = defaultBuildTransport[i];
        }

        DWORD temp;
        VirtualProtect((void*)addr, 2, curProtection, &temp);
    }

	addr = g_base + Offsets::buildTransportHookSub;

    if (enabled)
    {
		Utils::Nop((BYTE*)addr, 2);
	}
    else
    {
		DWORD curProtection;
		VirtualProtect((void*)addr, 2, PAGE_EXECUTE_READWRITE, &curProtection);

		for (int i = 0; i < defaultBuildTransportSub.size(); i++)
		{
			*(BYTE*)(addr + i) = defaultBuildTransportSub[i];
		}

		DWORD temp;
		VirtualProtect((void*)addr, 2, curProtection, &temp);
    }

	addr = g_base + Offsets::buildTransportHookColony;

    DWORD curProtection;
    VirtualProtect((void*)addr, 2, PAGE_EXECUTE_READWRITE, &curProtection);

	if (enabled)
	{
		for (int i = 0; i < Offsets::buildTransportColonyNew.size(); i++)
		{
			*(BYTE*)(addr + i) = Offsets::buildTransportColonyNew[i];
		}
	}
	else
	{
		for (int i = 0; i < defaultBuildTransportColony.size(); i++)
		{
			*(BYTE*)(addr + i) = defaultBuildTransportColony[i];
		}
	}

    DWORD temp;
    VirtualProtect((void*)addr, 2, curProtection, &temp);
}