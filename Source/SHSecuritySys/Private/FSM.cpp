// Fill out your copyright notice in the Description page of Project Settings.

#include "FSM.h"
#include "FSMStateBase.h"


// Sets default values for this component's properties
UFSM::UFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	pCurrentState = NULL;
	pPrevState = NULL;
}


// Called when the game starts
void UFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (pCurrentState)
		pCurrentState->OnUpdate(DeltaTime);
}

bool	UFSM::AddState(UFSMStateBase* pState)
{
	if (pState->GetIndex() == invalidState)
		return false;

	int stateIndx = pState->GetIndex();
	if (statesMap.Find(pState->GetIndex()) != NULL)
		return false;

	statesMap.Add(pState->GetIndex(), pState);
	return true;
}

void	UFSM::RemoveState(int stateIndx) 
{
	if (pCurrentState != NULL && pCurrentState->GetIndex() == stateIndx)
		ChangeState(invalidState);

	UFSMStateBase** pState = statesMap.Find(stateIndx);
	if (*pState == NULL)
		return;

	//delete pState;
	statesMap.Remove(stateIndx);
}

void	UFSM::RemoveAllStates()
{
	pCurrentState = NULL;
	pPrevState = NULL;

	statesMap.Reset();
}

bool	UFSM::ChangeState(int stateIndx)
{
	if (pCurrentState)
	{
		if (pCurrentState->GetIndex() == stateIndx)
			return false;
		else
			pCurrentState->OnExit();
	}
	pPrevState = pCurrentState;

	if (stateIndx == invalidState)
		pCurrentState = NULL;
	else
		pCurrentState = GetState(stateIndx);

	if (pCurrentState)
		pCurrentState->OnEnter();
	else
		return false;

	return true;
}


UFSMStateBase* UFSM::GetState(int stateIndx)
{
	if (stateIndx == invalidState)
		return NULL;

	UFSMStateBase** pState = statesMap.Find(stateIndx);
	if (*pState == NULL)
		return NULL;

	return *pState;
}