#include "base.h"

HWND GetProcessWindow();
BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);

void Base::Hooks::Init(bool full = false)
{
    kiero::Status::Enum res = kiero::init(kiero::RenderType::D3D9);

    if (res == kiero::Status::Success)
    {
        kiero::bind(42, (void**)&Base::Data::oEndScene, Hooks::hkEndScene);
        //kiero::bind(82, (void**)&Base::Data::oDrawIndexedPrimitive, Hooks::hkDrawIndexedPrimitive); //not needed atm
        //kiero::bind(84, (void**)&Base::Data::oDrawIndexedPrimitiveUp, Hooks::hkDrawIndexedPrimitiveUp); //^
        kiero::bind(16, (void**)&Base::Data::oReset, Hooks::hkReset);
    }

    if (full)
    {
        GetProcessWindow();
        Data::oWndProc = (WndProc_t)SetWindowLongPtr(Base::Data::window, WNDPROC_INDEX, (LONG_PTR)Base::Hooks::WndProc);
    }
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
    DWORD wndProcId = 0;
    GetWindowThreadProcessId(handle, &wndProcId);

    if (GetCurrentProcessId() != wndProcId)
        return TRUE;

    Base::Data::window = handle;
    return FALSE;
}

HWND GetProcessWindow()
{
    Base::Data::window = (HWND)NULL;
    EnumWindows(EnumWindowsCallback, NULL);
	
    return Base::Data::window;
}

std::vector<BYTE> Base::Hooks::GetOrigBytes(void* toHook, int len)
{
    std::vector<BYTE> origInstructions;

    for (int i = 0; i < len; i++)
    {
        BYTE data = *((BYTE*)toHook + i);
        origInstructions.push_back(data);
    }

    return origInstructions;
}

void Base::Hooks::FunctionHook(void* toHook, void* targetFunc, int len, int enable)
{
	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

    if (enable > 0)
    {
        //Setup hook

        if (enable == 1)
        {
            //Save hook backup

            std::vector<BYTE> origInstructions = GetOrigBytes(toHook, len);

            Base::SRU_Data::HookHolder newHook;
            newHook.addr = (uintptr_t)toHook;
            newHook.origBytes = origInstructions;

            Base::SRU_Data::Hooks::g_hookList.push_back(newHook);
        }

        memset(toHook, 0x90, len);

        uintptr_t relativeAddress = (uintptr_t)targetFunc - (uintptr_t)toHook - 5;

        *(BYTE*)toHook = 0xE9;
        *(DWORD*)((DWORD)toHook + 0x1) = relativeAddress;
    }
    else
    {
        //Restore backup

        Base::SRU_Data::HookHolder holder;

        for (int i = 0; i < Base::SRU_Data::Hooks::g_hookList.size(); i++)
        {
            if (Base::SRU_Data::Hooks::g_hookList[i].addr == (uintptr_t)toHook)
            {
                holder = Base::SRU_Data::Hooks::g_hookList[i];
                break;
            }
        }

        for (int i = 0; i < holder.origBytes.size(); i++)
        {
            *((BYTE*)toHook + i) = holder.origBytes[i];
        }
    }

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
}