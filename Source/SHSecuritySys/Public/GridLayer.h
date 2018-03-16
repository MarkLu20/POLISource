// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DelegateCombinations.h"
#include "GridLayer.generated.h"


class UGridLayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FGridLayerDirty, UGridLayer*, layer, int, sx, int, sy, int, ex, int, ey);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SHSECURITYSYS_API UGridLayer : public UObject
{
	GENERATED_BODY()

private:
	int ValidRect_MinX;
	int ValidRect_MinY;
	int ValidRect_MaxX;
	int ValidRect_MaxY;
	bool ValidRect_Dirty;

	TArray<int32> ValidNodes;
	bool ValidNode_Dirty;

public:
	TBitArray<> Bits;

public:
	UPROPERTY(BlueprintAssignable, Transient, Category = Blueprints)
		FGridLayerDirty OnDirty;
public:
	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void PaintRect(int x, int y, int brush_size, bool value);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void PaintCircle(int x, int y, int brush_size, bool value);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void GetSize(int& width, int& height);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		bool GetValidRect(int& min_x, int& min_y, int& max_x, int& max_y);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void Save(FString path);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void Load(FString path);

public:
	void Initialize();

	const TArray<int32>& GetValidNodes();
	FIntPoint IndexToPoint(int32 index);
};
