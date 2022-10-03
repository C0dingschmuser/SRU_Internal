#pragma once

#include "pch.h"

#define WNDPROC_INDEX GWL_WNDPROC

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef long(__stdcall* Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef LRESULT(CALLBACK* WndProc_t) (HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI dllThread(HMODULE hModule);

namespace Base
{
	void Init(bool full);

	namespace Data
	{
		extern EndScene oEndScene;
		extern Reset oReset;
		extern WndProc_t oWndProc;
		extern HWND window;
		extern bool init;
		extern bool ShowMenu;
		extern int subMenu;
	}

	namespace SRU_Data
	{
		namespace Hooks
		{
			extern uintptr_t g_selectedJmpBackAddr;
			extern uintptr_t g_hexSupplyJmpBackAddr;
			extern uintptr_t g_aiSurrenderJmpBackAddr;
			extern uintptr_t g_mouseClickedJmpBackAddr;
			extern uintptr_t g_posChangedJmpBackAddr;

			void SetupFunctionHooks();
			void SetProductionAdjustment(bool enabled);
		}

		namespace Asm
		{
			extern unsigned int g_hexReg0, g_hexReg1, g_hexReg2, g_hexReg3;
			extern byte g_lowestHexSupply, g_currentHexSupply;
			extern unsigned int g_aiSurrReg0, g_aiSurrReg1, g_aiSurrReg2, g_aiSurrReg3, g_aiSurrReg4, g_aiSurrReg5;
			extern int g_aiSurrSize, g_aiSurrFrom, g_aiSurrTo;
			extern int g_xPos, g_yPos;
			extern uintptr_t g_aiSurrBase;
		}

		struct FloatValue
		{
			float* valPtr;
			float freezeVal, origVal;
			bool freeze = false;
		};

		struct IntValue
		{
			uintptr_t* valPtr;
			int freezeVal, origVal;
			bool freeze = false;
		};

		struct Int8Value
		{
			uint8_t* valPtr;
			uint8_t freezeVal, origVal;
			bool freeze = false;
		};

		extern uintptr_t g_ownCountryBase;

		struct Country
		{
			void Init(uintptr_t base);
			void HandleFreeze();

			uintptr_t base;
			bool real;
			int id;
			int oId;
			int alive;
			std::string name;

			uintptr_t* populationPtr;
			unsigned int lastSurrenderTime = -1;

			std::shared_ptr<FloatValue> treasury;
			std::shared_ptr<FloatValue> domApproval;
			std::shared_ptr<FloatValue> milApproval;
			std::shared_ptr<FloatValue> milReserve;
			std::shared_ptr<FloatValue> researchEff;
			std::shared_ptr<FloatValue> financeCreditRating;
			std::shared_ptr<FloatValue> worldMarketOpinion;
			std::shared_ptr<FloatValue> treatyIntegrity;

			std::shared_ptr<FloatValue> agricultureStock;
			std::shared_ptr<FloatValue> rubberStock;
			std::shared_ptr<FloatValue> timberStock;
			std::shared_ptr<FloatValue> petroleumStock;
			std::shared_ptr<FloatValue> coalStock;
			std::shared_ptr<FloatValue> metalStock;
			std::shared_ptr<FloatValue> uranStock;
			std::shared_ptr<FloatValue> consumerStock;
			std::shared_ptr<FloatValue> industryStock;
			std::shared_ptr<FloatValue> militaryStock;

			std::shared_ptr<FloatValue> healthcareState;
			std::shared_ptr<FloatValue> educationState;
			std::shared_ptr<FloatValue> infrastructureState;
			std::shared_ptr<FloatValue> environmentState;
			std::shared_ptr<FloatValue> familyState;
			std::shared_ptr<FloatValue> lawEnforcementState;
			std::shared_ptr<FloatValue> culturalSubState;
			std::shared_ptr<FloatValue> socialAssistanceState;

			std::vector<std::shared_ptr<FloatValue>> allFloatValues;

			bool operator< (const Country& other) const {
				return name < other.name;
			}
		};

		struct UnitDefault
		{
			void Init(uintptr_t base);
			void AddUserCountry(int countryId);
			bool HasUser(int countryId);

			std::string name;
			uintptr_t base = 0;
			std::vector<int> countryIds;
			int spawnId = -1;

			bool flag = false; //internal use

			std::shared_ptr<IntValue> moveSpeed;
			std::shared_ptr<IntValue> spotting;
			std::shared_ptr<IntValue> moveRange;
			std::shared_ptr<IntValue> softGroundAttack;
			std::shared_ptr<IntValue> hardGroundAttack;
			std::shared_ptr<IntValue> closeGroundAttack;
			std::shared_ptr<IntValue> closeAirAttack;
			std::shared_ptr<IntValue> midAirAttack;
			std::shared_ptr<IntValue> highAirAttack;

			std::shared_ptr<FloatValue> groundRange;
			std::shared_ptr<FloatValue> navalRange;
			std::shared_ptr<FloatValue> airRange;
			std::shared_ptr<FloatValue> fuelCapacity;
			std::shared_ptr<FloatValue> supplyCapacity;
			std::shared_ptr<FloatValue> buildTime;
		};

		struct Unit
		{
			void Init(uintptr_t base);

			uintptr_t base = 0;
			uintptr_t* currentHex;
			int countryID;

			std::shared_ptr<UnitDefault> defaultStats;
			std::shared_ptr<FloatValue> fuel;
			std::shared_ptr<FloatValue> health;
			std::shared_ptr<FloatValue> maxHealth;
			std::shared_ptr<FloatValue> morale;
		};

		struct DiplTreaty
		{
			std::string name;
			int treatyId;
		};

		struct SurrenderEvent
		{
			int from;
			int to;
			int lastTime = -1;
		};

		struct GroundType
		{
			uint8_t id = 0;
			std::string name;
		};

		extern std::vector<std::shared_ptr<UnitDefault>> g_defaultUnitList;
		extern std::vector<Unit> g_selectedUnitList;
		extern std::vector<Unit> g_unitList;
		extern std::vector<DiplTreaty> g_diplTreatyList;
		extern std::vector<Country> g_countryList;
		extern std::vector<GroundType> g_groundTypeList;
		
		extern SurrenderEvent surrenderEvents[];
		extern uintptr_t g_nextUnitEntity;
		
		extern int g_unitEntityCountSelected;
		extern int g_clickedCountry;
		extern int g_clickedCountryRaw;
		extern int g_selectedTargetCountry;
		extern int g_lastClickedCountry;
		extern int g_lastClickedTargetCountry;
		extern int g_ownCountryId;
		extern int g_ownOtherCountryId;
		
		extern int g_paintSelectedComboCountry;
		extern int g_paintSelectedComboLoyalty;
		extern int g_paintSelectedComboGround;
		
		extern int g_unitSpawnSelectedUnitDesign;

		extern int g_surrenderEventCount;
		
		extern bool g_ingame;
		extern bool g_addOk;
		extern bool g_shift;
		extern bool g_uiHexSupplySet;
		extern bool g_mouseClicked;
		extern bool g_paintActive;
		extern bool g_paintEnabled;

		extern bool g_productionAdjustment;
		extern bool g_aiColony;

		extern uint8_t g_currentHexSupply;

		extern Country* clickedCountry;
		extern Country* clickedTargetCountry;
		extern Country* unitSpawnCountry;

		extern uintptr_t* g_clickedHexPtr;
		
		extern uintptr_t g_base;

		std::shared_ptr<UnitDefault> FindUnitDefault(uintptr_t base, int countryId);
		uint32_t ResolveUnitCountry(uint32_t country, int dir = 0);
		void HandleFreezes();
		void CheckSelectedUnits(uintptr_t* selectedUnitsCounter);
		void LoadUnits();
		void LoadDefaultUnits();
		void LoadDiplTreaties();
		void LoadGroundTypes();
	}

	namespace Execute
	{
		typedef void(__thiscall* _DiplFunc)(void*, char);
		typedef int(__fastcall* _SpawnUnitFunc)(int, int, int, int, int, int, int, int*, int);
		
		extern _DiplFunc diplFunc;
		extern _SpawnUnitFunc spawnUnitFunc;

		void SetupFunctions();
		void AnnexCountry(int from, int to);
		void RespawnCountry(int from, int to, int type);
		void SetRelations(int relationType, uintptr_t country, uintptr_t oCountry, int add);
		void SpawnUnit(int unitDesign, int amount, uintptr_t country, int spread = 1, bool reserve = true, uint16_t xPos = 0, uint16_t yPos = 0);
		int ExecuteTreaty(int diplTreatyIndex);
	}

	namespace Draw
	{
		void DrawSelectedCountryText(Base::SRU_Data::Country* cc, const char* text);
		void DrawCountry(Base::SRU_Data::Country* cc);
		void DrawCountryDiplo(Base::SRU_Data::Country* cc, int& treatyMsg);
		void DrawMap(Base::SRU_Data::Country* cc);
		void DrawUnit(Base::SRU_Data::Country* cc);
	}

	namespace Hooks
	{
		void Init(bool full);
		bool FunctionHook(void* toHook, void* targetFunc, int len);

		long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice);
		long __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}

	namespace Utils
	{
		bool MemCompare(const BYTE* bData, const BYTE* bMask, const char* szMask);
		bool CanReadPtr(void* ptr);
		uintptr_t PointerChain(uintptr_t ptr, std::vector<unsigned int> offsets);
		uintptr_t PatternScan(HANDLE hProcess, uintptr_t start, uintptr_t size, const char* sig, const char* mask);
		void Patch(BYTE* dst, BYTE* src, unsigned int size);
		void Nop(BYTE* dst, unsigned int size);
		void WPM(BYTE* dst, BYTE* src, unsigned int size);
		void RPM(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);

		template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
		{
			static const char* digits = "0123456789ABCDEF";
			std::string rc(hex_len, '0');
			for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
				rc[i] = digits[(w >> j) & 0x0f];
			return rc;
		}
	}
}