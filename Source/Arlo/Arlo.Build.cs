// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Arlo : ModuleRules
{
	public Arlo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GMCCore" });
	}
}
