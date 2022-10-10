#pragma comment (lib, "user32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")

#include "framework.h"
#include "offsets.h"
#include <string>
#include <vector>
#include <chrono>
#include <windows.h>
#include <iostream>
#include <algorithm>
#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_dx9.h"
#include "include/imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <kiero.h>
#include <stdint.h>
#include <TlHelp32.h>
#include <shellapi.h>
#include <numeric>
#include <thread>
#include <future>
#include <string>
#include <mutex>
#include <sstream>
#include <iomanip>

inline void SuspendThreads(bool suspend) {
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	te32.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(hThreadSnap, &te32)) {
		std::cout << "Error!" << std::endl;
		return;
	}

	do
	{
		if (te32.th32OwnerProcessID == GetCurrentProcessId() && te32.th32ThreadID != GetCurrentThreadId()) //Ignore threads from other processes AND the own thread of course
		{
			HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, 0, te32.th32ThreadID);
			if (hThread)
			{
				if (suspend) {
					SuspendThread(hThread);
				}
				else {
					ResumeThread(hThread);
				}
				
				CloseHandle(hThread);
			}
		}
	} while (Thread32Next(hThreadSnap, &te32));
	CloseHandle(hThreadSnap);
}
