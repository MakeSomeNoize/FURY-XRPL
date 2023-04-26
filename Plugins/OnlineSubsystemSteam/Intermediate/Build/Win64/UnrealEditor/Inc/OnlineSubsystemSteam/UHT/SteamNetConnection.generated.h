// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "SteamNetConnection.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ONLINESUBSYSTEMSTEAM_SteamNetConnection_generated_h
#error "SteamNetConnection.generated.h already included, missing '#pragma once' in SteamNetConnection.h"
#endif
#define ONLINESUBSYSTEMSTEAM_SteamNetConnection_generated_h

#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_SPARSE_DATA
#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_RPC_WRAPPERS
#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_ACCESSORS
#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSteamNetConnection(); \
	friend struct Z_Construct_UClass_USteamNetConnection_Statics; \
public: \
	DECLARE_CLASS(USteamNetConnection, UIpConnection, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/OnlineSubsystemSteam"), NO_API) \
	DECLARE_SERIALIZER(USteamNetConnection)


#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_INCLASS \
private: \
	static void StaticRegisterNativesUSteamNetConnection(); \
	friend struct Z_Construct_UClass_USteamNetConnection_Statics; \
public: \
	DECLARE_CLASS(USteamNetConnection, UIpConnection, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/OnlineSubsystemSteam"), NO_API) \
	DECLARE_SERIALIZER(USteamNetConnection)


#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USteamNetConnection(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USteamNetConnection) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USteamNetConnection); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USteamNetConnection); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USteamNetConnection(USteamNetConnection&&); \
	NO_API USteamNetConnection(const USteamNetConnection&); \
public: \
	NO_API virtual ~USteamNetConnection();


#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USteamNetConnection(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USteamNetConnection(USteamNetConnection&&); \
	NO_API USteamNetConnection(const USteamNetConnection&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USteamNetConnection); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USteamNetConnection); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USteamNetConnection) \
	NO_API virtual ~USteamNetConnection();


#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_14_PROLOG
#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_SPARSE_DATA \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_RPC_WRAPPERS \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_ACCESSORS \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_INCLASS \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_SPARSE_DATA \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_ACCESSORS \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_INCLASS_NO_PURE_DECLS \
	FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h_17_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class SteamNetConnection."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ONLINESUBSYSTEMSTEAM_API UClass* StaticClass<class USteamNetConnection>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Games_Fury_Plugins_OnlineSubsystemSteam_Source_Classes_SteamNetConnection_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
