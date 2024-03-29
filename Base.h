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
	const unsigned int g_version = 126;

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
		struct HookHolder
		{
			uintptr_t addr;
			std::vector<BYTE> origBytes;
			std::vector<BYTE> newBytes;
		};

		namespace Hooks
		{
			extern std::vector<HookHolder> g_hookList;

			extern int g_lastFunctionHookState;

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

			extern uintptr_t g_buildCheckJumpBackAddr;

			extern uintptr_t g_hexNameBigJumpBackAddr;
			extern uintptr_t g_hexNameBigJumpBackAddrNone;
			extern uintptr_t g_hexNameBigJumpBackAddrData;

			extern uintptr_t g_sphereNameJumpBackAddr;

			extern uintptr_t g_openSettingsJumpBackAddr;
			extern uintptr_t g_closeSettingsJumpBackAddr;

			extern uintptr_t g_autosaveBeginJumpBackAddr;
			extern uintptr_t g_autosaveEndJumpBackAddr;

			extern uintptr_t g_quitGameJumpBackAddr;

			void SetupFunctionHooks(int enabled = 1);
			void SetProductionAdjustment(bool enabled);
			void SetFastRoad(bool enabled);
			void SetUnitSelect(bool enabled);
		}

		namespace Asm
		{
			extern unsigned int g_selectedReg0, g_selectedReg1, g_selectedReg2, g_selectedReg3, g_selectedReg4, g_selectedReg5, g_selectedReg6, g_selectedReg7;
			extern unsigned int g_hexReg0, g_hexReg1, g_hexReg2, g_hexReg3, g_hexReg4, g_hexReg5, g_hexReg6, g_hexReg7;
			extern byte g_lowestHexSupply, g_currentHexSupply;
			extern unsigned int g_aiSurrReg0, g_aiSurrReg1, g_aiSurrReg2, g_aiSurrReg3, g_aiSurrReg4, g_aiSurrReg5, g_aiSurrReg6, g_aiSurrReg7;
			extern int g_aiSurrSize, g_aiSurrFrom, g_aiSurrTo;
			extern unsigned int g_defconReg0, g_defconReg1, g_defconReg2, g_defconReg3, g_defconReg4, g_defconReg5, g_defconReg6,
				g_defconReg01, g_defconReg11, g_defconReg21, g_defconReg31, g_defconReg41, g_defconReg51, g_defconReg61,
				g_defconReg12, g_defconReg22, g_defconReg32, g_defconReg42, g_defconReg52, g_defconReg62;
			extern int g_defconReg02, g_defconNew;
			extern unsigned int g_defconReg03, g_defconReg13, g_defconReg23, g_defconReg33, g_defconReg43, g_defconReg53, g_defconReg63;
			extern unsigned int g_diplFreeReg0, g_diplFreeReg1, g_diplFreeReg2, g_diplFreeReg3, g_diplFreeReg4, g_diplFreeReg5, g_diplFreeReg6;
			extern unsigned int g_mapSizeReg0, g_mapSizeReg1, g_mapSizeReg2, g_mapSizeReg3, g_mapSizeReg4, g_mapSizeReg5, g_mapSizeReg6;
			extern unsigned int g_buildCheckReg0, g_buildCheckReg1, g_buildCheckReg2, g_buildCheckReg3, g_buildCheckReg4, g_buildCheckReg5, g_buildCheckReg6, g_buildCheckReg7;
			extern unsigned int g_hexNameReg0, g_hexNameReg1, g_hexNameReg2, g_hexNameReg3, g_hexNameReg4, g_hexNameReg5, g_hexNameReg6, g_hexNameReg7;
			extern unsigned int g_sphereNameReg0, g_sphereNameReg1, g_sphereNameReg2, g_sphereNameReg3, g_sphereNameReg4, g_sphereNameReg5, g_sphereNameReg6, g_sphereNameReg7;
			extern unsigned int g_openSettingsReg0, g_openSettingsReg1, g_openSettingsReg2, g_openSettingsReg3, g_openSettingsReg4, g_openSettingsReg5, g_openSettingsReg6, g_openSettingsReg7;
			extern unsigned int g_closeSettingsReg0, g_closeSettingsReg1, g_closeSettingsReg2, g_closeSettingsReg3, g_closeSettingsReg4, g_closeSettingsReg5, g_closeSettingsReg6, g_closeSettingsReg7;
			extern unsigned int g_autosaveBeginReg0, g_autosaveBeginReg1, g_autosaveBeginReg2, g_autosaveBeginReg3, g_autosaveBeginReg4, g_autosaveBeginReg5, g_autosaveBeginReg6, g_autosaveBeginReg7;
			extern unsigned int g_autosaveEndReg0, g_autosaveEndReg1, g_autosaveEndReg2, g_autosaveEndReg3, g_autosaveEndReg4, g_autosaveEndReg5, g_autosaveEndReg6, g_autosaveEndReg7;
			extern unsigned int g_quitGameReg0, g_quitGameReg1, g_quitGameReg2, g_quitGameReg3, g_quitGameReg4, g_quitGameReg5, g_quitGameReg6, g_quitGameReg7;
			
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
		struct Leader; //fwd declare

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
			bool HasOrigin(int countryId);

			std::string name;
			uintptr_t base = 0;
			std::vector<int> countryIds;
			std::vector<int> originCountryIds;
			int spawnId = -1;
			int unitClass = 0;
			unsigned int unitOrigin = 0;
			
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

		struct MinisterProperty
		{
			std::string name;
			unsigned int changeVal = 0;
			int status = 0;
		};

		struct Minister
		{
			std::string name;
			std::vector<std::shared_ptr<MinisterProperty>> properties;
			uintptr_t* propertiesPtr;
		};

		struct Country
		{
			void Init(uintptr_t base);
			void HandleFreeze();
			void HandleUnits();
			void HandleROE();
			void HandleMinisters();
			void RefreshResearch();
			void RefreshLeader();
			void ChangeName(std::string newName, int type = 0);

			uintptr_t base;
			bool real;
			int id;
			int oId;
			int alive;
			int flagOption = 0;
			int leaderOption = 0;
			std::string name;
			std::string colonyName;

			unsigned long originalColor = 0;
			bool hasOwnColor = false;

			int selectedTechnologyId = -1;
			int selectedUnitDesignId = -1;
			
			std::vector<int> defenseMinisterInitiative = { 0, 0, 0 };

			uintptr_t* populationPtr;
			uintptr_t* colorPtr;
			uint8_t* govPtr;
			uint16_t* flagIdPtr;
			uint16_t* leaderIdPtr;
			uint16_t* colonyOwnerPtr;

			uint16_t originalFlagId;
			uint16_t originalLeaderId;
			
			unsigned int lastSurrenderTime = -1;

			std::vector<std::shared_ptr<Minister>> ministers;

			std::shared_ptr<FloatValue> treasury;
			std::shared_ptr<FloatValue> population;
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
			std::shared_ptr<FloatValue> sphereGov;
			std::shared_ptr<FloatValue> sphereCiv;

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

			std::shared_ptr<Base::SRU_Data::Leader> currentLeader;

			int* defconPtrs[2] = {0, 0};
			int defconState = -1;

			//roe
			int roeSpeed = 0;
			int roeRoute = 0;
			int roeInitiative = 0;
			int roeContactOptions = 0;
			int roeLossTolerance = 0;
			int roeOpportunityFire = 0;
			int roeApproach = 0;

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
			uintptr_t fromBase;
			int from;
			int to;
			int lastTime = -1;
		};

		struct GroundType
		{
			uint8_t id = 0;
			std::string name;
		};

		struct Facility
		{
			enum Type
			{
				Industrial,
				Military,
				Urban
			};

			int id;
			bool flag = false;
			std::vector<int> variantIDs;
			Type type;
			std::string name;
		};

		struct AddressHolder
		{
			uintptr_t addr;
			int id;
			int id2;
			std::string name;
		};

		struct Leader {
			int id;
			std::string name;
			std::string prefix;
		};

		struct Hex {
			uintptr_t base;
			int defaultName;
			std::string newName;
		};

		enum ResourceID : int
		{
			Resource_Agriculture,
			Resource_Rubber,
			Resource_Timber,
			Resource_Petroleum,
			Resource_Coal,
			Resource_MetalOre,
			Resource_Uranium,
			Resource_Electricity
		};

		enum PaintMode : int
		{
			Paint_Land,
			Paint_UnitStats,
			Paint_Resource,
			Paint_Infrastructure,
			Paint_Unit,
			Paint_Facility,
			Paint_FacilityDestroy,
			Paint_FacilityDisable,
			Paint_FacilityEnable,
		};

		extern std::vector<std::shared_ptr<UnitDefault>> g_defaultUnitList;
		extern std::vector<std::shared_ptr<Facility>> g_facilityList;
		extern std::vector<std::shared_ptr<Leader>> g_leaderList;
		extern std::vector<std::shared_ptr<Hex>> g_hexNameList;
		extern std::vector<Unit> g_selectedUnitList;
		extern std::vector<Unit> g_unitList;
		extern std::vector<DiplTreaty> g_diplTreatyList;
		extern std::vector<Country> g_countryList;
		extern std::vector<std::shared_ptr<Tech>> g_techList;
		extern std::vector<GroundType> g_groundTypeList;
		
		extern SurrenderEvent surrenderEvents[];
		extern uintptr_t g_nextUnitEntity;
		
		extern uint64_t g_steamId;

		extern int g_ingame;
		extern bool g_autosaving;
		extern bool g_quitting;

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

		extern int g_paintSelectedResource;
		extern int g_paintSelectedResourceAmount;
		
		extern int g_unitSpawnSelectedUnitDesign;
		extern int g_facilitySpawnSelectedFacility;

		extern int g_surrenderEventCount;

		extern int g_unitRefreshMaxTime;
		extern int g_mainRefreshTime;

		extern int g_mapSizeX;

		extern int g_paintMode;
		extern int g_paintBrushSize;
		extern int g_paintStyle;
		extern bool g_paintUnitTargetCountry;
		extern std::vector<int> g_paintUnitModes;
		
		extern int g_unitDesignType;
		extern int g_unitSpawnCount;
		extern int g_unitSpawnXPos;
		extern int g_unitSpawnYPos;
		extern bool g_unitSpawnReserve;

		extern int g_facilitySpawnXPos;
		extern int g_facilitySpawnYPos;
		extern int g_facilitySpawnCount;
		extern bool g_facilitySpawnConstruction;

		extern bool g_newClick;
		extern bool g_addOk;
		extern bool g_shift;
		extern bool g_mapSizeLoaded;
		extern bool g_uiHexSupplySet;
		extern bool g_mouseClicked;
		extern bool g_paintActive;
		extern bool g_paintEnabled;
		extern bool g_paintHexOwner;
		extern bool g_paintHexLoyalty;
		extern bool g_paintHexOnlySelected;

		extern bool g_disco;
		extern bool g_hexSupply;
		extern bool g_productionAdjustment;
		extern bool g_aiColony;
		extern bool g_fastRoad;
		extern bool g_unitSelect;

		extern uint8_t g_currentHexSupply;

		extern Country* clickedCountry;
		extern Country* clickedTargetCountry;
		extern Country* unitSpawnCountry;

		extern uintptr_t* g_clickedHexPtr;
		extern uintptr_t* g_playSpeedPtr;
		extern uint16_t* g_clickedXPtr;
		extern uint16_t* g_clickedYPtr;
		
		extern uintptr_t g_base;

		extern std::string g_sphereNames[];

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
		void LoadLeaders();
		void LoadSettings();
		void SaveSettings();
	}

	namespace Execute
	{
		typedef void(__thiscall* _DiplFunc)(DWORD*, char);
		typedef void(__thiscall* _UnlockTechFunc)(unsigned __int8*, int);
		typedef int(__fastcall* _SpawnUnitFunc)(int, int, int, int, int, int, int, int*, int);
		typedef int(__stdcall* _CreateTransportFuncEvent)(int, int, int, int, int, int);
		typedef int(__thiscall* _CreateFactoryFunc)(int, __int8, int, int, int, float, int, float, int);
		typedef int(__thiscall* _DestroyFactoryFunc)(int, int, int);
		typedef void(__thiscall* _TreatyFunc)(int, int, unsigned int, int, int, int, int, int, int);
		typedef void(__thiscall* _FacilityStatusFunc)(int, unsigned __int8, int, int, int);
		typedef void(__fastcall* _LiberateColonyFunc)(int*);
		//typedef int(__thiscall* _KillCountryFunc)(unsigned __int8*, int, int, int);
		typedef int(__thiscall* _KillCountryFunc)(int*, char, int, int, int, int, int);
		typedef void(__thiscall* _LiberateCountryFunc)(int*, int, int, int, int);
		typedef void(__thiscall* _CreateColonyFunc)(int*, int, int, int, void*);
		typedef int* (__thiscall* _CreateWarEventFunc)(int*, int, char);
		
		extern _DiplFunc diplFunc;
		extern _UnlockTechFunc unlockTechFunc;
		extern _SpawnUnitFunc spawnUnitFunc;
		extern _CreateTransportFuncEvent createTransportFuncEvent;
		extern _CreateFactoryFunc createFactoryFunc;
		extern _DestroyFactoryFunc destroyFactoryFunc;
		extern _TreatyFunc treatyFunc;
		extern _FacilityStatusFunc facilityStatusFunc;
		extern _LiberateColonyFunc liberateColonyFunc;
		extern _KillCountryFunc killCountryFunc;
		extern _KillCountryFunc killCountryFuncTarget;
		extern _LiberateCountryFunc liberateCountryFunc;
		extern _CreateColonyFunc createColonyFunc;
		extern _CreateWarEventFunc createWarEventFunc;

		//int __fastcall KillCountryFuncDetour(int* pThis, char c, int i1, int i2, int i3, int i4, int i5);

		void SetupFunctions();

		void AnnexCountry2(uintptr_t targetAddr, int newOwnerId);
		void AnnexCountry(int from, int to);
		void AnnexAllColonies(Base::SRU_Data::Country* cc);
		void RespawnCountry(int from, int to, int type);
		void RespawnCountryNew(uintptr_t addr, int id, int type);
		void RespawnAllColonies(Base::SRU_Data::Country* cc);
		void SetCountryAIStance(Base::SRU_Data::Country* cc, int newAIStance);
		void UnlockDesign(int to, int design, bool lock);
		void UnlockTech(int to, int tech, bool lock);
		void CreateTransport(int from, int to, int type, int noConstruction);
		void CreateTransport(__int16 fromX, __int16 fromY, __int16 toX, __int16 toY, int country, int type, int noConstruction);
		void CreateFacility(__int16 posX, __int16 posY, int countryOId, int facilityId, float constructionState);
		uintptr_t* GetTileAddr(uint16_t xPos, uint16_t yPos);
		uintptr_t* GetFacilityRoot(uint16_t xPos, uint16_t yPos);
		std::vector<struct Base::SRU_Data::AddressHolder> GetFacilities(uintptr_t* root);
		void DisableFacility(uintptr_t* tileAddr, uintptr_t* rootAddr, std::vector<Base::SRU_Data::AddressHolder> facilities, int index, int disable);
		void DisableAllFacilities(uint16_t xPos, uint16_t yPos, int disable);
		void DestroyFacility(uintptr_t* rootAddr, std::vector<Base::SRU_Data::AddressHolder> facilities, int index);
		void DestroyAllFacilities(uint16_t xPos, uint16_t yPos);
		void SetRelations(int relationType, uintptr_t country, uintptr_t oCountry, int add);
		void SetMapResource(uint8_t* byte, int resource, int level);
		int GetMapResource(uint8_t* byte, int resource);
		void SpawnUnit(int unitDesign, int amount, uintptr_t country, int spread = 1, bool reserve = true, uint16_t xPos = 0, uint16_t yPos = 0);
		void SetCheat(uint8_t cheat);
		int ExecuteTreaty(int diplTreatyIndex, int set = 1);
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
		void DrawCountryMinisters(Base::SRU_Data::Country* cc);
		void DrawCountryROE(Base::SRU_Data::Country* cc);
		void DrawCountryTech(Base::SRU_Data::Country* cc);
		void DrawCountryDesigns(Base::SRU_Data::Country* cc);
		void DrawMap(Base::SRU_Data::Country* cc);
		void DrawFacilities(Base::SRU_Data::Country* cc);
		void DrawUnitSpawn(Base::SRU_Data::Country* cc);
		void DrawUnitModifiers(Base::SRU_Data::Country* cc);
		void DrawUnitSelected(Base::SRU_Data::Country* cc);
		void DrawSpheres(Base::SRU_Data::Country* cc);
		void DrawSettings();
	}

	namespace Hooks
	{
		void Init(bool full);
		void EnableFastRoad(bool enable);
		std::vector<BYTE> GetOrigBytes(void* toHook, int len);
		void FunctionHook(void* toHook, void* targetFunc, int len, int enable);

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
		bool GetValueBool(uintptr_t* addr, int value);
		void SetValueMask(uintptr_t* addr, int offset, int value);
		void SetValueBool(uintptr_t* addr, int value, bool enable);
		unsigned short GetShortPopulationValue(unsigned int newPopulation);
		unsigned int GetFullPopulationValue(unsigned short populationValue);
		std::string FloatToPercent(float f, float max, bool simple = false);
		std::string FloatToPercent2(float f, float max, bool simple = false);
		std::string FloatToPercent3(float f, float max, bool simple = false);
		std::string FloatToPercentCustom(float f, float max, float minClamp, float maxClamp, bool simple = false);
		bool MemCompare(const BYTE* bData, const BYTE* bMask, const char* szMask);
		bool CanReadPtr(void* ptr);
		uintptr_t PointerChain(uintptr_t ptr, std::vector<unsigned int> offsets);
		uintptr_t PatternScan(HANDLE hProcess, uintptr_t start, uintptr_t size, const char* sig, const char* mask);
		void Patch(BYTE* dst, BYTE* src, unsigned int size);
		void Nop(BYTE* dst, unsigned int size);
		void WPM(BYTE* dst, BYTE* src, unsigned int size);
		void RPM(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
		
		void GetSettingsBool(std::string line, std::string token, bool& value);
		void GetSettingsUInt8(std::string line, std::string token, uint8_t& value);
		void GetSettingsInt(std::string line, std::string token, int& value);
		
		std::string SetSettingsBool(std::string token, bool value);
		std::string SetSettingsUInt8(std::string token, uint8_t value);
		std::string SetSettingsInt(std::string token, int value);
		
		std::string StreamToMem(std::string URL, bool https = true);
		std::string SHA256(std::string str);

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