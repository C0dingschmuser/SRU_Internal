//#ifndef MACHINEID_H
#define MACHINEID_H

#include <sstream>

namespace machineid {
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
	
inline uint16_t hashMacAddress(PIP_ADAPTER_INFO info) {
	uint16_t hash = 0;
	for (uint32_t i = 0; i < info->AddressLength; i++) {
		hash += (info->Address[i] << ((i & 1) * 8));
	}
	return hash;
}

inline void getMacHash(uint16_t& mac1, uint16_t& mac2) {
	IP_ADAPTER_INFO AdapterInfo[32];
	DWORD dwBufLen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus != ERROR_SUCCESS)
		return; // no adapters.

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	mac1 = hashMacAddress(pAdapterInfo);
	if (pAdapterInfo->Next)
		mac2 = hashMacAddress(pAdapterInfo->Next);

	// sort the mac addresses. We don't want to invalidate
	// both macs if they just change order.
	if (mac1 > mac2) {
		uint16_t tmp = mac2;
		mac2 = mac1;
		mac1 = tmp;
	}
}

/*inline wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}*/

inline uint16_t getVolumeHash() {
	DWORD serialNum = 0;

	// Determine if this volume uses an NTFS file system.

	GetVolumeInformationA("c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
	uint16_t hash = (uint16_t)((serialNum + (serialNum >> 16)) & 0xFFFF);

	return hash;
}

inline uint16_t getCpuHash() {
	int cpuinfo[4] = { 0, 0, 0, 0 };
	__cpuid(cpuinfo, 0);
	uint16_t hash = 0;
	uint16_t* ptr = (uint16_t*)(&cpuinfo[0]);
	for (uint32_t i = 0; i < 8; i++)
		hash += ptr[i];

	return hash;
}

inline const char* getMachineName() {
	static char computerName[1024];
	DWORD size = 1024;
	GetComputerNameA(computerName, &size);
	return &(computerName[0]);
}

/*inline unsigned short getCpuHash() {
	getCpuid();
	unsigned short hash = 0;
	unsigned int* ptr = (&regs[0]);
	for (unsigned int i = 0; i < 4; i++)
		hash += (ptr[i] & 0xFFFF) + (ptr[i] >> 16);

	return hash;
}*/

inline std::string generateHash(const std::string& bytes) {
	static char chars[] = "0123456789ABCDEF";
	std::stringstream stream;

	auto size = bytes.size();
	for (unsigned long i = 0; i < size; ++i) {
		unsigned char ch = ~((unsigned char)((unsigned short)bytes[i] +
			(unsigned short)bytes[(i + 1) % size] +
			(unsigned short)bytes[(i + 2) % size] +
			(unsigned short)bytes[(i + 3) % size])) * (i + 1);

		stream << chars[(ch >> 4) & 0x0F] << chars[ch & 0x0F];
	}

	return stream.str();
}

static std::string* cachedHash = nullptr;

inline std::string machineHash() {
	static const unsigned long TargetLength = 64;

	if (cachedHash != nullptr) {
		return *cachedHash;
	}

	std::stringstream stream;

	stream << getMachineName();
	stream << getCpuHash();
	stream << getVolumeHash();

	auto string = stream.str();

	while (string.size() < TargetLength) {
		string = string + string;
	}

	if (string.size() > TargetLength) {
		string = string.substr(0, TargetLength);
	}

	return generateHash(string);
}
}
