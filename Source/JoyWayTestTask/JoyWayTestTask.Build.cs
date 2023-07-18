// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JoyWayTestTask : ModuleRules
{
	public JoyWayTestTask(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

        PublicIncludePaths.AddRange(new string[] { "JoyWayTestTask", "JoyWayTestTask/Core", "JoyWayTestTask/Actors", "JoyWayTestTask/Inventory" });
    }
}
