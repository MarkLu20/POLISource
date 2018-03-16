// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/MeshComponent.h"
#include "MapTileResource.h"
#include "TileMeshComponent.generated.h"

class FPrimitiveSceneProxy;

/**
*
*/
UCLASS(hidecategories = (Object, LOD, Physics, Collision), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class SHSECURITYSYS_API UTileMeshComponent : public UMeshComponent
{
	GENERATED_UCLASS_BODY()

		/** Set the geometry to use on this triangle mesh */
		UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		void Initialize(int X, int Y);

	virtual int32 GetMaterialIndex(FName MaterialSlotName) const override;
	virtual TArray<FName> GetMaterialSlotNames() const override;
	virtual bool IsMaterialSlotNameValid(FName MaterialSlotName) const override;

	FVector ComputeLocalPositionImpl(double longitude, double latitude);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		FVector ComputeLocalPosition(const FString& longitude, const FString& latitude);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		FVector ComputeWorldPosition(const FString& longitude, const FString& latitude);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		FVector ComputeLocalPositionGCJ(const FString& longitude, const FString& latitude);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		FVector ComputeWorldPositionGCJ(const FString& longitude, const FString& latitude);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		FVector ComputeLocalPositionMars(const FString& longitude, const FString& latitude);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		FVector ComputeWorldPositionMars(const FString& longitude, const FString& latitude);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		TArray<int> FindNearRoutePoints(FVector InPoint, float InRange);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		FVector GetRoutePointWorldPosition(int id);

	UFUNCTION(BlueprintCallable, Category = "Components|TileMesh")
		TArray<int> GetRoutePointFollowPath(int id);

private:
	//~ Begin UPrimitiveComponent Interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin UMeshComponent Interface.
	virtual int32 GetNumMaterials() const override;
	//~ End UMeshComponent Interface.

	//~ Begin USceneComponent Interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ Begin USceneComponent Interface.

	UMaterialInterface* GetMaterialBySlot(FName SlotName) const;

	friend class FTileMeshSceneProxy;

private:
	int X, Y;
	int BaseX, BaseY;

	FRouteMap mRouteMap;
};
