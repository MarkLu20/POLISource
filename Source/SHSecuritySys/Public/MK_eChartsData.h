// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MK_eChartsData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SHSECURITYSYS_API UMK_eChartsData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Blueprints)
	TArray<FVector2D> Curve_Datas;
};
