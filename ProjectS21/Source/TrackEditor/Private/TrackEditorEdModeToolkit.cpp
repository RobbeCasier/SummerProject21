// Copyright Epic Games, Inc. All Rights Reserved.

#include "TrackEditorEdModeToolkit.h"
#include "TrackEditorEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "ProjectS21/Grid/CellPoint.h"

#define LOCTEXT_NAMESPACE "FTrackEditorEdModeToolkit"

FTrackEditorEdModeToolkit::FTrackEditorEdModeToolkit()
{
}

void FTrackEditorEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	const float factor = 200.f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Editor for making a simple node graph"))
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(2,0,0,0)
					.VAlign(VAlign_Center)
					[
						SNew(SButton)
						.Text(FText::FromString("New"))
						.OnClicked(this, &FTrackEditorEdModeToolkit::SpawnPoint)
						.IsEnabled(this, &FTrackEditorEdModeToolkit::CanSpawnPoint)
					]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.Text(FText::FromString("Add +X"))
					.OnClicked(this, &FTrackEditorEdModeToolkit::OnAddPoint, FVector( factor, 0, 0 ))
					.IsEnabled(this, &FTrackEditorEdModeToolkit::CanAddpoint)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SButton)
						.Text(FText::FromString("Add -Y"))
				.OnClicked(this, &FTrackEditorEdModeToolkit::OnAddPoint, FVector(0, -factor, 0))
				.IsEnabled(this, &FTrackEditorEdModeToolkit::CanAddpoint)
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString("Add +Y"))
						.OnClicked(this, &FTrackEditorEdModeToolkit::OnAddPoint, FVector(0, factor, 0))
						.IsEnabled(this, &FTrackEditorEdModeToolkit::CanAddpoint)
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.Text(FText::FromString("Add -X"))
				.OnClicked(this, &FTrackEditorEdModeToolkit::OnAddPoint, FVector(-factor, 0, 0))
				.IsEnabled(this, &FTrackEditorEdModeToolkit::CanAddpoint)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.Text(FText::FromString("Update"))
				.OnClicked(this, &FTrackEditorEdModeToolkit::UpdatePoints)
				]

		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FTrackEditorEdModeToolkit::GetToolkitFName() const
{
	return FName("TrackEditorEdMode");
}

FText FTrackEditorEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("TrackEditorEdModeToolkit", "DisplayName", "TrackEditorEdMode Tool");
}

class FEdMode* FTrackEditorEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FTrackEditorEdMode::EM_TrackEditorEdModeId);
}

FReply FTrackEditorEdModeToolkit::SpawnPoint()
{
	FVector newPoint{ 0,0,0 };
	FRotator rotation{ 0,0,0 };
	FActorSpawnParameters spawnInfo;
	spawnInfo.Name = "BaseCellPoint";
	GEditor->GetEditorWorldContext().World()->SpawnActor<ACellPoint>(newPoint, rotation, spawnInfo);
	return FReply::Handled();
}

bool FTrackEditorEdModeToolkit::CanSpawnPoint() const
{
	return !((FTrackEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FTrackEditorEdMode::EM_TrackEditorEdModeId))->CanAddPoint();
}

FReply FTrackEditorEdModeToolkit::OnAddPoint(FVector nextLocation)
{
	((FTrackEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FTrackEditorEdMode::EM_TrackEditorEdModeId))->AddPoint(nextLocation);
	return FReply::Handled();
}

bool FTrackEditorEdModeToolkit::CanAddpoint() const
{
	return ((FTrackEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FTrackEditorEdMode::EM_TrackEditorEdModeId))->CanAddPoint();
}

FReply FTrackEditorEdModeToolkit::UpdatePoints()
{
	((FTrackEditorEdMode*)GLevelEditorModeTools().GetActiveMode(FTrackEditorEdMode::EM_TrackEditorEdModeId))->UpdatePoints();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
