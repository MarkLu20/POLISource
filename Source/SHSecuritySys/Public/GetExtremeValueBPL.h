// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GetExtremeValueBPL.generated.h"

/**
 * 
 */
UCLASS()
class SHSECURITYSYS_API UGetExtremeValueBPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
		UFUNCTION(BlueprintCallable, Category = "ExtremeValue")
		static void GetArrayAverage(const TArray<float>& datalist, float &average);
	
	
};
