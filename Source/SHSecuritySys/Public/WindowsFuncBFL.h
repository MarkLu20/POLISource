// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WindowsFuncBFL.generated.h"

/**
 * 
 */
UCLASS()
class SHSECURITYSYS_API UWindowsFuncBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Open Folder", Keywords = "Open Folder"), Category = "FileDictCollection")
		static void OpenFolder(const FString &_Path);
	
	
};
