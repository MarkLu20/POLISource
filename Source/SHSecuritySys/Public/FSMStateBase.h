// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FSMStateBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class SHSECURITYSYS_API UFSMStateBase : public UObject
{
	GENERATED_BODY()	
	
public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "FSM")
		int GetIndex();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "FSM")
		void	OnEnter();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "FSM")
		void	OnUpdate(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "FSM")
		void	OnExit();
};
