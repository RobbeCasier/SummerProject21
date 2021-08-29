// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackEditor.h"
#include "TrackEditorEdMode.h"
#include "Logging.h"

#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FTrackEditor"

void FTrackEditor::StartupModule()
{
	FEditorModeRegistry::Get().RegisterMode<FTrackEditorEdMode>(FTrackEditorEdMode::EM_TrackEditorEdModeId, LOCTEXT("TrackEditorEdModeName", "TrackEditorEdMode"), FSlateIcon(), true);
}

void FTrackEditor::ShutdownModule()
{
	FEditorModeRegistry::Get().UnregisterMode(FTrackEditorEdMode::EM_TrackEditorEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTrackEditor, TrackEditor);