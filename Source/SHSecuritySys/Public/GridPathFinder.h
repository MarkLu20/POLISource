// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridPathFinder.generated.h"

/**
 * 
 */
UCLASS()
class SHSECURITYSYS_API UGridPathFinder : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Find Path", Keywords = "Find Path"), Category = "Grid Path Finder")
		static TArray<FIntPoint> FindPath(class UGridLayer* layer, FIntPoint startPoint, float range = 20);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Random Target", Keywords = "Random Target"), Category = "Grid Path Finder")
		static bool RandomTarget(class UGridLayer* layer, FIntPoint& target);
	
	
};
