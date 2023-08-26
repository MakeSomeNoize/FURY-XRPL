// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "SteamNetDriver.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ONLINESUBSYSTEMSTEAM_SteamNetDriver_generated_h
#error "SteamNetDriver.generated.h already included, missing '#pragma once' in SteamNetDriver.h"
#endif
#define ONLINESUBSYSTEMSTEAM_SteamNetDriver_generated_h

#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_SPARSE_DATA
#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_RPC_WRAPPERS
#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_ACCESSORS
#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSteamNetDriver(); \
	friend struct Z_Construct_UClass_USteamNetDriver_Statics; \
public: \
	DECLARE_CLASS(USteamNetDriver, UIpNetDriver, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/OnlineSubsystemSteam"), NO_API) \
	DECLARE_SERIALIZER(USteamNetDriver)


#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_INCLASS \
private: \
	static void StaticRegisterNativesUSteamNetDriver(); \
	friend struct Z_Construct_UClass_USteamNetDriver_Statics; \
public: \
	DECLARE_CLASS(USteamNetDriver, UIpNetDriver, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/OnlineSubsystemSteam"), NO_API) \
	DECLARE_SERIALIZER(USteamNetDriver)


#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USteamNetDriver(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USteamNetDriver) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USteamNetDriver); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USteamNetDriver); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USteamNetDriver(USteamNetDriver&&); \
	NO_API USteamNetDriver(const USteamNetDriver&); \
public: \
	NO_API virtual ~USteamNetDriver();


#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USteamNetDriver(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USteamNetDriver(USteamNetDriver&&); \
	NO_API USteamNetDriver(const USteamNetDriver&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USteamNetDriver); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USteamNetDriver); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USteamNetDriver) \
	NO_API virtual ~USteamNetDriver();


#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_17_PROLOG
#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_SPARSE_DATA \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_RPC_WRAPPERS \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_ACCESSORS \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_INCLASS \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_SPARSE_DATA \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_ACCESSORS \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_INCLASS_NO_PURE_DECLS \
	FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h_20_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class SteamNetDriver."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ONLINESUBSYSTEMSTEAM_API UClass* StaticClass<class USteamNetDriver>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE5_Projects_FuryForDev_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetDriver_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
