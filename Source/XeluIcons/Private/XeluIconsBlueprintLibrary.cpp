// Copyright 2022 Mickael Daniel. All Rights Reserved.


#include "XeluIconsBlueprintLibrary.h"

#include "InputMappingContext.h"
#include "XeluIconsDeveloperSettings.h"
#include "XeluIconsInputMetadata.h"
#include "XeluIconsLog.h"

const UXeluIconsDeveloperSettings* UXeluIconsBlueprintLibrary::GetPluginSettings()
{
	const UXeluIconsDeveloperSettings* Settings = GetDefault<UXeluIconsDeveloperSettings>();
	check(Settings);
	return Settings;
}

const UDataTable* UXeluIconsBlueprintLibrary::GetIconsDatatable()
{
	const TSoftObjectPtr<UDataTable> DataTable = GetPluginSettings()->IconsDataTable;
	if (!DataTable.IsValid())
	{
		DataTable.LoadSynchronous();
	}

	return DataTable.Get();
}

UTexture2D* UXeluIconsBlueprintLibrary::GetIconTextureForInputAction(UInputAction* InputAction, UInputMappingContext* MappingContext, const EXeluIconsIconType IconPreference)
{
	if (!InputAction)
	{
		XI_LOG(Error, TEXT("GetIconForInputAction: InputAction is invalid, please prodive one."))
		return nullptr;
	}

	if (!MappingContext)
	{
		XI_LOG(Error, TEXT("GetIconForInputAction: MappingContext is invalid, please prodive one."))
		return nullptr;
	}

	const TSoftObjectPtr<UTexture2D> Icon = GetSoftIconTextureForInputAction(InputAction, MappingContext, IconPreference);
	return Icon.IsValid() ? Icon.Get() : nullptr;
}

TSoftObjectPtr<UTexture2D> UXeluIconsBlueprintLibrary::GetSoftIconTextureForInputAction(UInputAction* InputAction, UInputMappingContext* MappingContext, const EXeluIconsIconType IconPreference, const bool bShouldLoadSynchronous)
{
	TSoftObjectPtr<UTexture2D> MatchingIcon = nullptr;

	if (!InputAction)
	{
		XI_LOG(Error, TEXT("GetSoftIconTextureForInputAction: InputAction is invalid, please prodive one."))
		return MatchingIcon;
	}

	if (!MappingContext)
	{
		XI_LOG(Error, TEXT("GetSoftIconTextureForInputAction: MappingContext is invalid, please prodive one."))
		return MatchingIcon;
	}

	const UDataTable* IconsDatatable = GetIconsDatatable();

	// Search for the first matching mappings in context
	const TArray<FEnhancedActionKeyMapping> Mappings = MappingContext->GetMappings();
	TArray<FEnhancedActionKeyMapping> KeyMappings = Mappings.FilterByPredicate([&InputAction](const FEnhancedActionKeyMapping& Other)
	{
		return Other.Action == InputAction;
	});

	if (KeyMappings.Num() == 0)
	{
		XI_LOG(Error, TEXT("GetSoftIconTextureForInputAction: MappingContext '%s' doesn't define any mapping for '%s' Input Action."), *GetNameSafe(MappingContext), *GetNameSafe(InputAction))
		return MatchingIcon;
	}

	// Iterate over mappings found for the input action and return the first icon matching icon preference
	for (const FEnhancedActionKeyMapping KeyMapping : KeyMappings)
	{
		TSoftObjectPtr<UTexture2D> FoundIcon = GetIconTextureInternal(KeyMapping.Key, IconsDatatable, IconPreference, bShouldLoadSynchronous);
		if (!FoundIcon.IsNull())
		{
			MatchingIcon = FoundIcon;
			break;
		}
	}

	return MatchingIcon;
}

UTexture2D* UXeluIconsBlueprintLibrary::GetIconTextureForKey(const FKey Key, const EXeluIconsIconType IconPreference)
{
	const TSoftObjectPtr<UTexture2D> Icon = GetSoftIconTextureForKey(Key, IconPreference);
	return Icon.IsValid() ? Icon.Get() : nullptr;
}

TSoftObjectPtr<UTexture2D> UXeluIconsBlueprintLibrary::GetSoftIconTextureForKey(const FKey Key, EXeluIconsIconType IconPreference, const bool bShouldLoadSynchronous)
{
	const UDataTable* IconsDatatable = GetIconsDatatable();

	// Check if passed in key is a keyboard / mouse and force icon preference to keyboard if it is
	const bool bIsGamepadKey = Key.IsGamepadKey();
	if (!bIsGamepadKey)
	{
		IconPreference = EXeluIconsIconType::Keyboard;
	}

	const TSoftObjectPtr<UTexture2D> Texture2D = GetIconTextureInternal(Key, IconsDatatable, IconPreference, bShouldLoadSynchronous);
	if (Texture2D.IsNull())
	{
		XI_LOG(Error, TEXT("GetSoftIconTextureForKey: Texture2D soft pointer is null and can never point to a live UObject"))
		return nullptr;
	}

	return Texture2D;
}

TSoftObjectPtr<UTexture2D> UXeluIconsBlueprintLibrary::GetIconTextureInternal(const FKey Key, const UDataTable* IconsDatatable, const EXeluIconsIconType IconPreference, const bool bShouldLoadSynchronous)
{
	TSoftObjectPtr<UTexture2D> MatchingIcon = nullptr;

	const bool bIsGamepadKey = Key.IsGamepadKey();

	const FString IconStr = UEnum::GetValueAsString(IconPreference).Replace(TEXT("EXeluIconsIconType::"), TEXT(""));
	const FString Context = FString(TEXT("UXeluIconsBlueprintLibrary::LookupInputsTable"));

	const FString RowNameStr = FString::Printf(TEXT("%s.%s"), *IconStr, *Key.ToString());
	FXeluIconsInputsMetaData* InputMetaData = IconsDatatable->FindRow<FXeluIconsInputsMetaData>(FName(*RowNameStr), Context, false);
	if (!InputMetaData)
	{
		return MatchingIcon;
	}

	if (bShouldLoadSynchronous)
	{
		UTexture2D* Icon = InputMetaData->Icon.LoadSynchronous();
		if (!Icon)
		{
			return MatchingIcon;
		}
	}

	if ((IconPreference == EXeluIconsIconType::Keyboard && !bIsGamepadKey) || (IconPreference != EXeluIconsIconType::Keyboard && bIsGamepadKey))
	{
		MatchingIcon = InputMetaData->Icon;
	}

	return MatchingIcon;
}
