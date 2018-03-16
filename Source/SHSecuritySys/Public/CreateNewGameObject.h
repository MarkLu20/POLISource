// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CreateNewGameObject.generated.h"

/**
 * 
 */
UCLASS()
class SHSECURITYSYS_API UCreateNewGameObject : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()	
	
	UFUNCTION(BlueprintPure, meta = ( DisplayName = "Create Object From Blueprint", CompactNodeTitle = "Create", Keywords = "new create blueprint"), Category = Game)
	static UObject* NewObjectFromBlueprint(UObject* owner, UClass* UC);
	
};
