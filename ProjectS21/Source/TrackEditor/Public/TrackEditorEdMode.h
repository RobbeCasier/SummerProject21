// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "HitProxies.h"

struct HPS21PointProxy : public HHitProxy
{
	DECLARE_HIT_PROXY();

	HPS21PointProxy(UObject* inRefObject, int inIndex)
		: HHitProxy(HPP_UI)
		, mpRefObject(inRefObject)
		, mIndex(inIndex)
	{}

	UObject* mpRefObject;
	int mIndex;
};

class UCellComponent;

class FTrackEditorEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_TrackEditorEdModeId;
public:
	FTrackEditorEdMode();
	virtual ~FTrackEditorEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface
	void AddPoint(const FVector& nextLocation);
	bool CanAddPoint() const;
	void SelectPoint(AActor* actor);
	void UpdatePoints();

	TWeakObjectPtr<AActor> currentSelectedTarget;
	int currentSelectedIndex = -1;

};
