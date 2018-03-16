// Fill out your copyright notice in the Description page of Project Settings.

#include "PSM.h"
#include "FSM.h"

// Sets default values for this component's properties
UPSM::UPSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPSM::ResetFSMs(int typeCount)
{
	allStates.Reset(typeCount);

	for(int i = 0; i<typeCount; ++i)
	{
		allStates[i] = new UFSM();
	}
}

bool UPSM::AddState(int stateType, UFSMStateBase* pState) 
{
	if (stateType > allStates.Num())
		return false;

	return allStates[stateType]->AddState(pState);
}

UFSMStateBase* UPSM::GetState(int stateType, int stateIndx)
{
	if (stateType > allStates.Num())
		return NULL;

	return allStates[stateType]->GetState(stateIndx);
}

UFSMStateBase* UPSM::GetCurrentState(int stateType) 
{
	if (stateType > allStates.Num())
		return NULL;

	return allStates[stateType]->GetCurState();
}

UFSMStateBase* UPSM::GetPrevState(int stateType)
{
	if (stateType > allStates.Num())
		return NULL;

	return allStates[stateType]->GetPrevState();
}

void UPSM::RemoveState(int stateType, int stateIndx)
{
	if (stateType > allStates.Num())
		return;

	return allStates[stateType]->RemoveState( stateIndx );
}

void UPSM::RemoveAllStatesByType(int stateType)
{
	if (stateType > allStates.Num())
		return;

	return allStates[stateType]->RemoveAllStates();
}

void UPSM::RemoveAllStates()
{
	for( int i = 0 ;i < allStates.Num(); i++)
	{
		allStates[i]->RemoveAllStates();
	}
}

void UPSM::ChangeState(int stateType, int stateIndx)
{
	if (stateType > allStates.Num())
		return;

	allStates[stateType]->ChangeState(stateIndx);
}