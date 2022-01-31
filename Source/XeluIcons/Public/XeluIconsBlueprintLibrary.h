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

	/** Returns Icons Datatable mapping from plugin settings */
	static const UDataTable* GetIconsDatatable();

	/** Returns matching icon texture given the passed in Input Action / Mapping Context, if there is a mapping defined for this configuration */
	UFUNCTION(BlueprintPure, Category = "Xelu Icons")
	static UTexture2D* GetIconTextureForInputAction(UInputAction* InputAction, UInputMappingContext* MappingContext, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard);

	/** Returns matching icon texture as soft object pointer given the passed in Input Action / Mapping Context, if there is a mapping defined for this configuration */
	UFUNCTION(BlueprintPure, Category = "Xelu Icons", Meta = (AdvancedDisplay = "bShouldLoadSynchronous"))
	static TSoftObjectPtr<UTexture2D> GetSoftIconTextureForInputAction(UInputAction* InputAction, UInputMappingContext* MappingContext, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard, bool bShouldLoadSynchronous = true);

	/** Returns matching icon texture for the given key. IconPreference is only relevant for gamepad keys */
	UFUNCTION(BlueprintPure, Category = "Xelu Icons")
	static UTexture2D* GetIconTextureForKey(FKey Key, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard);

	/** Returns matching icon texture as soft object pointer for the given key. IconPreference is only relevant for gamepad keys */
	UFUNCTION(BlueprintPure, Category = "Xelu Icons")
	static TSoftObjectPtr<UTexture2D> GetSoftIconTextureForKey(FKey Key, EXeluIconsIconType IconPreference = EXeluIconsIconType::Keyboard, bool bShouldLoadSynchronous = true);

	/** Main lookup helper to return the matching icon texture for the passed in key */
	static TSoftObjectPtr<UTexture2D> GetIconTextureInternal(FKey Key, const UDataTable* IconsDatatable, EXeluIconsIconType IconPreference, bool bShouldLoadSynchronous = true);
};
