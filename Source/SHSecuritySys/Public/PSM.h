// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PSM.generated.h"


class UFSM;
class UFSMStateBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API UPSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UFUNCTION(BlueprintCallable, Category = "FSM")
		void ResetFSMs(int typeCount);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		bool AddState(int stateType, UFSMStateBase* pState);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		UFSMStateBase* GetState(int stateType, int stateIndx);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		UFSMStateBase *GetCurrentState(int stateType);
	
	UFUNCTION(BlueprintCallable, Category = "FSM")
		UFSMStateBase* GetPrevState(int stateType);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveState(int stateType, int stateIndx);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStatesByType(int stateType);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void RemoveAllStates();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void ChangeState(int stateType, int stateIndx);

protected:

	TArray<UFSM*>	allStates;
};