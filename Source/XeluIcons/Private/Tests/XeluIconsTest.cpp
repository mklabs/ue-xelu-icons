// Copyright 2021 Mickael Daniel. All Rights Reserved.


#include "XeluIconsBlueprintLibrary.h"
#include "XeluIconsDeveloperSettings.h"

BEGIN_DEFINE_SPEC(FXeluIconsSpec, "XeluIcons", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

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

		xIt("GetSoftIconTextureForInputAction()", [this]()
		{
			// TODO ...
		});

		xIt("GetIconTextureForInputAction()", [this]()
		{
			// TODO ...
		});

		AfterEach([this]()
		{
			// Teardown
		});
	});
}

