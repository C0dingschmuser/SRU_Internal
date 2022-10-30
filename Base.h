#pragma once

#include "pch.h"

#define WNDPROC_INDEX GWL_WNDPROC

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef long(__stdcall* DrawIndexedPrimitive)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
typedef long(__stdcall* DrawIndexedPrimitiveUp)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, UINT, UINT, UINT, CONST void*, D3DFORMAT, CONST void*, UINT);
typedef long(__stdcall* Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
typedef LRESULT(CALLBACK* WndProc_t) (HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI dllThread(HMODULE hModule);

namespace Base
{
	void Init(bool full);

	namespace Data
	{
		extern EndScene oEndScene;
		extern DrawIndexedPrimitive oDrawIndexedPrimitive;
		extern DrawIndexedPrimitiveUp oDrawIndexedPrimitiveUp;
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

			extern uintptr_t g_defconJmpBackAddr;
			extern uintptr_t g_defconJmpBackAddr2;
			extern uintptr_t g_defconJmpBackAddr3;
			extern uintptr_t g_defconJmpBackAddr4;

			extern uintptr_t g_diplFreeJmpBackAddr;
			extern uintptr_t g_diplFreeJmpBackAddrDefault;

			extern uintptr_t g_mapSizeJumpBackAddr;

			void SetupFunctionHooks();
			void SetProductionAdjustment(bool enabled);
		}

		namespace Asm
		{
			extern unsigned int g_selectedReg0, g_selectedReg1, g_selectedReg2, g_selectedReg3, g_selectedReg4, g_selectedReg5, g_selectedReg6;
			extern unsigned int g_hexReg0, g_hexReg1, g_hexReg2, g_hexReg3;
			extern byte g_lowestHexSupply, g_currentHexSupply;
			extern unsigned int g_aiSurrReg0, g_aiSurrReg1, g_aiSurrReg2, g_aiSurrReg3, g_aiSurrReg4, g_aiSurrReg5;
			extern int g_aiSurrSize, g_aiSurrFrom, g_aiSurrTo;
			extern unsigned int g_defconReg0, g_defconReg1, g_defconReg2, g_defconReg3, g_defconReg4, g_defconReg5, g_defconReg6,
				g_defconReg01, g_defconReg11, g_defconReg21, g_defconReg31, g_defconReg41, g_defconReg51, g_defconReg61,
				g_defconReg12, g_defconReg22, g_defconReg32, g_defconReg42, g_defconReg52, g_defconReg62;
			extern int g_defconReg02, g_defconNew;
			extern unsigned int g_defconReg03, g_defconReg13, g_defconReg23, g_defconReg33, g_defconReg43, g_defconReg53, g_defconReg63;
			extern unsigned int g_diplFreeReg0, g_diplFreeReg1, g_diplFreeReg2, g_diplFreeReg3, g_diplFreeReg4, g_diplFreeReg5, g_diplFreeReg6;
			extern unsigned int g_mapSizeReg0, g_mapSizeReg1, g_mapSizeReg2, g_mapSizeReg3, g_mapSizeReg4, g_mapSizeReg5, g_mapSizeReg6;
			extern uintptr_t g_aiSurrBase;

			extern std::vector<uintptr_t> g_ownAllocs;
		}

		struct FloatValue
		{
			float* valPtr;
			float freezeVal = 0, origVal = 0;
			bool freeze = false;
			bool saved = false;

			inline void OverrideVal(float val)
			{
				origVal = *valPtr;
				*valPtr = val;
			}
			inline void RestoreVal()
			{
				*valPtr = origVal;
			}
		};

		struct IntValue
		{
			uintptr_t* valPtr;
			int freezeVal, origVal;
			bool freeze = false;

			inline void OverrideVal(int val)
			{
				origVal = *valPtr;
				*valPtr = val;
			}
			inline void RestoreVal()
			{
				*valPtr = origVal;
			}
		};

		struct Int8Value
		{
			uint8_t* valPtr;
			uint8_t freezeVal, origVal;
			bool freeze = false;
		};

		extern uintptr_t g_ownCountryBase;

		struct Resource
		{
			std::string name;

			std::shared_ptr<FloatValue> stock;
			std::shared_ptr<FloatValue> production;
			std::shared_ptr<FloatValue> productionCost;
			std::shared_ptr<FloatValue> marketPrice; //shared between countries
			std::shared_ptr<FloatValue> margin; //shared between countries
		};

		struct Country; //fwd declare

		struct UnitDefault
		{
			enum Property
			{
				MoveSpeed,
				Spotting,
				MoveRange,
				SoftGroundAttack,
				HardGroundAttack,
				CloseGroundAttack,
				CloseAirAttack,
				MidAirAttack,
				HighAirAttack,
				GroundRange,
				NavalRange,
				AirRange,
				FuelCapacity,
				SupplyCapacity,
				BuildTime,
				MAX
			};

			union HolderValue
			{
				float f;
				uint16_t ui16;
			};

			struct ChangeHolder
			{
				Property p;
				uint32_t change = 0;
				HolderValue val;
			};

			void Init(uintptr_t base);
			void RefreshUserCountrys();
			void AddUserCountry(int countryId);
			bool HasUser(int countryId);

			std::string name;
			uintptr_t base = 0;
			std::vector<int> countryIds;
			int spawnId = -1;
			int unitClass = 0;
			
			uintptr_t* customDefault = nullptr;

			bool flag = false; //internal use

			std::vector<std::shared_ptr<ChangeHolder>> propertyChanges;

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
			enum Property
			{
				Health,
				MaxHalth,
				Fuel,
				Supply,
				Efficiency,
				Experience,
				Morale,
				MAX
			};

			bool Init(uintptr_t base);
			void SetDesignPropertySimple(Country* c, UnitDefault::Property p, int range);
			void SetDesignProperty(Country* c, UnitDefault::Property p, UnitDefault::HolderValue v);
			void RestoreDesignProperty(Country* c, UnitDefault::Property p);

			uintptr_t base = 0;
			uintptr_t* deployedState;
			uint8_t* countryId;
			uint16_t* xPos;
			uint16_t* yPos;
			int oldCountry = -1;

			std::shared_ptr<UnitDefault> defaultStats;

			std::vector<std::shared_ptr<FloatValue>> properties;
		};

		struct Tech
		{
			int id;
			bool flag = false;
			uintptr_t base;
			uint8_t category;
			std::string name;
		};

		struct Country
		{
			void Init(uintptr_t base);
			void HandleFreeze();
			void HandleUnits();
			void RefreshResearch();
			void ChangeName(std::string newName);

			uintptr_t base;
			bool real;
			int id;
			int oId;
			int alive;
			int flagOption = 0;
			std::string name;

			unsigned long originalColor = 0;
			bool hasOwnColor = false;

			int selectedTechnologyId = -1;
			int selectedUnitDesignId = -1;

			uintptr_t* populationPtr;
			uintptr_t* colorPtr;
			uint16_t* flagIdPtr;

			uint16_t originalFlagId;
			
			unsigned int lastSurrenderTime = -1;

			std::shared_ptr<FloatValue> treasury;
			std::shared_ptr<FloatValue> domApproval;
			std::shared_ptr<FloatValue> milApproval;
			std::shared_ptr<FloatValue> milReserve;
			std::shared_ptr<FloatValue> researchEff;
			std::shared_ptr<FloatValue> financeInflation;
			std::shared_ptr<FloatValue> financeGDPc;
			std::shared_ptr<FloatValue> financeCreditRating;
			std::shared_ptr<FloatValue> unemployment;
			std::shared_ptr<FloatValue> worldMarketOpinion;
			std::shared_ptr<FloatValue> treatyIntegrity;

			std::vector<Resource> resources;
			std::vector<std::shared_ptr<Tech>> technologies;

			std::shared_ptr<FloatValue> healthcareState;
			std::shared_ptr<FloatValue> educationState;
			std::shared_ptr<FloatValue> infrastructureState;
			std::shared_ptr<FloatValue> environmentState;
			std::shared_ptr<FloatValue> familyState;
			std::shared_ptr<FloatValue> lawEnforcementState;
			std::shared_ptr<FloatValue> culturalSubState;
			std::shared_ptr<FloatValue> socialAssistanceState;

			int* defconPtrs[2] = {0, 0};
			int defconState = -1;

			//could be upgraded to int to make options scalable

			bool invincibleUnits = false;
			bool maxFuelUnits = false;
			bool maxSupplyUnits = false;
			bool maxEfficiencyUnits = false;
			bool maxExperienceUnits = false;
			bool maxMoraleUnits = false;

			struct ModifierValue
			{
				int range = 0;
			};

			std::vector<ModifierValue> unitDefaultModifiers;

			std::vector<std::shared_ptr<FloatValue>> allFloatValues;
			std::vector<Base::SRU_Data::Unit> allUnits;

			bool operator< (const Country& other) const {
				return name < other.name;
			}
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
		extern std::vector<std::shared_ptr<Tech>> g_techList;
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

		extern int g_unitRefreshMaxTime;
		extern int g_mainRefreshTime;

		extern int g_mapSizeX;

		extern int g_paintMode;
		extern int g_paintBrushSize;
		extern bool g_paintUnitTargetCountry;
		extern std::vector<int> g_paintUnitModes;
		
		extern int g_unitDesignType;
		extern int g_unitSpawnCount;
		extern int g_unitSpawnXPos;
		extern int g_unitSpawnYPos;
		extern bool g_unitSpawnReserve;

		extern bool g_ingame;
		extern bool g_addOk;
		extern bool g_shift;
		extern bool g_mapSizeLoaded;
		extern bool g_uiHexSupplySet;
		extern bool g_mouseClicked;
		extern bool g_paintActive;
		extern bool g_paintEnabled;
		extern bool g_paintUnitSpawn;

		extern bool g_disco;
		extern bool g_productionAdjustment;
		extern bool g_aiColony;

		extern uint8_t g_currentHexSupply;

		extern Country* clickedCountry;
		extern Country* clickedTargetCountry;
		extern Country* unitSpawnCountry;

		extern uintptr_t* g_clickedHexPtr;
		extern uintptr_t* g_playSpeedPtr;
		extern uint16_t* g_clickedXPtr;
		extern uint16_t* g_clickedYPtr;
		
		extern uintptr_t g_base;

		std::shared_ptr<UnitDefault> FindUnitDefault(uintptr_t base, int countryId);
		uint32_t ResolveUnitCountry(uint32_t country, int dir = 0);
		void HandleFreezes();
		void CheckSelectedUnits(uintptr_t* selectedUnitsCounter);
		bool IsValidUnit(uintptr_t base);
		bool IsValidDefaultUnit(uintptr_t base);
		void LoadUnits(bool refresh = false);
		void LoadDefaultUnits();
		void LoadDiplTreaties();
		void LoadGroundTypes();
		void LoadTechnologies();
	}

	namespace Execute
	{
		typedef void(__thiscall* _DiplFunc)(DWORD*, char);
		typedef void(__thiscall* _UnlockTechFunc)(unsigned __int8*, int);
		typedef int(__fastcall* _SpawnUnitFunc)(int, int, int, int, int, int, int, int*, int);
		
		extern _DiplFunc diplFunc;
		extern _UnlockTechFunc unlockTechFunc;
		extern _SpawnUnitFunc spawnUnitFunc;

		void SetupFunctions();
		void AnnexCountry(int from, int to);
		void RespawnCountry(int from, int to, int type);
		void UnlockDesign(int to, int design, bool lock);
		void UnlockTech(int to, int tech, bool lock);
		void SetRelations(int relationType, uintptr_t country, uintptr_t oCountry, int add);
		void SpawnUnit(int unitDesign, int amount, uintptr_t country, int spread = 1, bool reserve = true, uint16_t xPos = 0, uint16_t yPos = 0);
		void SetCheat(uint8_t cheat);
		int ExecuteTreaty(int diplTreatyIndex);
		void ExecDipl(DWORD* buffer, char c);
		bool HasTechUnlocked(int countryId, int techId);
		bool HasDesignUnlocked(int countryId, int designId);
		bool HasDesignUnlocked(int countryId, uintptr_t designBase);
	}

	namespace Draw
	{
		extern bool g_countryColorLoaded;
		
		void DrawSelectedCountryText(Base::SRU_Data::Country* cc, const char* text);
		void DrawCountry(Base::SRU_Data::Country* cc);
		void DrawCountryDiplo(Base::SRU_Data::Country* cc, int& treatyMsg);
		void DrawCountryTech(Base::SRU_Data::Country* cc);
		void DrawCountryDesigns(Base::SRU_Data::Country* cc);
		void DrawMap(Base::SRU_Data::Country* cc);
		void DrawUnitSpawn(Base::SRU_Data::Country* cc);
		void DrawUnitModifiers(Base::SRU_Data::Country* cc);
		void DrawUnitSelected(Base::SRU_Data::Country* cc);
	}

	namespace Hooks
	{
		void Init(bool full);
		bool FunctionHook(void* toHook, void* targetFunc, int len);

		long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice);
		long __stdcall hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
		long __stdcall hkDrawIndexedPrimitiveUp(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
		long __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}

	namespace Utils
	{
		struct RGB
		{
			float r = 0;
			float g = 0;
			float b = 0;
		};

		struct RGB ColorConverter(unsigned long hexValue);
		unsigned long ColorConverter(int r, int g, int b, int a);
		unsigned long ColorConverter(float r, float g, float b, float a);
		bool CMPF(float A, float B, float E = 0.005f);
		std::string FloatToPercent(float f, float max, bool simple = false);
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