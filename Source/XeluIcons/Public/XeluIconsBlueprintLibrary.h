// Copyright 2022 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XeluIconsInputMetadata.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XeluIconsBlueprintLibrary.generated.h"

class UDataTable;
class UInputAction;
class UInputMappingContext;
class UXeluIconsDeveloperSettings;

/**
 * Main library interface for Xelu Icons plugin
 */
UCLASS()
class XELUICONS_API UXeluIconsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Returns plugin settings from Editor config */
	static const UXeluIconsDeveloperSettings* GetPluginSettings();

	static const UDataTable* GetIconsDatatable();

	UFUNCTION(BlueprintPure, Category = "Xelu Icons")
	static UTexture2D* GetIconTextureForInputAction(UInputAction* InputAction, UInputMappingContext* MappingContext, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard);

	UFUNCTION(BlueprintPure, Category = "Xelu Icons", Meta = (AdvancedDisplay = "bShouldLoadSynchronous"))
	static TSoftObjectPtr<UTexture2D> GetSoftIconTextureForInputAction(UInputAction* InputAction, UInputMappingContext* MappingContext, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard, bool bShouldLoadSynchronous = true);

	UFUNCTION(BlueprintPure, Category = "Xelu Icons")
	static UTexture2D* GetIconTextureForKey(FKey Key, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard);

	UFUNCTION(BlueprintPure, Category = "Xelu Icons")
	static TSoftObjectPtr<UTexture2D> GetSoftIconTextureForKey(FKey Key, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard, bool bShouldLoadSynchronous = true);

	static TSoftObjectPtr<UTexture2D> GetIconTextureInternal(FKey Key, const UDataTable* IconsDatatable, EXeluIconsIconType IconPreference, bool bShouldLoadSynchronous = true);
};
