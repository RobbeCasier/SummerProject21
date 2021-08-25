// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectS21Editor.h"
#include "Logging.h"

#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FProjectS21Editor"

void FProjectS21Editor::StartupModule()
{
}

void FProjectS21Editor::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProjectS21Editor, ProjectS21Editor);