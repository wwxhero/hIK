// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class tester_unreal_VR : ModuleRules
{
	public tester_unreal_VR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				//"$(EIGEN)\\",
				"$(LIB_VRLOG_INC)\\"
			}
			);

		PublicAdditionalLibraries.AddRange(
			new string [] {
				"$(LIB_HIK_BIN)\\lib_VRLog.lib"
			}
			);
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "hIK" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features

		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		if (Target.Configuration == UnrealTargetConfiguration.DebugGame)
		{
			//OutCPPEnvironmentConfiguration.Definitions.Add("_DEBUG=1");
			PublicDefinitions.Add("_DEBUG");
			// PublicDefinitions.Add("LEAK_CHECK");
		}
	}
}
