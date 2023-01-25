#include "Base.h"

struct Base::Utils::RGB Base::Utils::ColorConverter(unsigned long hexValue)
{
	struct RGB rgbColor;
	rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;
	rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;
	rgbColor.b = ((hexValue) & 0xFF) / 255.0;

	return rgbColor;
}

unsigned long Base::Utils::ColorConverter(int r, int g, int b, int a)
{
	return ((a & 0xff) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

unsigned long Base::Utils::ColorConverter(float r, float g, float b, float a)
{
	int ir = (int)(r * 255.0f);
	int ig = (int)(g * 255.0f);
	int ib = (int)(b * 255.0f);
	int ia = (int)(a * 255.0f);

	return ColorConverter(ir, ig, ib, ia);
}

std::string Base::Utils::FloatToPercent(float f, float max, bool simple)
{
	float p;
	if (!simple)
	{
		p = (f / max) * 100;
	}
	else
	{
		p = f * 100;
	}

	p = std::clamp(p, 0.0f, 100.0f);

	std::stringstream str;
	str << std::fixed << std::setprecision(2) << p << "%%";
	return str.str();
}

std::string Base::Utils::FloatToPercent2(float f, float max, bool simple)
{
	float p;
	if (!simple)
	{
		p = (f / max) * 100;
	}
	else
	{
		p = f * 100;
	}

	p = std::clamp(p, -150.0f, 150.0f);

	std::stringstream str;
	str << std::fixed << std::setprecision(2) << p << "%%";
	return str.str();
}

std::string Base::Utils::FloatToPercent3(float f, float max, bool simple)
{
	float p;
	if (!simple)
	{
		p = (f / max) * 100;
	}
	else
	{
		p = f * 100;
	}

	p = std::clamp(p, -300.0f, 300.0f);

	std::stringstream str;
	str << std::fixed << std::setprecision(2) << p << "%%";
	return str.str();
}

bool Base::Utils::CMPF(float A, float B, float E)
{
	return (std::fabs(A - B) < E);
}

bool Base::Utils::GetValueBool(uintptr_t* addr, int value)
{
	unsigned int testVal = ~*addr & value | *addr & (0xFFFFFFFF - value);

	if (testVal > *addr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Base::Utils::SetValueMask(uintptr_t* addr, int offset, int value)
{
	int bitmask = (3 << offset);
	value = value << offset;
	*addr = (*addr & (~bitmask)) | (value & bitmask);
}

void Base::Utils::SetValueBool(uintptr_t* addr, int value, bool enable)
{
	if (GetValueBool(addr, value) != enable)
	{
		*addr = ~*addr & value | *addr & (0xFFFFFFFF - value);
	}
}

unsigned short Base::Utils::GetShortPopulationValue(unsigned int newPopulation)
{
	unsigned short v3827 = 0;

	if ((newPopulation & 0xFFFF0000) != 0)
	{
		newPopulation >>= 4;
		v3827 = 1;
	}
	else
	{
		v3827 = 0;
	}
	if ((newPopulation & 0xFFFF0000) != 0)
	{
		newPopulation >>= 4;
		++v3827;
	}
	if ((newPopulation & 0xFFFF0000) != 0)
	{
		newPopulation >>= 4;
		++v3827;
	}

	int data = v3827 | newPopulation & 0xFFFC;
	return (unsigned short)data;
}

unsigned int Base::Utils::GetFullPopulationValue(unsigned short populationValue)
{
	// Check if the original value was smaller than 4 bytes
	if ((populationValue & 0x0003) == 0)
	{
		// Return the input value unchanged
		return populationValue;
	}

	unsigned int newPopulation = populationValue & 0xFFFC;
	int shiftCount = populationValue & 0x0003;
	int shiftsPerformed = 0;
	while (shiftCount > 0)
	{
		if (shiftCount & 0x0001)
		{
			newPopulation <<= 4;
			++shiftsPerformed;
		}
		shiftCount >>= 1;
	}
	if (shiftCount > 3)
	{
		shiftsPerformed += shiftCount - 3;
	}
	while (shiftsPerformed > 0)
	{
		newPopulation <<= 4;
		--shiftsPerformed;
	}
	return newPopulation;
}

bool Base::Utils::MemCompare(const BYTE* bData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++bData, ++bMask)
	{
		if (*szMask == 'x' && *bData != *bMask)
		{
			return false;
		}
	}
	return (*szMask == NULL);
}

bool Base::Utils::CanReadPtr(void* ptr)
{
	if (ptr == nullptr) return false;

	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery(ptr, &mbi, sizeof(mbi));

	return (!(mbi.Protect & PAGE_GUARD) && mbi.Protect != PAGE_NOACCESS && mbi.State == MEM_COMMIT &&
		mbi.Protect & (PAGE_READONLY | PAGE_EXECUTE_READ | PAGE_READWRITE | PAGE_EXECUTE_READWRITE));
}

uintptr_t Base::Utils::PointerChain(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

uintptr_t Base::Utils::PatternScan(HANDLE hProcess, uintptr_t start, uintptr_t size, const char* sig, const char* mask)
{
	BYTE* data = new BYTE[size];
	SIZE_T bytesRead;

	ReadProcessMemory(hProcess, (LPVOID)start, data, size, &bytesRead);

	for (uintptr_t i = 0; i < size; i++)
	{
		if (MemCompare((const BYTE*)(data + i), (const BYTE*)sig, mask))
		{
			return start + i;
		}
	}
	delete[] data;
	return NULL;
}

void Base::Utils::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Base::Utils::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Base::Utils::WPM(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void Base::Utils::RPM(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	ReadProcessMemory(hProcess, dst, src, size, nullptr);
}

std::string Base::Utils::StreamToMem(std::string url)
{
	DeleteUrlCacheEntry(url.c_str());
	std::string header = "Accept: *" "/" "*\r\n\r\n";
	HANDLE hInter = InternetOpen("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HANDLE hURL = InternetOpenUrl(hInter, url.c_str(), header.c_str(), strlen(header.c_str()), INTERNET_FLAG_DONT_CACHE, 0);

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

std::string Base::Utils::SHA256(std::string str)
{
	std::string hex;
	picosha2::hash256_hex_string(str, hex);

	return hex;
}