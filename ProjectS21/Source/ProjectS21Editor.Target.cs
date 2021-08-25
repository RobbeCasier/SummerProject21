// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectS21EditorTarget : TargetRules
{
	public ProjectS21EditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ProjectS21" } );
		ExtraModuleNames.AddRange(new string[] { "ProjectS21Editor" });
	}
}
