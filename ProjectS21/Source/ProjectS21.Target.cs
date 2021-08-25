// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectS21Target : TargetRules
{
	public ProjectS21Target( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ProjectS21" } );
		//ExtraModuleNames.AddRange( new string[] { "ProjectS21Editor" } );
	}
}
