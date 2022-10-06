#include "base.h"

using namespace Base::SRU_Data;
using namespace Base::SRU_Data::Hooks;
using namespace Base::SRU_Data::Asm;

void CreateUnit(uintptr_t base);
void AddSurrenderEvent(int from, int to);

void __declspec(naked) GetSelectedUnits()
{
    __asm {
        xor eax, eax
        cmp byte ptr[ebp + 0x16], 00
        mov[g_nextUnitEntity], ebp
    }

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

    __asm {
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
    std::cout << std::dec << " " << from << " " << to << std::endl;

    SurrenderEvent* s = &surrenderEvents[g_surrenderEventCount];
    s->lastTime = *(uintptr_t*)(g_base + Offsets::currentDayTime) + 500;
    s->from = from;
    s->to = to;

    //int state = *(uintptr_t*)(0x199ED3B0);
    //std::cout << state << std::endl;

    g_surrenderEventCount++;
    if (g_surrenderEventCount >= 32)
    {
        g_surrenderEventCount = 0;
    }
}

void __declspec(naked) HandleNewPos()
{
    __asm {
		mov [esp + 0xBC4], ecx
		mov [g_xPos], eax
		mov [g_yPos], ecx
		jmp [g_posChangedJmpBackAddr]
    }
}

void Base::SRU_Data::Hooks::SetupFunctionHooks()
{
    //Hook selected units

    int hookLength = 6;
    uintptr_t hookAddress = g_base + Offsets::selectedUnitsHook;
    Base::SRU_Data::Hooks::g_selectedJmpBackAddr = hookAddress + hookLength;
    //Base::Hooks::FunctionHook((void*)hookAddress, GetSelectedUnits, hookLength);

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

	//Hook position

    hookLength = 7;
    hookAddress = g_base + Offsets::posHook;
	Base::SRU_Data::Hooks::g_posChangedJmpBackAddr = hookAddress + hookLength;
	Base::Hooks::FunctionHook((void*)hookAddress, HandleNewPos, hookLength);
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