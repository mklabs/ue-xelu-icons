// Copyright 2021 Mickael Daniel. All Rights Reserved.


#include "InputMappingContext.h"
#include "XeluIconsBlueprintLibrary.h"
#include "XeluIconsDeveloperSettings.h"
#include "XeluIconsLog.h"

BEGIN_DEFINE_SPEC(FXeluIconsSpec, "XeluIcons", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

	UInputAction* InputAction01 = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/XeluIcons/Tests/IA_Xelu_Icons_Test_01.IA_Xelu_Icons_Test_01")));
	UInputAction* InputAction02 = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/XeluIcons/Tests/IA_Xelu_Icons_Test_02.IA_Xelu_Icons_Test_02")));
	UInputAction* InputAction03 = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/XeluIcons/Tests/IA_Xelu_Icons_Test_03.IA_Xelu_Icons_Test_03")));
	UInputAction* InputAction04 = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/XeluIcons/Tests/IA_Xelu_Icons_Test_04.IA_Xelu_Icons_Test_04")));
	UInputAction* InputAction05 = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, TEXT("/XeluIcons/Tests/IA_Xelu_Icons_Test_05.IA_Xelu_Icons_Test_05")));

	UInputMappingContext* Context = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr, TEXT("/XeluIcons/Tests/MC_Xelu_Icons_Test.MC_Xelu_Icons_Test")));

END_DEFINE_SPEC(FXeluIconsSpec)

void FXeluIconsSpec::Define()
{
	Describe("UXeluIconsBlueprintLibrary", [this]()
	{
		BeforeEach([this]()
		{
			// Setup tests
		});

		It("GetPluginSettings()", [this]()
		{
			const UXeluIconsDeveloperSettings* Settings = UXeluIconsBlueprintLibrary::GetPluginSettings();
			TestTrue("Returns developer settings", Settings != nullptr);

			const TSoftObjectPtr<UDataTable> DataTable = Settings->IconsDataTable;
			TestTrue("Datatable is not null", !DataTable.IsNull());
		});

		It("GetIconsDatatable()", [this]()
		{
			const UDataTable* DataTable = UXeluIconsBlueprintLibrary::GetIconsDatatable();
			TestTrue("Resolved Datatable is valid", DataTable != nullptr);
		});

		It("GetSoftIconTextureForInputAction()", [this]()
		{
			TMap<UInputAction*, FString> InputsToPath;
			InputsToPath.Add(InputAction01, "/XeluIcons/Icons/Keyboard_Icons/Dark/Mouse_Left_Key_Dark.Mouse_Left_Key_Dark");
			InputsToPath.Add(InputAction02, "/XeluIcons/Icons/Keyboard_Icons/Dark/Mouse_Right_Key_Dark.Mouse_Right_Key_Dark");
			InputsToPath.Add(InputAction03, "/XeluIcons/Icons/Keyboard_Icons/Dark/Space_Key_Dark.Space_Key_Dark");
			InputsToPath.Add(InputAction04, "/XeluIcons/Icons/Keyboard_Icons/Dark/U_Key_Dark.U_Key_Dark");
			InputsToPath.Add(InputAction05, "/XeluIcons/Icons/Keyboard_Icons/Dark/E_Key_Dark.E_Key_Dark");

			for (const auto Entry : InputsToPath)
			{
				FString ExpectedPath = Entry.Value;
				UInputAction* InputAction = Entry.Key;

				const TSoftObjectPtr<UTexture2D> Texture = UXeluIconsBlueprintLibrary::GetSoftIconTextureForInputAction(InputAction, Context, EXeluIconsIconType::Keyboard, false);

				TestTrue(FString::Printf(TEXT("Texture for %s is valid"), *GetNameSafe(InputAction)), Texture.IsNull() == false);
				TestEqual(FString::Printf(TEXT("Texture path for %s is matching"), *GetNameSafe(InputAction)), Texture.ToSoftObjectPath().ToString(), ExpectedPath);
			}

			InputsToPath.Empty();
			InputsToPath.Add(InputAction01, "/XeluIcons/Icons/PS4_Icons/PS4_Square.PS4_Square");
			InputsToPath.Add(InputAction02, "/XeluIcons/Icons/PS4_Icons/PS4_Triangle.PS4_Triangle");
			InputsToPath.Add(InputAction03, "/XeluIcons/Icons/PS4_Icons/PS4_Circle.PS4_Circle");
			InputsToPath.Add(InputAction04, "/XeluIcons/Icons/PS4_Icons/PS4_Cross.PS4_Cross");
			InputsToPath.Add(InputAction05, "/XeluIcons/Icons/PS4_Icons/PS4_R1.PS4_R1");

			for (const auto Entry : InputsToPath)
			{
				FString ExpectedPath = Entry.Value;
				UInputAction* InputAction = Entry.Key;

				const TSoftObjectPtr<UTexture2D> Texture = UXeluIconsBlueprintLibrary::GetSoftIconTextureForInputAction(InputAction, Context, EXeluIconsIconType::PS4, false);

				TestTrue(FString::Printf(TEXT("Texture for %s is valid"), *GetNameSafe(InputAction)), Texture.IsNull() == false);
				TestEqual(FString::Printf(TEXT("Texture path for %s is matching"), *GetNameSafe(InputAction)), Texture.ToSoftObjectPath().ToString(), ExpectedPath);
			}
		});

		It("GetIconTextureForInputAction()", [this]()
		{
			UTexture2D* Texture = UXeluIconsBlueprintLibrary::GetIconTextureForInputAction(InputAction01, Context);

			TestTrue(FString::Printf(TEXT("Texture for %s is valid"), *GetNameSafe(InputAction01)), Texture != nullptr);
			TestEqual(FString::Printf(TEXT("Texture path for %s is matching"), *GetNameSafe(InputAction01)), *GetNameSafe(Texture), "Mouse_Left_Key_Dark");
		});

		Describe("GetIconTextureForKey()", [this]()
		{
			It("Should return valid textures", [this]()
			{
				FKey Key = EKeys::LeftMouseButton;

				UTexture2D* Texture = UXeluIconsBlueprintLibrary::GetIconTextureForKey(Key);
				TestTrue(FString::Printf(TEXT("Texture for %s key is valid"), *Key.GetDisplayName().ToString()), Texture != nullptr);
				TestEqual(FString::Printf(TEXT("Texture path for %s key is matching"), *Key.GetDisplayName().ToString()), *GetNameSafe(Texture), "Mouse_Left_Key_Dark");

				Key = EKeys::Gamepad_FaceButton_Left;

				Texture = UXeluIconsBlueprintLibrary::GetIconTextureForKey(Key, EXeluIconsIconType::PS4);
				TestTrue(FString::Printf(TEXT("Texture for %s key is valid"), *Key.GetDisplayName().ToString()), Texture != nullptr);
				TestEqual(FString::Printf(TEXT("Texture path for %s key is matching"), *Key.GetDisplayName().ToString()), *GetNameSafe(Texture), "PS4_Square");

				Texture = UXeluIconsBlueprintLibrary::GetIconTextureForKey(Key, EXeluIconsIconType::PS5);
				TestTrue(FString::Printf(TEXT("Texture for %s key is valid"), *Key.GetDisplayName().ToString()), Texture != nullptr);
				TestEqual(FString::Printf(TEXT("Texture path for %s key is matching"), *Key.GetDisplayName().ToString()), *GetNameSafe(Texture), "PS5_Square");

				Texture = UXeluIconsBlueprintLibrary::GetIconTextureForKey(Key, EXeluIconsIconType::XboxOne);
				TestTrue(FString::Printf(TEXT("Texture for %s key is valid"), *Key.GetDisplayName().ToString()), Texture != nullptr);
				TestEqual(FString::Printf(TEXT("Texture path for %s key is matching"), *Key.GetDisplayName().ToString()), *GetNameSafe(Texture), "XboxOne_X");

				Texture = UXeluIconsBlueprintLibrary::GetIconTextureForKey(Key, EXeluIconsIconType::XboxSeries);
				TestTrue(FString::Printf(TEXT("Texture for %s key is valid"), *Key.GetDisplayName().ToString()), Texture != nullptr);
				TestEqual(FString::Printf(TEXT("Texture path for %s key is matching"), *Key.GetDisplayName().ToString()), *GetNameSafe(Texture), "XboxSeriesX_X");
			});

			It("Should fallback to default IconPreference when a wrong one is provided", [this]()
			{
				FKey Key = EKeys::LeftMouseButton;

				UTexture2D* Texture = UXeluIconsBlueprintLibrary::GetIconTextureForKey(Key, EXeluIconsIconType::XboxOne);
				TestTrue(FString::Printf(TEXT("Texture for %s key is valid"), *Key.GetDisplayName().ToString()), Texture != nullptr);
				TestEqual(FString::Printf(TEXT("Texture path for %s key is matching"), *Key.GetDisplayName().ToString()), *GetNameSafe(Texture), "Mouse_Left_Key_Dark");

				Key = EKeys::Gamepad_FaceButton_Left;

				Texture = UXeluIconsBlueprintLibrary::GetIconTextureForKey(Key, EXeluIconsIconType::Keyboard);
				TestTrue(FString::Printf(TEXT("Texture for %s key is valid"), *Key.GetDisplayName().ToString()), Texture != nullptr);
				TestEqual(FString::Printf(TEXT("Texture path for %s key is matching"), *Key.GetDisplayName().ToString()), *GetNameSafe(Texture), "XboxOne_X");
			});
		});

		AfterEach([this]()
		{
			// Teardown
		});
	});
}
