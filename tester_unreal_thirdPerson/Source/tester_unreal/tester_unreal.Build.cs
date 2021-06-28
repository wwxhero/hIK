// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class tester_unreal : ModuleRules
{
	public tester_unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
