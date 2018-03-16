// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridLayer.h"
#include "GridPalette.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SHSECURITYSYS_API UGridPalette : public UObject
{
	GENERATED_BODY()

private:
	struct FPixelData {
		UINT8 B;
		UINT8 G;
		UINT8 R;
		UINT8 A;
	};

	struct FGridLayerInfo {
		FLinearColor Color;
	};


	TMap<UGridLayer*, FGridLayerInfo> Layers;
	int DirtyRect_MinX;
	int DirtyRect_MinY;
	int DirtyRect_MaxX;
	int DirtyRect_MaxY;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blueprints)
		class UTexture2D* Texture;

public:
	UFUNCTION(BlueprintCallable, Category = Blueprints)
		bool AddLayer(UGridLayer* layer, FLinearColor color);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		bool RemoveLayer(UGridLayer* layer);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void RemoveAllLayers();

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void UpdateTexture();

public:
	void Initialize();

private:
	UFUNCTION()
		void OnDirtyLayer(UGridLayer* layer, int min_x, int min_y, int max_x, int max_y);
};
