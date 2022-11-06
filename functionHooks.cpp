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
        cmp byte ptr[ebp + 0x16], 00
        jmp[g_selectedJmpBackAddr]
    }
}

void CreateUnit(uintptr_t base)
{
    Unit newUnit{};
    newUnit.Init(base);

    g_selectedUnitList.push_back(newUnit);
}

void __declspec(naked) SetHexSupply()
{
    __asm {
        lea eax, [ebp - 01]
        mov[Asm::g_currentHexSupply], al
        mov[g_hexReg0], eax
        mov[g_hexReg1], ecx
        mov[g_hexReg2], edx
        mov[g_hexReg3], ebx
    }

    if (Asm::g_currentHexSupply < g_lowestHexSupply ||
        Asm::g_hexReg0 == 0xFFFFFFFF)
    {
        Asm::g_currentHexSupply = g_lowestHexSupply;
    }

    __asm {
        mov eax, g_hexReg0
        mov ecx, g_hexReg1
        mov edx, g_hexReg2
        mov ebx, g_hexReg3
        mov al, Asm::g_currentHexSupply
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
    }

    AddSurrenderEvent(Asm::g_aiSurrFrom, Asm::g_aiSurrTo);

    __asm {
        mov eax, g_aiSurrReg0
        mov ecx, g_aiSurrReg1
        mov edx, g_aiSurrReg2
        mov ebp, g_aiSurrReg3
        mov edi, g_aiSurrReg4
        mov esi, g_aiSurrReg5
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

    bool found = false;

    for (int i = 0; i < Asm::g_ownAllocs.size(); i++)
    {
        if (Asm::g_ownAllocs[i] == (uintptr_t)g_diplFreeReg3)
        {
            found = true;
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
    if (g_buildCheckReg6 == g_ownOtherCountryId)
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

    SurrenderEvent* s = &surrenderEvents[g_surrenderEventCount];
    s->lastTime = *(uintptr_t*)(g_base + Offsets::currentDayTime) + 500;
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

void Base::SRU_Data::Hooks::SetupFunctionHooks()
{
    //Hook selected units

    int hookLength = 6;
    uintptr_t hookAddress = g_base + Offsets::selectedUnitsHook;
    Base::SRU_Data::Hooks::g_selectedJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, GetSelectedUnits, hookLength);

    //Hook hex supply

    hookAddress = g_base + Offsets::hexSupplyNop;
    Base::Utils::Nop((BYTE*)hookAddress, 2);

    hookAddress = g_base + Offsets::hexSupplyHook;
    Base::SRU_Data::Hooks::g_hexSupplyJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, SetHexSupply, hookLength);

    //Hook ai surrender
    hookAddress = g_base + Offsets::aiSurrenderHook;
    Base::SRU_Data::Hooks::g_aiSurrenderJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleAISurrender, hookLength);

    //Hook mouse click
    hookLength = 5;
    hookAddress = g_base + Offsets::mouseClickHook;
    Base::SRU_Data::Hooks::g_mouseClickedJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleMouseClick, hookLength);

    //Hook defcon

    hookLength = 6;
    hookAddress = g_base + Offsets::defconHook;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw, hookLength);

    hookLength = 6;
    hookAddress = g_base + Offsets::defconHook2;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr2 = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw2, hookLength);

    hookLength = 8;
    hookAddress = g_base + Offsets::defconHook3;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr3 = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw3, hookLength);

    hookLength = 7;
    hookAddress = g_base + Offsets::defconHook4;
    Base::SRU_Data::Hooks::g_defconJmpBackAddr4 = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, HandleDefconRaw4, hookLength);

    hookLength = 5;
    hookAddress = g_base + Offsets::diplFreeHook;
    Base::SRU_Data::Hooks::g_diplFreeJmpBackAddr = hookAddress + hookLength;
    Base::SRU_Data::Hooks::g_diplFreeJmpBackAddrDefault = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, DiplFree, hookLength);

    hookLength = 6;
    hookAddress = g_base + Offsets::mapSizeHook;
    Base::SRU_Data::Hooks::g_mapSizeJumpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, GetMapSize, hookLength);

    hookLength = 5;
    hookAddress = g_base + Offsets::buildCheckHook;
    Base::SRU_Data::Hooks::g_buildCheckJumpBackAddr = hookAddress + hookLength;
    Base::Hooks::FunctionHook((void*)hookAddress, BuildCheckOwner, hookLength);

    //overwrite transport build

    uintptr_t addr = g_base + Offsets::buildTransportHook;
    Utils::Nop((BYTE*)addr, 6);
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