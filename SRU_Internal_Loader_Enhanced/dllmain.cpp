// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "pch.h"

DWORD GetProcessByName(const char* lpProcessName)
{
    char lpCurrentProcessName[255];

    PROCESSENTRY32 ProcList{};
    ProcList.dwSize = sizeof(ProcList);

    const HANDLE hProcList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcList == INVALID_HANDLE_VALUE)
        return -1;

    if (!Process32First(hProcList, &ProcList))
        return -1;

    wcstombs_s(nullptr, lpCurrentProcessName, ProcList.szExeFile, 255);

    if (lstrcmpA(lpCurrentProcessName, lpProcessName) == 0)
        return ProcList.th32ProcessID;

    while (Process32Next(hProcList, &ProcList))
    {
        wcstombs_s(nullptr, lpCurrentProcessName, ProcList.szExeFile, 255);

        if (lstrcmpA(lpCurrentProcessName, lpProcessName) == 0)
            return ProcList.th32ProcessID;
    }

    return -1;
}

std::string GetExecutableDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

void GetSettingsBool(std::string line, std::string token, bool& value)
{
    if (line.find(token) != std::string::npos)
    {
        std::string valueStr = line.substr(line.find(token) + token.length() + 1);
        value = (valueStr == "true");
    }
}

DWORD WINAPI dllThread(HMODULE hModule)
{
    Sleep(500);

    DWORD dwExit = 0;

    std::string exePath = GetExecutableDirectory();

    std::string modPath = exePath.substr(0, exePath.find("common")) +
        "workshop\\content\\314980\\2874935554\\Dll\\SRU_Internal.dll";

    if (std::filesystem::exists(modPath))
    {
        bool ok = true;

        if (std::filesystem::exists("sru_internal_config.ini"))
        {
            std::ifstream file("sru_internal_config.ini");

            for (std::string line; getline(file, line); )
            {
                GetSettingsBool(line, "modenabled", ok);
            }
        }

        if (ok)
        {
            //load mod

            LoadLibraryA(modPath.c_str());
        }
    }

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

