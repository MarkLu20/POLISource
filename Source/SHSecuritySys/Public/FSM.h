// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "FSMStateBase.h"
#include "FSM.generated.h"

class UFSMStateBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API UFSM : public UActorComponent
{
	GENERATED_BODY()

	const static int invalidState = -1;

public:	
	// Sets default values for this component's properties
	UFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

			
public:

	UFUNCTION(BlueprintCallable, Category = "FSM")
		bool	AddState(UFSMStateBase* pState);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void	RemoveState(int stateIndx);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		void	RemoveAllStates();

	UFUNCTION(BlueprintCallable, Category = "FSM")
		UFSMStateBase*	GetState(int stateIndx);

	UFUNCTION(BlueprintCallable, Category = "FSM")
		UFSMStateBase*	GetCurState() { return pCurrentState; }

	UFUNCTION(BlueprintCallable, Category = "FSM")
		UFSMStateBase*	GetPrevState() { return pPrevState; }

	UFUNCTION(BlueprintCallable, Category = "FSM")
		bool			ChangeState(int stateIndx);

protected:

	TMap<uint32, UFSMStateBase*>	statesMap;
	UFSMStateBase*			pCurrentState;
	UFSMStateBase*			pPrevState;
};
