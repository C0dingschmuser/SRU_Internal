#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <direct.h>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <WinInet.h>
#include <fstream>
#include "Injector/injector.h"
#include "data.h"
#include "animation.h"

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

#define DISABLE_OUTPUT

static const int version = 100;
static bool updated = false;

std::string VersionToString(int version)
{
	std::string raw = std::to_string(version);

	return "Version: " + raw.substr(0, 1) + "." + raw.substr(1, 1) + raw.substr(2, 1);
}

DWORD GetProcId(const char* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_stricmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

void InjectDLL(DWORD PID, const char* dllPath)
{
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	std::ifstream File(dllPath, std::ios::binary | std::ios::ate);
	auto FileSize = File.tellg();

	BYTE* pSrcData = new BYTE[(UINT_PTR)FileSize];
	
	File.seekg(0, std::ios::beg);
	File.read((char*)(pSrcData), FileSize);
	File.close();

	ManualMapDll(hProc, pSrcData, FileSize, false, false);

	delete[] pSrcData;
	CloseHandle(hProc);
}

void DumpBinary(const char* szFilePath, void* pBytes, const size_t uSize)
{
	DWORD n = 0;
	HANDLE hFile = CreateFileA(szFilePath, FILE_WRITE_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, pBytes, uSize, &n, NULL);
	CloseHandle(hFile);
}

bool file_exists(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

std::string GenName(int len)
{
	std::string s = "";

	std::string preset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	for (int i = 0; i < len; i++)
	{
		s += preset[1 + (rand() % preset.length())];
	}

	return s;
}

std::string HttpsWebRequestPost(std::string end)
{
    std::string response = "";

    LPSTR tmp = new CHAR[MAX_PATH];

    GetTempPathA(MAX_PATH, tmp);

    std::string a(tmp);

	std::string pp = a + GenName(32) + std::string(".dat");

	std::string final = "https://bruh.games/internal/sru/" + end;

    DeleteUrlCacheEntry(final.c_str());

    HRESULT hr = URLDownloadToFile(NULL, final.c_str(), pp.c_str(), 0, NULL);

    //Write(final);

    if (hr == S_OK)
    {
        if (file_exists(pp))
        {
            std::ifstream file;
            file.open(pp.c_str());

            std::stringstream stream;
            stream << file.rdbuf();
            std::string str = stream.str();

            file.close();

            remove(pp.c_str());

            return str;
        }
    }

    return response;
}

void CheckForUpdate(std::string origName)
{
	if (file_exists("update.bat"))
	{
		updated = true;
		remove("update.bat");
		return;
	}

	std::string response = HttpsWebRequestPost("update.php?v=1");

	if (response.length() != 3) return;

	int serverVersion = std::stoi(response);

	if(serverVersion > version)
	{
		//Initiate update

		char strDir[260];
		_getcwd(strDir, 260); 
		char exeName[] = "\\SRU_Internal_Loader_New.exe"; 
		strncat_s(strDir, exeName, sizeof(exeName)); 
		LPCSTR exePath = strDir; 

		std::string final = "https://bruh.games/internal/sru/SRU_Internal_Loader.exe";

		DeleteUrlCacheEntry(final.c_str());
		HRESULT hr = URLDownloadToFile(NULL, final.c_str(), exePath, 0, NULL);

		if (hr == S_OK)
		{
			if (file_exists(std::string(exePath)))
			{
				std::ofstream file;
				file.open("update.bat");
				file << "@echo off" << std::endl;
				file << "echo Updating SRU_Internal..." << std::endl;
				file << "timeout /t 1 /nobreak>nul" << std::endl;
				file << "del " << origName << std::endl;
				file << "ren SRU_Internal_Loader_New.exe SRU_Internal_Loader.exe" << std::endl;
				file << "start " << origName << std::endl;
				file << "exit";
				file.close();

				system("start update.bat");
				exit(0);
			}
		}
	}
}

int main(int argc, char** argv)
{
	SetConsoleTitle("SRU_Internal");

	//Update Check

	CheckForUpdate(argv[0]);

	//Main Code

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, 1028, 400, TRUE);

	AnimateIntro();

	std::string vstr = "\n\n" + std::string("\x1B[93m") + VersionToString(version) + std::string("\033[0m\t\t\n");

	if (updated)
	{
		printf("\n\x1B[92mUpdate successful!\033[0m\t\t");
	}
	printf(vstr.c_str());
	printf("\x1B[93mWaiting for SRU to open...\033[0m\t\t");

	std::string tmp_prefix;
	char char_path[MAX_PATH];
	
	auto s = GetTempPathA(MAX_PATH, char_path);
	tmp_prefix = std::string(char_path, std::size_t(s));	
	std::string dPath = tmp_prefix + "SRUI.dll";

	remove(dPath.c_str());

	const char* procName = "SupremeRulerUltimate.exe";
	DWORD procId = 0;
	
	while (!procId)
	{
		procId = GetProcId(procName);
		Sleep(50);
	}

	Sleep(250);

	DumpBinary(dPath.c_str(), (void*)dllData, sizeof(dllData));

	InjectDLL(procId, dPath.c_str());

	remove(dPath.c_str());

}
