// Copyright Epic Games, Inc. All Rights Reserved.

#include "TrackEditorEdMode.h"
#include "TrackEditorEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "ScopedTransaction.h"
#include "EditorModeManager.h"
#include "Engine/Selection.h"
#include "ProjectS21/Grid/CellPoint.h"

IMPLEMENT_HIT_PROXY(HPS21PointProxy, HHitProxy);

const FEditorModeID FTrackEditorEdMode::EM_TrackEditorEdModeId = TEXT("EM_TrackEditorEdMode");

FTrackEditorEdMode::FTrackEditorEdMode()
{

}

FTrackEditorEdMode::~FTrackEditorEdMode()
{

}

void FTrackEditorEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FTrackEditorEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}

	//reset
	currentSelectedTarget = nullptr;
}

void FTrackEditorEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FTrackEditorEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	const FColor normalColor(200, 200, 200);
	const FColor selectedColor(255, 128, 0);

	UWorld* world = GetWorld();
	for (TObjectIterator<ACellPoint> it; it; ++it)
	{
		ACellPoint* actor = (*it);
		if (actor)
		{
			FVector actorLoc = actor->GetActorLocation();
			TArray<ACellPoint*> neighbours;
			actor->maNeighbours.GenerateValueArray(neighbours);
			for (auto i = 0; i < actor->maNeighbours.Num(); ++i)
			{
				ACellPoint* neighbour = neighbours[i];
				if (neighbour == nullptr)
					continue;
				bool bSelected = (actor == currentSelectedTarget);
				const FColor& color = bSelected ? selectedColor : normalColor;
				// set hit proxy and draw
				PDI->SetHitProxy(new HPS21PointProxy(actor, i));
				PDI->DrawPoint(neighbour->GetActorLocation(), color, 25.f, SDPG_Foreground);
				PDI->DrawLine(neighbour->GetActorLocation(), actorLoc, color, SDPG_Foreground);
				PDI->SetHitProxy(NULL);
			}
		}
	}

	FEdMode::Render(View, Viewport, PDI);
}

bool FTrackEditorEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	bool isHandled = false;

	if (HitProxy)
	{
		if (HitProxy->IsA(HPS21PointProxy::StaticGetType()))
		{
			isHandled = true;
			HPS21PointProxy* ps21PointProxy = (HPS21PointProxy*)HitProxy;
			ACellPoint* actor = Cast<ACellPoint>(ps21PointProxy->mpRefObject);
			int32 index = ps21PointProxy->mIndex;
			if (actor && index >= 0 && index < actor->maNeighbours.Num())
			{
				SelectPoint(actor);
			}
		}
	}

	return isHandled;
}

bool FTrackEditorEdMode::UsesToolkits() const
{
	return true;
}

ACellPoint* GetSelectedTargetPointActor()
{
	TArray<UObject*> selectedObjects;
	GEditor->GetSelectedActors()->GetSelectedObjects(selectedObjects);
	if (selectedObjects.Num() == 1)
	{
		return Cast<ACellPoint>(selectedObjects[0]);
	}
	return nullptr;
}

bool ValidConnection(ACellPoint* actor, const FVector& direction)
{
	return (actor->maNeighbours.Find(direction) != nullptr);
}

void FTrackEditorEdMode::FindAndConnectExistingNodes(ACellPoint* actor)
{
	TArray<FVector> directions;
	directions.Add({ 1,0,0 });
	directions.Add({ -1,0,0 });
	directions.Add({ 0,1,0 });
	directions.Add({ 0,-1,0 });

	for (size_t i = 0; i < directions.Num(); i++)
	{
		if (!actor->maNeighbours.Contains(directions[i]))
		{
			for (TObjectIterator<ACellPoint> it; it; ++it)
			{
				ACellPoint* worldActor = (*it);
				if (worldActor && worldActor != actor)
				{
					FVector newLocation = actor->GetActorLocation() + (directions[i] * mCellPointOffset);
					if (worldActor->GetActorLocation().Equals(newLocation))
					{
						actor->maNeighbours.Add(directions[i], worldActor);
						worldActor->maNeighbours.Add(directions[i] * -1.f, actor);
					}
				}
			}
		}
	}
}

void FTrackEditorEdMode::AddPoint(const FVector& nextLocation)
{
	ACellPoint* actor = GetSelectedTargetPointActor();
	if (actor)
	{
		//if (!ValidConnection(actor, nextLocation))
			//return;
		const FScopedTransaction Transaction(FText::FromString("Add Point"));

		const float offset = 200.f;
		FVector newLocation = actor->GetActorLocation() + (nextLocation * offset);
		// add new point, slightly in front of camera
		/*FEditorViewportClient* client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		FVector newPoint = client->GetViewLocation() + client->GetViewRotation().Vector() * 50.f;*/
		actor->Modify();

		//spawn new cell
		FRotator rotation{ 0,0,0 };
		FActorSpawnParameters spawnInfo;
		ACellPoint* newActor = GEditor->GetEditorWorldContext().World()->SpawnActor<ACellPoint>(newLocation, rotation, spawnInfo);
		newActor->Modify();
		//set neighbours
		actor->maNeighbours.Add(nextLocation, newActor);
		newActor->maNeighbours.Add(nextLocation * -1.f, actor);
		FindAndConnectExistingNodes(newActor);
		// auto select this new point
		SelectPoint(newActor);
	}
}

bool FTrackEditorEdMode::CanAddPoint() const
{
	return GetSelectedTargetPointActor() != nullptr;
}

void FTrackEditorEdMode::SelectPoint(AActor* actor)
{
	currentSelectedTarget = actor;

	if (currentSelectedTarget.IsValid())
	{
		GEditor->SelectNone(true, true);
		GEditor->SelectActor(currentSelectedTarget.Get(), true, true);
	}
}

void FTrackEditorEdMode::UpdatePoints()
{
	UWorld* world = GetWorld();
	for (TObjectIterator<ACellPoint> it; it; ++it)
	{
		ACellPoint* actor = (*it);
		if (actor)
		{
			actor->Update();
		}
	}
}




