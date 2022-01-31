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

TSoftObjectPtr<UTexture2D> UXeluIconsBlueprintLibrary::GetSoftIconTextureForInputAction(UInputAction* InputAction, UInputMappingContext* MappingContext, const EXeluIconsIconType IconPreference, bool bShouldLoadSynchronous)
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

	// Build up the enum value string (useful later on to build row name str)
	const FString IconStr = UEnum::GetValueAsString(IconPreference).Replace(TEXT("EXeluIconsIconType::"), TEXT(""));

	const FString Context = FString(TEXT("UXeluIconsBlueprintLibrary::LookupInputsTable"));

	// Iterate over mappings found for the input action and return the first icon matching icon preference
	for (const FEnhancedActionKeyMapping KeyMapping : KeyMappings)
	{
		FKey Key = KeyMapping.Key;
		const bool bIsGamepadKey = Key.IsGamepadKey();

		const FString RowNameStr = FString::Printf(TEXT("%s.%s"), *IconStr, *Key.ToString());
		FXeluIconsInputsMetaData* InputMetaData = IconsDatatable->FindRow<FXeluIconsInputsMetaData>(FName(*RowNameStr), Context, false);
		if (!InputMetaData)
		{
			continue;
		}

		if (bShouldLoadSynchronous)
		{
			UTexture2D* Icon = InputMetaData->Icon.LoadSynchronous();
			if (!Icon)
			{
				continue;
			}
		}

		if ((IconPreference == EXeluIconsIconType::Keyboard && !bIsGamepadKey) || (IconPreference != EXeluIconsIconType::Keyboard && bIsGamepadKey))
		{
			MatchingIcon = InputMetaData->Icon;
			break;
		}
	}

	return MatchingIcon;
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
