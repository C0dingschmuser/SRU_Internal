﻿#include <iostream>
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
//#include "data.h"
#include "animation.h"
#include "machineid.h"

//#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

#define DISABLE_OUTPUT

static const int version = 125;
static bool updated = false;

std::string VersionToString(int version)
{
	std::string raw = std::to_string(version);

	return "Version: " + raw.substr(0, 1) + "." + raw.substr(1, 1) + raw.substr(2, 1);
}

std::string GetExecutableDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	return std::string(buffer).substr(0, pos);
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

	ManualMapDll(hProc, pSrcData, FileSize);

	delete[] pSrcData;
	CloseHandle(hProc);
}

/*void DumpBinary(const char* szFilePath, void* pBytes, const size_t uSize)
{
	DWORD n = 0;
	HANDLE hFile = CreateFileA(szFilePath, FILE_WRITE_DATA, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, pBytes, uSize, &n, NULL);
	CloseHandle(hFile);
}*/

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

std::string StreamToMem(std::string URL)
{
	DeleteUrlCacheEntry(URL.c_str());
	std::string header = "Accept: *" "/" "*\r\n\r\n";
	HANDLE hInter = InternetOpen("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HANDLE hURL = InternetOpenUrl(hInter, URL.c_str(), header.c_str(), strlen(header.c_str()), INTERNET_FLAG_DONT_CACHE, 0);

	char* Buffer = new char[5000000]; //100mb
	memset(Buffer, 0, 5000000);
	DWORD BytesRead = 1;

	std::string data;

	if (InternetReadFile(hURL, Buffer, 5000000, &BytesRead))
	{
		data = std::string(Buffer);
	}

	delete[] Buffer;
	InternetCloseHandle(hInter);
	InternetCloseHandle(hURL);

	return data;
}

std::vector<unsigned char> StreamToMemBinary(std::string URL)
{
	DeleteUrlCacheEntry(URL.c_str());
	std::string header = "Accept: *" "/" "*\r\n\r\n";
	HANDLE hInter = InternetOpen("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HANDLE hURL = InternetOpenUrl(hInter, URL.c_str(), header.c_str(), strlen(header.c_str()), INTERNET_FLAG_DONT_CACHE, 0);

	unsigned char* Buffer = new unsigned char[5000000]; //100mb
	memset(Buffer, 0, 5000000);
	DWORD BytesRead = 1;

	std::vector<unsigned char> data;

	if (InternetReadFile(hURL, Buffer, 5000000, &BytesRead))
	{
		for (int i = 0; i < BytesRead; i++)
		{
			data.push_back(Buffer[i]);
		}
	}

	delete[] Buffer;
	InternetCloseHandle(hInter);
	InternetCloseHandle(hURL);

	return data;
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string HttpsWebRequestPost(std::string end)
{
    std::string response = "";

    LPSTR tmp = new CHAR[MAX_PATH];

    GetTempPathA(MAX_PATH, tmp);

	std::string final = "http://bruh.games/internal/sru/" + end;

	response = StreamToMem(final);
	/*std::cout << StreamToMem(final) << std::endl;
	system("pause");

	int count = 10;
	HRESULT hr;

	for (int i = 0; i < 10; i++)
	{
		DeleteUrlCacheEntry(final.c_str());
		Sleep(25);
		hr = URLDownloadToFileW(NULL, s2ws(final).c_str(), s2ws(pp).c_str(), 0, NULL);

		if (hr != 0x800C0008)
		{
			break;
		}
	}

	std::cout << std::hex << hr << std::endl;
	system("pause");

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
    }*/

    return response;
}

void CheckForUpdate(std::string origName)
{
	if (file_exists("update.bat"))
	{
		updated = true;
		remove("update.bat");

		if (file_exists("changelog.txt"))
		{
			remove("changelog.txt");
		}

		return;
	}

	system("COLOR 0E");
	std::cout << "Checking for Update..." << std::endl;

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

		std::string final = "http://bruh.games/internal/sru/SRU_Internal_Loader.exe";

		std::vector<unsigned char> response = StreamToMemBinary(final);
		size_t strSize = response.size();

		std::ofstream loaderFile(exePath, std::ios::out | std::ios::binary);
		loaderFile.write((char*)&response[0], strSize);
		loaderFile.close();

		//DeleteUrlCacheEntry(final.c_str());
		//HRESULT hr = URLDownloadToFile(NULL, final.c_str(), exePath, 0, NULL);

		std::string base_filename = origName.substr(origName.find_last_of("/\\") + 1);

		//if (hr == S_OK)
		{
			if (file_exists(std::string(exePath)))
			{
				std::string fullPath = GetExecutableDirectory() + "\\SRU_Internal.dll";
				if (file_exists(fullPath))
				{
					remove(fullPath.c_str());
				}

				std::ofstream file;
				file.open("update.bat");
				file << "@echo off" << std::endl;
				file << "cd /d %~dp0" << std::endl;
				file << "echo Updating SRU_Internal..." << std::endl;
				file << "timeout /t 1 /nobreak>nul" << std::endl;
				file << "del " << base_filename << std::endl;
				file << "ren SRU_Internal_Loader_New.exe SRU_Internal_Loader.exe" << std::endl;
				file << "start SRU_Internal_Loader.exe" << std::endl;
				file << "exit";
				file.close();

				system("start update.bat");
				exit(0);
			}
		}
	}
}

void CheckDLL()
{
	std::string fullPath = GetExecutableDirectory() + "\\SRU_Internal.dll";
	if (!file_exists(fullPath))
	{
		std::cout << "DLL not found, downloading..." << std::endl;

		std::string final = "http://bruh.games/internal/sru/SRU_Internal.dll";

		std::vector<unsigned char> response = StreamToMemBinary(final);
		size_t strSize = response.size();

		std::ofstream loaderFile(fullPath, std::ios::out | std::ios::binary);
		loaderFile.write((char*)&response[0], strSize);
		loaderFile.close();

		Sleep(1000);
	}
}

int main(int argc, char** argv)
{
	SetConsoleTitle("SRU_Internal");

	//Update Check

	CheckForUpdate(argv[0]);
	CheckDLL();

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

	/*std::string tmp_prefix;
	char char_path[MAX_PATH];
	
	auto s = GetTempPathA(MAX_PATH, char_path);
	tmp_prefix = std::string(char_path, std::size_t(s));	
	std::string dPath = tmp_prefix + "SRUI.dll";

	remove(dPath.c_str());*/

	const char* procName = "SupremeRulerUltimate.exe";
	DWORD procId = 0;
	
	while (!procId)
	{
		procId = GetProcId(procName);
		Sleep(50);
	}

	Sleep(250);

	//DumpBinary(dPath.c_str(), (void*)dllData, sizeof(dllData));

	std::string dPath = GetExecutableDirectory() + "\\SRU_Internal.dll";

	if (!file_exists(dPath.c_str()))
	{
		std::cout << "ERROR: Dll is missing, please restart Loader!" << std::endl;
		std::cout << "Tried Path: " << dPath << std::endl;
		system("pause");
		exit(0);
	}

	InjectDLL(procId, dPath.c_str());

	printf("\n\x1B[92mInjection successful!\033[0m\t\t");
	Sleep(2000);
}
